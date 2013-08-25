/* -----------------------------------------------------------------------
                            PolarFlow String Parser

    The RStrParser class is used to create a RStr that has member functions
   for extracting, parsing, and manipulating the contents of the string.

                            Michael T. Duffy
   ----------------------------------------------------------------------- */

// RStrParser.cpp
// Copyright 2002, 2008, Michael T. Duffy.  
// contact:  mduffor@users.sourceforge.net

// This file is part of PolarFlow.
//
// PolarFlow is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 3 as
// published by the Free Software Foundation.
//
// PolarFlow is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with PolarFlow.  If not, see <http://www.gnu.org/licenses/>.


// *****************************************************************************
//  Compile Directives
// *****************************************************************************

// #pragma pack (1);

// *****************************************************************************
//  Header Files
// *****************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#ifdef LINUX
#include <unistd.h>
#endif

#include "Types.hpp"

#include "Debug.hpp"
ASSERTFILE (__FILE__)

#include "RStrParser.hpp"
#include "FilePath.hpp"

const UINT32   RStrParser::kNULL         = 0x00;
const UINT32   RStrParser::kLF           = 0x0a;
const UINT32   RStrParser::kCR           = 0x0d;
const UINT32   RStrParser::kSPACE        = 0x20;
const UINT32   RStrParser::kTAB          = 0x09;
const UINT32   RStrParser::kDOUBLEQUOTES = 0x22;
const UINT32   RStrParser::kSINGLEQUOTES = 0x27;
const UINT32   RStrParser::kESCAPE       = 0x5c;

RStr  operator*  (const RStrParser &  strIn, RegEx &  regexIn)    {return (regexIn.Match (strIn, strIn.GetCursorStart ()));};
RStr  operator*  (RegEx &  regexIn, const RStrParser &    strIn)  {return (regexIn.Match (strIn, strIn.GetCursorStart ()));};

//------------------------------------------------------------------------------
RStrParser::RStrParser  ()
  {

  Init ();
  };


//------------------------------------------------------------------------------
RStrParser::RStrParser  (const RStr &   strIn)
  {
  Init ();
  Set (strIn);
  };


//------------------------------------------------------------------------------
RStrParser::RStrParser  (const RStrParser &   parserIn)
  {
  Init ();
  Set (parserIn);
  };


//------------------------------------------------------------------------------
RStrParser::RStrParser  (const char *   pszIn)
  {
  Init ();
  Set (pszIn);
  };


//------------------------------------------------------------------------------
RStrParser::~RStrParser  ()
  {
  };


//------------------------------------------------------------------------------
VOID  RStrParser::Init  (VOID)
  {
  eSkipComments = kNone;
  ResetCursor  ();

  bBufferedWriting     = false;
  strBufferedFilename  = "";
  iBufferedWriteSize   = 4096;

  bGreedyRead = true;
  };


//------------------------------------------------------------------------------
VOID  RStrParser::ResetCursor  (VOID)
  {

  iCursorStart = 0;
  iCursorEnd   = 0;
  iLineEnd     = 0;
  FindLineEnd ();
  };


//------------------------------------------------------------------------------
VOID  RStrParser::FindLineEnd  (VOID)
  {

  iLineEnd = -1;
  INT32  iEndSearch = iCursorStart;

  while (! IsEOL (iEndSearch))
    {
    if (IsComment (iEndSearch))
      {
      break;
      };
    ++iEndSearch;
    };
  iLineEnd = iEndSearch;
  };




//------------------------------------------------------------------------------
INT  RStrParser::SkipWhitespace  (BOOL  bSkipEOL)
  {
  INT  iStartingPos = iCursorStart;
  
  
  if (IsEOF (iCursorStart)) return (0);
  
  UINT32   uCurr;

  while (! IsEOF (iCursorStart))
    {
    uCurr = GetAt (iCursorStart);
    
    if (IsWhitespace (uCurr) ||
        ((bSkipEOL) && (uCurr == kLF)) ||
        ((bSkipEOL) && (uCurr == kCR)) )
      {
      ++iCursorStart;
      }
    else if (IsComment (iCursorStart))
      {
      // skip comments as whitespace, if comment skipping is turned on.
      SkipComment ();
      if (bSkipEOL)
        {
        GotoNextLine ();
        }
      else
        {
        break;
        };
      }
    else
      {
      break;
      };
    };
  return (iCursorStart - iStartingPos);
  };


//------------------------------------------------------------------------------
VOID  RStrParser::SkipEOL  (VOID)
  {
  UINT32   uCurr = GetAt (iCursorStart);

  if (uCurr == kCR)
    {
    ++iCursorStart;
    uCurr = GetAt (iCursorStart);
    };

  if (uCurr == kLF)
    {
    ++iCursorStart;
    };
  FindLineEnd ();
  };


//------------------------------------------------------------------------------
BOOL  RStrParser::IsEOL  (INT32  iPosition)
  {
  if (iPosition == -1)  iPosition = iCursorStart;

  if (IsEOF (iPosition)) return (true);

  UINT32   uCurr = GetAt (iPosition);

  return ((uCurr == kNULL) ||
          (uCurr == kLF) ||
          (uCurr == kCR) ||
          (IsEOFAscii (iPosition)) ||
          (iPosition == iLineEnd));
  };


//------------------------------------------------------------------------------
BOOL  RStrParser::IsEOF  (INT32  iPosition)
  {
  if (iPosition == -1)  iPosition = iCursorStart;

  if (uStringLength == 0) return (true);

  return (iPosition >= static_cast <int> (uStringLength));
  };


//------------------------------------------------------------------------------
BOOL  RStrParser::IsEOFAscii  (INT32  iPosition)
  {
  if (iPosition == -1)  iPosition = iCursorStart;

  if (uStringLength == 0) return (true);

  if (iPosition >= static_cast <int> (uStringLength)) return (true);
  
  if (GetAt (iPosition) == kNULL) return (true);
  
  return (false);
  };


//------------------------------------------------------------------------------
BOOL  RStrParser::GotoNextLine  (VOID)
  {
  if (IsEOF ()) return (false);

  // reset the line end so it won't trigger the EOL.
  iLineEnd = -1;

  // skip until an actual EOL
  while (! IsEOL ())
    {
    ++iCursorStart;
    };
  SkipEOL ();

  // calculate the line end.
  FindLineEnd ();

  return (true);
  };


//------------------------------------------------------------------------------
RStr  RStrParser::GetLine  (VOID)
  {
  // read from the current position until the line end, and return
  //  a RStr with that info.
  FindLineEnd ();

  INT32  iStringLength = iLineEnd - iCursorStart;
  RStr   strOut ("");

  GetMiddle (iCursorStart, iStringLength, strOut);
  SkipChars (strOut.GetLength ());
  return (strOut);
  };


//------------------------------------------------------------------------------
RStr  RStrParser::GetWord  (BOOL          bSkipEOL,
                            const char *  szAlsoInvalidChars)
  {
  RStr   strOut;


  GetWord (strOut, bSkipEOL, szAlsoInvalidChars);

  return (strOut);
  };


//------------------------------------------------------------------------------
VOID  RStrParser::GetWord  (RStr &        strOut,
                            BOOL          bSkipEOL,
                            const char *  szAlsoInvalidChars)
  {
  GetWord (&strOut, bSkipEOL, szAlsoInvalidChars);
  };


//------------------------------------------------------------------------------
VOID  RStrParser::GetWord  (RStr *        pstrOut,
                            BOOL          bSkipEOL,
                            const char *  szAlsoInvalidChars)
  {

  // Note:  GetWord needs to properly handle strings contained within quotes.
  //  to be implemented
  
  //UINT32  uCharCurr;

  SkipWhitespace (bSkipEOL);

  // check for quoted strings
  if ((PeekChar () == kDOUBLEQUOTES) ||
      (PeekChar () == kSINGLEQUOTES))
    {
    GetQuoteString  (pstrOut, bSkipEOL);
    }
  else
    {
    // normal word

    UINT32   uCurr = GetAt (iCursorStart);
    INT32    iCopyStart = iCursorStart;

    while ((! IsWhitespace (uCurr)) &&
           (! IsEOL ()            ))
      {
      if (szAlsoInvalidChars != NULL)
        {
        if (strchr (szAlsoInvalidChars, uCurr) != NULL)
          {
          // found a char we want to signal end of word
          break;
          };
        };
      
      
      ++iCursorStart;
      uCurr = GetAt (iCursorStart);
      };

    if (pstrOut != NULL)
      {
      INT32  iStringLength = iCursorStart - iCopyStart;
      GetMiddle (iCopyStart, iStringLength, *pstrOut);
      };
    };

  SkipWhitespace (bSkipEOL);
  };


//------------------------------------------------------------------------------
RStr  RStrParser::GetAlphaNum  (BOOL  bSkipEOL)
  {
  RStr   strOut;


  GetAlphaNum (strOut, bSkipEOL);

  return (strOut);
  };


//------------------------------------------------------------------------------
VOID  RStrParser::GetAlphaNum  (RStr &  strOut,
                                BOOL    bSkipEOL)
  {
  GetAlphaNum (&strOut, bSkipEOL);
  };


//------------------------------------------------------------------------------
VOID  RStrParser::GetAlphaNum  (RStr *  pstrOut,
                                BOOL    bSkipEOL)
  {

  // Note:  GetAlphaNum needs to properly handle strings contained within quotes.
  //  to be implemented

  // this routine reads A-Za-z0-9_
  //UINT32  uCharCurr;


  SkipWhitespace (bSkipEOL);

  // check for quoted strings
  if ((PeekChar () == kDOUBLEQUOTES) ||
      (PeekChar () == kSINGLEQUOTES))
    {
    GetQuoteString  (pstrOut, bSkipEOL);
    }
  else
    {
    // normal word

    UINT32   uCurr = GetAt (iCursorStart);
    INT32    iCopyStart = iCursorStart;

    while ((! IsWhitespace (uCurr)) &&
           ((isalnum (uCurr)) || (uCurr == '_')) && 
           (! IsEOL ()            ))
      {
      ++iCursorStart;
      uCurr = GetAt (iCursorStart);
      };

    if (pstrOut != NULL)
      {
      INT32  iStringLength = iCursorStart - iCopyStart;
      GetMiddle (iCopyStart, iStringLength, *pstrOut);
      };
    };

  SkipWhitespace (bSkipEOL);
  };



//------------------------------------------------------------------------------
INT32  RStrParser::GetChar (VOID)
  {

  // extract the next character
  SkipWhitespace ();

  INT32   iCurr = GetAt (iCursorStart);

  if (! IsEOL ())
    {
    ++iCursorStart;
    };

  return (iCurr);
  };


//------------------------------------------------------------------------------
INT32  RStrParser::GetInt (BOOL  bSkipEOL)
  {
  RStr   strWord;

  // extract the characters that make up a valid number
  SkipWhitespace (bSkipEOL);

  UINT32   uCurr = GetAt (iCursorStart);
  INT32    iCopyStart = iCursorStart;

  while ( ((isdigit (uCurr)) ||
           (((uCurr == '-') || (uCurr == '+')) && (iCopyStart == iCursorStart)) ||
           (((uCurr == 'x') || (uCurr == 'X')) && (iCopyStart == iCursorStart + 1)) )
               &&
          (! IsEOL ()) )
    {
    ++iCursorStart;
    uCurr = GetAt (iCursorStart);
    };

  INT32  iStringLength = iCursorStart - iCopyStart;
  GetMiddle (iCopyStart, iStringLength, strWord);

  SkipWhitespace (bSkipEOL);

  return (static_cast <INT32> (strtol (strWord.AsChar (), NULL, 10)));
  };


//------------------------------------------------------------------------------
BOOL  RStrParser::IsInt (BOOL  bSkipEOL)
  {
  INT32  iSavedPos = iCursorStart;

  // extract the characters that make up a valid number
  SkipWhitespace (bSkipEOL);

  UINT32   uCurr = GetAt (iCursorStart);
  INT32    iCopyStart = iCursorStart;

  while ( ((isdigit (uCurr)) ||
           (((uCurr == '-') || (uCurr == '+')) && (iCopyStart == iCursorStart)) ||
           (((uCurr == 'x') || (uCurr == 'X')) && (iCopyStart == iCursorStart + 1)) )
               &&
          (! IsEOL ()) )
    {
    ++iCursorStart;
    uCurr = GetAt (iCursorStart);
    };

  INT32  iStringLength = iCursorStart - iCopyStart;

  iCursorStart = iSavedPos;

  // special case out strings that start with a minus sign.
  if ((iStringLength == 1) && (PeekChar () == '-')) return (FALSE);
    
  return (iStringLength > 0);
  };


//------------------------------------------------------------------------------
UINT32  RStrParser::GetUInt  (BOOL  bSkipEOL)
  {
  RStr   strWord;

  // extract the characters that make up a valid number
  SkipWhitespace (bSkipEOL);

  UINT32   uCurr = GetAt (iCursorStart);
  INT32    iCopyStart = iCursorStart;

  while ( ((isdigit (uCurr)) ||
           (((uCurr == '-') || (uCurr == '+')) && (iCopyStart == iCursorStart)) ||
           (((uCurr == 'x') || (uCurr == 'X')) && (iCopyStart == iCursorStart + 1)) )
               &&
          (! IsEOL ()) )
    {
    ++iCursorStart;
    uCurr = GetAt (iCursorStart);
    };

  INT32  iStringLength = iCursorStart - iCopyStart;
  GetMiddle (iCopyStart, iStringLength, strWord);

  SkipWhitespace (bSkipEOL);

  return (static_cast <UINT32> (strtoul (strWord.AsChar (), NULL, 10)));
  };


//------------------------------------------------------------------------------
BOOL  RStrParser::IsUInt  (BOOL  bSkipEOL)
  {
  INT32  iSavedPos = iCursorStart;

  // extract the characters that make up a valid number
  SkipWhitespace (bSkipEOL);

  UINT32   uCurr = GetAt (iCursorStart);
  INT32    iCopyStart = iCursorStart;

  while ( ((isdigit (uCurr)) ||
           (((uCurr == '-') || (uCurr == '+')) && (iCopyStart == iCursorStart)) ||
           (((uCurr == 'x') || (uCurr == 'X')) && (iCopyStart == iCursorStart + 1)) )
               &&
          (! IsEOL ()) )
    {
    ++iCursorStart;
    uCurr = GetAt (iCursorStart);
    };

  INT32  iStringLength = iCursorStart - iCopyStart;
  
  iCursorStart = iSavedPos;
  
  // special case out strings that start with a minus sign.
  if ((iStringLength == 1) && (PeekChar () == '-')) return (FALSE);

  return (iStringLength > 0);
  };


//------------------------------------------------------------------------------
FLOAT  RStrParser::GetFloat  (BOOL  bSkipEOL)
  {
  RStr   strWord ("");

  // extract the characters that make up a valid number
  SkipWhitespace (bSkipEOL);

  UINT32   uCurr = GetAt (iCursorStart);
  INT32    iCopyStart = iCursorStart;

  while ( ((isdigit (uCurr)) ||
           (uCurr == 'e') || (uCurr == 'E') ||
           (((uCurr == '-') || (uCurr == '+')) && ((iCopyStart == iCursorStart) || ( (GetAt (iCursorStart-1) == 'e') || (GetAt (iCursorStart-1) == 'E'))  )) ||
           (uCurr == '.') )
               &&
          (! IsEOL ()) )
    {
    ++iCursorStart;
    uCurr = GetAt (iCursorStart);
    };

  INT32  iStringLength = iCursorStart - iCopyStart;
  if (iStringLength)
    {
    GetMiddle (iCopyStart, iStringLength, strWord);
    };

  SkipWhitespace (bSkipEOL);

  return (FLOAT (strtod (strWord.AsChar (), NULL)));
  };


//------------------------------------------------------------------------------
BOOL  RStrParser::IsFloat  (BOOL  bSkipEOL)
  {
  INT32  iSavedPos = iCursorStart;

  // extract the characters that make up a valid number
  SkipWhitespace (bSkipEOL);

  UINT32   uCurr = GetAt (iCursorStart);
  INT32    iCopyStart = iCursorStart;

  while ( ((isdigit (uCurr)) ||
           (uCurr == 'e') || (uCurr == 'E') ||
           (((uCurr == '-') || (uCurr == '+')) && ((iCopyStart == iCursorStart) || ( (GetAt (iCursorStart-1) == 'e') || (GetAt (iCursorStart-1) == 'E'))  )) ||
           (uCurr == '.') )
               &&
          (! IsEOL ()) )
    {
    ++iCursorStart;
    uCurr = GetAt (iCursorStart);
    };

  INT32  iStringLength = iCursorStart - iCopyStart;
  
  iCursorStart = iSavedPos;
  
  // special case out strings that start with a minus sign.
  if ((iStringLength == 1) && (PeekChar () == '-')) return (FALSE);
  
  return (iStringLength > 0);
  };


//------------------------------------------------------------------------------
DOUBLE  RStrParser::GetDouble  (BOOL  bSkipEOL)
  {
  RStr   strWord ("");

  // extract the characters that make up a valid number
  SkipWhitespace (bSkipEOL);

  UINT32   uCurr = GetAt (iCursorStart);
  INT32    iCopyStart = iCursorStart;

  while ( ((isdigit (uCurr)) ||
           (uCurr == 'e') || (uCurr == 'E') ||
           (((uCurr == '-') || (uCurr == '+')) && (iCopyStart == iCursorStart)) ||
           (uCurr == '.')  )
               &&
          (! IsEOL ()) )
    {
    ++iCursorStart;
    uCurr = GetAt (iCursorStart);
    };

  INT32  iStringLength = iCursorStart - iCopyStart;
  if (iStringLength)
    {
    GetMiddle (iCopyStart, iStringLength, strWord);
    };

  SkipWhitespace (bSkipEOL);

  return (strtod (strWord.AsChar (), NULL));
  };


//------------------------------------------------------------------------------
VOID  RStrParser::GetQuoteString  (RStr *  pstrOut,
                                   BOOL    bSkipEOL)
  {

  // Note:  May still need to deal with slashes on the end of lines to
  //         split strings across multiple lines.
  //  to be implemented
  UINT32       uCharCurr;

  if (pstrOut != NULL)
    {
    *pstrOut = "";
    };

  if (PeekChar () == kDOUBLEQUOTES)
    {
    // Get everything between the quotes, and unescape escaped characters.
    SkipChars (1);

    while (! IsEOF ())
      {
      uCharCurr = PeekChar ();
      SkipChars (1);
      if (uCharCurr == kDOUBLEQUOTES)
        {
        break;
        };
        
      // pass escaped quotes explicitly so they don't terminate parsing.
      if (uCharCurr == kESCAPE)
        {
        uCharCurr = PeekChar ();
        SkipChars (1);
        if (pstrOut != NULL)
          {
          pstrOut->AppendChar (kESCAPE);
          };
        };
        
      
      // Note:  I'm skipping translation of escaped characters here
      //        and leaving them escaped, so that they may be passed
      //        to library functions.  
      if (uCharCurr == kESCAPE)
        {
        uCharCurr = PeekChar ();
        SkipChars (1);
        switch (uCharCurr)
          {
          case 'a':  uCharCurr = '\a'; break;
          case 'b':  uCharCurr = '\b'; break;
          case 'f':  uCharCurr = '\f'; break;
          case 'n':  uCharCurr = '\n'; break;
          case 'r':  uCharCurr = '\r'; break;
          case 't':  uCharCurr = '\t'; break;
          case 'v':  uCharCurr = '\v'; break;
          case '0':  uCharCurr = '\0'; break;
          default:   break;
          };
        };
      
      if (pstrOut != NULL)
        {
        pstrOut->AppendChar (uCharCurr);
        };
      };
    }
  else if (PeekChar () == kSINGLEQUOTES)
    {
    // Get everything between the quotes, and unescape escaped characters.
    SkipChars (1);

    while (! IsEOF ())
      {
      uCharCurr = PeekChar ();
      SkipChars (1);
      // pass escaped quotes explicitly so they don't terminate parsing.
      if (uCharCurr == kSINGLEQUOTES)
        {
        break;
        };
      if (uCharCurr == kESCAPE)
        {
        uCharCurr = PeekChar ();
        SkipChars (1);
        if (pstrOut != NULL)
          {
          pstrOut->AppendChar (kESCAPE);
          };
        };
        
      if (pstrOut != NULL)
        {
        pstrOut->AppendChar (uCharCurr);
        };
      };
    }
  else
    {
    // get everything until the end of the line.
    
    while (! IsEOL ())
      {
      if ((! bGreedyRead) && (IsWhitespace (PeekChar ())))
        {
        break;
        };
      
      if (pstrOut != NULL)
        {
        pstrOut->AppendChar (PeekChar ());
        };
      SkipChars (1);
      };
    };

  SkipWhitespace (bSkipEOL);
  };


//------------------------------------------------------------------------------
VOID  RStrParser::IncIntAtCursor (INT  iMultiplierIn)
  {
  // calc which column the cursor is under
  if (isdigit (GetAt (iCursorStart)))
    {
    StripTrailingWhitespace (TRUE);
    INT  iOrigLength = GetLength ();
    INT  iTensExp = iOrigLength - iCursorStart - 1;
    
    iCursorStart = 0;
    INT  iValueOrig = GetInt ();
    INT  iValue = iValueOrig + iMultiplierIn * (INT) pow (10, iTensExp);
    // debugging
    //printf ("Orig %d  New %d  Tens %d\n", iValueOrig, iValue, iTensExp);
    Format ("%d", iValue);
    INT  iNewLength = GetLength ();

    // now try to put the cursor under the appropriate digit
    iCursorStart = iNewLength - iTensExp - 1;
    };
  };


//------------------------------------------------------------------------------
VOID  RStrParser::IncFloatAtCursor (FLOAT fMultiplierIn)
  {
  // calc which column the cursor is under
  if (isdigit (GetAt (iCursorStart)))
    {
    StripTrailingWhitespace (TRUE);
    INT  iOrigLength = GetLength ();
    INT  iDecimal = FindChar ('.');
    INT  iTensExp = iOrigLength - iCursorStart - 1;
    if (iDecimal != -1)
      {
      // there is a decimal point.  Calc the correct tens exponent.
      if (iCursorStart < iDecimal)
        {
        // cursor over whole number part
        iTensExp = iDecimal - iCursorStart - 1;
        }
      else
        {
        // cursor over fractional part
        //               2            1
        iTensExp = iDecimal - iCursorStart;
        };
      };
    
    iCursorStart = 0;
    FLOAT  fValueOrig = GetFloat ();
    FLOAT  fValue = fValueOrig + fMultiplierIn * FLOAT (pow (10.0, (DOUBLE) iTensExp));
    // debugging
    //printf ("Orig %d  New %d  Tens %d\n", iValueOrig, iValue, iTensExp);
    Format ("%f", fValue);
    
    INT  iNewDecimal = FindChar ('.');
    if (iNewDecimal == -1)
      {
      // no decimal.  set decimal position to end of string.
      iNewDecimal = GetLength ();
      };
    
    // now try to put the cursor under the appropriate digit
    if (iTensExp >= 0)
      {
      iCursorStart = iNewDecimal - iTensExp - 1;
      }
    else
      {
      iCursorStart = iNewDecimal - iTensExp;
      };
    };
  };


//------------------------------------------------------------------------------
VOID  RStrParser::GetBracketString  (RStr &  strOut,
                                     RStr    strOpenBracket,
                                     RStr    strCloseBracket,
                                     BOOL    bSkipEOL)
  {

  // Note:  This is similar to GetQuoteString, except that you can define
  //         what the opening and closing bracket or quote characters are

  // Note:  May still need to deal with slashes on the end of lines to
  //         split strings across multiple lines.
  //  to be implemented

  UINT32   uCharCurr;

  strOut = "";

  INT32  iSearch;

  iSearch = strOpenBracket.FindChar (PeekChar ());
  if (iSearch != -1)
    {
    // Get everything between the brackets, and unescape escaped characters.
    SkipChars (1);

    while (! IsEOFAscii ())
      {
      uCharCurr = PeekChar ();
      SkipChars (1);
      if (uCharCurr == strCloseBracket [iSearch])
        {
        break;
        };
        
      if (uCharCurr == kESCAPE)
        {
        uCharCurr = PeekChar ();
        SkipChars (1);
        switch (uCharCurr)
          {
          case 'a':  uCharCurr = '\a'; break;
          case 'b':  uCharCurr = '\b'; break;
          case 'f':  uCharCurr = '\f'; break;
          case 'n':  uCharCurr = '\n'; break;
          case 'r':  uCharCurr = '\r'; break;
          case 't':  uCharCurr = '\t'; break;
          case 'v':  uCharCurr = '\v'; break;
          case '0':  uCharCurr = '\0'; break;
          default:   break;
          };
        };
      
      strOut.AppendChar (uCharCurr);
      };
    }
  else
    {
    // get everything until the end of the line.
    
    while (! IsEOL ())
      {
      if ((! bGreedyRead) && (IsWhitespace (PeekChar ())))
        {
        break;
        };
      
      strOut.AppendChar (PeekChar ());
      SkipChars (1);
      };
    };

  SkipWhitespace (bSkipEOL);
  };


//------------------------------------------------------------------------------
VOID  RStrParser::MakeQuoteString (VOID)
  {
  MakeEscapedString (TRUE);
  };
  
  
//------------------------------------------------------------------------------
VOID  RStrParser::MakeEscapedString (BOOL   bAddQuotes)
  {
  RStr    strQuoted ;

  // guesstimate that 1 out of every 10 characters needs to be escaped, and preallocate for that
  strQuoted.GrowAbsolute (this->GetLength () + (this->GetLength () / 10) );

  if (bAddQuotes) {strQuoted.AppendChar (kDOUBLEQUOTES);};

  for (UINT32  uIndex = 0; uIndex < uStringLength; ++uIndex)
    {
    UINT32  uChar = GetAt (uIndex);
    
    // skip stuff that is already escaped
    if (uChar == kESCAPE)
      {
      strQuoted.AppendChar (uChar);
      ++uIndex;
      uChar = GetAt (uIndex);
      strQuoted.AppendChar (uChar);
      continue;
      };
    
    switch (uChar)
      {
      case kDOUBLEQUOTES:  {strQuoted.AppendChar (kESCAPE);}; break;
      case kESCAPE:        {strQuoted.AppendChar (kESCAPE);}; break;
      case '\a':           {strQuoted.AppendChar (kESCAPE); uChar = 'a';}; break;
      case '\b':           {strQuoted.AppendChar (kESCAPE); uChar = 'b';}; break;
      case '\f':           {strQuoted.AppendChar (kESCAPE); uChar = 'f';}; break;
      case '\n':           {strQuoted.AppendChar (kESCAPE); uChar = 'n';}; break;
      case '\r':           {strQuoted.AppendChar (kESCAPE); uChar = 'r';}; break;
      case '\t':           {strQuoted.AppendChar (kESCAPE); uChar = 't';}; break;
      case '\v':           {strQuoted.AppendChar (kESCAPE); uChar = 'v';}; break;
      case '\0':           {strQuoted.AppendChar (kESCAPE); uChar = '0';}; break;
      default: break;
      };
    strQuoted.AppendChar (uChar);
    };

  if (bAddQuotes) {strQuoted.AppendChar (kDOUBLEQUOTES);};
  *this = strQuoted;
  };


//------------------------------------------------------------------------------
UINT32  RStrParser::PeekChar  (INT32  iPosition)
  {

  if (iPosition == -1)  iPosition = iCursorStart;

  return (GetAt (iPosition));
  };


//------------------------------------------------------------------------------
UINT32  RStrParser::ReadChars  (PCHAR    pszBufferOut,
                                INT32    iCharsToCopy)
  {
  INT32  lBytesLeft = GetLength () - iCursorStart;

  iCharsToCopy = TMin (iCharsToCopy, lBytesLeft);


  if (iCharsToCopy > 0)
    {
    memcpy (pszBufferOut, GetString () + iCursorStart, iCharsToCopy);

    iCursorStart += iCharsToCopy;
    };

  return (TMax (iCharsToCopy, 0));
  };


//------------------------------------------------------------------------------
VOID  RStrParser::WriteChars  (PCHAR    pszBufferIn,
                               INT32    iCharsToCopy)
  {
  AppendChars (pszBufferIn, UINT32 (iCharsToCopy));
  iCursorStart = uStringLength;
  };


//------------------------------------------------------------------------------
VOID  RStrParser::StripComments  (VOID)
  {
  // to be implemented
  };


  
//------------------------------------------------------------------------------
EStatus  RStrParser::ReadFromFile  (const char *  szFilenameIn,
                                    INT           iStartIndex,
                                    INT           iMaxBytesToRead)
  {
  

  UINT    uFileSize    = FilePath::GetFileSize (szFilenameIn);
  INT     iBytesToRead = uFileSize;

  if (iMaxBytesToRead > 0)
    {
    iBytesToRead = TMin ((UINT32) iMaxBytesToRead, uFileSize);
    };

  GrowAbsolute (iBytesToRead + 1);
  
  EStatus  rStatus = FilePath::ReadFromFile  (szFilenameIn,
                                               iStartIndex,
                                               iBytesToRead,
                                               (unsigned char *) pszBuffer);
                                               
  uStringLength = iBytesToRead;
  iCursorStart  = 0;
  iCursorEnd    = 0;
  iLineEnd      = iBytesToRead;
  pszBuffer [iBytesToRead] = '\0';

  return (rStatus);
  };


//------------------------------------------------------------------------------
EStatus  RStrParser::WriteToFile  (const char *  szFilenameIn,
                                   INT           iStartOffsetIn,
                                   INT           iBytesToWriteIn)
  {
  // Note:  You eventually need to create a progressive write function so that
  //         you don't have to keep the entire write buffer in memory before
  //         it is sent to disk.  This will matter when you start dealing with
  //         large datasets.

  INT                iBytesToWrite = iBytesToWriteIn;
  
  if (iBytesToWrite == -1)
    {
    iBytesToWrite = uStringLength;
    };
  
  return (FilePath::WriteToFile (szFilenameIn,
                                  iStartOffsetIn,
                                  iBytesToWriteIn,
                                  (unsigned char *) pszBuffer));
  };
                                   
                                   
#ifdef DEPRECATED
  
//------------------------------------------------------------------------------
EStatus  RStrParser::ReadFromFile  (RStr  strFilenameIn,
                                    INT   iStartIndex,
                                    INT   iMaxBytesToRead)
  {
  FILE *             fp;
  RStr               strFilenameFinal = strFilenameIn;
  UINT32             uBytesToRead;


  // Make sure input values are valid
  //ASSERT (strFilenameFinal != "");
  if (strFilenameFinal == RStr("")) return EStatus (EStatus::kFailure, "RStrParser::ReadFromFile - empty filename passed");

  #ifdef WIN32
  strFilenameFinal.ReplaceChar ('/', '\\');
  #endif

  fp = fopen (strFilenameFinal.AsChar (), "rb");
  if (fp == NULL)
    {
    RStr  strOut;
    //sprintf (szDbgBuffer, "Unable to open file %s", szFilename); DebugMessage (szDbgBuffer);
    strOut.Format ("RStrParser::ReadFromFile - Unable to open file %s", strFilenameFinal.AsChar ());
    return (EStatus (EStatus::kFailure, strOut.AsChar ()));
    };

  fseek (fp, 0, SEEK_END);
  UINT32  uSize = ftell (fp);
  fseek (fp, iStartIndex, SEEK_SET);

  if (iMaxBytesToRead > 0)
    {
    uBytesToRead = TMin ((UINT32) iMaxBytesToRead, uSize);
    }
  else
    {
    uBytesToRead = uSize;
    };

  GrowAbsolute (uBytesToRead + 1);

  if (fread (pszBuffer, 1, uBytesToRead, fp) != uBytesToRead)
    {
    fclose (fp);
    return (EStatus (EStatus::kFailure, "RStrParser::ReadFromFile - Unable to read entire file"));
    };

  uStringLength = uBytesToRead;
  iCursorStart  = 0;
  iCursorEnd    = 0;
  iLineEnd      = uBytesToRead;
  pszBuffer [uBytesToRead] = '\0';

  fclose (fp);
  return EStatus::kSuccess;
  };


//------------------------------------------------------------------------------
EStatus  RStrParser::WriteToFile  (RStr  strFilenameIn,
                                   INT   iStartOffsetIn,
                                   INT   iBytesToWriteIn)
  {
  // Note:  You eventually need to create a progressive write function so that
  //         you don't have to keep the entire write buffer in memory before
  //         it is sent to disk.  This will matter when you start dealing with
  //         large datasets.

  FILE *             fp;
  INT                iBytesToWrite = iBytesToWriteIn;
  RStr               strFilenameFinal = strFilenameIn;


  #ifdef WIN32
  strFilenameFinal.ReplaceChar ('/', '\\');
  #endif

  // Make sure input values are valid
  //ASSERT (strFilenameFinal != "");
  if (strFilenameFinal == RStr("")) return EStatus (EStatus::kFailure, "Empty Filename");

  fp = fopen (strFilenameFinal.AsChar (), "wb");
  if (fp == NULL)
    {
    //sprintf (szDbgBuffer, "Unable to open file %s", szFilename); DebugMessage (szDbgBuffer);
    RStr  strOut ("Unable to open file ");
    strOut += strFilenameFinal;
    return EStatus (EStatus::kFailure, strOut.AsChar ());
    };

  fseek (fp, 0, SEEK_END);
  int  iFileLength = ftell (fp);
  
  if (iStartOffsetIn > iFileLength)
    {
    // we are seeking to a position past the end of the file.  Grow the file
    //  to make this offset valid

    int   iGrowSize = iStartOffsetIn - iFileLength;
    char *  pchBlank = new char [iGrowSize];
    memset (pchBlank, 0, iGrowSize);

    if (fwrite (pchBlank, 1, iGrowSize, fp) != 1)
      {
      // error, couldn't write a blank buffer
      };
    delete [] pchBlank;
    };

  if (iBytesToWrite == -1)
    {
    iBytesToWrite = uStringLength;
    };

  // set the starting offset.
  fseek (fp, iStartOffsetIn, SEEK_SET);

  if (fwrite (pszBuffer, 1, iBytesToWrite, fp) != (unsigned int) iBytesToWrite)
    {
    fclose (fp);

    RStr  strOut ("Unable to write to file ");
    strOut += strFilenameFinal;
    return EStatus (EStatus::kFailure, strOut.AsChar ());

    };

  fclose (fp);
  return EStatus::kSuccess;
  };
#endif // DEPRECATED
  
//------------------------------------------------------------------------------
EStatus  RStrParser::SkipComment  (VOID)
  {
  while (! IsEOF ())
    {
    if (IsEOLComment (iCursorStart))
      {
      // skip EOL comment
      
      // reset the line end so it won't trigger the EOL.
      iLineEnd = -1;

      // skip until an actual EOL
      while (! IsEOL ())
        {
        ++iCursorStart;
        };
      }
    else if (IsBlockComment (iCursorStart))
      {
      // skip block comment
      iCursorStart += GetBlockCommentTagLength ();
      while ((! IsEOF ()) && (! IsBlockCommentEnd  (iCursorStart)))
        {
        ++iCursorStart;
        };
      if (IsBlockCommentEnd  (iCursorStart))
        {
        iCursorStart += GetBlockCommentTagLength ();
        }
      else
        {
        // unable to find end of block comment
        EStatus (EStatus::kFailure, "RStrParser::SkipComment (): Unable to find end of block comment");
        };       
      }
    else
      {
      // didn't find either comment type.  Done skipping comments.
      break;
      };
    };
  return (EStatus::kSuccess);
  };

  
//------------------------------------------------------------------------------
BOOL  RStrParser::IsComment  (INT  iIndexIn)
  {
  if (IsEOLComment (iIndexIn) ||
      IsBlockComment (iIndexIn))
    {
    return (TRUE);
    };
  return (FALSE);  
  };


//------------------------------------------------------------------------------
BOOL  RStrParser::IsBlockComment  (INT  iIndexIn)
  {
  if (eSkipComments == kCStyle)
    {
    // check for C-style Block comments

    UINT32  uCurr = GetAt (iIndexIn);
    if (uCurr == '/')
      {
      uCurr = GetAt (iIndexIn + 1);
      if (uCurr == '*')
        {
        // found a block comment
        return (TRUE);
        };
      }
    }
  else if (eSkipComments == kShellStyle)
    {
    // check for python-style block comments, since the shell doesn't have
    //  block commenting.

    UINT32  uCurr = GetAt (iIndexIn);
    if (uCurr == '"')
      {
      uCurr = GetAt (iIndexIn + 1);
      if (uCurr == '"')
        {
        uCurr = GetAt (iIndexIn + 2);
        if (uCurr == '"')
          {
          // found a block comment
          return (TRUE);
          };
        };
      };
    };
  return (FALSE);
  };

  
//------------------------------------------------------------------------------
BOOL  RStrParser::IsBlockCommentEnd  (INT  iIndexIn)
  {
  if (eSkipComments == kCStyle)
    {
    // check for C-style Block comments

    UINT32  uCurr = GetAt (iIndexIn);
    if (uCurr == '*')
      {
      uCurr = GetAt (iIndexIn + 1);
      if (uCurr == '/')
        {
        // found a block comment
        return (TRUE);
        };
      }
    }
  else if (eSkipComments == kShellStyle)
    {
    // check for python-style block comments, since the shell doesn't have
    //  block commenting.

    UINT32  uCurr = GetAt (iIndexIn);
    if (uCurr == '"')
      {
      uCurr = GetAt (iIndexIn + 1);
      if (uCurr == '"')
        {
        uCurr = GetAt (iIndexIn + 2);
        if (uCurr == '"')
          {
          // found a block comment
          return (TRUE);
          };
        };
      };
    };
  return (FALSE);
  };

  
//------------------------------------------------------------------------------
INT  RStrParser::GetBlockCommentTagLength (VOID)
  {
  if (eSkipComments == kCStyle)
    {
    return (2);
    }
  else if (eSkipComments == kShellStyle)
    {
    return (3);
    };
  return (0);
  };

  
//------------------------------------------------------------------------------
BOOL  RStrParser::IsEOLComment  (INT  iIndexIn)
  {
  if (eSkipComments == kCStyle)
    {
    // check for C-style EOL comments

    UINT32  uCurr = GetAt (iIndexIn);
    if (uCurr == '/')
      {
      uCurr = GetAt (iIndexIn + 1);
      if (uCurr == '/')
        {
        // found an EOL comment
        return (TRUE);
        };
      }
    }
  else if (eSkipComments == kShellStyle)
    {
    // check for shell-style EOL comments

    UINT32  uCurr = GetAt (iIndexIn);
    if (uCurr == '#')
      {
      // found an EOL comment
      return (TRUE);
      };
    };
  return (FALSE);
  };


//------------------------------------------------------------------------------
UINT32  RStrParser::GetTrailingUInt (RStr *  pstrDigits)
  {
  FindLineEnd ();
  INT32   iIndex      = iLineEnd - 1;
  INT32   iMultiplier = 1;
  INT32   iReturn = 0;

  if (pstrDigits != NULL)
    {
    pstrDigits->Empty ();
    };

  while (isdigit (GetAt (iIndex)))
    {
    if (pstrDigits != NULL)
      {
      pstrDigits->PrependChar (GetAt (iIndex));
      };

    iReturn += iMultiplier * CharToInt (iIndex);

    --iIndex;
    iMultiplier *= 10;
    };
  return (iReturn);
  };


//------------------------------------------------------------------------------
UINT32 RStrParser::Tokenize  (UINT32       uSeparatorIn,
                              RStrArray &  arrayOut,
                              BOOL         bStripWhitespace,
                              BOOL         bSkipEOL,
                              BOOL         bKeepEmptyStrings)
  {
  UINT32     uCurr;
  INT32      iCopyStart;
  UINT32     uNumStringsFound = 0;


  // if we are tokenizing on whitespace, be sure not to skip it.
  if (uSeparatorIn == ' ') bStripWhitespace = false;

  while (! IsEOFAscii ())
    {
    // skip leading whitespace
    if (bStripWhitespace) SkipWhitespace (bSkipEOL);

    // search for the separator
    iCopyStart = iCursorStart;
    while (uSeparatorIn != (uCurr = GetAt (iCursorStart)))
      {
      if (uCurr == kDOUBLEQUOTES)
        {
        // skip the quoted string
        ++iCursorStart;
        while ( (! IsEOL ()) && (GetAt (iCursorStart) != kDOUBLEQUOTES) )
          {
          // escaped sequences skip two characters
          if (GetAt (iCursorStart) == kESCAPE)
            {
            ++iCursorStart;
            };
          ++iCursorStart;
          };
        };

      ++iCursorStart;
      if ((uSeparatorIn != '\n') && (IsEOL ())) break;
      };

    INT32  iStringLength = iCursorStart - iCopyStart;

    if ((iStringLength > 0) || (bKeepEmptyStrings))
      {
      arrayOut.Append (RStr (""));
      RStr &  strOut = arrayOut [arrayOut.Length () - 1];

      if (iStringLength > 0)
        {
        GetMiddle (iCopyStart, iStringLength, strOut);
        };
      if (bStripWhitespace)
        {
        strOut.StripTrailingWhitespace ();
        };
      ++uNumStringsFound;
      };


    // skip the separator
    ++iCursorStart;

    // skip trailing whitespace
    if (bStripWhitespace)
      {
      SkipWhitespace (bSkipEOL);
      };

    // break if we are at the end of a line, unless we are supposed to skip EOLs
    if ((uSeparatorIn != '\n') && (IsEOL ()))
      {
      break;
      };
    };

  return (uNumStringsFound);
  };




//------------------------------------------------------------------------------
VOID  RStrParser::StripTrailingWhitespace (BOOL  bStripEOL)
  {
  while ((uStringLength > 0) &&
         ((IsWhitespace (pszBuffer [uStringLength - 1])) ||
          (bStripEOL && ((UINT32 (pszBuffer [uStringLength - 1]) == kLF) || (UINT32 (pszBuffer [uStringLength - 1]) == kCR)) )
         )
        )
    {
    --uStringLength;
    };
  };


//------------------------------------------------------------------------------
const RStrParser &  RStrParser::operator=  (const RStrParser &  parserIn)
  {
  Set (parserIn);
  ResetCursor ();
  return (*this);
  };


//------------------------------------------------------------------------------
const RStrParser &  RStrParser::operator=  (const RStr & strIn)
  {
  Set (strIn);
  ResetCursor ();
  return (*this);
  };


//------------------------------------------------------------------------------
const RStrParser &  RStrParser::operator=  (const char *  pszIn)
  {
  Set (pszIn);
  ResetCursor ();
  return (*this);
  };


//------------------------------------------------------------------------------
INT32  RStrParser::CountWords  (bool  bStopAtEOL)
  {
  INT32  iCount = 0;
  INT32  iSavedCursor = iCursorStart;

  SkipWhitespace (FALSE);
  while (! IsEOFAscii ())
    {
    if ((bStopAtEOL) && (IsEOL ()))
      {
      break;
      };
    ++iCount;
    SkipWhitespace (TRUE);
    GetWord (NULL, FALSE);
    };

  iCursorStart = iSavedCursor;
  return (iCount);
  };




//------------------------------------------------------------------------------
FLOAT  RStrParser::GetF4_LEnd  (VOID)
  {
  UINT32   uReturn = GetU4_LEnd ();
  return (* ((FLOAT *) (&uReturn)));
  };


//------------------------------------------------------------------------------
UINT32  RStrParser::GetU4_LEnd  (VOID)
  {
  UINT32 uReturn = ((GetAt (iCursorStart + 0) << 0)  |
                    (GetAt (iCursorStart + 1) << 8)  |
                    (GetAt (iCursorStart + 2) << 16) |
                    (GetAt (iCursorStart + 3) << 24));
  //printf ("GetU4_LEnd %x  reading %d of %d (max %d)\n", this, iCursorStart, uStringLength, uBufferSize);

  SkipChars (4);
  return (uReturn);
  };


//------------------------------------------------------------------------------
UINT32  RStrParser::GetU2_LEnd  (VOID)
  {
  UINT32 uReturn = ((GetAt (iCursorStart + 0) << 0)  |
                    (GetAt (iCursorStart + 1) << 8));
  //printf ("GetU2_LEnd %x  reading %d of %d (max %d)\n", this, iCursorStart, uStringLength, uBufferSize);
  SkipChars (2);
  return (uReturn);
  };


//------------------------------------------------------------------------------
UINT32  RStrParser::GetU1_LEnd  (VOID)
  {
  UINT32 uReturn = GetAt (iCursorStart + 0);
  SkipChars (1);
  return (uReturn);
  };


//------------------------------------------------------------------------------
FLOAT  RStrParser::GetF4_BEnd  (VOID)
  {
  UINT32   uReturn = GetU4_BEnd ();
  return (* ((FLOAT *) (&uReturn)));
  };


//------------------------------------------------------------------------------
UINT32  RStrParser::GetU4_BEnd  (VOID)
  {
  UINT32 uReturn = ((GetAt (iCursorStart + 0) << 24)  |
                    (GetAt (iCursorStart + 1) << 16)  |
                    (GetAt (iCursorStart + 2) <<  8) |
                    (GetAt (iCursorStart + 3) <<  0));
  SkipChars (4);
  return (uReturn);
  };


//------------------------------------------------------------------------------
UINT32  RStrParser::GetU2_BEnd  (VOID)
  {
  UINT32 uReturn = ((GetAt (iCursorStart + 0) << 8)  |
                    (GetAt (iCursorStart + 1) << 0));
  SkipChars (2);
  return (uReturn);
  };


//------------------------------------------------------------------------------
UINT32  RStrParser::GetU1_BEnd  (VOID)
  {
  UINT32 uReturn = GetAt (iCursorStart + 0);
  SkipChars (1);
  return (uReturn);
  };


//------------------------------------------------------------------------------
VOID  RStrParser::SetF4_LEnd  (FLOAT  fIn)
  {
  SetU4_LEnd (* ((UINT32 *) &fIn));
  };

//------------------------------------------------------------------------------
VOID  RStrParser::SetF8_LEnd  (DOUBLE  dIn)
  {
  unsigned char *  pVal = reinterpret_cast<unsigned char *>(&dIn);
  

  AppendChar (*pVal); ++pVal;
  AppendChar (*pVal); ++pVal;
  AppendChar (*pVal); ++pVal;
  AppendChar (*pVal); ++pVal;
  
  AppendChar (*pVal); ++pVal;
  AppendChar (*pVal); ++pVal;
  AppendChar (*pVal); ++pVal;
  AppendChar (*pVal); ++pVal;

  iCursorStart = uStringLength;
  };



//------------------------------------------------------------------------------
VOID  RStrParser::SetU4_LEnd  (UINT32  uIn)
  {
  AppendChar ((uIn >>  0) & 0x000000ff);
  AppendChar ((uIn >>  8) & 0x000000ff);
  AppendChar ((uIn >> 16) & 0x000000ff);
  AppendChar ((uIn >> 24) & 0x000000ff);
  iCursorStart = uStringLength;
  };


//------------------------------------------------------------------------------
VOID  RStrParser::SetU2_LEnd  (UINT32  uIn)
  {
  AppendChar ((uIn >>  0) & 0x000000ff);
  AppendChar ((uIn >>  8) & 0x000000ff);
  iCursorStart = uStringLength;
  };


//------------------------------------------------------------------------------
VOID  RStrParser::SetU1_LEnd  (UINT32  uIn)
  {
  AppendChar ((uIn >>  0) & 0x000000ff);
  iCursorStart = uStringLength;
  };


//------------------------------------------------------------------------------
VOID  RStrParser::SetF4_BEnd  (FLOAT  fIn)
  {
  SetU4_BEnd (* ((UINT32 *) &fIn));
  };


//------------------------------------------------------------------------------
VOID  RStrParser::SetU4_BEnd  (UINT32  uIn)
  {
  AppendChar ((uIn >> 24) & 0x000000ff);
  AppendChar ((uIn >> 16) & 0x000000ff);
  AppendChar ((uIn >>  8) & 0x000000ff);
  AppendChar ((uIn >>  0) & 0x000000ff);
  iCursorStart = uStringLength;
  };


//------------------------------------------------------------------------------
VOID  RStrParser::SetU2_BEnd  (UINT32  uIn)
  {
  AppendChar ((uIn >>  8) & 0x000000ff);
  AppendChar ((uIn >>  0) & 0x000000ff);
  iCursorStart = uStringLength;
  };


//------------------------------------------------------------------------------
VOID  RStrParser::SetU1_BEnd  (UINT32  uIn)
  {
  AppendChar ((uIn >>  0) & 0x000000ff);
  iCursorStart = uStringLength;
  };


//------------------------------------------------------------------------------
VOID  RStrParser::GetData  (unsigned char *  pbyDataOut,
                            INT32            iDataSizeIn)
  {
  ReadChars (PCHAR (pbyDataOut), iDataSizeIn);
  };

//------------------------------------------------------------------------------
VOID  RStrParser::SetData  (const unsigned char *  pbyDataIn,
                            INT32                  iDataSizeIn)
  {
  WriteChars (PCHAR (pbyDataIn), iDataSizeIn);
  };


//------------------------------------------------------------------------------
EStatus  RStrParser::FlushBuffer  (VOID)
  {
  if (bBufferedWriting)
    {
    FILE *             fp;

    // Make sure input values are valid
    if (strBufferedFilename == RStr("")) return EStatus::kFailure;

    fp = fopen (strBufferedFilename.AsChar (), "ab");
    if (fp == NULL)
      {
      //sprintf (szDbgBuffer, "Unable to open file %s", szFilename); DebugMessage (szDbgBuffer);
      return EStatus::kFailure;
      };

    if (fwrite (pszBuffer, 1, uStringLength, fp) != uStringLength)
      {
      fclose (fp);
      return EStatus::kFailure;
      };

    fclose (fp);

    // clear the buffer, but leave memory allocated
    Empty        ();
    ResetCursor  ();

    return EStatus::kSuccess;
    };

  return EStatus::kFailure;
  };


//------------------------------------------------------------------------------
VOID  RStrParser::Grow  (UINT32  uSizeIn)
  {
  if (bBufferedWriting)
    {
    if (uStringLength > UINT32 (iBufferedWriteSize))
      {
      FlushBuffer ();
      };
    };

  // Reset the line end since the buffer is changing
  iLineEnd  = -1;

  RStr::Grow (uSizeIn);
  };


//------------------------------------------------------------------------------
VOID  RStrParser::GrowAbsolute  (UINT32  uSizeIn)
  {
  if (bBufferedWriting)
    {
    if (uStringLength > UINT32 (iBufferedWriteSize))
      {
      FlushBuffer ();
      };
    };
  // Reset the line end since the buffer is changing
  iLineEnd  = -1;

  RStr::GrowAbsolute (uSizeIn);
  };


//------------------------------------------------------------------------------
VOID  RStrParser::ExpandEnvironmentVars (VOID)
  {
  // scan for environment variables, and expand as you find them.
 
  INT32  iEnvStart = FindChar ('$');
  INT32  iEnvCount;
  INT32  iTotalStart;
  INT32  iTotalCount;
  RStr   strEnvVar;
  INT32  iSkipEnd = 0;
  INT32  iSavedCursorPos = GetCursorStart ();
 
  while (iEnvStart != -1)
    {
    
    iTotalStart = iEnvStart;
    iTotalCount = 1;
   
    iCursorStart = iEnvStart;
    SkipChars (1);
    // skip the opening bracket
    if (GetAt (iCursorStart) == '{')
      {
      iSkipEnd = '}';
      SkipChars (1);
      ++iTotalCount;
      }
    else if (GetAt (iCursorStart) == '(')
      {
      iSkipEnd = ')';
      SkipChars (1);
      ++iTotalCount;
      };
    iEnvStart = iCursorStart;
   
    INT32  iCurr = GetAt (iCursorStart);
   
    iEnvCount   = 0;
    while ( (isalnum (iCurr)) ||
            (iCurr == '_') )
      {
      SkipChars (1);
      iCurr = GetAt (iCursorStart);
      ++iEnvCount;
      ++iTotalCount;
      };

    // skip the ending bracket
    if ((iSkipEnd != 0) && (iCurr == iSkipEnd))
      {
      SkipChars (1);
      ++iTotalCount;
      };

    GetMiddle (iEnvStart, iEnvCount, strEnvVar);

    char *  pEnvValue = getenv (strEnvVar.AsChar ());
   
    if (pEnvValue != NULL)
      {
      RStr  strEnvValue = pEnvValue;
     
      //printf ("Found environment variable %s, value of %s\n", strEnvVar.AsChar (), pEnvValue);

      ClipMiddle (UINT32 (iTotalStart), UINT32 (iTotalCount));
      InsertString (strEnvValue, iTotalStart);
     
      iCursorStart = iTotalStart;
      }
    else
      {
      printf ("Unable to find environment variable %s\n", strEnvVar.AsChar ());
      };
   
    iEnvStart = FindChar ('$', iCursorStart);
    };
 
 
  SetCursorStart (iSavedCursorPos);
  };


//------------------------------------------------------------------------------
RStr  RStrParser::GetNextVariable (VOID)
  {
  // scan for environment variables, and return when you find one
 
  INT32  iVarNameStart = FindChar ('$');  // start and size of variable name only
  INT32  iVarNameCount;
  INT32  iTotalStart;    // start and size of the variable, including '$' and any brackets
  INT32  iTotalCount;
  RStr   strVarName ("");
  INT32  iSkipEnd = 0;
 
  if (iVarNameStart != -1)
    {
    // we found a variable
    iTotalStart = iVarNameStart;
    iTotalCount = 1;
   
    // skip the $
    iCursorStart = iVarNameStart;
    SkipChars (1);
    // skip the opening bracket
    if (GetAt (iCursorStart) == '{')
      {
      iSkipEnd = '}';
      SkipChars (1);
      ++iTotalCount;
      }
    else if (GetAt (iCursorStart) == '(')
      {
      iSkipEnd = ')';
      SkipChars (1);
      ++iTotalCount;
      };
    iVarNameStart = iCursorStart;

    // skip all chars in the variable name
    INT32  iCurr = GetAt (iCursorStart);
   
    iVarNameCount   = 0;
    while ( (isalnum (iCurr)) ||
            (iCurr == '_') )
      {
      SkipChars (1);
      iCurr = GetAt (iCursorStart);
      ++iVarNameCount;
      ++iTotalCount;
      };

    // skip the ending bracket
    if ((iSkipEnd != 0) && (iCurr == iSkipEnd))
      {
      SkipChars (1);
      ++iTotalCount;
      };

    // extract the variable name from the string.
    GetMiddle (iVarNameStart, iVarNameCount, strVarName);

    // set the cursor to the location where a substitution should occur
    SetCursorStart (iTotalStart);
    };
  return (strVarName);  
  };



//------------------------------------------------------------------------------
VOID  RStrParser::ExpandSymlinks (VOID)
  {
  // this routine probably should be part of a file class, but I'm making it 
  //  part of RStrParser for convenience.  The current string is expected to
  //   be a file path.  If you are in Linux, expand this to get rid of symlinks.

  #ifdef LINUX  
  char  achBuffer [512];
  memset (achBuffer, 0, 512);
  
  if (readlink (this->AsChar (), achBuffer, 511) != -1)
    {
    *this = achBuffer;
    };
  #endif // LINUX
  };


//------------------------------------------------------------------------------
VOID  RStrParser::ExpandSymlinksDir (VOID)
  {
  // this routine probably should be part of a file class, but I'm making it 
  //  part of RStrParser for convenience.  This routine expands symlinks
  //  up to the directory of the current file, but not the file itself.
  //  The current string is expected to be a file path.  If you are in Linux,
  //  expand this to get rid of symlinks.

  #ifdef LINUX  
  
  INT    iFinalSlash = ReverseFindChar ('/');
  RStr   strPathOnly = *this;
  RStr   strNameOnly = "";

  if (iFinalSlash != -1)
    {
    strNameOnly = *this;
    
    strPathOnly.TruncateRight (iFinalSlash);
    strNameOnly.TruncateLeft  (iFinalSlash + 1);
    };
  
  char  achBuffer [512];
  memset (achBuffer, 0, 512);
  
  if (readlink (strPathOnly.AsChar (), achBuffer, 511) != -1)
    {
    strPathOnly = achBuffer;
    if (GetAt (strPathOnly.GetLength () - 1) != '/')
      {
      strPathOnly += "/";
      };
    };
    
  *this = strPathOnly + strNameOnly;
    
  #endif // LINUX
  };
