/* -----------------------------------------------------------------
                            TEdit Base Array

     This module implements the base class from which all array objects
       should be derived.  This allows us to build arrays that are aware
       of each other and can have operations applied across multiple
       related arrays at once.

                            Michael T. Duffy
   ----------------------------------------------------------------- */

// BaseArray.hpp
// Copyright 2004, 2008, 2013 Michael T. Duffy.  
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

#include "Debug.hpp"
ASSERTFILE (__FILE__);
#include "BaseArray.hpp"

//-----------------------------------------------------------------------------
//  BaseArray
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
BaseArray::BaseArray ()
  {
  Init ();
  };


//-----------------------------------------------------------------------------
BaseArray::BaseArray (const BaseArray &  arrayIn)
  {
  Init ();
//  SetLength (arrayIn.Length ());
//  CopyValues (arrayIn.pArray, 0, 0, arrayIn.Length ());
  };


//-----------------------------------------------------------------------------
BaseArray::BaseArray (INT  iInitialSize)
  {
  Init ();
//  SetLength (iInitialSize);
//  InitValues (0, iAllocSize);
  };


//-----------------------------------------------------------------------------
BaseArray::~BaseArray ()
  {
//  DeleteArray (&pArray);
  //SetSparseArray (FALSE);
  };


//-----------------------------------------------------------------------------
VOID  BaseArray::Init  (VOID)
  {
  pArray     = NULL;
  iLength    = 0;
  iAllocSize = 0;
  iAllocInc  = 10;
  iCurrIndex = -1;


  pPrev     = this;
  pNext     = this;
  };


//-----------------------------------------------------------------------------
EStatus BaseArray::SetMinLength (INT  iLengthIn)
  {
  // Make sure the lenght of the array is at least iLengthIn
  if (iLengthIn > Length ())
    {
    return (SetLength (iLengthIn));
    };
  return (EStatus::kSuccess);
  };


//-----------------------------------------------------------------------------
EStatus BaseArray::SetLength (INT  iLengthIn)
  {
  INT    iNewAllocSize = iAllocSize;

  // calculate how much to grow the array.
  while (iLengthIn > iNewAllocSize)
    {
    iNewAllocSize += iAllocInc;
    };

  // if more memory is to be allocated, allocate it, copy over old values, and delete old memory.
  if (iNewAllocSize > iAllocSize)
    {
    VOID *  pNewArray = AllocArray (iNewAllocSize);

    // Note:  Ideally you need to detect an allocation failure here and return a failure code.
    if (pNewArray == NULL)  return (EStatus::kFailure);

    INT     iOldAllocSize = iAllocSize;
    VOID *  pOldArray     = pArray;

    pArray = pNewArray;
    iAllocSize = iNewAllocSize;

    if (iOldAllocSize > 0)
      {
      CopyValues (pOldArray, 0, 0, iOldAllocSize);
      DeleteArray (&pOldArray);
      };
    };
  iLength = iLengthIn;
  return (EStatus::kSuccess);
  };


//-----------------------------------------------------------------------------
EStatus BaseArray::Clear ()
  {
  if (this == NULL) return (EStatus::kFailure);
  iLength = 0;
  return (EStatus::kSuccess);
  };


//-----------------------------------------------------------------------------
EStatus BaseArray::Remove (INT  iStartIndex,
                           INT  iNumToRemove)
  {
  if (iLength == 0) return (EStatus::kFailure);
  
  RemoveSequentialValues (iStartIndex, iNumToRemove);

  // update siblings
  BaseArray *  pCurr = pNext;
  while (pCurr != this)
    {
    pCurr->Remove (iStartIndex, iNumToRemove);
    pCurr = pCurr->pNext;
    };

  return (EStatus::kSuccess);
  };


//-----------------------------------------------------------------------------
EStatus  BaseArray::RemoveSequentialValues (INT  iStartIndex,
                                            INT  iNumToRemove)
  {
  // perform the removal on the actual sequential data
  if ((iStartIndex + iNumToRemove) > iLength) return (EStatus::kFailure);

  if ((iStartIndex + iNumToRemove) != iLength)
    {
    CopyValues (pArray, iStartIndex + iNumToRemove, iStartIndex, iLength - iStartIndex - iNumToRemove);
    };
  iLength -= iNumToRemove;
  return (EStatus::kSuccess);
  };


//-----------------------------------------------------------------------------
EStatus BaseArray::Insert (INT  iStartIndex,
                           INT  iNumToInsert,
                           BOOL bDebug)
  {
  // This routine inserts new entries before the iStartIndex.
  INT  iStartIndexActual = iStartIndex;

  // perform the insertion on the actual sequential data
  INT  iOldLength = iLength;

  if (iStartIndexActual > iLength) return (EStatus::kFailure);

  if (SetLength (iOldLength + iNumToInsert) == EStatus::kFailure) {return EStatus::kFailure;};
  if (Length () < (iOldLength + iNumToInsert) )  {return EStatus::kFailure;};

  if (bDebug)
    {
    DBG_INFO ("BA:Ins %d old:%d cur:%d\n", iStartIndexActual, iOldLength, Length ());
    };
  //printf ("start index %d old length %d\n", iStartIndexActual,iOldLength);
  if ((iStartIndexActual != iLength) && (iOldLength - iStartIndexActual > 0))
    {
    //DBG_INFO ("Copy values rev %d %d %d\n", iStartIndexActual, iStartIndexActual + iNumToInsert, iOldLength - iStartIndexActual);
    CopyValuesRev (pArray, iStartIndexActual, iStartIndexActual + iNumToInsert, iOldLength - iStartIndexActual);
    };



  // update siblings
  BaseArray *  pCurr = pNext;
  while (pCurr != this)
    {
    pCurr->Insert (iStartIndex, iNumToInsert);
    pCurr = pCurr->pNext;
    };
    
  return (EStatus::kSuccess);
  };


//-----------------------------------------------------------------------------
VOID BaseArray::SetSizeIncrement (INT iNewIncrement)
  {
  iAllocInc = iNewIncrement;
  if (iAllocInc <= 0)  iAllocInc = 1;
  };

  
//-----------------------------------------------------------------------------
INT BaseArray::SizeIncrement () const
  {
  return (iAllocInc);
  };

  
//-----------------------------------------------------------------------------
VOID  BaseArray::AddAsSibling  (BaseArray *  parrayIn)
  {
  // first, remove ourself from any existing lists.
  RemoveFromSiblings ();

  // add ourselves to the linked list
  pNext           = parrayIn;
  pPrev           = parrayIn->pPrev;
  parrayIn->pPrev = this;
  pPrev->pNext    = this;
  };


//-----------------------------------------------------------------------------
VOID  BaseArray::RemoveFromSiblings  (VOID)
  {
  if (pPrev != this)
    {
    pPrev->pNext = pNext;
    pNext->pPrev = pPrev;

    pNext = this;
    pPrev = this;
    };
  };

//-----------------------------------------------------------------------------
VOID  BaseArray::RemoveIndexes (INT  iStartIndex,
                                 INT  iNumToRemove)
  {
  // indexes are always stored as INT32
  INT32  iEndIndex = iStartIndex + iNumToRemove;

  // Remove any matches, and collapse any followers.

  for (INT32  iIndex = 0; iIndex < iLength; ++iIndex)
    {
    if (((INT32 *) pArray) [iIndex] >= iStartIndex)
      {
      // found an index that is changing
      if (((INT32 *) pArray) [iIndex] <  iEndIndex)
        {
        // found an index that is going away
        Remove (iIndex, 1);

        // since this index is gone now, we want to check it again next
        //  time through the loop since it now contains a new value that
        //  was collapsed down into it.
        --iIndex;
        }
      else
        {
        // decrement this index

        ((INT32 *) pArray) [iIndex] -= iNumToRemove;
        };
      };
    };
  };


//-----------------------------------------------------------------------------
VOID  BaseArray::InsertIndexes (INT  iStartIndex,
                                 INT  iNumToInsert)
  {
  // indexes are always stored as INT32
  for (INT32  iIndex = 0; iIndex < iLength; ++iIndex)
    {
    if (((INT32 *) pArray) [iIndex] >= iStartIndex)
      {
      // found an index that is changing

      ((INT32 *) pArray) [iIndex] += iNumToInsert;
      };
    };
  };
