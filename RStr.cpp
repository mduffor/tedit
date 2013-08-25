/* -----------------------------------------------------------------
                            TEdit String

    This module implements a string class.

                            Michael T. Duffy
   ----------------------------------------------------------------- */

// RStr.cpp
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Types.hpp"
#include "Debug.hpp"
#include "RStr.hpp"

ASSERTFILE (__FILE__);

const UINT32   RStr::kESCAPE       = 0x5c;

RStr   RStr::kEmpty ("");
const char  RStr::szEmpty [1] = {'\0'};


//------------------------------------------------------------------------
RStr::RStr  ()
  {
  // strings should always have a valid pszBuffer value, so we initialize it
  // to a string that contains a null
  Init (0);
  };


//------------------------------------------------------------------------
RStr::RStr  (const RStr &  strIn)
  {
  // strings should always have a valid pszBuffer value, so we initialize it
  // to a string that contains a null
  Init (0);

  this->AppendChars (strIn.pszBuffer, strIn.GetLength ());
  };


//------------------------------------------------------------------------
RStr::RStr           (const char *  pszIn)
  {
  // strings should always have a valid pszBuffer value, so we initialize it
  // to a string that contains a null
  Init (0);

  this->AppendChars (pszIn, strlen (pszIn));
  };


//------------------------------------------------------------------------
//RStr::RStr (const char * szFormatString, ...)
//  {
//  Init (0);
//  
//  // Step through the args, and process the string as you go through each
//  va_list    vaArgList;
//
//  va_start         (vaArgList, szFormatString);
//  FormatProcessArg (szFormatString, vaArgList);
//  va_end           (vaArgList);
//  };

  
//------------------------------------------------------------------------
RStr::RStr  (UINT32  uSizeIn)
  {
  Init (uSizeIn);
  };


//------------------------------------------------------------------------
VOID  RStr::Init  (UINT32  uInitialSizeIn)
  {
  pszBuffer      = NULL;
  uStringLength  = 0;
  uBufferSize    = 0;
  uGrowIncrement = 128; // grow 128 chars at a time as a default

  Grow (uInitialSizeIn);
  };


  
  
//------------------------------------------------------------------------
RStr::~RStr          ()
  {
  if ((pszBuffer != NULL) && (pszBuffer != const_cast <char *> (RStr::szEmpty)))
    {
    free (pszBuffer);
    };
  };


//------------------------------------------------------------------------
VOID  RStr::SetAt  (UINT32  uPosIn,
                    UINT32  uIn)
  {

  if (uPosIn > uBufferSize)
    {
    // past the end of the buffer.
    Grow (uPosIn);
    };

  if (uPosIn >= uStringLength)
    {
    // extend the string to include the requested position
    uStringLength = uPosIn + 1;
    pszBuffer [uStringLength] = '\0';
    };

  // set the character
  pszBuffer [uPosIn] = static_cast <char> (uIn);
  };


//------------------------------------------------------------------------
VOID  RStr::GetLeft  (UINT32  uCount,
                      RStr &  strResult) const
  {

  // Determine the final string length, grow the buffer, copy the characters, and cap the string with a null.

  if (uStringLength < uCount) {uCount = uStringLength;};

  strResult.GrowAbsolute (uCount);

  if (uCount > 0)
    {
    strncpy (strResult.pszBuffer, pszBuffer, uCount);
    };
  strResult.uStringLength = uCount;

  strResult.pszBuffer [uCount] = 0;
  };


//------------------------------------------------------------------------
VOID  RStr::GetRight  (UINT32  uCount,
                       RStr &  strResult) const
  {

  // Determine the final string length, grow the buffer, copy the characters, and cap the string with a null.

  if (uStringLength < uCount) {uCount = uStringLength;};

  strResult.GrowAbsolute (uCount);

  if (uCount > 0)
    {
    strncpy (strResult.pszBuffer, pszBuffer + uStringLength - uCount, uCount);
    };
  strResult.uStringLength = uCount;

  strResult.pszBuffer [uCount] = 0;
  };


//------------------------------------------------------------------------
VOID  RStr::GetMiddle  (UINT32  uStart,
                        UINT32  uCount,
                        RStr &  strResult) const
  {
  UINT32           uRemainingLength;


  // Determine the final string length, grow the buffer, copy the characters, and cap the string with a null.

  uRemainingLength = uStringLength - uStart;

  if (uRemainingLength < uCount) {uCount = uRemainingLength;};

  strResult.GrowAbsolute (uCount);

  if (uCount > 0)
    {
    strncpy (strResult.pszBuffer, pszBuffer + uStart, uCount);
    };
  strResult.uStringLength = uCount;

  strResult.pszBuffer [uCount] = 0;
  };


//------------------------------------------------------------------------
INT32  RStr::FindChar (UINT32  uIn,
                       UINT32  uStartAt) const
  {

  if (uStartAt >= uStringLength) return -1;


  char *  pszPos = strchr (pszBuffer + uStartAt, uIn);

  if (pszPos == NULL)  return -1;

  return (pszPos - pszBuffer);
  };


//------------------------------------------------------------------------
INT32  RStr::ReverseFindChar  (UINT32  uIn,
                               UINT32  uStartAt) const
  {
  INT32  iIndex;

  if (uStringLength == 0) return -1;

  if (uStartAt == (UINT32) -1)  {uStartAt = uStringLength - 1;};

  for (iIndex = (INT32) uStartAt; iIndex >= 0; --iIndex)
    {
    if (pszBuffer [iIndex] == static_cast <char> (uIn)) break;
    };
  return (iIndex);
  };


//------------------------------------------------------------------------
INT32  RStr::Find (const RStr &  strIn,
                   UINT32        uStartAt) const
  {
  return (Find (strIn.AsChar (), uStartAt));
  };


//------------------------------------------------------------------------
INT32  RStr::Find (const char *  szIn,
                   UINT32        uStartAt) const
  {

  if (uStartAt >= uStringLength) return -1;


  char *  pszPos = strstr (pszBuffer + uStartAt, szIn);

  if (pszPos == NULL)  return -1;

  return (pszPos - pszBuffer);
  };


//------------------------------------------------------------------------------
INT  RStr::Replace  (RStr  strSearchString,
                     RStr  strReplaceString,
                     BOOL  bReplaceAll)
  {
  INT  iFound = 0;
  INT  iDifference = strReplaceString.GetLength () - strSearchString.GetLength ();
  INT  iNumReplaced = 0;
  
  
  while ((iFound = Find (strSearchString, iFound)) != -1)
    {
    if (iDifference > 0)
      {
      // replace is longer than search.  Need to insert characters

      InsertChars (' ', UINT32 (iDifference), UINT32 (iFound));
      }
    else if (iDifference < 0)
      {
      // replace is shorter than search.  Need to remove characters.

      ClipMiddle (UINT32 (iFound), UINT32 (-iDifference));
      };
    // now replace the string
    strncpy (GetBufferPtr (iFound), strReplaceString.AsChar (), strReplaceString.GetLength ());
    ++iNumReplaced;
    if (!bReplaceAll) break;
    };
  return (iNumReplaced);  
  };


//------------------------------------------------------------------------
INT32  RStr::Compare        (const char *  pszMatchIn,
                             INT32         iNumCharsToMatch,
                             INT32         iStartPosition) const
  {
  if (iNumCharsToMatch == -1)
    {
    return (strcmp (&pszBuffer [iStartPosition], pszMatchIn));
    return (strcmp (&pszBuffer [iStartPosition], pszMatchIn));
    };
  return (strncmp (&pszBuffer [iStartPosition], pszMatchIn, iNumCharsToMatch));
  };


//------------------------------------------------------------------------
INT32  RStr::Compare        (RStr          strMatchIn,
                             INT32         iNumCharsToMatch,
                             INT32         iStartPosition) const
  {
  if (iNumCharsToMatch == -1)
    {
    return (strcmp (&pszBuffer [iStartPosition], strMatchIn.pszBuffer));
    };
  return (strncmp (&pszBuffer [iStartPosition], strMatchIn.pszBuffer, iNumCharsToMatch));
  };


//------------------------------------------------------------------------
INT32  RStr::CompareNoCase  (const char *  pszMatchIn,
                             INT32         iNumCharsToMatch,
                             INT32         iStartPosition) const
  {
  if (iNumCharsToMatch == -1)
    {
    #ifndef LINUX
      return (stricmp (&pszBuffer [iStartPosition], pszMatchIn));
    #else
      return (strcasecmp (&pszBuffer [iStartPosition], pszMatchIn));
    #endif
    };
  #ifndef LINUX
    return (strnicmp (&pszBuffer [iStartPosition], pszMatchIn, iNumCharsToMatch));
  #else
    return (strncasecmp (&pszBuffer [iStartPosition], pszMatchIn, iNumCharsToMatch));
  #endif
  };


//------------------------------------------------------------------------
INT32  RStr::CompareNoCase  (RStr          strMatchIn,
                             INT32         iNumCharsToMatch,
                             INT32         iStartPosition) const
  {
  if (iNumCharsToMatch == -1)
    {
    #ifndef LINUX
      return (stricmp (&pszBuffer [iStartPosition], strMatchIn.pszBuffer));
    #else
      return (strcasecmp (&pszBuffer [iStartPosition], strMatchIn.pszBuffer));
    #endif
    };
  #ifndef LINUX
    return (strnicmp (&pszBuffer [iStartPosition], strMatchIn.pszBuffer, iNumCharsToMatch));
  #else
    return (strncasecmp (&pszBuffer [iStartPosition], strMatchIn.pszBuffer, iNumCharsToMatch));
  #endif
  };


//------------------------------------------------------------------------
INT32  RStr::ReplaceChar  (UINT32        uSearchIn,
                           UINT32        uReplaceIn,
                           UINT32        uStartIn,
                           UINT32        uEndIn)
  {
  INT32            iNumReplaced = 0;
  UINT32           uEnd = (uEndIn != 0) ? uEndIn : uStringLength;



  UINT32           uCurr;

  for (uCurr = uStartIn; uCurr < uEnd; ++uCurr)
    {
    if (UINT32 (((UCHAR *) pszBuffer) [uCurr]) == uSearchIn)
      {
      pszBuffer [uCurr] = CHAR (uReplaceIn);
      ++iNumReplaced;
      };
    };
  return (iNumReplaced);
  };


//------------------------------------------------------------------------
INT32  RStr::CountChar  (UINT32        uSearchIn,
                         UINT32        uStartIn) 
  {
  UINT32  uCountOut = 0;
  
  for (UINT32  uIndex = uStartIn; uIndex < uStringLength; ++uIndex)
    {
    if (UINT32 (((UCHAR *) pszBuffer) [uIndex]) == uSearchIn)
      {
      ++uCountOut;
      };
    };
  return (uCountOut);
  };
  

//------------------------------------------------------------------------
INT32  RStr::TranslateEscapedChars  (UINT32        uStartIn,
                                     UINT32        uEndIn)
  {
  UINT32           uNumReplaced = 0;
  UINT32           uEnd = (uEndIn != 0) ? uEndIn : uStringLength;

  UINT32           uCurrRead;
  UINT32           uCurrWrite;
  UINT32           uCharCurr;

  for (uCurrRead = uCurrWrite = uStartIn; uCurrRead < uEnd; ++uCurrRead, ++uCurrWrite)
    {
    uCharCurr = UINT32 (((UCHAR *) pszBuffer) [uCurrRead]);
    
    if (uCharCurr == kESCAPE)
      {
      uCurrRead++;
      uCharCurr = UINT32 (((UCHAR *) pszBuffer) [uCurrRead]);
      switch (uCharCurr)
        {
        case 'a':  {uCharCurr = '\a'; break;};
        case 'b':  {uCharCurr = '\b'; break;};
        case 'f':  {uCharCurr = '\f'; break;};
        case 'n':  {uCharCurr = '\n'; break;};
        case 'r':  {uCharCurr = '\r'; break;};
        case 't':  {uCharCurr = '\t'; break;};
        case 'v':  {uCharCurr = '\v'; break;};
        case '0':  {uCharCurr = '\0'; break;};
        default:   break;
        };
      ++uNumReplaced;
      };
    (((UCHAR *) pszBuffer) [uCurrWrite]) = uCharCurr;
    };
  // the string is now shorter since we collapsed two-byte codes into one byte codes  
  uStringLength -= uNumReplaced;
  (((UCHAR *) pszBuffer) [uCurrWrite]) = '\0';


  return (INT32 (uNumReplaced));
  };

  
//------------------------------------------------------------------------
VOID  RStr::Empty          (VOID)
  {

  uStringLength = 0;
  if (pszBuffer != const_cast <char *> (RStr::szEmpty))
    {
    pszBuffer [0] = '\0';
    };
  };


//------------------------------------------------------------------------
VOID  RStr::Reset          (VOID)
  {

  uStringLength = 0;
  SetBufferSize (0);
  };


//------------------------------------------------------------------------
VOID  RStr::Grow  (UINT32  uSizeIn)
  {
  UINT32           uGrowSize = uBufferSize;

  // special case zero sized grows for speed
  if (uSizeIn == 0)
    {
    if (pszBuffer == NULL)
      {
      pszBuffer = const_cast <char *> (RStr::szEmpty);
      };
    // zero growth.  Leave as-is
    return;
    };

  // grow as needed
  if ((uStringLength + uSizeIn + 1) > uBufferSize)
    {
    // grow the buffer.

    while (uGrowSize < (uStringLength + uSizeIn))
      {
      uGrowSize += uGrowIncrement;
      };
    SetBufferSize (uGrowSize);
    };
  };


//------------------------------------------------------------------------
VOID  RStr::GrowAbsolute  (UINT32  uSizeIn)
  {
  // Pass this function the number of characters you need the string to hold.
  
  UINT32           uGrowSize = uBufferSize;

  if ((uSizeIn + 1) > uBufferSize)
    {
    // grow the buffer.

    while (uGrowSize < uSizeIn)
      {
      uGrowSize += uGrowIncrement;
      };
    SetBufferSize (uGrowSize);
    };
  };


//------------------------------------------------------------------------
const RStr &  RStr::Set     (const RStr & strIn)
  {

  GrowAbsolute (strIn.uStringLength);

  memcpy  (pszBuffer, strIn.pszBuffer, strIn.uStringLength);

  uStringLength = strIn.uStringLength;

  pszBuffer [uStringLength] = '\0';

  return *this;
  };


//------------------------------------------------------------------------
const RStr &  RStr::Set     (const char *  pszIn)
  {
  UINT32          uNewLength;

  if (pszIn == NULL)
    {
    return (Set (""));
    };

  uNewLength = strlen (pszIn);

  GrowAbsolute (uNewLength);

  if (uNewLength > 0)
    {
    memcpy (pszBuffer, pszIn, uNewLength);
    };

  uStringLength = uNewLength;

  pszBuffer [uStringLength] = '\0';

  return *this;
  };


//------------------------------------------------------------------------
const RStr &  RStr::operator =     (const RStr & strIn)
  {

  return Set (strIn);
  };


//------------------------------------------------------------------------
const RStr &  RStr::operator =     (const char *  pszIn)
  {
  return Set (pszIn);
  };


//------------------------------------------------------------------------
VOID  RStr::operator +=    (const RStr &  strIn)
  {
  UINT32           uNewLength;

  if (strIn.uStringLength == 0) return;

  uNewLength = uStringLength + strIn.uStringLength;

  Grow (strIn.uStringLength);

  memcpy (pszBuffer + uStringLength, strIn.pszBuffer, strIn.uStringLength);

  uStringLength = uNewLength;
  pszBuffer [uStringLength] = '\0';
  };


//------------------------------------------------------------------------
VOID  RStr::operator +=    (const char *  pszIn)
  {
  UINT32          uNewLength;
  UINT32          uLengthIn;


  uLengthIn = strlen (pszIn);

  if (uLengthIn == 0) return;

  uNewLength = uStringLength + uLengthIn;

  Grow (uLengthIn);

  memcpy (pszBuffer + uStringLength, pszIn, uLengthIn);

  uStringLength = uNewLength;
  pszBuffer [uStringLength] = '\0';
  };


//------------------------------------------------------------------------
VOID  RStr::operator +=    (const UINT32    uIn)
  {
  UINT32          uNewLength;


  uNewLength = uStringLength + 1;

  Grow (1);

  uStringLength = uNewLength;
  pszBuffer [uStringLength - 1] = static_cast <char> (uIn);
  pszBuffer [uStringLength]     = '\0';
  //printf ("RStr+= char set %d, null at %d (%c)\n", uStringLength - 1, uStringLength, static_cast <char> (uIn));
  };


//------------------------------------------------------------------------
VOID  RStr::AppendString   (const RStr &  strIn)
  {
  *this += strIn;
  };


//------------------------------------------------------------------------
VOID  RStr::AppendString   (const char *  pszIn)
  {
  *this += pszIn;
  };


//------------------------------------------------------------------------
VOID  RStr::AppendChar     (UINT32  uIn)
  {
  *this += uIn;
  };


//------------------------------------------------------------------------
VOID  RStr::AppendChars    (const char *   pszIn,
                            INT32          iCopyLengthIn)
  {
  UINT32          uNewLength;

  if (iCopyLengthIn == -1)  
    {
    iCopyLengthIn = strlen (pszIn);
    };

  if (iCopyLengthIn <= 0) return;

  uNewLength = uStringLength + UINT (iCopyLengthIn);

  Grow (UINT (iCopyLengthIn));

  memcpy (pszBuffer + uStringLength, pszIn, UINT (iCopyLengthIn));

  uStringLength = uNewLength;
  pszBuffer [uStringLength] = '\0';
  };


//------------------------------------------------------------------------
VOID  RStr::PrependString  (const RStr &  strIn)
  {
  UINT32           uNewLength;
  UINT32           uLengthIn;


  uLengthIn = strIn.uStringLength;
  if (uLengthIn == 0) return;

  uNewLength = uStringLength + uLengthIn;

  Grow (uLengthIn);

  memmove (pszBuffer + uLengthIn, pszBuffer, uStringLength);
  strncpy (pszBuffer, strIn.pszBuffer, uLengthIn);

  uStringLength = uNewLength;
  pszBuffer [uStringLength] = '\0';
  };


//------------------------------------------------------------------------
VOID  RStr::PrependString  (const char *  pszIn)
  {
  UINT32           uNewLength;
  UINT32           uLengthIn;


  uLengthIn = strlen (pszIn);
  if (uLengthIn == 0) return;

  uNewLength = uStringLength + uLengthIn;

  Grow (uLengthIn);

  memmove (pszBuffer + uLengthIn, pszBuffer, uStringLength);
  strncpy (pszBuffer, pszIn, uLengthIn);

  uStringLength = uNewLength;
  pszBuffer [uStringLength] = '\0';
  };


//------------------------------------------------------------------------
VOID  RStr::PrependChar    (UINT32          uIn)
  {
  UINT32           uNewLength;


  uNewLength = uStringLength + 1;

  Grow (1);

  memmove (pszBuffer + 1, pszBuffer, uStringLength);

  uStringLength = uNewLength;
  pszBuffer [0]             = static_cast <char> (uIn);
  pszBuffer [uStringLength] = '\0';
  };


//------------------------------------------------------------------------
VOID  RStr::InsertChars  (UINT32        uCharIn,
                          UINT32        uCount,
                          UINT32        uPosIn)
  {
  UINT32           uNewLength;
  UINT32           uRemainingLength;


  if (uCount == 0) return;

  uNewLength       = uCount + uStringLength;
  uRemainingLength = uStringLength - uPosIn;

  Grow (uCount);

  // the first part of the string is already in place.  Move the end of the string,
  //  then insert the new part.

  if (uRemainingLength > 0)
    {
    memmove (pszBuffer + uPosIn + uCount, pszBuffer + uPosIn, uRemainingLength);
    };

  memset (pszBuffer + uPosIn, uCharIn, uCount);

  uStringLength = uNewLength;
  pszBuffer [uStringLength] = '\0';
  };


//------------------------------------------------------------------------
VOID  RStr::InsertString  (const RStr &  strIn,
                           UINT32        uPosIn)
  {
  UINT32           uNewLength;
  UINT32           uCopyLength;
  UINT32           uRemainingLength;


  uCopyLength = strIn.uStringLength;
  if (uCopyLength == 0) return;

  uNewLength       = uCopyLength + uStringLength;
  uRemainingLength = uStringLength - uPosIn;

  Grow (uCopyLength);

  // the first part of the string is already in place.  Move the end of the string,
  //  then insert the new part.

  if (uRemainingLength > 0)
    {
    memmove (pszBuffer + uPosIn + uCopyLength, pszBuffer + uPosIn, uRemainingLength);
    };

  strncpy (pszBuffer + uPosIn, strIn.pszBuffer, uCopyLength);

  uStringLength = uNewLength;
  pszBuffer [uStringLength] = '\0';
  };


//------------------------------------------------------------------------
VOID  RStr::InsertString  (const char *  pszIn,
                           UINT32        uPosIn)
  {
  UINT32           uNewLength;
  UINT32           uCopyLength;
  UINT32           uRemainingLength;


  uCopyLength = strlen (pszIn);
  if (uCopyLength == 0) return;

  uNewLength       = uCopyLength + uStringLength;
  uRemainingLength = uStringLength - uPosIn;

  Grow (uCopyLength);

  // the first part of the string is already in place.  Move the end of the string,
  //  then insert the new part.

  if (uRemainingLength > 0)
    {
    memmove (pszBuffer + uPosIn + uCopyLength, pszBuffer + uPosIn, uRemainingLength);
    };

  strncpy (pszBuffer + uPosIn, pszIn, uCopyLength);

  uStringLength = uNewLength;
  pszBuffer [uStringLength] = '\0';
  };


//------------------------------------------------------------------------
VOID  RStr::ClipLeft  (UINT32  uCountIn)
  {

  if (uCountIn > uStringLength)
    {
    uCountIn = uStringLength;
    };

  if (uCountIn == 0) return;

  memmove (pszBuffer, pszBuffer + uCountIn, uStringLength - uCountIn);

  uStringLength -= uCountIn;
  pszBuffer [uStringLength] = '\0';
  };


//------------------------------------------------------------------------
VOID  RStr::ClipRight  (UINT32  uCountIn)
  {

  if (uCountIn > uStringLength)
    {
    uCountIn = uStringLength;
    };

  uStringLength -= uCountIn;
  pszBuffer [uStringLength] = '\0';
  };


//------------------------------------------------------------------------
VOID  RStr::ClipMiddle  (UINT32  uStartCharIn,  // removes count chars from the middle of string
                         UINT32  uCountIn)
  {


  if (uStartCharIn >= uStringLength)  return;

  // clip uCountIn against the end of the string.

  if ((uStartCharIn + uCountIn) > uStringLength)
    {
    uCountIn = uStringLength - uStartCharIn;
    };
  if (uCountIn == 0)  return;

  // collapse the second part of the string over the middle.

  memmove (pszBuffer + uStartCharIn, pszBuffer + uStartCharIn + uCountIn, uStringLength - uCountIn - uStartCharIn);

  uStringLength -= uCountIn;
  pszBuffer [uStringLength] = '\0';
  };


//------------------------------------------------------------------------
RStr  RStr::ChompLeft  (UINT32  uCountIn)
  {
  RStr             strReturn;


  strReturn.AppendChars (this->AsChar (), uCountIn);

  ClipLeft (uCountIn);
  return (strReturn);
  };


//------------------------------------------------------------------------
RStr  RStr::ChompRight  (UINT32  uCountIn)
  {
  RStr             strReturn;


  strReturn.AppendChars (this->AsChar (GetLength () - uCountIn), uCountIn);

  ClipRight (uCountIn);
  return (strReturn);
  };


//------------------------------------------------------------------------
RStr  RStr::SubString  (UINT32  uStartCharIn,  
                        UINT32  uCountIn)
  {
  RStr   strReturn;

  if (uStartCharIn >= uStringLength)  return (strReturn);

  // clip uCountIn against the end of the string.

  if ((uStartCharIn + uCountIn) > uStringLength)
    {
    uCountIn = uStringLength - uStartCharIn;
    };
  if (uCountIn == 0)  return (strReturn);

  // extract the needed characters
  strReturn.Grow (uCountIn);
  for (INT  iIndex = 0; iIndex < INT (uCountIn); ++iIndex)
    {
    strReturn.AppendChar (pszBuffer [uStartCharIn + iIndex]);
    };
  return (strReturn);  
  };



//------------------------------------------------------------------------
VOID  RStr::TruncateLeft   (UINT32  uPosIn)
  {
  UINT32           uCount;

  if (uPosIn == 0) return;

  uCount = uPosIn;

  ClipLeft (uCount);
  };


//------------------------------------------------------------------------
VOID  RStr::TruncateRight  (UINT32  uPosIn)
  {
  UINT32           uCount;

  if (uPosIn >= uStringLength) return;

  uCount = uStringLength - uPosIn - 1;

  ClipRight (uCount);
  };


//------------------------------------------------------------------------
VOID RStr::Format  (const char *  szFormatString, ...)
  {

  // clear the current contents
  Empty ();


  // Step through the args, and process the string as you go through each
  va_list    vaArgList;


  va_start         (vaArgList, szFormatString);
  FormatProcessArg (szFormatString, vaArgList);
  va_end           (vaArgList);
  };


//------------------------------------------------------------------------
VOID RStr::AppendFormat  (const char *  szFormatString, ...)
  {
  RStr             strTemp;

  // clear the current contents
  strTemp.Empty ();

  // Step through the args, and process the string as you go through each
  va_list    vaArgList;


  va_start                 (vaArgList, szFormatString);
  strTemp.FormatProcessArg (szFormatString, vaArgList);
  va_end                   (vaArgList);
  *this += strTemp;
  };


//------------------------------------------------------------------------
VOID RStr::FormatProcessArg (const char *  pszFormatStringIn,
                             va_list &     vaArgListIn)
  {

  for (;;)
    {
    const char *   pszNext;

    pszNext = strchr (pszFormatStringIn, '%');

    // if no format markeris found, break so you can copy the last of the string.
    if (pszNext == NULL) break;

    // if there is anything before the format marker, then append it to this string.
    if (pszNext > pszFormatStringIn)
      {
      AppendChars (pszFormatStringIn, pszNext - pszFormatStringIn);
      };

    // increment the string pointer to the character past the format marker
    pszFormatStringIn = pszNext + 1;

    // process this parameter

    if (! FormatMarker (pszFormatStringIn, vaArgListIn))
      {
      // marker processing error.  Copy the rest of the string as-is and exit.
      break;
      };
    };

  // copy the remaining part of the string

  *this += pszFormatStringIn;
  };



//------------------------------------------------------------------------
BOOL RStr::FormatMarker (const char * &  pszFormatStringIn,
                         va_list &       vaArgListIn)
  {
  CHAR             szFormatString [64];
  UINT32           uFSIndex;
  CHAR             cNextChar;
  const char       szPrefixChars    [] = "-+0 #*.123456789hlL";
  const char       szIntegerMarkers [] = "cCdiouxX";
  const char       szDoubleMarkers  [] = "eEfgG";
  CHAR             szBufferOut    [256];


  // this routine returns TRUE if the marker was successfully processed, or
  //  false if there was an error.


  // process the format marker in a printf type string


  // we will use sprintf to actually process the various argument-to-ASCII
  //   conversion.  Step through the marker and copy all of its elements
  //   to the szFormatString buffer, then pass this string and the arg
  //   to sprintf to get the text to write.

  // Note:  You may want to add a marker for RStr *


  szFormatString [0] = '%';
  uFSIndex = 1;

  // step through the prefix
  for (;;)
    {
    cNextChar = pszFormatStringIn [0];


    if (uFSIndex >= sizeof (szFormatString))
      {
      // read too many characters in!!!
      DBG_ERROR ("Incorrect format specifier in string.");
      return FALSE;
      };

    if (cNextChar == 0)
      {
      // found a terminating null.  This shouldn't be here after a format
      //  specifier.
      return FALSE;
      };

    ++pszFormatStringIn;

    if (strchr (szPrefixChars, cNextChar) != NULL)
      {
      // prefix character.  Add to format string and grab another character.

      szFormatString [uFSIndex] = cNextChar;

      ++uFSIndex;
      }
    else
      {
      // not a prefix char, so it must be a format marker.  Process it.
      break;
      };
    };

  // Process the format marker

  // strings
  if (cNextChar == 's')
    {
    const char *  pszStringOut = va_arg (vaArgListIn, const char *);
    INT32         iStrLength = strlen (pszStringOut);
    INT32         iCopyLength = iStrLength;
    char *        pszEnd;

    // get the string precision, in case a maximum number of chars to copy
    //  was specified
    szFormatString [uFSIndex] = 0;

    char *  pszPrecision = strchr (szFormatString, '.');
    if ((pszPrecision != NULL) &&
        (pszPrecision [1] != 0))
      {
      INT32  iPrecision = (UINT32) strtol (pszPrecision + 1, &pszEnd, 10);
      if ((iPrecision >= 0) && (iPrecision < iCopyLength))
        {
        iCopyLength = iPrecision;
        };
      };
    if (pszPrecision != NULL)
      {
      // replace the decimal with a null, so we can read the first part of the
      //  format string as an integer
      pszPrecision [0] = 0;
      };
    INT32  iPaddedLength = (UINT32) strtol (szFormatString + 1, &pszEnd, 10);

    // write any padded space
    INT32  iPadding = iPaddedLength - iCopyLength;
    while (iPadding > 0)
      {
      AppendChar (' ');
      --iPadding;
      };

    // write the string
    AppendChars (pszStringOut, (UINT32) iCopyLength);
    return (TRUE);
    };

  // integers
  if (strchr (szIntegerMarkers, cNextChar) != NULL)
    {
    szFormatString [uFSIndex    ] = cNextChar;
    szFormatString [uFSIndex + 1] = 0;

    INT32 iValue =  va_arg (vaArgListIn, INT32);

    sprintf (szBufferOut, szFormatString, iValue);

    AppendString (szBufferOut);

    return (TRUE);
    };

  // doubles
  if (strchr (szDoubleMarkers, cNextChar) != NULL)
    {
    szFormatString [uFSIndex    ] = cNextChar;
    szFormatString [uFSIndex + 1] = 0;

    DOUBLE dValue =  va_arg (vaArgListIn, DOUBLE);

    sprintf (szBufferOut, szFormatString, dValue);

    AppendString (szBufferOut);

    return (TRUE);
    };

  // pointers

  if (cNextChar == 'p')
    {
    szFormatString [uFSIndex    ] = cNextChar;
    szFormatString [uFSIndex + 1] = 0;

    PVOID pValue =  va_arg (vaArgListIn, PVOID);

    sprintf (szBufferOut, szFormatString, pValue);

    AppendString (szBufferOut);

    return (TRUE);

    };

  // we don't support 'n', which is the number of chars written so far.
  // we don't support 'S', which is a Unicode string

  if (cNextChar == 'n')
    {
    DBG_ERROR ("The 'n' (store number of chars written so far) format marker is not supported by RStr");
    return FALSE;
    };
  if (cNextChar == 'S')
    {
    DBG_ERROR ("The 'S' (print Unicode string) format marker is not supported by RStr");
    return FALSE;
    };

  // Not a processed marker.  Just write the character

  AppendChar (cNextChar);

  return (TRUE);
  };


//------------------------------------------------------------------------
VOID  RStr::ToLower  (VOID)
  {
  for (UINT  uIndex = 0; uIndex < uStringLength; ++uIndex)
    {
    pszBuffer [uIndex] = tolower (pszBuffer [uIndex]);
    };
  };


//------------------------------------------------------------------------
VOID  RStr::ToUpper  (VOID)
  {
  for (UINT  uIndex = 0; uIndex < uStringLength; ++uIndex)
    {
    pszBuffer [uIndex] = toupper (pszBuffer [uIndex]);
    };
  };


//------------------------------------------------------------------------
INT32  RStr::StripTrailingChar (UINT32  uCharIn)
  {
  INT32   uNumCharsStripped = 0;

  while ((uStringLength > 0) && (UINT32 (pszBuffer [uStringLength - 1]) == uCharIn))
    {
    pszBuffer [uStringLength - 1] = 0;
    --uStringLength;
    ++uNumCharsStripped;
    };
  return (uNumCharsStripped);
  };


//------------------------------------------------------------------------
INT32  RStr::StripTrailingWhitespace (BOOL  bStripEOL)
  {
  INT32   uNumCharsStripped = 0;

  while ((uStringLength > 0) &&
         ((pszBuffer [uStringLength - 1] == ' ') ||
          (pszBuffer [uStringLength - 1] == '\t') ||

          ((bStripEOL) && (pszBuffer [uStringLength - 1] == '\r')) ||
          ((bStripEOL) && (pszBuffer [uStringLength - 1] == '\n')) 

          )
         )
    {
    pszBuffer [uStringLength - 1] = 0;
    --uStringLength;
    ++uNumCharsStripped;
    };
  return (uNumCharsStripped);
  };


//------------------------------------------------------------------------
VOID RStr::SetBufferSize  (UINT32   uSizeIn)
  {

  if (uBufferSize == (uSizeIn + 1)) return;

  //if (uSizeIn == 0) return;

  // allocate one larger than uSizeIn to account for the terminating null.
  if ((pszBuffer == NULL) || (pszBuffer == const_cast <char *> (RStr::szEmpty)))
    {
    pszBuffer = (char *) malloc (uSizeIn + 1);
    }
  else
    {
    pszBuffer = (char *) realloc (pszBuffer, uSizeIn + 1);
    };

  if ((uBufferSize > 0) &&
      (uSizeIn < (uBufferSize - 1)))
    {
    // smaller buffer.

    if (uStringLength > uSizeIn)
      {
      uStringLength = uSizeIn;
      };
    };

  uBufferSize = uSizeIn + 1;

  // set the terminating null
  pszBuffer [uSizeIn] = '\0';
  };


//------------------------------------------------------------------------
RAVEN_EXPORT RStr  operator+      (const RStr & strOne, const RStr & strTwo)
  {
  RStr   strReturn (strOne.uStringLength + strTwo.GetLength ());


  strReturn =  strOne;
  strReturn += strTwo;
  return (strReturn);
  };


//------------------------------------------------------------------------
RAVEN_EXPORT RStr  operator+      (const RStr & strOne, const char pszTwo [])
  {
  RStr   strReturn (strOne.uStringLength + strlen (pszTwo));


  strReturn =  strOne;
  strReturn += pszTwo;
  return (strReturn);
  };

//------------------------------------------------------------------------
RAVEN_EXPORT RStr  operator+      (const char * pszOne, const RStr & strTwo)
  {
  RStr   strReturn (strlen (pszOne) + strTwo.uStringLength);


  strReturn =  pszOne;
  strReturn += strTwo;
  return (strReturn);
  };

//------------------------------------------------------------------------
RAVEN_EXPORT RStr  operator+      (const RStr & strOne, const UINT32   uTwo)
  {
  RStr   strReturn (strOne.uStringLength + 1);


  strReturn =  strOne;
  strReturn += uTwo;
  return (strReturn);
  };

//------------------------------------------------------------------------
RAVEN_EXPORT RStr  operator+      (const UINT32   uOne,  const RStr & strTwo)
  {
  RStr   strReturn (1 + strTwo.uStringLength);


  strReturn += uOne;
  strReturn += strTwo;
  return (strReturn);
  };

  
//------------------------------------------------------------------------
BOOL  RStr::StartsWith  (const char *  szIn)
  {
  INT  iLenIn = strlen (szIn);
  if ((UINT32)iLenIn < GetLength ()) return (FALSE);
  if (strncmp (GetBufferPtr (0), szIn, iLenIn) == 0) return (TRUE);
  return (FALSE);
  };
    
//------------------------------------------------------------------------
BOOL  RStr::EndsWith  (const char *  szIn)
  {
  INT  iLenIn = strlen (szIn);
  if ((UINT32)iLenIn < GetLength ()) return (FALSE);
  if (strncmp (GetBufferPtr (GetLength () - iLenIn), szIn, iLenIn) == 0) return (TRUE);
  return (FALSE);
  };
  


//------------------------------------------------------------------------
RAVEN_EXPORT BOOL operator== (const RStr & strOne,
                              const RStr & strTwo)
  {
  if (strOne.GetLength () != strTwo.GetLength ()) return 0;
  return (strcmp (strOne.AsChar (), strTwo.AsChar ()) == 0);
  };


//------------------------------------------------------------------------
RAVEN_EXPORT BOOL operator== (const RStr &  strOne,
                              const char *  pszTwo)
  {
  return (strcmp (strOne.AsChar (), pszTwo) == 0);
  };


//------------------------------------------------------------------------
//BOOL operator== (RStr &  strOne,
//                 char *     pszTwo)
//  {
//  return (strcmp (strOne.AsChar (), pszTwo) == 0);
//  };


//------------------------------------------------------------------------
//BOOL operator== (RStr &        strOne,
//                 const char *  pszTwo)
//
//  {
//  return (strcmp (strOne.AsChar (), pszTwo) == 0);
//  };

//------------------------------------------------------------------------
RAVEN_EXPORT BOOL operator== (const char *  pszOne,
                              const RStr &  strTwo)
  {
  return (strcmp (pszOne, strTwo.AsChar ()) == 0);
  };


//------------------------------------------------------------------------
RAVEN_EXPORT BOOL operator!= (const RStr & strOne,
                              const RStr & strTwo)
  {
  return (strcmp (strOne.AsChar (), strTwo.AsChar ()) != 0);
  };


//------------------------------------------------------------------------
RAVEN_EXPORT BOOL operator!= (const RStr &  strOne,
                              const char *  pszTwo)
  {
  return (strcmp (strOne.AsChar (), pszTwo) != 0);
  };


//------------------------------------------------------------------------
RAVEN_EXPORT BOOL operator!= (const char *  pszOne,
                              const RStr &  strTwo)
  {
  return (strcmp (pszOne, strTwo.AsChar ()) != 0);
  };



//------------------------------------------------------------------------
RAVEN_EXPORT BOOL operator< (const RStr & strOne,
                             const RStr & strTwo)
  {
  return (strcmp (strOne.AsChar (), strTwo.AsChar ()) < 0);
  };


//------------------------------------------------------------------------
RAVEN_EXPORT BOOL operator< (const RStr &  strOne,
                             const char *  pszTwo)
  {
  return (strcmp (strOne.AsChar (), pszTwo) < 0);
  };


//------------------------------------------------------------------------
RAVEN_EXPORT BOOL operator< (const char *  pszOne,
                             const RStr &  strTwo)
  {
  return (strcmp (pszOne, strTwo.AsChar ()) < 0);
  };


//------------------------------------------------------------------------
RAVEN_EXPORT BOOL operator> (const RStr & strOne,
                             const RStr & strTwo)
  {
  return (strcmp (strOne.AsChar (), strTwo.AsChar ()) > 0);
  };


//------------------------------------------------------------------------
RAVEN_EXPORT BOOL operator> (const RStr &  strOne,
                             const char *  pszTwo)
  {
  return (strcmp (strOne.AsChar (), pszTwo) > 0);
  };


//------------------------------------------------------------------------
RAVEN_EXPORT BOOL operator> (const char *  pszOne,
                             const RStr &  strTwo)
  {
  return (strcmp (pszOne, strTwo.AsChar ()) > 0);
  };


//------------------------------------------------------------------------
RAVEN_EXPORT BOOL operator<= (const RStr & strOne,
                              const RStr & strTwo)
  {
  return (strcmp (strOne.AsChar (), strTwo.AsChar ()) <= 0);
  };


//------------------------------------------------------------------------
RAVEN_EXPORT BOOL operator<= (const RStr &  strOne,
                              const char *  pszTwo)
  {
  return (strcmp (strOne.AsChar (), pszTwo) <= 0);
  };


//------------------------------------------------------------------------
RAVEN_EXPORT BOOL operator<= (const char *  pszOne,
                              const RStr &  strTwo)
  {
  return (strcmp (pszOne, strTwo.AsChar ()) <= 0);
  };


//------------------------------------------------------------------------
RAVEN_EXPORT BOOL operator>= (const RStr & strOne,
                              const RStr & strTwo)
  {
  return (strcmp (strOne.AsChar (), strTwo.AsChar ()) >= 0);
  };


//------------------------------------------------------------------------
RAVEN_EXPORT BOOL operator>= (const RStr &  strOne,
                              const char *  pszTwo)
  {
  return (strcmp (strOne.AsChar (), pszTwo) >= 0);
  };


//------------------------------------------------------------------------
RAVEN_EXPORT BOOL operator>= (const char *  pszOne,
                              const RStr &  strTwo)
  {
  return (strcmp (pszOne, strTwo.AsChar ()) >= 0);
  };

















