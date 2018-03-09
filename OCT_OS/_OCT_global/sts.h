
#ifndef _STS_H
#define _STS_H

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************************/
/*                                          TYPES                                         */
/******************************************************************************************/
   
// Natural types. 
typedef int                intn;
typedef unsigned		   cardn;
typedef long int           intl;
typedef long unsigned	   cardl;
typedef float              floatn;
typedef double             doublen;
typedef long double        doublel;
typedef long long signed   intll;								/* 64-bit intll */
typedef long long unsigned cardll;								/* 64-bit cardll */

// Fixed size types.
typedef unsigned long long card64;
typedef unsigned int	   card32;
typedef unsigned short	   card16;
typedef unsigned char	   card8;
typedef char               char8;
typedef wchar_t			   char32;
typedef signed long long   int64;
typedef signed int	       int32;
typedef signed short       int16;
typedef signed char        int8;
typedef float		       float32;
typedef double             float64;

// card and int types the size of a natural pointer.
typedef card32		   	   cardptsz;
typedef int32		   	   intptsz;

// Booleans are built on top of STS types.
typedef cardn			   booln;									/* The natural bool. */
typedef	card8			   bool8;									/* For structs. */
typedef card32			   bool32;									/* For old (enum) 'bool's in structs. */
#define False			   0
#define True			   1


/******************************************************************************************/
/*                                         LIMITS                                         */
/******************************************************************************************/
   
/* ------>  The limits for each of the STS types. */
#define INT8_MIN				(-128)
#ifndef INT8_MAX
	#define INT8_MAX			127
#endif 
#define CARD8_MAX				255U
#ifdef __CHAR_UNSIGNED__
  	#define CHAR8_MIN			0
  	#define CHAR8_MAX			CARD8_MAX
#else
  	#define CHAR8_MIN			INT8_MIN
  	#define CHAR8_MAX			INT8_MAX
#endif 
#ifndef INT16_MIN
	#define INT16_MIN			(-32768)
#endif 
#ifndef INT16_MAX
	#define INT16_MAX			32767
#endif 
#define CARD16_MIN				0
#define CARD16_MAX				65535
#ifndef INT32_MIN
	#define INT32_MIN			(- INT32_MAX - 1)
#endif 
#ifndef INT32_MAX
	#define INT32_MAX			2147483647L
#endif 
#define CARD32_MIN				0UL
#define CARD32_MAX				4294967295UL
#ifndef INT64_MIN
	#define INT64_MIN			(- INT64_MAX -1)
#endif 
#ifndef INT64_MAX
	#define INT64_MAX			9223372036854775807LL
#endif 
#define CARD64_MIN				0ULL
#define CARD64_MAX				18446744073709551615ULL


/******************************************************************************************/
/*                                     SPLIT and JOIN                                     */
/******************************************************************************************/
   
/* -------> These split and join int64 and card64 values. */
#define HighInt32(ll)					( (int32) ( ((ll) >> 32) & 0xffffffffL) )
#define LowInt32(ll)					( (int32) ((ll) & 0xffffffffL) )
#define HighCard32(ll)					( (card32) ( ((ll) >> 32) & 0xffffffffUL) )
#define LowCard32(ll)					( (card32) ((ll) & 0xffffffffUL) )
#define Join3Int64(up, lo)				( (((int64) (up)) << 32) | ((card32) (lo)) )
#define Join2Card64(up, lo)				( (((card64) (up)) << 32) | ((card64) (lo)) )

/* -------> These split and join int32 and card32 values. */
#define HighInt16(l)					( (int16) ( ((l) >> 16) & 0x0000ffff) )
#define LowInt16(l)          		    ( (int16) ((l) & 0x0000ffff) )
#define HighCard16(l)					( (card16) ( ((l) >> 16) & 0x0000ffff) )
#define LowCard16(l)          		    ( (card16) ((l) & 0x0000ffff) )
#define Join2Int32(up, lo)   		    ( (((int32) (up)) << 16) | ((card16) (lo)) )
#define Join2Card32(up, lo)   		    ( (((card32) (up)) << 16) | ((card16) (lo)) )

/* -------> These split and join int16 and card16 values. */
#define HighInt8(i)					    ( (int8) (((i)>>8) & 0xff) )
#define LowInt8(i)						( (int8) ((i) & 0xff) )
#define HighCard8(i)				    ( (card8) (((i)>>8) & 0xff) )
#define LowCard8(i)						( (card8) ((i) & 0xff) )
#define Join2Int16(up, lo)				( (((int16) (up)) << 8) | ((card8) (lo)) )
#define Join2Card16(up, lo)				( (((card16) (up)) << 8) | ((card8) (lo)) )


#ifdef __cplusplus
}
#endif

#endif	/* _STS_H */




