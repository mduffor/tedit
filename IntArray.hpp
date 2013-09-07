/* -----------------------------------------------------------------
                            TEdit Int Array

     This module implements an array of floats and an array of integers

                            TEdit T. Duffy
   ----------------------------------------------------------------- */

// IntArray.hpp
// Copyright 2004, 2008, TEdit T. Duffy.  All Rights Reserved
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

#ifndef INTARRAY_HPP
#define INTARRAY_HPP

#include "Types.hpp"
#include "BaseArray.hpp"

//------------------------------------------------------------------------
// Defines
//------------------------------------------------------------------------

//------------------------------------------------------------------------
// Class Definitions
//------------------------------------------------------------------------


//-----------------------------------------------------------------------------
class IntArray : public BaseArray
  {
  private:

  public:

                   IntArray      ();
                   IntArray      (const IntArray &  arrayIn);
                   IntArray      (INT   iInitialSize);
                   ~IntArray     ();

    VOID *         AllocArray       (INT      iSizeIn);
    VOID           DeleteArray      (PVOID *  pvArrayIn);
    VOID           InitValues       (INT      iStartOffset,
                                     INT      iNumToInit,
                                     INT      iValue = 0);
    VOID           CopyValues       (PVOID    pvSourceData,
                                     INT      iSourceOffsetIn,
                                     INT      iStartOffset,
                                     INT      iNumToCopy);
    VOID           CopyValuesRev    (PVOID    pvData,
                                     INT      iSourceOffsetIn,
                                     INT      iStartOffset,
                                     INT      iNumToCopy);
    VOID           AverageValues    (INT      iIndexAverage,
                                     INT      iIndexValueOne,
                                     INT      iIndexValueTwo,
                                     FLOAT    fBias = 0.5f);
    VOID           SwapIndexes      (INT  iIndexOne,
                                     INT  iIndexTwo); 
    INT            CompareIndexes   (INT  iIndexOne,
                                     INT  iIndexTwo); 

    EStatus        Copy             (const  IntArray &  arraySource);
    EStatus        Set              (INT32  iElement, INT  iIndex);
    EStatus        Append           (INT32  iElement);
    EStatus        GetArray         (INT32  []) const;

    INT32          operator[]       (INT  iIndex) const;
    INT32 &        operator[]       (INT  iIndex);
    INT32          Index            (INT  iIndex) const {return (*this)[iIndex];};
    IntArray &    operator=        (const  IntArray & arrayIn);
    VOID           Set              (const  IntArray & arrayIn)  {*this = arrayIn;};
    BOOL           operator==       (const  IntArray & arrayIn);
    BOOL           operator!=       (const  IntArray & arrayIn)  {return (!(*this == arrayIn));};

    INT            Find             (INT32  iSearchIn);
    VOID           DebugPrint       (UINT  uCount);

    VOID           CalcRange        (INT &  iMinOut,
                                     INT &  iMaxOut);

  };
  

VOID  SortIndexes (BaseArray &  arrayToSortIn,
                   IntArray &   arrayOut);



#endif // INTARRAY_HPP


