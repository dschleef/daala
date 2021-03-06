/*Daala video codec
Copyright (c) 2006-2010 Daala project contributors.  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

- Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.*/

#if !defined(_internal_H)
# define _internal_H (1)
# include <limits.h>
# include "../include/daala/codec.h"
# include "odintrin.h"

# if defined(_MSC_VER)
#  pragma warning(disable:4554 4799)
#  define _USE_MATH_DEFINES
# elif OD_GNUC_PREREQ(4,2)
#  pragma GCC diagnostic ignored "-Wparentheses"
#  pragma GCC diagnostic ignored "-Wlong-long"
#  pragma GCC diagnostic ignored "-Woverlength-strings"
# endif

# define OD_VERSION_MAJOR (0)
# define OD_VERSION_MINOR (0)
# define OD_VERSION_SUB   (0)

# define OD_VENDOR_STRING "Xiph's experimental encoder library " __DATE__

/*Constants for the packet state machine common between encoder and decoder.*/

/*Next packet to emit/read: Codec info header.*/
#define OD_PACKET_INFO_HDR    (-3)
/*Next packet to emit/read: Comment header.*/
#define OD_PACKET_COMMENT_HDR (-2)
/*Next packet to emit/read: Codec setup header.*/
#define OD_PACKET_SETUP_HDR   (-1)
/*Next more packets to emit/read.*/
#define OD_PACKET_DONE        (INT_MAX)

/*Smallest blocks are 4x4*/
#define OD_LOG_BSIZE0 (2)
/*There are 3 block sizes total (4x4, 8x8, 16x16).*/
#define OD_NBSIZES    (3)
/*The largest block size.*/
#define OD_BSIZE_MAX  (1<<OD_LOG_BSIZE0+OD_NBSIZES-1)



# if defined(OD_ENABLE_ASSERTIONS)
#  include <stdio.h>
#  include <stdlib.h>
#  if OD_GNUC_PREREQ(2,5)
__attribute__((noreturn))
#  endif
void od_fatal_impl(const char *_str,const char *_file,int _line);

#  define OD_FATAL(_str) (od_fatal_impl(_str,__FILE__,__LINE__))

#  define OD_ASSERT(_cond) \
  do{ \
    if(!(_cond)){ \
      OD_FATAL("assertion failed: " #_cond); \
    } \
  } \
  while(0)

#  define OD_ASSERT2(_cond,_message) \
  do{ \
    if(!(_cond)){ \
      OD_FATAL("assertion failed: " #_cond "\n" _message); \
    } \
  } \
  while(0)

# else
#  define OD_ASSERT(_cond)
#  define OD_ASSERT2(_cond,_message)
# endif

/*Currently this structure is only in Tremor, and is read-only.*/
typedef struct oggbyte_buffer oggbyte_buffer;

/*Simple libogg1-style buffer.*/
struct oggbyte_buffer{
  unsigned char *buf;
  unsigned char *ptr;
  long           storage;
};

/*Encoding functions.*/
void oggbyte_writeinit(oggbyte_buffer *_b);
void oggbyte_writetrunc(oggbyte_buffer *_b,long _bytes);
void oggbyte_write1(oggbyte_buffer *_b,unsigned _value);
void oggbyte_write4(oggbyte_buffer *_b,ogg_uint32_t _value);
void oggbyte_writecopy(oggbyte_buffer *_b,const void *_source,long _bytes);
void oggbyte_writeclear(oggbyte_buffer *_b);
/*Decoding functions.*/
void oggbyte_readinit(oggbyte_buffer *_b,unsigned char *_buf,long _bytes);
int oggbyte_look1(oggbyte_buffer *_b);
int oggbyte_look4(oggbyte_buffer *_b,ogg_uint32_t *_val);
void oggbyte_adv1(oggbyte_buffer *_b);
void oggbyte_adv4(oggbyte_buffer *_b);
int oggbyte_read1(oggbyte_buffer *_b);
int oggbyte_read4(oggbyte_buffer *_b,ogg_uint32_t *_val);
/*Shared functions.*/
void oggbyte_reset(oggbyte_buffer *_b);
long oggbyte_bytes(oggbyte_buffer *_b);
unsigned char *oggbyte_get_buffer(oggbyte_buffer *_b);

int od_ilog(ogg_uint32_t _v);
void **od_malloc_2d(size_t _height,size_t _width,size_t _sz);
void **od_calloc_2d(size_t _height,size_t _width,size_t _sz);
void od_free_2d(void *_ptr);

# define OD_DIVU_DMAX (32)

extern ogg_uint32_t OD_DIVU_SMALL_CONSTS[OD_DIVU_DMAX][2];

/*Translate unsigned division by small divisors into multiplications.*/
# define OD_DIVU_SMALL(_x,_d) \
  ((ogg_uint32_t)(OD_DIVU_SMALL_CONSTS[(_d)-1][0]* \
  (unsigned long long)(_x)+OD_DIVU_SMALL_CONSTS[(_d)-1][1]>>32)>> \
  OD_ILOG(_d)-1)

#endif
