#ifndef TYPES_H
#define TYPES_H


//Just shortening phrases so that we can use appropriate sized types 
//in order to save memory because I am programming everything on the stack

typedef unsigned int   u32;
typedef          int   s32;
typedef unsigned short u16;
typedef          short s16;
typedef unsigned char  u8;
typedef          char  s8;

#define low_16(address) (u16)((address) & 0xFFFF)
#define high_16(address) (u16)(((address) >> 16) & 0xFFFF)

#endif
