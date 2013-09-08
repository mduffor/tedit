//
// GapBuffer.cpp
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

#include <string.h>
#include "Types.hpp"
#include "Debug.hpp"
ASSERTFILE (__FILE__)

#include "FilePath.hpp"

static const INT DEFAULT_BUFFER_GROW_SIZE = 512;
static const INT LINE_FEED_CHAR = '\n';
static const INT NULL_CHAR = '\0';

#include "GapBuffer.hpp"

//-----------------------------------------------------------------------------
BOOL operator== (const Location & locOne, 
                 const Location & locTwo)
  {
  return ((locOne.iCol == locTwo.iCol) && (locOne.iLine == locTwo.iLine));
  }


//-----------------------------------------------------------------------------
GapBuffer::GapBuffer ()
  {
    pNext       = NULL;
    pPrev       = NULL;
    strName     = "";
    strFileName = "";
    
    pBuffer      = NULL;
    iBufferSize  = 0;
    iGapStart    = 0;
    iGapSize     = 0;
    bCursorMoved = TRUE;
    
    SetGrowSize (DEFAULT_BUFFER_GROW_SIZE);
    
    locCursor.Set (0,0);
    locSelect.Set (0,0);
    
    CalcLineOffsets ();
    bModified = FALSE;
  };
  
//-----------------------------------------------------------------------------
GapBuffer::~GapBuffer ()
  {
  FreeBuffer ();
  };

//-----------------------------------------------------------------------------
VOID  GapBuffer::FillGap ()
  {
  // used for debugging.  Fills gap characters with the last digit of their index.
  if ((iBufferSize > 0) && (iGapSize > 0)) 
    {
    for (INT iIndex = iGapStart; iIndex < iGapStart + iGapSize; ++iIndex)
      {
      pBuffer[iIndex] = '0' + (iIndex % 10);
      }
    }
  }
  
//-----------------------------------------------------------------------------
VOID  GapBuffer::Clear ()
  {
  if (pBuffer != NULL) 
    {
    if (iBufferSize > 0)
      {
      pBuffer [iBufferSize - 1] = NULL_CHAR;
      };
    iGapStart   = 0;
    iGapSize    = iBufferSize - 1;
    SetCursor (1, 0);
    CalcLineOffsets ();
    };
  };

//-----------------------------------------------------------------------------
VOID GapBuffer::FreeBuffer ()
  {
  if (pBuffer != NULL)
    {
    delete [] pBuffer;
    pBuffer = NULL;
    iBufferSize = 0;
    }
  };

//-----------------------------------------------------------------------------
VOID GapBuffer::AllocBuffer (INT  iSizeIn)
  {
  FreeBuffer ();
  pBuffer = new char [iSizeIn];
  iBufferSize = iSizeIn;
  Clear ();
  CalcLineOffsets ();
  };
  
//-----------------------------------------------------------------------------
VOID GapBuffer::Resize (INT  iIncrementSizeIn)
  {
  // allocate new buffer and copy over memory
  char * pNewBuffer = new char [iBufferSize + iIncrementSizeIn];
  if (iGapStart > 0)
    {
    memcpy (pNewBuffer, pBuffer, iGapStart);
    };
  if (iGapStart + iGapSize < iBufferSize)
    {
    memcpy (pNewBuffer + iGapStart + iGapSize + iIncrementSizeIn, 
            pBuffer + iGapStart + iGapSize, 
            iBufferSize - iGapSize - iGapStart);
    };
  
  // Update line offsets after the gap
  for (INT  iIndex = 0; iIndex < aiLineOffsets.Length (); ++iIndex)
    {
    if (aiLineOffsets[iIndex] > iGapStart) 
      {
      for (; iIndex < aiLineOffsets.Length (); ++iIndex)
        {
        aiLineOffsets[iIndex] += iIncrementSizeIn;
        }
      break;
      }
    }
    
  // replace old buffer with new 
  delete [] pBuffer;
  pBuffer = pNewBuffer;
  iBufferSize += iIncrementSizeIn;
  iGapSize += iIncrementSizeIn;
  };
  
//-----------------------------------------------------------------------------
EStatus  GapBuffer::SetCursor (Location &  locIn)
  {
  locCursor = locIn;  
  ASSERT (locIn.iLine >= 1);
  ASSERT (locIn.iCol >= 0);
  bCursorMoved = TRUE;
  return (EStatus::kSuccess);
  };
  
//-----------------------------------------------------------------------------
EStatus  GapBuffer::SetCursor (INT  iLineIn, 
                               INT  iColIn)
  {
  ASSERT (iLineIn >= 1);
  ASSERT (iColIn >= 0);
  locCursor.Set (iLineIn, iColIn);  
  bCursorMoved = TRUE;
  return (EStatus::kSuccess);
  };
  
//-----------------------------------------------------------------------------
EStatus  GapBuffer::MoveCursor (INT iCount)
  {
  // Note:  Implement this after you figure out how it will be used, and
  //  how virtual spaces factor in.
  // TODO
  bCursorMoved = TRUE;
  return (EStatus::kSuccess);
  };
  
//-----------------------------------------------------------------------------
VOID  GapBuffer::GetString (char * szStringOut, INT iCount)
  {
  INT  iCharsPastGap = iBufferSize - iGapStart - iGapSize;
  INT  iCharsToCopy = TMin (iCharsPastGap, iCount);
  
  memcpy (szStringOut, pBuffer + iGapStart + iGapSize, iCharsToCopy);
  };
  
//-----------------------------------------------------------------------------
INT  GapBuffer::GetNumChars (VOID)
  {
  return (iBufferSize - iGapSize);
  };
  
//-----------------------------------------------------------------------------
INT  GapBuffer::GetNumLines (VOID)
  {
  return (aiLineOffsets.Length () - 1);
  };
  
//-----------------------------------------------------------------------------
BOOL  GapBuffer::IsFileChanged (VOID)  // compare timestamp and size on disk
  {
  // TODO
  return (FALSE);
  };
  
//-----------------------------------------------------------------------------
VOID  GapBuffer::BeginEdit (VOID)
  {
  INT  iLine = locCursor.iLine; // local variable is 1-based
  INT  iCol  = locCursor.iCol;

  bCursorMoved = FALSE;
  INT  iNumLines = GetNumLines ();
  if (iLine > iNumLines)
    {
    //DBG_INFO ("Cursor is past end of buffer\n");
    // cursor is past end of buffer.  Move gap to the end, leaving trailing null character alone.
    INT  iSecondLength = iBufferSize - iGapSize - iGapStart - 1; 
    if (iSecondLength > 0)
      {
      memmove (pBuffer + iGapStart, pBuffer + iGapStart + iGapSize, iSecondLength);
      iGapStart += iSecondLength;
      }
    // set the cursor to the end of the buffer so it can be updated properly by the InsertChar calls.
    locCursor.iLine = GetNumLines ();
    locCursor.iCol = GetLineLength (locCursor.iLine);
    
    // add lines until we reach our desired line
    while (iNumLines < iLine)
      {
      InsertChar (LINE_FEED_CHAR);
      ++iNumLines;
      }
    // add spaces until we reach our desired column
    while (iCol)
      {
      InsertChar (' ');
      --iCol;
      }
    }
  else
    {
    // cursor is on an existing line
    INT  iLineStartOffset = aiLineOffsets[iLine - 1];
    INT  iOffset = iLineStartOffset;
    INT  iCharsPastEnd = 0;
    
    // find the column
    INT  iLineLength = GetLineLength (iLine);
    if (iCol > iLineLength) 
      {
      // cursor is past the end of the line, on a virual space.  
      iCharsPastEnd = iCol - iLineLength;
      iCol = iLineLength;
      }
    iOffset += iCol;
    if ((iLineStartOffset <= iGapStart) && (iOffset > iGapStart))
      {
      iOffset += iGapSize;
      }
    // move the gap
    INT  iPrevGapStart = iGapStart;
    if (iOffset < iGapStart)
      {
      INT  iCharsToMove = iGapStart - iOffset;
      memmove (pBuffer + iGapStart + iGapSize - iCharsToMove, pBuffer + iOffset, iCharsToMove);
      iGapStart = iOffset;
      }
    else if (iOffset > iGapStart)
      {
      // oooo....OOOO         
      //     g     o
      INT  iCharsToMove = iOffset - iGapStart - iGapSize;
      memmove (pBuffer + iGapStart, pBuffer + iOffset - iCharsToMove, iCharsToMove);
      iGapStart += iCharsToMove;
      };
    ShiftLineOffsets (iPrevGapStart, iGapStart);
    // add any characters past end of current line as needed
    while (iCharsPastEnd)
      {
      InsertChar (' ');
      --iCharsPastEnd;
      }
    };
  };

  
//-----------------------------------------------------------------------------
INT  GapBuffer::GetLineLength  (INT  iLine)
  {
  if ((iLine >= aiLineOffsets.Length()) || (iLine <= 0))
    {
    return 0;
    }
  INT  iLength = aiLineOffsets[iLine] - aiLineOffsets[iLine - 1];
  if ((aiLineOffsets[iLine - 1] <= iGapStart) && (aiLineOffsets[iLine] > iGapStart))
    {
    iLength -= iGapSize;
    }
  if ((pBuffer[aiLineOffsets[iLine] - 1] == LINE_FEED_CHAR) ||
      (pBuffer[aiLineOffsets[iLine] - 1] == NULL_CHAR))
    {
    --iLength;
    }
  return iLength;
  }
  
//-----------------------------------------------------------------------------
VOID  GapBuffer::InsertChar  (INT iCharIn)
  {
  if (bCursorMoved)
    {
    BeginEdit ();
    }
  if (iGapSize == 0) 
    {
    Resize (iGrowSize);
    };
  pBuffer[iGapStart] = iCharIn;
  iGapStart += 1;
  iGapSize -= 1;
  locCursor.iCol += 1;
  if (iCharIn == LINE_FEED_CHAR)
    {
    locCursor.iLine += 1;
    locCursor.iCol = 0;
    for (INT iIndex = 0; iIndex < aiLineOffsets.Length(); ++iIndex)
      {
      if (iGapStart < aiLineOffsets[iIndex])
        {
        aiLineOffsets.Insert (iIndex, 1);
        aiLineOffsets[iIndex] = iGapStart;
        break;
        }
      }
    }
  };
  
//-----------------------------------------------------------------------------
VOID  GapBuffer::InsertString  (const char *  szStringIn)
  {
  if (bCursorMoved)
    {
    BeginEdit ();
    }
  INT  iStrLen = strlen (szStringIn);
  
  //DBG_INFO ("gap size %d str len %d\n", iGapSize, iStrLen);
  if (iGapSize <= iStrLen) 
    {
    Resize (iGrowSize);
    };
  memcpy (pBuffer + iGapStart, szStringIn, iStrLen);
  iGapStart += iStrLen;
  iGapSize -= iStrLen;
  locCursor.iCol += iStrLen;
  INT iIndex = 0; 
  for (INT  iSearch = iGapStart - iStrLen; iSearch < iGapStart; ++iSearch)
    {
    if (pBuffer[iSearch] == LINE_FEED_CHAR)
      {
      for (; iIndex < aiLineOffsets.Length(); ++iIndex)
        {
        if (iSearch < aiLineOffsets[iIndex])
          {
          aiLineOffsets.Insert (iIndex, 1);
          aiLineOffsets[iIndex] = iSearch + 1;
          ++iIndex;
          break;
          }
        }
      locCursor.iLine += 1;
      locCursor.iCol = iGapStart - iSearch;
      }
    }
  };
  
//-----------------------------------------------------------------------------
VOID  GapBuffer::ReplaceChar  (INT iCharIn)
  {
  // can be optimized
  InsertChar (iCharIn);
  if (GetChar () != LINE_FEED_CHAR)
    {
    DeleteChars (1);
    };
  };
  
//-----------------------------------------------------------------------------
VOID  GapBuffer::ReplaceString (const char *  szStringIn)
  {
  // can be optimized
  InsertString(szStringIn);
  INT  iStrLen = strlen(szStringIn);
  while (iStrLen)
    {
    if (GetChar() == LINE_FEED_CHAR)
      {
      break;
      }
    DeleteChars (1);
    --iStrLen;
    }
  };
  
//-----------------------------------------------------------------------------
VOID  GapBuffer::DeleteChars   (INT iCountIn)
  {
  if (bCursorMoved)
    {
    BeginEdit ();
    }
  if (iCountIn < 0)
    {
    iCountIn = -iCountIn;
    iCountIn = TMin (iCountIn, iGapStart);
    iGapStart -= iCountIn;
    iGapSize += iCountIn;
    }
  else if (iCountIn > 0)
    {
    iCountIn = TMin (iCountIn, iBufferSize - iGapStart - iGapSize - 1);
    iGapSize += iCountIn;
    }
  };
  
//-----------------------------------------------------------------------------
VOID  GapBuffer::DeleteRegion (const char *  szMarkName)
  {
  // TODO
  };
  
//-----------------------------------------------------------------------------
VOID  GapBuffer::CopyRegion (const char *  szBufferName, const char *  szMarkName)
  {
  // TODO
  };
  
//-----------------------------------------------------------------------------
VOID GapBuffer::CalcLineOffsets (VOID)
  {
  /** Fill the aiLineOffsets array with the offset into the buffer for the
     start of each line.  The index into aiLineOffsets is 0-based, so you
     will need to subtract 1 from the cursor line to get an index.  
     The first entry, line index 0, starts at offset 0.  The last index is
     set to the size of the buffer.  This way you can use arithmatic to 
     figure out the length of each line, as long as you take the gap into account.
  **/
  
  aiLineOffsets.Clear ();
    
  // count the number of newlines
  INT  iNumLines = 2;
  INT  iIndex = 0;
  for (iIndex = 0; iIndex < iGapStart; ++iIndex)
    {
    if (pBuffer[iIndex] == LINE_FEED_CHAR) ++iNumLines;
    }
  
  for (iIndex = iGapStart + iGapSize; iIndex < iGapSize; ++iIndex)
    {
    if (pBuffer[iIndex] == LINE_FEED_CHAR) ++iNumLines;
    }
  //DBG_INFO ("Setting line offset length to %d", iNumLines);
  aiLineOffsets.SetLength(iNumLines);

  // now go through again and save the offsets.

  INT iLine = 0;
  aiLineOffsets[iLine] = 0;
  ++iLine;
  for (iIndex = 0; iIndex < iGapStart; ++iIndex)
    {
    if (pBuffer[iIndex] == LINE_FEED_CHAR) 
      {
      aiLineOffsets[iLine] = iIndex + 1;
      ++iLine;
      }
    }
  for (iIndex = iGapStart + iGapSize; iIndex < iBufferSize; ++iIndex)
    {
    if (pBuffer[iIndex] == NULL_CHAR) 
      {
      break;
      }
    if (pBuffer[iIndex] == LINE_FEED_CHAR) 
      {
      aiLineOffsets[iLine] = iIndex + 1;
      ++iLine;
      }
    }
  aiLineOffsets[iLine] = iBufferSize;
  };

//-----------------------------------------------------------------------------
VOID GapBuffer::ShiftLineOffsets (INT  iPrevGapStart,
                                  INT  iNewGapStart)
  {
  INT  iFirstGap;
  INT  iLastGap;
  INT  iDelta;

  if (iPrevGapStart > iNewGapStart)
    {
    // moving the gap left
    iFirstGap = iNewGapStart;
    iLastGap = iPrevGapStart;
    iDelta = iGapSize;
    } 
  else
    {
    // moving the gap right
    // PN
    iFirstGap = iPrevGapStart + iGapSize;
    iLastGap = iNewGapStart + iGapSize;
    iDelta = -iGapSize;
    }

  // abCd4567eFghZ  0 3 10 13

  // prev = 4 new = 3
  // abC3456deFghZ  0 7 10 13
    
  // prev = 4 new = 8
  // abCdeFgh89012Z  0 3 10 13
    
  // prev = 4 new = 7
  // abCdeFg78901hZ  0 3 10 13
    
  
  
  for (INT  iIndex = 0; iIndex < aiLineOffsets.Length(); ++iIndex)
    {
    if (aiLineOffsets [iIndex] >= iFirstGap)
      {
      for (; iIndex < aiLineOffsets.Length(); ++iIndex)
        {
        if (aiLineOffsets [iIndex] >= iLastGap) break;
        aiLineOffsets [iIndex] += iDelta;
        }
      break;  
      }
    }
  }
  
  
//-----------------------------------------------------------------------------
VOID GapBuffer::DebugPrintLineOffsets (VOID)
  {
  printf ("GapBuffer %s : LineOffsets\n", GetName());
  for (INT iIndex = 0; iIndex < aiLineOffsets.Length (); ++iIndex)
    {
    printf ("  %6d\n", (INT)aiLineOffsets[iIndex]);
    }
  };
  
//-----------------------------------------------------------------------------
EStatus  GapBuffer::Load  (VOID)
  {
  INT    iFileSize    = FilePath::GetFileSize (GetFileName ());

  AllocBuffer (iFileSize + iGrowSize);
  
  EStatus  eStatus = FilePath::ReadFromFile  (GetFileName (),
                                              0,
                                              iFileSize,
                                              (unsigned char *) pBuffer);
  SetCursor (1, 0);
  iGapStart = iFileSize;
  iGapSize -= iFileSize;
  CalcLineOffsets ();
  return (eStatus);
  };
  
//-----------------------------------------------------------------------------
EStatus  GapBuffer::Save  (VOID)
  {
  EStatus eStatus = EStatus::kSuccess;
  INT  iFirstLength = iGapStart;
  INT  iSecondLength = iBufferSize - iGapSize - iGapStart - 1; 
  
  if (iFirstLength > 0)
    {
    eStatus = FilePath::WriteToFile (GetFileName (),
                                     FALSE,
                                     iFirstLength,
                                     (unsigned char *) pBuffer);
    };
  if ((iSecondLength > 0) && (eStatus.Success ()))
    {
    eStatus = FilePath::WriteToFile (GetFileName (),
                                     TRUE,
                                     iSecondLength,
                                     (unsigned char *) &pBuffer[iGapStart + iGapSize]);
    };
  return (eStatus);
  };

//-----------------------------------------------------------------------------
EStatus  GapBuffer::LoadInsert  (VOID)
  {
  INT    iFileSize    = FilePath::GetFileSize (GetFileName ());
  
  if (bCursorMoved)
    {
    BeginEdit ();
    }
  Resize (iFileSize);
  
  EStatus  eStatus = FilePath::ReadFromFile  (GetFileName (),
                                              0,
                                              iFileSize,
                                              (unsigned char *) &pBuffer[iGapStart]);
  iGapStart += iFileSize;
  iGapSize -= iFileSize;
  CalcLineOffsets ();
  return (eStatus);
  };
  
 