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

#ifndef BASEARRAY_HPP
#define BASEARRAY_HPP

#include "Types.hpp"

/**

  This is the parent class for objects which represent arrays of data.  It
  requires the child class to implement only the data-specific routines,
  and then the members of this class handle all the functionality that is
  common across all arrays.
  
  In addition, this class implements the concept of "linked arrays".  This 
  is so you can have multiple arrays that store data in parallel to each
  other.  When a size change or element insertion/deletion is performed, 
  all of the linked arrays will likewise have elements removed or inserted
  so that they remain synchronized.  This is useful, for example, if you 
  have an array of vertices and an array of vertex colors.  If you delete
  the vertexes at indexes 7-10, likewise the vertex color indexes 7-10
  will be deleted if the vertex color array is linked to the vertex array.
  This approach allows flexibility in the number of linked elements that
  might be present, without having to pre-define and pre-allocate space for 
  every possible type of data as you would have to do if you had an array
  of structs or classes.

  When implementing a child class, the following member routines need
  to be defined:

    AllocArray
    DeleteArray
    InitValues
    CopyValues
    CopyValuesRev
    AverageValues

  The following member routines should also be defined in the child class,
  though they are not inherited classes.  This will keep all children 
  of the BaseArray class consistent.  The parameters and return values
  are data type specific, which is why they are not implemented as 
  virtual functions.  The below example is for a child class named
  RFloatArray which implements an array of data type FLOAT>

    EStatus        Copy             (const RFloatArray &  arraySource);
    EStatus        Set              (FLOAT  fElement, INT  iIndex);
    EStatus        Append           (FLOAT  fElement);
    EStatus        GetArray         (FLOAT  []) const;

    FLOAT          operator[]       (INT  iIndex) const;
    FLOAT &        operator[]       (INT  iIndex);
    RFloatArray &  operator=        (const  RFloatArray & arrayIn);

  */

// REFACTOR: Strip this class down further. Probably do not need linked array functionality

//-----------------------------------------------------------------------------
class BaseArray
  {
  protected:
    VOID *        pArray;      ///< Pointer to the actual data
    INT           iLength;     ///< Number of elements in the array.
    INT           iAllocSize;  ///< Number of elements space has been allocated for.
    INT           iAllocInc;   ///< Number of new elements added when reallocation is needed.
    INT           iCurrIndex;  ///< Index into the array.  Allows the array to track a position.


    BaseArray *  pPrev;       ///< Pointer to previous sibling for linked arrays.
    BaseArray *  pNext;       ///< Pointer to next sibling for linked arrays.

  public:


    // these need to be overridden in the child class
                                     /** @brief  Constructor
                                         @return None
                                     */
                    BaseArray       ();
                                     /** @brief  Copy constructor
                                         @param  arrayIn The source object to copy from.
                                         @return None
                                     */
                    BaseArray       (const BaseArray &  arrayIn);
                                     /** @brief  Constructor
                                         @param  iInitialSize The number of elements to allocate upon initialization.
                                         @return None
                                     */
                    BaseArray       (INT   iInitialSize);
                                     /** @brief  Destructor
                                         @return None
                                     */
    virtual         ~BaseArray      ();


                                     /** @brief  Return a pointer to the raw data array.  Used to pass array on to other APIs.  Be very careful when using this function.
                                         @return Pointer to the raw buffer of memory that stores the array data.
                                     */
    VOID *          GetRawBuffer     (VOID)           {return pArray;};
    
                                     /** @brief  Test an index against the array boundaries.
                                         @return True if the given index is within the array bounds, and False if not.
                                     */
    BOOL            BoundsCheck      (INT  iIndexIn) const  {return ((iIndexIn < iLength) && (iIndexIn >= 0));};
    

                                     /** @brief  Allocate an array of data elements.  This routine needs to be specific to the data type.
                                         @param  iSizeIn The number of data elements to allocate
                                         @return Pointer to the allocated data
                                     */
    virtual VOID *  AllocArray       (INT      iSizeIn)      = 0;
                                     /** @brief  Delete the array of data elements.  This routine needs to be specific to the data type.
                                         @param  pvArrayIn Pointer to the array to deallocate.
                                         @return None
                                     */
    virtual VOID    DeleteArray      (PVOID *  pvArrayIn)    = 0;
                                     /** @brief  Copies a number of data elements from a source buffer to the target elements.  This routine needs to be specific to the data type.
                                         @param  pvSourceData Pointer to a memory buffer containing an array of the data-type, sequential in memory.
                                         @param  iSourceOffsetIn The index into pvSourceData where the copy operation will start reading.  Given in units of number of data elements to skip.
                                         @param  iStartOffset The index into the array of data elements within this class where the writing operation will begin.
                                         @param  iNumToCopy The number of data elements to copy from the source data.
                                         @return None
                                     */
    virtual VOID    CopyValues       (PVOID    pvSourceData,
                                      INT      iSourceOffsetIn,
                                      INT      iStartOffset,
                                      INT      iNumToCopy)   = 0;
                                     /** @brief  Copies a number of data elements from a source buffer to the target elements, starting at the last data element and working towards the first.  Used for overlapping copy operations.  This routine needs to be specific to the data type.
                                         @param  pvSourceData Pointer to a memory buffer containing an array of the data-type, sequential in memory.
                                         @param  iSourceOffsetIn The index into pvSourceData where the copy operation will start reading.  Given in units of number of data elements to skip.
                                         @param  iStartOffset The index into the array of data elements within this class where the writing operation will begin.
                                         @param  iNumToCopy The number of data elements to copy from the source data.
                                         @return None
                                     */
    virtual VOID    CopyValuesRev    (PVOID    pvSourceData,
                                      INT      iSourceOffsetIn,
                                      INT      iStartOffset,
                                      INT      iNumToCopy)   = 0;
                                     /** @brief  Calculates the average value of two indexed data elements, and stores them in a third data element.  This routine needs to be specific to the data type.
                                         @param  iIndexAverage The data element index to receive the interpolated value.
                                         @param  iIndexValueOne The data element index to use for the first value in the interpolation.
                                         @param  iIndexValueTwo The data element index to use for the second value in the interpolation.
                                         @param  fBias The LERP position between value one and value two where the interpolated value will be calculated.  Defaults to 0.0f
                                         @return None
                                     */
    virtual VOID    AverageValues    (INT      iIndexAverage,
                                      INT      iIndexValueOne,
                                      INT      iIndexValueTwo,
                                      FLOAT    fBias = 0.5f)  = 0;

    virtual VOID    SwapIndexes         (INT  iIndexOne,
                                         INT  iIndexTwo) = 0; 

    virtual INT     CompareIndexes      (INT  iIndexOne,
                                         INT  iIndexTwo) = 0; 


    // These routines need to be defined in the child class (Examples given for RFloatArray)
    //  They are not virtual functions because they are type specific.

    //EStatus        Copy             (const RFloatArray &  arraySource);
    //EStatus        Set              (FLOAT  fElement, INT  iIndex);
    //EStatus        Append           (FLOAT  fElement);
    //EStatus        GetArray         (FLOAT  []) const;

    //FLOAT          operator[]       (INT  iIndex) const;
    //FLOAT &        operator[]       (INT  iIndex);
    //RFloatArray &  operator=        (const  RFloatArray & arrayIn);


    // routines handled by the base class
    virtual VOID    Init                    (VOID);
    EStatus         SetLength               (INT  iLengthIn);
    EStatus         SetMinLength            (INT  iLengthIn);
    INT             Length                  (VOID) const                           {return iLength;};
    INT             AllocSize               (VOID) const                           {return iAllocSize;};
    EStatus         Clear                   (VOID);
    EStatus         Remove                  (INT  iStartIndex,
                                             INT  iNumToRemove = 1);
    EStatus         RemoveSequentialValues  (INT  iStartIndex,  // actual index
                                             INT  iNumToRemove = 1);
    virtual EStatus Insert                  (INT  iStartIndex,
                                             INT  iNumToInsert = 1,
                                             BOOL bDebug = 0);
    VOID            SetSizeIncrement        (INT  iNewIncrement);
    INT             SizeIncrement           (VOID) const;
    INT32           MapLogicalIndex         (INT32  iLogicalIndexIn) const;

    // link related classes

    VOID            AddAsSibling        (BaseArray *  parrayIn);
    VOID            RemoveFromSiblings  (VOID);

    VOID            SetIndex            (BaseArray *  pIndexIn);

    VOID            RemoveIndexes       (INT  iStartIndex,
                                         INT  iNumToRemove);
    VOID            InsertIndexes       (INT  iStartIndex,
                                         INT  iNumToInsert);

    INT             GetCurrIndex        (VOID)                {return iCurrIndex;};
    VOID            SetCurrIndex        (INT  iValueIn)       {iCurrIndex = iValueIn;};
  };

#endif // BASEARRAY_HPP

