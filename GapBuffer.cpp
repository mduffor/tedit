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
#include "GapBuffer.hpp"

static const INT DEFAULT_BUFFER_GROW_SIZE = 512;
static const INT LINE_FEED_CHAR = '\n';
static const INT NULL_CHAR = '\0';

const Location locInvalid (0,0);



//-----------------------------------------------------------------------------
BOOL operator== (const Location & locOne, 
                 const Location & locTwo)
  {
  return ((locOne.iCol == locTwo.iCol) && (locOne.iLine == locTwo.iLine));
  };

//-----------------------------------------------------------------------------
BOOL operator!= (const Location & locOne, 
                 const Location & locTwo)
  {
  return ((locOne.iCol != locTwo.iCol) || (locOne.iLine != locTwo.iLine));
  };

//-----------------------------------------------------------------------------
BOOL operator< (const Location & locOne, 
                const Location & locTwo)
  {
  return ((locOne.iLine < locTwo.iLine) ||
          ((locOne.iLine == locTwo.iLine) && (locOne.iCol < locTwo.iCol)));
  };

//-----------------------------------------------------------------------------
BOOL operator> (const Location & locOne, 
                const Location & locTwo)
  {
  return ((locOne.iLine > locTwo.iLine) ||
          ((locOne.iLine == locTwo.iLine) && (locOne.iCol > locTwo.iCol)));
  };

//-----------------------------------------------------------------------------
BOOL operator<= (const Location & locOne, 
                 const Location & locTwo)
  {
  if (locOne == locTwo) return TRUE;
  return ((locOne.iLine < locTwo.iLine) ||
          ((locOne.iLine == locTwo.iLine) && (locOne.iCol < locTwo.iCol)));
  };
  
//-----------------------------------------------------------------------------
BOOL operator>= (const Location & locOne, 
                 const Location & locTwo)
  {
  if (locOne == locTwo) return TRUE;
  return ((locOne.iLine > locTwo.iLine) ||
          ((locOne.iLine == locTwo.iLine) && (locOne.iCol > locTwo.iCol)));
  };
  
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
  //printf ("Set Cursor %d %d\n", locIn.iLine, locIn.iCol);
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
Location  GapBuffer::GetSelectionStart (VOID)
  {
  if (!locSelect.IsValid () || !locCursor.IsValid ())
    {
    return (locInvalid);
    }
  if (locCursor < locSelect)
    {
    return (locCursor);
    }
  return (locSelect);
  }

//-----------------------------------------------------------------------------
Location  GapBuffer::GetSelectionEnd   (VOID)
  {
  if (!locSelect.IsValid () || !locCursor.IsValid ())
    {
    return (locInvalid);
    }
  if (locCursor < locSelect)
    {
    return (locSelect);
    }
  Location  locReturn (locCursor.iLine, locCursor.iCol - 1);
  return (locReturn); 
  };
  
//-----------------------------------------------------------------------------
VOID  GapBuffer::GetString (char * szStringOut, INT iCount)
  {
  if (bCursorMoved)
    {
    BeginEdit ();
    }
  INT  iCharsPastGap = iBufferSize - iGapStart - iGapSize;
  INT  iCharsToCopy = TMin (iCharsPastGap, iCount);
  
  memcpy (szStringOut, pBuffer + iGapStart + iGapSize, iCharsToCopy);
  };

//-----------------------------------------------------------------------------
INT  GapBuffer::GetCharAtLocation (Location & locIn)
  {
  return (GetCharAtLocation (locIn.iLine, locIn.iCol));
  }

//-----------------------------------------------------------------------------
INT  GapBuffer::GetCharAtLocation (INT  iLine, INT  iCol)
  {
  INT  iOffset = LocationToOffset (iLine, iCol);
  if (iOffset == -1) return (-1);
  return (pBuffer[iOffset]);
  };
  
//-----------------------------------------------------------------------------
INT  GapBuffer::LocationToOffset (Location & locIn)
  {
  return (LocationToOffset (locIn.iLine, locIn.iCol));
  }

//-----------------------------------------------------------------------------
INT  GapBuffer::LocationToOffset (INT  iLine, INT  iCol)
  {
  if (iLine > GetNumLines ())
    {
    // past end of buffer.
    return (-1);
    }
  // cursor is on an existing line
 
  // find the column
  if (iCol > GetLineLength (iLine)) 
    {
    // cursor is past the end of the line, on a virtual space.  
    return (-1);
    }
    
  INT  iLineStartOffset = aiLineOffsets[iLine - 1];
  INT  iOffset = iLineStartOffset + iCol;    
    
  if ((iLineStartOffset <= iGapStart) && (iOffset > iGapStart))
    {
    iOffset += iGapSize;
    }
  return (iOffset);
  }

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
VOID  GapBuffer::ClampLocationToValidChar (Location &  locInOut)
  {
  if (locInOut.iLine > GetNumLines ())
    {
    INT  iLastLine = GetNumLines ();
    locInOut.Set (iLastLine, GetLineLength (iLastLine));
    return;
    }
  
  locInOut.iCol = TMin (locInOut.iCol, GetLineLength (locInOut.iLine));
  if (locInOut.iCol < 0) 
    {
    locInOut.Set (locInOut.iLine - 1, GetLineLength (locInOut.iLine - 1));
    }
  if (locInOut.iLine <= 0)
    {
    locInOut = locInvalid;
    }
  }

//-----------------------------------------------------------------------------
INT GapBuffer::GetLine (INT     iLine,
                        char *  pszBufferOut,
                        INT     iMaxBuffer)
  {
  if (pszBufferOut == NULL) return (0);
  INT  iStartingOffset = LocationToOffset (iLine, 0);
  if (iStartingOffset == -1)
    {
    pszBufferOut[0] = '\0';
    return (0);
    }
  INT  iLineLength = GetLineLength (iLine);
  INT  iCharsToCopy = TMin (iMaxBuffer, iLineLength);
  if ((iStartingOffset > iGapStart + iGapSize) ||
      (iStartingOffset + iCharsToCopy <= iGapStart))
    {
    // line is entirely before or after the gap
    strncpy (pszBufferOut, &pBuffer[iStartingOffset], iCharsToCopy);
    } 
  else 
    {
    //DebugPrintLineOffsets();
    //printf ("starting offset %d for line %d of length %d copying %d\n", iStartingOffset, iLine, iLineLength, iCharsToCopy);
    // line is split across the gap
    INT  iCharsBeforeGap = iGapStart - iStartingOffset;
    //printf ("strncpy gap start %d, start offset %d\n", iGapStart , iStartingOffset);
    //printf ("strncpy making two copies, %d and %d in length\n", iCharsBeforeGap,  iCharsToCopy - iCharsBeforeGap);
    strncpy (pszBufferOut, &pBuffer[iStartingOffset], iCharsBeforeGap);
    strncpy (&pszBufferOut[iCharsBeforeGap], &pBuffer[iGapStart + iGapSize], iCharsToCopy - iCharsBeforeGap);
    }
  return (iCharsToCopy);
  }
  
//-----------------------------------------------------------------------------
INT GapBuffer::GetCharsBetween (Location &  locOne,
                                Location &  locTwo)
  {
  // result is inclusive of locOne, exclusive of locTwo.
  Location  locSearch = locOne;
  Location  locEnd = locTwo;
  if (locOne > locTwo)
    {
    locSearch = locTwo;
    locEnd    = locOne;
    }
    
  // move search and end to nearest valid buffer character.
  ClampLocationToValidChar (locSearch);
  ClampLocationToValidChar (locEnd);
  
  // count the characters between the locations
  INT  iCharsOut = 0;
  INT  iLineLength = 0;
  if (locSearch.iLine != locEnd.iLine)
    {
    iLineLength = GetLineLength (locSearch.iLine);
    if (locSearch.iCol < iLineLength) 
      {
      iCharsOut = GetLineLength (locSearch.iLine) - locSearch.iCol + 1; 
      };
    locSearch.iCol = 0;
    ++locSearch.iLine;
    }
  while (locSearch.iLine < locEnd.iLine)  
    {
    iCharsOut += GetLineLength (locSearch.iLine) + 1;
    ++locSearch.iLine;
    }
  iCharsOut += locEnd.iCol - locSearch.iCol;  
  //printf ("Mark3 %d (%d, %d) to (%d, %d)\n", iCharsOut, locSearch.iLine, locSearch.iCol, locEnd.iLine, locEnd.iCol);
  return (iCharsOut);
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
    //printf ("BeginEdit: Gap start is at %d\n", iGapStart);
    ShiftLineOffsets (iPrevGapStart, iGapStart);
    // add any characters past end of current line as needed
    while (iCharsPastEnd)
      {
      InsertChar (' ');
      // remove the cursor increment performed by InsertChar() since we are actually
      // just trying to move up to the cursor here.
      locCursor.iCol -= 1;
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
  //printf ("DeleteChars (%d)\n", (int)iCountIn);
  if (bCursorMoved)
    {
    BeginEdit ();
    }
  INT  iDeleteStart = 0;  
  INT  iDeleteEnd = 0;  
  if (iCountIn < 0)
    {
    iCountIn = -iCountIn;
    iCountIn = TMin (iCountIn, iGapStart);
    iGapStart -= iCountIn;
    iGapSize += iCountIn;
    iDeleteStart = iGapStart;
    }
  else if (iCountIn > 0)
    {
    iCountIn = TMin (iCountIn, iBufferSize - iGapStart - iGapSize - 1);
    iDeleteStart = iGapStart + iGapSize;
    iGapSize += iCountIn;
    }
  iDeleteEnd = iDeleteStart + iCountIn;
  // remove any deleted lines from line offsets
  for (INT  iIndex = 0; iIndex < aiLineOffsets.Length(); ++iIndex)
    {
    //DBG_INFO ("Checking if %d is between %d and %d\n", aiLineOffsets [iIndex] - 1,  iDeleteStart, iDeleteEnd);
    if (aiLineOffsets [iIndex] - 1 >= iDeleteStart)
      {
      while ((iIndex < aiLineOffsets.Length ()) && 
             (aiLineOffsets [iIndex] - 1 < iDeleteEnd))
        {
        //printf ("Deleting line %d\n", aiLineOffsets [iIndex]);
        aiLineOffsets.Remove (iIndex);
        };
      break;  
      };
    };
  
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
    if (aiLineOffsets [iIndex] > iFirstGap)
      {
      for (; iIndex < aiLineOffsets.Length(); ++iIndex)
        {
        if (aiLineOffsets [iIndex] > iLastGap) break;
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
    //printf ("  %6d\n", (INT)aiLineOffsets[iIndex]);
    printf (" %4d:  %6d\n", iIndex, (INT)aiLineOffsets[iIndex]);
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

//-----------------------------------------------------------------------------
VOID  GapBuffer::MoveGapToEnd  (VOID)
  {
  ClearSelection ();
  INT  iLastLine = GetNumLines ();
  SetCursor (iLastLine, GetLineLength (iLastLine));
  BeginEdit ();
  };

 