//
// GapBuffer.hpp
// Copyright 2013  Michael T. Duffy.  
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
//

#ifndef GAPBUFFER_HPP
#define GAPBUFFER_HPP

#include "Types.hpp"
#include "IntArray.hpp"
#include "RStr.hpp"

//=============================================================================
class Location 
{
  public:
    INT iLine;  // one based
    INT iCol;   // zero based
    
          Location  ()                           {iLine = 1; iCol = 0;};
          Location  (Location & locIn)           {iLine = locIn.iLine; iCol = locIn.iCol;};
          Location  (INT  iLineIn, INT  iColIn)  {iLine = iLineIn; iCol = iColIn;};
    VOID  Set       (INT  iLineIn, INT  iColIn)  {iLine = iLineIn; iCol = iColIn;};
};

BOOL operator== (const Location & locOne, const Location & locTwo);
BOOL operator!= (const Location & locOne, const Location & locTwo);
BOOL operator<  (const Location & locOne, const Location & locTwo);
BOOL operator>  (const Location & locOne, const Location & locTwo);
BOOL operator<= (const Location & locOne, const Location & locTwo);
BOOL operator>= (const Location & locOne, const Location & locTwo);

//=============================================================================
class Mark
{
  public:
    RStr      strName;
    Location  loc;
};


//=============================================================================
class GapBuffer
{
  public:
    GapBuffer *  pNext;
    GapBuffer *  pPrev;
    
  private:
    RStr         strName;
    RStr         strFileName;
    
    char *       pBuffer;
    INT          iBufferSize;
    INT          iGapStart;
    INT          iGapSize;
    INT          iGrowSize; /// Size by which the buffer grows when it runs out of gap space.
    
    Location     locCursor;
    Location     locSelect;
    
    BOOL         bModified;
    BOOL         bCursorMoved;
    
    IntArray     aiLineOffsets;
    
  public:
    
                  GapBuffer   ();
                  ~GapBuffer  ();
    VOID          FillGap     ();
    VOID          Clear       ();
    VOID          FreeBuffer  ();
    VOID          AllocBuffer (INT  iSizeIn);
    VOID          SetGrowSize (INT  iIncrementSizeIn)  {iGrowSize = iIncrementSizeIn;};
    VOID          Resize      (INT  iIncrementSizeIn);
    
    VOID          SetName     (const char * szNameIn)  {strName = szNameIn;};
    const char *  GetName     (VOID)                   {return (strName.AsChar ());};

    VOID          SetFileName (const char *  szFileNameIn)  {strFileName = szFileNameIn;};
    const char *  GetFileName (VOID)                        {return (strFileName.AsChar ());};

    const char *  GetBuffer   (VOID)  {return pBuffer;};
    
    EStatus       SetCursor     (Location & locIn);
    EStatus       SetCursor     (INT  iLineIn, 
                                 INT  iColIn);
    EStatus       MoveCursor    (INT iCount);
    Location &    GetCursor     (VOID)                      {return (locCursor);};;
    INT           GetCursorLine (VOID)                      {return (locCursor.iLine);};

    VOID          SetSelection     (Location &  locIn)      {SetSelection (locIn.iLine, locIn.iCol);};
    VOID          SetSelection     (INT  iLine, 
                                    INT  iCol)              {locSelect.Set (iLine, iCol);};
    Location &    GetSelection     (VOID)                   {return (locSelect);};;
    BOOL          IsSelectionValid (VOID)                   {return (locSelect.iLine != 0 || locSelect.iCol != 0);};
    VOID          ClearSelection   (VOID)                   {locSelect.Set (0,0);};

    INT           GetChar           (VOID)                  {return (pBuffer[iGapStart + iGapSize]);};
    VOID          GetString         (char * szStringOut, 
                                     INT    iCount);
    INT           GetCharAtLocation (Location & locIn);
    INT           GetCharAtLocation (INT  iLine, 
                                     INT  iCol);
    INT           GetNumChars       (VOID);
    INT           GetNumLines       (VOID);
    INT           GetCharsBetween   (Location &  locOne,
                                     Location &  locTwo);
                                     
    VOID          ClampLocationToValidChar (Location &  locInOut);
    
    BOOL          IsFileChanged (VOID);  // compare timestamp and size on disk
    VOID          SetIsModified (BOOL bModifiedIn)   {bModified = bModifiedIn;};
    BOOL          GetIsModified (VOID)               {return (bModified);};
    
                                /** @brief Prepares the gap buffer for insertion, deletion, and repacing by moving the gap start to the cursor position.
                                *  @return None
                                */
    VOID          BeginEdit     (VOID);
                                /** @brief Returns the length of the current line, not including any terminating newline.
                                *  @param iLine The line whose length will be returned.  Line numbers are 1-based.
                                *  @return Length of the line.  If an invalid line is given, the returned value will be 0.
                                */
    INT           GetLineLength (INT  iLine);
    
                                /** @brief Insert a single character to the left of the point. 
                                *  @param iCharIn The character to insert.
                                *  @return None
                                */
    VOID          InsertChar    (INT iCharIn);
                                /** @brief Insert a string to the left of the point. 
                                *  @param szStringIn The string to insert.
                                *  @return None
                                */
    VOID          InsertString  (const char *  szStringIn);
    VOID          ReplaceChar   (INT iCharIn);
    VOID          ReplaceString (const char *  szStringIn);
                                /** @brief Delete characters from the buffer. 
                                *  @param iCountIn Negative count deletes to the left of the gap start, positive deletes from the right.
                                *  @return None
                                */
    VOID          DeleteChars   (INT iCountIn);
    
    VOID          DeleteRegion  (const char *  szMarkName);
    VOID          CopyRegion    (const char *  szBufferName, const char *  szMarkName);
    VOID          CalcLineOffsets       (VOID);
    VOID          ShiftLineOffsets      (INT  iPrevGapStart,
                                         INT  iNewGapStart);
    VOID          DebugPrintLineOffsets (VOID);
    
    EStatus       Load                  (VOID);
    EStatus       Save                  (VOID);
    EStatus       LoadInsert            (VOID);
    
};

#endif // GAPBUFFER_HPP
