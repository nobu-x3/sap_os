/* stdio/printf.c -- drop-in replacement for kernel printf */
#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

static bool print(const char *data, size_t length) {
  const unsigned char *bytes = (const unsigned char *)data;
  for (size_t i = 0; i < length; i++)
    if (putchar(bytes[i]) == EOF)
      return false;
  return true;
}

static int print_char(char c) {
  if (putchar(c) == EOF)
    return -1;
  return 1;
}

static size_t utoa(unsigned long long value, unsigned base, char *buf,
                   bool upper) {
  const char *digits_l = "0123456789abcdef";
  const char *digits_u = "0123456789ABCDEF";
  const char *digits = upper ? digits_u : digits_l;
  if (value == 0) {
    buf[0] = '0';
    buf[1] = '\0';
    return 1;
  }
  char tmp[64];
  size_t pos = 0;
  while (value) {
    tmp[pos++] = digits[value % base];
    value /= base;
  }
  for (size_t i = 0; i < pos; ++i)
    buf[i] = tmp[pos - i - 1];
  buf[pos] = '\0';
  return pos;
}

static int pad_output(char pad, int count) {
  int out = 0;
  for (int i = 0; i < count; ++i) {
    if (print_char(pad) < 0)
      return -1;
    out++;
  }
  return out;
}

int printf(const char *restrict format, ...) {
  va_list ap;
  va_start(ap, format);
  int written = 0;
  while (*format != '\0') {
    // safety check
    size_t maxrem = INT_MAX - written;
    if (maxrem <= 0) {
      va_end(ap);
      return -1;
    }
    // literal or escaped %
    if (format[0] != '%' || format[1] == '%') {
      if (format[0] == '%')
        format++;
      const char *start = format;
      while (*format && *format != '%')
        format++;
      size_t amount = (size_t)(format - start);
      if (amount == 0)
        continue;
      if (maxrem < amount) {
        va_end(ap);
        return -1;
      }
      if (!print(start, amount)) {
        va_end(ap);
        return -1;
      }
      written += (int)amount;
      continue;
    }
    // process format specifier
    format++;
    bool left = false, plus = false, space = false, alt = false, zero = false;
    bool parsing_flags = true;
    while (parsing_flags) {
      switch (*format) {
      case '-':
        left = true;
        format++;
        break;
      case '+':
        plus = true;
        format++;
        break;
      case ' ':
        space = true;
        format++;
        break;
      case '#':
        alt = true;
        format++;
        break;
      case '0':
        zero = true;
        format++;
        break;
      default:
        parsing_flags = false;
        break;
      }
    }

    // width
    int width = -1;
    if (*format == '*') {
      format++;
      width = va_arg(ap, int);
      if (width < 0) {
        left = true;
        width = -width;
      }
    } else {
      if (*format >= '0' && *format <= '9') {
        int v = 0;
        while (*format >= '0' && *format <= '9') {
          v = v * 10 + (*format - '0');
          format++;
        }
        width = v;
      }
    }
    // precision
    int precision = -1;
    if (*format == '.') {
      format++;
      if (*format == '*') {
        format++;
        precision = va_arg(ap, int);
        if (precision < 0)
          precision = -1;
      } else {
        int v = 0;
        while (*format >= '0' && *format <= '9') {
          v = v * 10 + (*format - '0');
          format++;
        }
        precision = v;
      }
    }
    // length modifiers: hh, h, l, ll
    enum { LEN_DEFAULT, LEN_HH, LEN_H, LEN_L, LEN_LL } length = LEN_DEFAULT;
    if (*format == 'h') {
      format++;
      if (*format == 'h') {
        length = LEN_HH;
        format++;
      } else
        length = LEN_H;
    } else if (*format == 'l') {
      format++;
      if (*format == 'l') {
        length = LEN_LL;
        format++;
      } else
        length = LEN_L;
    }
    // specifier
    char spec = *format++;
    if (spec == '\0')
      break;
    // handling
    if (spec == 'c') {
      int c = va_arg(ap, int);
      if (maxrem < 1) {
        va_end(ap);
        return -1;
      }
      int pad = (width > 1) ? width - 1 : 0;
      int out = 0;
      if (!left && pad) {
        int r = pad_output(zero ? '0' : ' ', pad);
        if (r < 0) {
          va_end(ap);
          return -1;
        }
        out += r;
      }
      if (print_char((char)c) < 0) {
        va_end(ap);
        return -1;
      }
      out++;
      if (left && pad) {
        int r = pad_output(' ', pad);
        if (r < 0) {
          va_end(ap);
          return -1;
        }
        out += r;
      }
      written += out;
      continue;
    } else if (spec == 's') {
      const char *s = va_arg(ap, const char *);
      if (s == NULL)
        s = "(null)";
      size_t len = strlen(s);
      if (precision >= 0 && (size_t)precision < len)
        len = (size_t)precision;
      if ((int)len > width && width > 0) { // width smaller than len -> no pad
      }
      int pad = (width > (int)len) ? width - (int)len : 0;
      int out = 0;
      if (!left && pad) {
        int r = pad_output(' ', pad);
        if (r < 0) {
          va_end(ap);
          return -1;
        }
        out += r;
      }
      if (!print(s, len)) {
        va_end(ap);
        return -1;
      }
      out += (int)len;
      if (left && pad) {
        int r = pad_output(' ', pad);
        if (r < 0) {
          va_end(ap);
          return -1;
        }
        out += r;
      }
      written += out;
      continue;
    } else if (spec == 'd' || spec == 'i' || spec == 'u' || spec == 'o' ||
               spec == 'x' || spec == 'X' || spec == 'p') {
      // extract integer value according to length
      unsigned long long val = 0;
      signed long long sval = 0;
      bool is_signed = (spec == 'd' || spec == 'i');
      if (spec == 'p') {
        // pointer prints as void* in hex
        void *ptr = va_arg(ap, void *);
        val = (uintptr_t)ptr;
        // force alt and hex
        alt = true;
      } else {
        if (is_signed) {
          if (length == LEN_LL)
            sval = va_arg(ap, long long);
          else if (length == LEN_L)
            sval = va_arg(ap, long);
          else if (length == LEN_H)
            sval = (short)va_arg(ap, int);
          else if (length == LEN_HH)
            sval = (signed char)va_arg(ap, int);
          else
            sval = va_arg(ap, int);
          // convert to absolute if negative
          if (sval < 0)
            val = (unsigned long long)(-(sval + 1)) + 1ULL;
          else
            val = (unsigned long long)sval;
        } else {
          if (length == LEN_LL)
            val = va_arg(ap, unsigned long long);
          else if (length == LEN_L)
            val = va_arg(ap, unsigned long);
          else if (length == LEN_H)
            val = (unsigned short)va_arg(ap, unsigned int);
          else if (length == LEN_HH)
            val = (unsigned char)va_arg(ap, unsigned int);
          else
            val = va_arg(ap, unsigned int);
        }
      }
      // decide base
      unsigned base = 10;
      bool upper = false;
      if (spec == 'o')
        base = 8;
      else if (spec == 'x' || spec == 'p')
        base = 16;
      else if (spec == 'X') {
        base = 16;
        upper = true;
      }
      // convert
      char numbuf[65];
      size_t numlen = utoa(val, base, numbuf, upper);
      // precision: leading zeros for numeric conversions
      if (precision == 0 && val == 0)
        numlen = 0; // when precision is zero and value is zero, nothing is
                    // printed
      int prec_zeros = 0;
      if (precision > (int)numlen)
        prec_zeros = precision - (int)numlen;
      // sign
      char signchar = 0;
      if (is_signed) {
        if (sval < 0)
          signchar = '-';
        else if (plus)
          signchar = '+';
        else if (space)
          signchar = ' ';
      }
      // alternate prefix
      int alt_len = 0;
      const char *alt_prefix = "";
      if (alt && val != 0) {
        if (base == 16) {
          alt_prefix = upper ? "0X" : "0x";
          alt_len = 2;
        } else if (base == 8) {
          // ensure leading zero
          if (numlen == 0 || numbuf[0] != '0') {
            alt_prefix = "0";
            alt_len = 1;
          }
        }
      } else if (spec == 'p') {
        alt_prefix = "0x";
        alt_len = 2;
      }
      int total_len = (int)numlen + prec_zeros + alt_len + (signchar ? 1 : 0);
      int pad = (width > total_len) ? width - total_len : 0;
      int out = 0;
      char padchar = (zero && precision < 0 && !left) ? '0' : ' ';
      if (!left && pad) {
        int r = pad_output(padchar, pad);
        if (r < 0) {
          va_end(ap);
          return -1;
        }
        out += r;
      }

      // sign
      if (signchar) {
        if (print_char(signchar) < 0) {
          va_end(ap);
          return -1;
        }
        out++;
      }
      // alternate prefix
      if (alt_len) {
        if (!print(alt_prefix, alt_len)) {
          va_end(ap);
          return -1;
        }
        out += alt_len;
      }
      // precision zeros
      if (prec_zeros) {
        int r = pad_output('0', prec_zeros);
        if (r < 0) {
          va_end(ap);
          return -1;
        }
        out += r;
      }
      // number itself
      if (numlen) {
        if (!print(numbuf, numlen)) {
          va_end(ap);
          return -1;
        }
        out += (int)numlen;
      }
      // left padding
      if (left && pad) {
        int r = pad_output(' ', pad);
        if (r < 0) {
          va_end(ap);
          return -1;
        }
        out += r;
      }
      written += out;
      continue;
    } else {
      // unknown specifier: print it literally including % and char
      const char unknown_seq[2] = {'%', spec};
      if (maxrem < 2) {
        va_end(ap);
        return -1;
      }
      if (!print(unknown_seq, 2)) {
        va_end(ap);
        return -1;
      }
      written += 2;
      continue;
    }
  }
  va_end(ap);
  return written;
}
