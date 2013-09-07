/* -----------------------------------------------------------------
                            TEdit String

    This module implements a string array class.

                            Michael T. Duffy
   ----------------------------------------------------------------- */

// RStrArray.cpp
// Copyright 2004, 2008, Michael T. Duffy.  
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Types.hpp"
#include "Debug.hpp"
#include "RStr.hpp"
#include "RStrArray.hpp"

ASSERTFILE (__FILE__);

//-----------------------------------------------------------------------------
//  RStrArray
//-----------------------------------------------------------------------------

// Note: This class should really be optimized by implementing it as an array of
//        pointers to RStr classes, instead of a straight array of RStrs.
//        Currently insert and remove calls perform copying entire strings,
//        instead of just moving pointers around.


//-----------------------------------------------------------------------------
RStrArray::RStrArray  ()
  {
  Init ();
  };


//-----------------------------------------------------------------------------
RStrArray::RStrArray  (const RStrArray &  arrayIn)
  {
  Init ();
  Copy (arrayIn);
  };


//-----------------------------------------------------------------------------
RStrArray::RStrArray  (INT   iInitialSize)
  {
  Init ();
  SetLength (iInitialSize);
  InitValues (0, iAllocSize);
  };


//-----------------------------------------------------------------------------
RStrArray::~RStrArray  ()
  {
  DeleteArray (&pArray);
  };


//-----------------------------------------------------------------------------
VOID *  RStrArray::AllocArray  (INT  iSizeIn)
  {
  return new RStr [iSizeIn];
  };


//-----------------------------------------------------------------------------
VOID  RStrArray::DeleteArray  (PVOID *  pvArrayIn)
  {
  delete [] ((RStr *) *pvArrayIn);
  *pvArrayIn = NULL;
  };


//-----------------------------------------------------------------------------
VOID  RStrArray::InitValues  (INT    iStartOffset,
                              INT    iNumToInit)
  {
  // zero out the requested range of the array

  INT iEndOffset = iStartOffset + iNumToInit;

  for (INT  iIndex = iStartOffset; iIndex < iEndOffset; ++iIndex)
    {
    ((RStr *) pArray) [iIndex] = "";
    };
  };


//-----------------------------------------------------------------------------
VOID  RStrArray::CopyValues  (PVOID  pvSourceDataIn,
                              INT    iSourceOffsetIn,
                              INT    iStartOffsetIn,
                              INT    iNumToCopyIn)
  {
  for (INT  iIndex = 0; iIndex < iNumToCopyIn; ++iIndex)
    {
    ((RStr *) pArray) [iIndex + iStartOffsetIn] = ((RStr *) pvSourceDataIn) [iIndex + iSourceOffsetIn];
    };
  };


//-----------------------------------------------------------------------------
VOID  RStrArray::CopyValuesRev  (PVOID  pvSourceDataIn,
                                 INT    iSourceOffsetIn,
                                 INT    iStartOffsetIn,
                                 INT    iNumToCopyIn)
  {
  // this routine is the opposite of CopyValues, and is used for
  //  properly shifting values to the right in the array

  for (INT  iIndex = iNumToCopyIn - 1; iIndex >= 0; --iIndex)
    {
    ((RStr *) pArray) [iStartOffsetIn + iIndex] = ((RStr *) pvSourceDataIn) [iIndex + iSourceOffsetIn];
    };
  };


//-----------------------------------------------------------------------------
VOID  RStrArray::AverageValues  (INT    iIndexAverage,
                                 INT    iIndexValueOne,
                                 INT    iIndexValueTwo,
                                 FLOAT  fBias)
  {
  // note:  You can't average strings.

  //((RStr *) pArray) [iIndexAverage] = ( (((RStr *) pArray) [iIndexValueOne] * fBias) +
  //                                       (((RStr *) pArray) [iIndexValueTwo] * (1.0f - fBias)) );
  };


//-----------------------------------------------------------------------------
VOID  RStrArray::SwapIndexes  (INT  iIndexOne,
                               INT  iIndexTwo)
  {
  RStr                  strTemp = ((RStr *) pArray) [iIndexOne];
  ((RStr *) pArray) [iIndexOne] = ((RStr *) pArray) [iIndexTwo];
  ((RStr *) pArray) [iIndexTwo] = strTemp;
  };
  
  
//-----------------------------------------------------------------------------
INT  RStrArray::CompareIndexes  (INT  iIndexOne,
                                 INT  iIndexTwo)
  {
  return (  (((RStr *) pArray) [iIndexOne]).Compare (((RStr *) pArray) [iIndexTwo]) );
  };


//-----------------------------------------------------------------------------
EStatus RStrArray::Set (RStr   dElement,
                        INT     iIndex)
  {
  if (iIndex >= iLength) return (EStatus::kFailure);
  ((RStr *) pArray) [iIndex] = dElement;
  return (EStatus::kSuccess);
  };


//-----------------------------------------------------------------------------
EStatus RStrArray::Append (RStr  dElement)
  {
  INT  iOldLength = iLength;

  if (SetLength (iOldLength + 1) == EStatus::kFailure) {return EStatus::kFailure;};

  ((RStr *) pArray) [iOldLength] = dElement;
  return (EStatus::kSuccess);
  };

//-----------------------------------------------------------------------------
EStatus RStrArray::GetArray (RStr  aOut []) const
  {
  if (iLength == 0) return (EStatus::kFailure);

  for (INT  iIndex = 0; iIndex < iLength; ++iIndex)
    {
    aOut [iIndex] = ((RStr *) pArray) [iIndex];
    };
  return (EStatus::kSuccess);
  };


//-----------------------------------------------------------------------------
EStatus RStrArray::Copy (const RStrArray &  arraySource)
  {
  if (SetLength (arraySource.iLength) == EStatus::kFailure) {return EStatus::kFailure;};

  CopyValues (arraySource.pArray, 0, 0, arraySource.Length ());

  return (EStatus::kSuccess);
  };


//-----------------------------------------------------------------------------
RStr RStrArray::operator[]  (INT  iIndex) const
  {
  if (this == NULL) {return (RStr::kEmpty);};
  //ASSERT (iIndex < iLength);
  if (iIndex >= iLength) {return (RStr::kEmpty);};
  return ((RStr *) pArray) [iIndex];
  };


//-----------------------------------------------------------------------------
RStr &  RStrArray::operator[]  (INT  iIndex)
  {
  if (this == NULL) {return (RStr::kEmpty);};
  //ASSERT (iIndex < iLength);
  if (iIndex >= iLength) {return (RStr::kEmpty);};
  return ((RStr *) pArray) [iIndex];
  };


//-----------------------------------------------------------------------------
RStrArray &  RStrArray::operator=  (const RStrArray &  arrayIn)
  {
  Copy (arrayIn);
  return (*this);
  };


//-----------------------------------------------------------------------------
BOOL  RStrArray::operator==  (const  RStrArray & arrayIn)
  {
  if (iLength == 0) return (EStatus::kFailure);

  for (INT  iIndex = 0; iIndex < iLength; ++iIndex)
    {
    if (arrayIn [iIndex] != ((RStr *) pArray) [iIndex]) return (false);
    };
  return (true);
  };


//-----------------------------------------------------------------------------
EStatus  RStrArray::Insert  (INT          iIndexIn,
                             RStrArray &  arrayToInsertIn)
   {
   INT    iNumToInsert = arrayToInsertIn.Length ();
   
   this->BaseArray::Insert (iIndexIn, iNumToInsert);
   for (INT  iCurr = 0; iCurr < iNumToInsert; ++iCurr)
     {
     Set (arrayToInsertIn [iCurr], iIndexIn + iCurr); 
     };
   return (EStatus::kSuccess);
   };


//-----------------------------------------------------------------------------
INT  RStrArray::Find  (RStr &  strIn)
  {
  for (INT iIndex = 0; iIndex < iLength; ++iIndex)
    {
    if (((RStr *) pArray) [iIndex] == strIn)
      {
      return (iIndex);
      };
    };
  return (-1);
  };


//-----------------------------------------------------------------------------
INT  RStrArray::Find  (const char *  pszIn)
  {
  for (INT iIndex = 0; iIndex < iLength; ++iIndex)
    {
    if (((RStr *) pArray) [iIndex] == pszIn)
      {
      return (iIndex);
      };
    };
  return (-1);
  };

//-----------------------------------------------------------------------------
VOID  RStrArray::SortIndexes (IntArray &  arrayOut)
  {
  // Perform a quicksort on the indexes of the array.
  
  
  INT32  iLeft;
  INT32  iRight;
  INT32  iIndex;


  // initialize the index array to be returned
  
  INT  iLength = this->Length ();
  
  arrayOut.SetLength (iLength);
  for (iIndex = 0; iIndex < iLength; ++iIndex)
    {
    arrayOut [iIndex] = iIndex;
    };

  iLeft = 0;
  iRight = iLength - 1;
  
  IntArray  arrayStack;
  
  arrayStack.Append (iLeft);
  arrayStack.Append (iRight);

  do
    {
    if (iRight > iLeft) 
      {
      // partition
      INT  iScanLeft = iLeft;
      INT  iScanRight = iRight;

      //printf ("partitioning between %d (%s) and %d (%s)\n", iScanLeft, (*this) [arrayOut [iScanLeft]].AsChar (),iScanRight, (*this) [arrayOut [iScanRight]].AsChar ());

      for (;;)
        {
        iScanLeft = iLeft;
        iScanRight = iRight;
        
        while (iScanLeft < iRight)
          {
          if ((*this) [arrayOut [iScanLeft]] > (*this) [arrayOut [iRight]])
            {
            break;
            };
          ++iScanLeft;
          };
        while (iScanRight > iLeft)
          {
          if ((*this) [arrayOut [iScanRight]] < (*this) [arrayOut [iRight]])
            {
            break;
            };
          --iScanRight;
          };

        if (iScanLeft < iScanRight)
          {
          INT32  iTemp = arrayOut [iScanRight];
          arrayOut [iScanRight] = arrayOut [iScanLeft];
          arrayOut [iScanLeft] = iTemp;
          }
        else
          {
          break;
          };
        }; // while scan left < scan right

        
      INT32  iTemp = arrayOut [iRight];
      arrayOut [iRight] = arrayOut [iScanLeft];
      arrayOut [iScanLeft] = iTemp;

      iIndex = iScanLeft;
      // partition done
      
      if ((iIndex - iLeft) > (iRight - iIndex))
        {
        arrayStack.Append (iLeft);
        arrayStack.Append (iIndex - 1);
        iLeft = iIndex + 1;
        }
      else
        {
        arrayStack.Append (iIndex + 1);
        arrayStack.Append (iRight);
        iRight = iIndex - 1;
        };
      
      }
    else
      {
      iRight = arrayStack [arrayStack.Length () - 1];
      iLeft  = arrayStack [arrayStack.Length () - 2];
      arrayStack.Remove (arrayStack.Length () - 2, 2);
      };
    } while (arrayStack.Length () > 0);
  };






