/* -----------------------------------------------------------------
                            TEdit Standard Types

     This header contains type definitions for platform independence.

                            Michael T. Duffy
   ----------------------------------------------------------------- */

// TTypes.hpp
// Copyright 2004, 2008, Michael T. Duffy.  All Rights Reserved
// contact:  mduffor@gmail.com

// This file is part of TEdit.
//
// TEdit is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 3 as
// published by the Free Software Foundation.
//
// TEdit is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with TEdit.  If not, see <http://www.gnu.org/licenses/>.


// Note:  Be sure to include platform specific header files before including
//         this file, because we will need to detect what has already been
//         defined by the system before trying to create our own types


#ifndef TYPES_HPP
#define TYPES_HPP

#include "stdio.h"

#define RAVEN_EXPORT 

#ifdef RAVEN_WIN32_COMPATIBLE
  #define WIN32_LEAN_AND_MEAN
  #include <windows.h>
  //#include <windowsx.h>
#endif

#ifdef LINUX
  //#define strnicmp strncasecmp
  //#define stricmp strcasecmp
#endif

#ifdef ANDROID_NDK
  #define stricmp strcasecmp
  #define strnicmp strncasecmp
  #define powl pow
  #undef BOOL
#endif


// variable types

// If this is being compiled under windows, check for the windows header
//  that would define these same kind of typedefs
#ifndef _BASETSD_H_

  typedef unsigned long      UINT32;  ///< 32 bit unsigned integer.  Use UINT32 when the size of the variable matters, and use UINT if it does not.
  typedef          long      INT32;   ///< 32 bit signed integer.  Use INT32 when the size of the variable matters, and use INT if it does not.


  typedef unsigned int       UINT;    ///< 32 bit unsigned integer.  Defined to be the native size for the target platform, and will thus not incur any possible speed penalties.
  typedef          int       INT;     ///< 32 bit signed integer.  Defined to be the native size for the target platform, and will thus not incur any possible speed penalties.

  typedef bool               BOOL;    ///< boolean value
  typedef bool *             PBOOL;   ///< pointer to a boolean

  typedef unsigned long *    PUINT32; ///< pointer to 32-bit unsigned integer
  typedef          long *    PINT32;  ///< pointer to 32-bit signed integer

  typedef unsigned int *     PUINT;   ///< pointer to unsigned integer
  typedef          int *     PINT;    ///< pointer to signed integer

  typedef unsigned char      UINT8;   // use UINT8 or INT8 when the size of the
  typedef          char      INT8;    //  integer variable matters, and UINT or INT

  typedef unsigned short     UINT16;  // use UINT16 or INT16 when the size of the
  typedef          short     INT16;   //  integer variable matters, and UINT or INT

  typedef unsigned char *    PUINT8;  // use UINT8 or INT8 when the size of the
  typedef          char *    PINT8;   //  integer variable matters, and UINT or INT

  typedef unsigned short *   PUINT16; // use UINT16 or INT16 when the size of the
  typedef          short *   PINT16;  //  integer variable matters, and UINT or INT

#endif // _BASETSD_H_

typedef float              FLOAT;
typedef double             DOUBLE;

typedef float              FLOAT32;
typedef double             FLOAT64;

#define VOID               void

typedef char *             PSZ;
typedef const char *       CPSZ;
typedef char               CHAR;
typedef unsigned char      UCHAR;

// pointer types


typedef float  *           PFLOAT;
typedef double *           PDOUBLE;

typedef void *             PVOID;
typedef char *             PCHAR;
typedef unsigned char *    PUCHAR;

typedef unsigned long      TErrorCode;

const TErrorCode           errorNone    = 0;
const TErrorCode           errorSuccess = 0;
const TErrorCode           errorFailure = 1;

#ifndef TRUE
  #define TRUE  BOOL(1)
  #define FALSE BOOL(0)
#endif

#ifndef NULL
  #define NULL  null
#endif

//#endif // UINT32

#ifndef TMax
  #define TMax(a,b) ((a)>(b)?(a):(b))
  #define TMin(a,b) ((a)<(b)?(a):(b))
#endif


// Math related defines

#ifndef R_PI
  #define R_PI          3.141592654f   ///< Constant value for Pi, in float precision
#endif

#ifndef R_2PI
  #define R_2PI         6.283185307f   ///< Constant value for 2 * Pi, in float precision
#endif

#ifndef R_DEG_TO_RAD
  #define R_DEG_TO_RAD  0.017453293f   ///< Constant multiplier for converting degrees into radians
  #define R_RAD_TO_DEG  57.29577951f   ///< Constant multiplier for converting radians into degress
#endif

// Epsilon is used for floating point equivalency tests.
#ifndef R_EPSILON
  #define R_EPSILON     0.0000000001f
#endif
  
//#define MAKE_FOUR_CODE(a,b,c,d) ((UINT32) (((UINT32) a) + (((UINT32) b) << 8) + (((UINT32) c) << 16) + (((UINT32) d) << 24)))
#define MAKE_FOUR_CODE(a) ((UINT32) (((UINT32) a[0]) + (((UINT32) a[1]) << 8) + (((UINT32) a[2]) << 16) + (((UINT32) a[3]) << 24)))


class RStr;

//-----------------------------------------------------------------------------
class EStatus
  {
  public:
    enum ECode  {kSuccess = 0,
                 kFailure = 1};

    RStr *   pstrDescription;


  private:
    ECode        eStatusCode;

  public:
                  EStatus    ();
                  EStatus    (ECode         codeIn);
                  EStatus    (ECode         codeIn,
                              const char *  pszDescriptionIn);
                  EStatus    (const EStatus &  statIn);
                  ~EStatus   ();

    EStatus &     operator=  (const EStatus &  statIn);
    BOOL          operator== (const EStatus &  statIn) const       {return (eStatusCode == statIn.eStatusCode);};
    BOOL          operator== (const EStatus::ECode  codeIn) const  {return (eStatusCode == codeIn);};
    BOOL          operator!= (const EStatus &  statIn) const       {return (eStatusCode != statIn.eStatusCode);};
    BOOL          operator!= (const EStatus::ECode  codeIn) const  {return (eStatusCode != codeIn);};

    BOOL          Success (VOID)  {return (eStatusCode == kSuccess);};
    BOOL          Failure (VOID)  {return (eStatusCode == kFailure);};
    
    VOID          SetDescription  (const char *  pszDescriptionIn);
    const char *  GetDescription  (VOID);

  };


#endif // TYPES_HPP














