#ifndef IO_H
#define IO_H

#ifdef __cplusplus
extern "C" {
#endif

extern unsigned char insb(unsigned short port);
extern unsigned short insw(unsigned short port);
extern void outb(unsigned short port, unsigned char val);
extern void outw(unsigned short port, unsigned short val);

#ifdef __cplusplus
}
#endif

#endif