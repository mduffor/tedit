/* -----------------------------------------------------------------
                            TEdit String Array

    This module implements a string array class.

                            Michael T. Duffy
   ----------------------------------------------------------------- */

// RStrArray.hpp
// Copyright 2004, 2008, Michael T. Duffy.  
// contact:  mduffor@users.sourceforge.net

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


#ifndef RSTRARRAY_HPP
#define RSTRARRAY_HPP

#include <ctype.h>
#include <stdarg.h>   // for variable argument functions (va_*)
#include "Types.hpp"
#include "BaseArray.hpp"
#include "IntArray.hpp"
#include "RStr.hpp"


/// An array of RStr instances
//-----------------------------------------------------------------------------
class RStrArray : public BaseArray
  {
  private:

  public:

                 RStrArray      ();
                 RStrArray      (const RStrArray &  arrayIn);
                 RStrArray      (INT   iInitialSize);
                 ~RStrArray     ();

    VOID *       AllocArray     (INT      iSizeIn);
    VOID         DeleteArray    (PVOID *  pvArrayIn);
    VOID         InitValues     (INT      iStartOffset,
                                 INT      iNumToInit);
    VOID         CopyValues     (PVOID    pvSourceData,
                                 INT      iSourceOffsetIn,
                                 INT      iStartOffset,
                                 INT      iNumToCopy);
    VOID         CopyValuesRev  (PVOID    pvSourceData,
                                 INT      iSourceOffsetIn,
                                 INT      iStartOffset,
                                 INT      iNumToCopy);
    VOID         AverageValues  (INT      iIndexAverage,
                                 INT      iIndexValueOne,
                                 INT      iIndexValueTwo,
                                 FLOAT    fBias = 0.5f);
    VOID         SwapIndexes    (INT  iIndexOne,
                                 INT  iIndexTwo); 
    INT          CompareIndexes (INT  iIndexOne,
                                 INT  iIndexTwo); 

    EStatus      Copy           (const RStrArray &  arraySource);
    EStatus      Set            (RStr  strElement, INT  iIndex);
    EStatus      Append         (RStr  strElement);
    EStatus      GetArray       (RStr  []) const;

    RStr         Index          (INT  iIndex) const  {return ((*this)[iIndex]);};  
    RStr         operator[]     (INT  iIndex) const;
    RStr &       operator[]     (INT  iIndex);
    RStrArray &  operator=      (const  RStrArray & arrayIn);
    BOOL         operator==     (const  RStrArray & arrayIn);
    BOOL         operator!=     (const  RStrArray & arrayIn)   {return (!(*this == arrayIn));};
    
                                /**
                                @brief Insert another array into this array at a given position
                                @param iIndexIn Position where the new array should be inserted
                                @param arrayToInsertIn The array that you want to insert.
                                @return None
                                */
    EStatus      Insert         (INT          iIndexIn,
                                 RStrArray &  arrayToInsertIn);

    INT          Find           (RStr &  strIn);
    
    INT          Find           (const char *  pszIn);

    VOID         SortIndexes    (IntArray &  arrayOut);

  };




#endif // RSTRARRAY_HPP
