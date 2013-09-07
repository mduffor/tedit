/* -----------------------------------------------------------------
                     TEdit Regular Expression Library

     This module implements regular expression and glob matching.

                            Michael T. Duffy
   ----------------------------------------------------------------- */

// RegEx.cpp
// Copyright 2004  Michael T. Duffy.  
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

#include "RegEx.hpp"
#include "RStr.hpp"
#include <cctype>
#include <cstdlib>
//#include <cstdio>
#include <cstring>


RStr  operator*  (const RStr &    strIn,   RegEx &  regexIn)  {return (regexIn.Match (strIn));};
RStr  operator*  (RegEx &  regexIn, const RStr &    strIn)    {return (regexIn.Match (strIn));};

//#define DEBUGMSG

//------------------------------------------------------------------------
// RegExMatch
//------------------------------------------------------------------------

//------------------------------------------------------------------------
RegExMatch::RegExMatch  ()
  {
  eMatchType = kChar;
  uStartMatch = 0x20;
  uEndMatch   = 0x20;
  
  bInvertMatch = FALSE;
  iNextOne = 0;
  iNextTwo = 0;

  iMinMatches = -1;
  iMaxMatches = -1;
  iNumMatches = 0;
  };


//------------------------------------------------------------------------
RegExMatch::RegExMatch  (EType  eMatchTypeIn,
                           UINT   uMatchStartIn,
                           UINT   uMatchEndIn,
                           INT    iNextOneIn,
                           INT    iNextTwoIn,
                           BOOL   bInvertMatchIn)
  {
  eMatchType   = eMatchTypeIn;
  uStartMatch  = uMatchStartIn;
  uEndMatch    = uMatchEndIn;
  iNextOne     = iNextOneIn;
  iNextTwo     = iNextTwoIn;
  bInvertMatch = bInvertMatchIn;
  };


//------------------------------------------------------------------------
BOOL  RegExMatch::IsMatch   (const char *  pchIn,
                              const char *  pchBufferStart)
  {
  if (bInvertMatch)
    {
    return (! RawMatch (pchIn, pchBufferStart));
    }
  else
    {
    return (RawMatch (pchIn, pchBufferStart));
    };
  };


//------------------------------------------------------------------------
BOOL  RegExMatch::RawMatch   (const char *  pchIn,
                               const char *  pchBufferStart)
  {
  switch (eMatchType)
    {
    case kAnyChar:      {return TRUE;};
    case kChar:         {return (UINT (*pchIn) == uStartMatch);}; 
    case kCharRange:    {return ((UINT (*pchIn) >= uStartMatch) &&
                                 (UINT (*pchIn) <= uEndMatch));}; 
    case kAlnum:        {return (isalnum  (int (*pchIn)));}; 
    case kAlpha:        {return (isalpha  (int (*pchIn)));};
    case kBlank:        {return ((*pchIn == ' ') || (*pchIn == '\t'));};
    case kCntrl:        {return (iscntrl  (int (*pchIn)));}; 
    case kDigits:       {return (isdigit  (int (*pchIn)));}; 
    case kGraph:        {return (isgraph  (int (*pchIn)));}; 
    case kLower:        {return (islower  (int (*pchIn)));}; 
    case kPrint:        {return (isprint  (int (*pchIn)));}; 
    case kPunct:        {return (ispunct  (int (*pchIn)));}; 
    case kSpace:        {return (isspace  (int (*pchIn)));}; 
    case kUpper:        {return (isupper  (int (*pchIn)));}; 
    case kXDigit:       {return (isxdigit (int (*pchIn)));}; 
    case kWordBoundary: {return ( (! IsWordChar (pchIn - 1) &&   IsWordChar (pchIn) ) ||
                                  (  IsWordChar (pchIn - 1) && ! IsWordChar (pchIn) ) );}; 
    
    
    case kWordInside:   {return (IsWordChar (pchIn - 1) && IsWordChar (pchIn));}; 
    case kWordBegin:    {return (! IsWordChar (pchIn - 1) &&   IsWordChar (pchIn) );}; 
    case kWordEnd:      {return (  IsWordChar (pchIn - 1) && ! IsWordChar (pchIn) );};
    case kWordChar:     {return (IsWordChar (pchIn));}; 
    case kNotWordChar:  {return (! IsWordChar (pchIn));}; 
    case kBufferStart:  {return (pchIn == pchBufferStart);};
    case kBufferEnd:    {return (*pchIn == '\0');}; break;
    case kLineStart:    {return ((pchIn == pchBufferStart) || (*(pchIn - 1) == '\n'));}; 
    case kLineEnd:      {return ((*pchIn == '\0') || (*pchIn == '\n'));};
    default:            break;
    };

  return (FALSE);
  };


//------------------------------------------------------------------------
BOOL  RegExMatch::IncOnMatch  (VOID)
  {
  switch (eMatchType)
    {
    case kAnyChar:
    case kChar:          
    case kCharRange:     
    case kAlnum:         
    case kAlpha:         
    case kBlank:         
    case kCntrl:         
    case kDigits:        
    case kGraph:         
    case kLower:         
    case kPrint:         
    case kPunct:         
    case kSpace:         
    case kUpper:         
    case kXDigit:        
         return (TRUE);
    
    case kWordBoundary:  
    case kWordInside:    
    case kWordBegin:     
    case kWordEnd:       
    case kWordChar:      
    case kNotWordChar:   
    case kBufferStart:   
    case kBufferEnd:     
    case kLineStart:     
    case kLineEnd:       
         return (FALSE);

    default:
         break;
    };

  return (TRUE);
  };


//------------------------------------------------------------------------
BOOL  RegExMatch::IsWordChar  (const char *  pchIn)
  {
  // returns true if the character is word-constituent, false if not.
  
  // Although some GNU RegEx libraries allow one to define a syntax table
  //   that defines what is part of a word and what isn't I am hard coding
  //   to the default value.  Letters, numbers, and the underscore are
  //   word-constituent characters.

  if (isalpha (int (*pchIn)) ||
      isdigit (int (*pchIn)) ||
      (*pchIn == '_'))
    {
    return (TRUE);
    };
  return (FALSE);  
  };



//------------------------------------------------------------------------
BOOL   RegExMatch::GetType  (const char *   pszPatternIn,
                              BOOL           bInList,
                              INT &          iSizeMatchedOut)
  {
  // try to match each type
 
  eMatchType = kNull;
 
  #ifdef DEBUGMSG
  printf ("GetType %s \n", pszPatternIn);
  #endif

  // Make sure you are within a list bracket to match the following
  
  if (bInList)
    {
         if (strncmp (pszPatternIn, "[:alnum:]",   9) == 0)  {iSizeMatchedOut =  9; eMatchType = kAlnum; }
    else if (strncmp (pszPatternIn, "[:alpha:]",   9) == 0)  {iSizeMatchedOut =  9; eMatchType = kAlpha; }
    else if (strncmp (pszPatternIn, "[:blank:]",   9) == 0)  {iSizeMatchedOut =  9; eMatchType = kBlank; }
    else if (strncmp (pszPatternIn, "[:cntrl:]",   9) == 0)  {iSizeMatchedOut =  9; eMatchType = kCntrl; }
    else if (strncmp (pszPatternIn, "[:digits:]", 10) == 0)  {iSizeMatchedOut = 10; eMatchType = kDigits;}
    else if (strncmp (pszPatternIn, "[:graph:]",   9) == 0)  {iSizeMatchedOut =  9; eMatchType = kGraph; }
    else if (strncmp (pszPatternIn, "[:lower:]",   9) == 0)  {iSizeMatchedOut =  9; eMatchType = kLower; }
    else if (strncmp (pszPatternIn, "[:print:]",   9) == 0)  {iSizeMatchedOut =  9; eMatchType = kPrint; }
    else if (strncmp (pszPatternIn, "[:punct:]",   9) == 0)  {iSizeMatchedOut =  9; eMatchType = kPunct; }
    else if (strncmp (pszPatternIn, "[:space:]",   9) == 0)  {iSizeMatchedOut =  9; eMatchType = kSpace; }
    else if (strncmp (pszPatternIn, "[:upper:]",   9) == 0)  {iSizeMatchedOut =  9; eMatchType = kUpper; }
    else if (strncmp (pszPatternIn, "[:xdigit:]", 10) == 0)  {iSizeMatchedOut = 10; eMatchType = kXDigit;}
                   
    else if (isalnum (*pszPatternIn) &&
             (*(pszPatternIn + 1) == '-') &&
             isalnum (*(pszPatternIn + 2)))
      {
      uStartMatch = *(pszPatternIn);
      uEndMatch   = *(pszPatternIn + 2);
      
      iSizeMatchedOut = 3;
      eMatchType      = kCharRange; 
      };
    }
  else
    {
    };

  // check the chars that appear outside of lists
  
  if (*pszPatternIn == '.')  {iSizeMatchedOut = 1; eMatchType = kAnyChar;}

  else if (*pszPatternIn == '\\')
    {

  #ifdef DEBUGMSG
  printf ("Escaping character %c\n", *(pszPatternIn + 1));
  #endif

    // escaped char
    iSizeMatchedOut = 2;
    uStartMatch     = *(pszPatternIn + 1);
    eMatchType      = kChar;
    }

  else if (strncmp (pszPatternIn, "\\b", 2) == 0)  {iSizeMatchedOut = 2; eMatchType = kWordBoundary;}
  else if (strncmp (pszPatternIn, "\\B", 2) == 0)  {iSizeMatchedOut = 2; eMatchType = kWordInside  ;}
  else if (strncmp (pszPatternIn, "\\<", 2) == 0)  {iSizeMatchedOut = 2; eMatchType = kWordBegin   ;}
  else if (strncmp (pszPatternIn, "\\>", 2) == 0)  {iSizeMatchedOut = 2; eMatchType = kWordEnd     ;}
  else if (strncmp (pszPatternIn, "\\w", 2) == 0)  {iSizeMatchedOut = 2; eMatchType = kWordChar    ;}
  else if (strncmp (pszPatternIn, "\\W", 2) == 0)  {iSizeMatchedOut = 2; eMatchType = kNotWordChar ;}
  else if (strncmp (pszPatternIn, "\\`", 2) == 0)  {iSizeMatchedOut = 2; eMatchType = kBufferStart ;}
  else if (strncmp (pszPatternIn, "\\'", 2) == 0)  {iSizeMatchedOut = 2; eMatchType = kBufferEnd   ;}

  else if (strncmp (pszPatternIn, "^",   1) == 0)  {iSizeMatchedOut = 1; eMatchType = kLineStart   ;}
  else if (strncmp (pszPatternIn, "$",   1) == 0)  {iSizeMatchedOut = 1; eMatchType = kLineEnd     ;}
    
  else if ((*pszPatternIn != '(') &&
           (*pszPatternIn != ')') &&
           (*pszPatternIn != '[') &&
           (*pszPatternIn != ']') &&
           (*pszPatternIn != '{') &&
           (*pszPatternIn != '}') &&
           (*pszPatternIn != '?') &&
           (*pszPatternIn != '+') &&
           (*pszPatternIn != '*'))
    {
    iSizeMatchedOut = 1;
    uStartMatch     = *(pszPatternIn);
    eMatchType      = kChar;
    };

  return (eMatchType != kNull);
  };


//------------------------------------------------------------------------
const char *  RegExMatch::GetTypeText  (VOID)
  {
  
  switch (eMatchType)
    {
    case kNull:          return "Null";
    case kOr:            return "Or";
    case kRepeat:        return "Repeat";
    case kAnyChar:       return (bInvertMatch) ? "NotAnyChar" : "AnyChar";
    case kChar:          return (bInvertMatch) ? "NotChar" : "Char";
    case kCharRange:     return (bInvertMatch) ? "NotCharRange" : "CharRange";
    case kAlnum:         return (bInvertMatch) ? "NotAlnum" : "Alnum";
    case kAlpha:         return (bInvertMatch) ? "NotAlpha" : "Alpha";
    case kBlank:         return (bInvertMatch) ? "NotBlank" : "Blank";
    case kCntrl:         return (bInvertMatch) ? "NotCntrl" : "Cntrl";
    case kDigits:        return (bInvertMatch) ? "NotDigits" : "Digits";
    case kGraph:         return (bInvertMatch) ? "NotGraph" : "Graph";
    case kLower:         return (bInvertMatch) ? "NotLower" : "Lower";
    case kPrint:         return (bInvertMatch) ? "NotPrint" : "Print";
    case kPunct:         return (bInvertMatch) ? "NotPunct" : "Punct";
    case kSpace:         return (bInvertMatch) ? "NotSpace" : "Space";
    case kUpper:         return (bInvertMatch) ? "NotUpper" : "Upper";
    case kXDigit:        return (bInvertMatch) ? "NotXDigit" : "XDigit";
    case kWordBoundary:  return "WordBoundary";
    case kWordInside:    return "WordInside";
    case kWordBegin:     return "WordBegin";
    case kWordEnd:       return "WordEnd";
    case kWordChar:      return "WordChar";
    case kNotWordChar:   return "NotWordChar";
    case kBufferStart:   return "BufferStart";
    case kBufferEnd:     return "BufferEnd";
    case kLineStart:     return "LineStart";
    case kLineEnd:       return "LineEnd";
    default:
         break;
    };

  return ("");
  };




//------------------------------------------------------------------------
// RegExDeque
//------------------------------------------------------------------------

// Note:  This is a bit of a hack job, but should be faster to implement
//          than a full deque class.  Essentially instead of allocating
//          elements that contain an index, a next pointer, and a prev pointer,
//          I'm using three parallel arrays.  Additionally I'm setting the
//          first three values as sentinels for the queue head, queue tail,
//          and pool of unallocated entries.  Since there will be a lot of 
//          allocation and deallocation, a pool is useful for speed.


//------------------------------------------------------------------------
RegExDeque::RegExDeque ()
  {
  // clearing will initialize everything.
  Clear ();
  };


//------------------------------------------------------------------------
RegExDeque::~RegExDeque ()
  {
  };


//------------------------------------------------------------------------
VOID  RegExDeque::InitSentinels (VOID)
  {
  // Allocate and initialize the head and tail sentinels
  iHead = GetFromPool ();
  iTail = GetFromPool ();

  aiNext [iHead] = iTail;
  aiPrev [iHead] = iTail;
  
  aiNext [iTail] = iHead;
  aiPrev [iTail] = iHead;
  };


//------------------------------------------------------------------------
VOID  RegExDeque::Grow  (INT   iGrowSizeIn)
  {
  
  INT  iNewStart = aiValues.Length ();
  
  // allocate new entries
  
  aiValues.Insert (iNewStart, iGrowSizeIn);
  aiNext.  Insert (iNewStart, iGrowSizeIn);
  aiPrev.  Insert (iNewStart, iGrowSizeIn);
  
  // setup the Next pointer to link all the allocations together
  for (INT  iIndex = 0; iIndex < iGrowSizeIn; ++iIndex)  
    {
    aiNext [iNewStart + iIndex] = iNewStart + iIndex + 1;
    };

  // add the new allocations to the pool
  aiNext [iNewStart + iGrowSizeIn - 1] = iPool;
  iPool = iNewStart;
  };


//------------------------------------------------------------------------
INT  RegExDeque::GetFromPool (VOID)
  {
  if (iPool == -1)
    {
    // empty pool.  Allocate more entries
    
    Grow (50);
    };
    
  INT   iReturn = iPool;
  iPool = aiNext [iPool];
    
  return (iReturn);
  };
  
  
//------------------------------------------------------------------------
VOID  RegExDeque::ReturnToPool (INT   iIndexIn)
  {
  aiNext [iIndexIn] = iPool;
  iPool = iIndexIn;
  };

    
//------------------------------------------------------------------------
VOID  RegExDeque::Push  (INT  iIn)
  {
  // push to head of queue
  
  INT  iNew = GetFromPool ();
  
  aiValues [iNew] = iIn;

  aiNext [iNew] = aiNext [iHead];
  aiPrev [iNew] = iHead;
  
  aiNext [iHead] = iNew;
  aiPrev [aiNext [iNew]] = iNew;
  };


//------------------------------------------------------------------------
INT   RegExDeque::Pop  (VOID)
  {
  // pop from head of queue

  INT  iReturnValue;
  INT  iPopIndex;
  
  if (IsEmpty ())
    {
    // empty queue
    return (-1);
    }
    
  iPopIndex = aiNext [iHead];

  aiPrev [aiNext [iPopIndex]] = iHead;
  aiNext [iHead] = aiNext [iPopIndex];
  
  iReturnValue = aiValues [iPopIndex];
  
  ReturnToPool (iPopIndex);
  
  return (iReturnValue);
  };
  
  
//------------------------------------------------------------------------
VOID  RegExDeque::Put  (INT  iIn)
  {
  // put to tail of queue

  INT  iNew = GetFromPool ();

  aiValues [iNew] = iIn;

  aiNext [iNew] = iTail;
  aiPrev [iNew] = aiPrev [iTail];
  
  aiPrev [iTail] = iNew;
  aiNext [aiPrev [iNew]] = iNew;
  };


//------------------------------------------------------------------------
VOID  RegExDeque::Clear (VOID)
  {
  // force the return of all entries to the pool

  INT  iNumEntries = aiValues.Length ();
  
  // setup the Next pointer to link all the allocations together
  for (INT  iIndex = 0; iIndex < iNumEntries; ++iIndex)  
    {
    aiNext [iIndex] = iIndex + 1;
    };
  
  if (iNumEntries > 0)
    {
    iPool = 0;
    aiNext [iNumEntries - 1] = -1;
    }
  else
    {
    iPool = -1;
    };

  InitSentinels ();
  };


//------------------------------------------------------------------------
BOOL  RegExDeque::InQueueHead  (INT  iIn)
  {
  INT   iCurrIndex = aiNext [iHead];
  
  while ((iCurrIndex != iTail) && (aiValues [iCurrIndex] != -1))
    {
    if (aiValues [iCurrIndex] == iIn)
      {
      return (TRUE);
      };
    iCurrIndex = aiNext [iCurrIndex];
    };
  return (FALSE);
  };


//------------------------------------------------------------------------
BOOL  RegExDeque::InQueueTail  (INT  iIn)
  {
  INT   iCurrIndex = aiPrev [iTail];
  
  while ((iCurrIndex != iHead) && (aiValues [iCurrIndex] != -1))
    {
    if (aiValues [iCurrIndex] == iIn)
      {
      return (TRUE);
      };
    iCurrIndex = aiPrev [iCurrIndex];
    };
  return (FALSE);
  };


//------------------------------------------------------------------------
// RegEx
//------------------------------------------------------------------------

//------------------------------------------------------------------------
RegEx::RegEx  ()
  {
  strPattern  = "";
  iPatternPos = 0;
  iState      = 0;
  
  };


//------------------------------------------------------------------------
RegEx::~RegEx  ()
  {
  };

//------------------------------------------------------------------------
RegEx::RegEx  (const RegEx &  reIn)
  {
  arrayStates = reIn.arrayStates;
  strPattern  = reIn.strPattern;
  iPatternPos = reIn.iPatternPos;
  iState      = reIn.iState;
  };


//------------------------------------------------------------------------
RegEx::RegEx  (RStr          strExprIn,
               EParser       eParseType)
  {
  Set (strExprIn, eParseType);
  };


//------------------------------------------------------------------------
RegEx::RegEx  (const char *  pszExprIn,
                 EParser       eParseType)
  {
  Set (RStr (pszExprIn), eParseType);
  };


//------------------------------------------------------------------------
VOID  RegEx::Set  (RStr     strExprIn,
                   EParser  eParseType)
  {
  switch (eParseType)
    {
    case kRegEx:  ParseRegEx (strExprIn); break;
    case kGlob:   ParseGlob  (strExprIn); break;
    };
  };


//------------------------------------------------------------------------
RStr  RegEx::Match  (const RStr &  strSourceIn,
                      INT32         iSearchStartIn,
                      RStrArray *   pstraSubstringsOut)
  {
  INT32         iStartMatch   = 0;
  INT32         iMaxMatch     = strSourceIn.GetLength ();
  INT           iMatchState;
  RStr          strReturn ("");
  

  if (strPattern.GetLength () == 0) return ("");
  
  for (iStartMatch = iSearchStartIn; iStartMatch < iMaxMatch; ++iStartMatch)
    {
    INT           iLongestMatch = -1;
    const char *  pszStartMatch = strSourceIn.AsChar (iStartMatch);
    const INT     iScan = -1;
    INT32         iPos          = iStartMatch;

    iPatternPos = 0;
    iMatchState = 1;

    // clear the repeat flags
    for (UINT32  uIndex = 0; uIndex < UINT32 (arrayStates.Length ()); ++uIndex)
      {
      arrayStates [uIndex].ClearNumMatches ();
      };
    

    deque.Clear ();
    deque.Put (iScan);
    
    iMatchState = arrayStates [0].GetNextOne ();
    
    do
      {
      if (iMatchState == iScan)
        {
        // We read the scan value off the queue.  move to next character
        iPos = iPos + 1;
        deque.Put (iScan);
        }
      else if ((arrayStates [iMatchState].IsNull ()) && (iMatchState != 0))
        {
        if (! deque.InQueueHead (arrayStates [iMatchState].GetNextOne ()))
          {
          deque.Push (arrayStates [iMatchState].GetNextOne ());
          };
        }
      else if (arrayStates [iMatchState].IsOr ())
        {
        if (! deque.InQueueHead (arrayStates [iMatchState].GetNextOne ()))
          {
          deque.Push (arrayStates [iMatchState].GetNextOne ());
          };

        if (! deque.InQueueHead (arrayStates [iMatchState].GetNextTwo ()))
          {
          deque.Push (arrayStates [iMatchState].GetNextTwo ());
          };
        }
      else if (arrayStates [iMatchState].IsRepeat ())
        {
        // The first branch is the continue state.  The second is the repeat state.
        
        // Note: Check for iterations here.  To be implemented.
        // Note:  How do we handle zero matches?


        // check curr match against min and max.
        //  If less than min, we must only repeat.
        //  If greater than max, we must fail and not add further checks.
        //  If between min and max inclusive, we add the continue state.
        
        INT  iNumMatches = arrayStates [iMatchState].GetNumMatches ();
        INT  iMin        = arrayStates [iMatchState].GetMatchMin ();
        INT  iMax        = arrayStates [iMatchState].GetMatchMax ();

        if (iNumMatches < iMin)
          {
          if (! deque.InQueueHead (arrayStates [iMatchState].GetNextTwo ()))
            {
            deque.Push (arrayStates [iMatchState].GetNextTwo ());
            };
          }
        else if ((iNumMatches > iMax) && (iMax != -1))
          {
          // do nothing
          }
        else  
          {
          if (! deque.InQueueHead (arrayStates [iMatchState].GetNextOne ()))
            {
            deque.Push (arrayStates [iMatchState].GetNextOne ());
            };
          if (! deque.InQueueHead (arrayStates [iMatchState].GetNextTwo ()))
            {
            deque.Push (arrayStates [iMatchState].GetNextTwo ());
            };
          };
        
        arrayStates [iMatchState].IncNumMatches ();   
        }
      else if ((iPos < iMaxMatch) && (arrayStates [iMatchState].IsMatch (strSourceIn.AsChar (iPos), pszStartMatch)))
        {
  #ifdef DEBUGMSG
  printf ("%c matches %c %s\n",
        *(strSourceIn.AsChar (iPos)),
        arrayStates [iMatchState].GetStartMatch (),
        arrayStates [iMatchState].GetTypeText ());
  printf ("%s\n",   pszStartMatch );
  #endif
        
        if (arrayStates [iMatchState].IncOnMatch ())
          {
          if (! deque.InQueueTail (arrayStates [iMatchState].GetNextOne ()))
            {
            deque.Put (arrayStates [iMatchState].GetNextOne ());
            };
          }
        else
          {
          if (! deque.InQueueHead (arrayStates [iMatchState].GetNextOne ()))
            {
            deque.Push (arrayStates [iMatchState].GetNextOne ());
            };
          };
        }
      iMatchState = deque.Pop ();

      if (iMatchState == 0)
        {
        // found a match
        iLongestMatch = iPos;
        };
      }
    while ((iPos <= (iMaxMatch + 1)) && (! deque.IsEmpty ())); // (iMatchState != 0) && 
    
    if (iLongestMatch > 0)
      {
      // found a successful match.  This is the leftmost one we can find, so run with it.
      
      strSourceIn.GetMiddle (iStartMatch, iLongestMatch - iStartMatch, strReturn);
      break;
      };
    }; // for each char 

  return (strReturn);
  };
                   

//------------------------------------------------------------------------
VOID  RegEx::Debug (VOID)
  {
  INT  iMax = arrayStates.Length ();
  
  for (iState = 0; iState < iMax; ++iState)
    {
    printf ("%4d %12s %c %3d %3d\n", int (iState),
                                     arrayStates [iState].GetTypeText (), 
                                     char (arrayStates [iState].GetStartMatch ()), 
                                     arrayStates [iState].GetNextOne (),
                                     arrayStates [iState].GetNextTwo ());
    };
  };


//------------------------------------------------------------------------
RStr  RegEx::Substitute  (RStr  strSourceIn,
                          RStr  strReplaceIn)
  {
  // to be implemented
  return (RStr (""));
  };


//------------------------------------------------------------------------
VOID  RegEx::ParseRegEx (RStr  strExprIn)
  {
  arrayStates.Clear ();
  
  iState = 1;
  iPatternPos = 0;
  strPattern = strExprIn;
 
  INT  iStartIndex = RegExExpression ();
 
  arrayStates [0] = RegExMatch  (RegExMatch::kNull, 0, 0, iStartIndex, -1);

  // the last state needs to be the null which signifies the end has been reached.
  arrayStates [iState] = RegExMatch  (RegExMatch::kNull, 0, 0, 0, 0);
  };


//------------------------------------------------------------------------
VOID  RegEx::ParseGlob  (RStr  strExprIn)
  {
  arrayStates.Clear ();
  
  iState = 1;
  iPatternPos = 0;
  strPattern = strExprIn;


  arrayStates [0] = RegExMatch  (RegExMatch::kNull, 0, 0, iState, iState);

  UINT32  uPatternLength = strPattern.GetLength ();

  for (iPatternPos = 0; iPatternPos < uPatternLength; ++iPatternPos)
    {
    if (strPattern [iPatternPos] == '*')
      {
      RegExMatch    matchNew (RegExMatch::kRepeat, 0, 0, iState + 2, iState + 1);
      matchNew.SetNumMatches (0, -1);
      arrayStates [iState] = matchNew;
      ++iState;

      arrayStates [iState] = RegExMatch  (RegExMatch::kAnyChar, 0, 0, iState - 1, iState - 1);
      }
    else if (strPattern [iPatternPos] == '?')
      {
      arrayStates [iState] = RegExMatch  (RegExMatch::kAnyChar, 0, 0, iState + 1, iState + 1);
      }
    else if (strPattern [iPatternPos] == '\\')
      {
      ++iPatternPos;
      arrayStates [iState] = RegExMatch  (RegExMatch::kChar, strPattern [iPatternPos], 0, iState + 1, iState + 1);
      }
    else 
      {
      arrayStates [iState] = RegExMatch  (RegExMatch::kChar, strPattern [iPatternPos], 0, iState + 1, iState + 1);
      };
    ++iState;
    };
  arrayStates [iState] = RegExMatch  (RegExMatch::kNull, 0, 0, 0, 0);
  };


//------------------------------------------------------------------------
INT  RegEx::RegExExpression  (BOOL  bNegativeMatchIn)
  {
  INT   iBranchOne;
  INT   iBranchTwo;
  INT   iReturn;
  
  // this routine returns the entrance into the expression.  The exits from the expression are all 
  //  recursively set up.
  
  iBranchOne = RegExTerm (bNegativeMatchIn);
  iReturn    = iBranchOne;
  
  if (strPattern [iPatternPos] == '|')
    {
    // handle alternation
    
    ++iPatternPos;
    ++iState;
    
    iReturn = iBranchTwo = iState;
    ++iState;
    
    arrayStates [iBranchTwo]     = RegExMatch  (RegExMatch::kOr,   0, 0, RegExExpression (), iBranchOne);
    arrayStates [iBranchTwo - 1] = RegExMatch  (RegExMatch::kNull, 0, 0, iState, iState);
    };
  return (iReturn);  
  };

         
//------------------------------------------------------------------------
INT  RegEx::RegExTerm  (BOOL  bNegativeMatchIn)
  {
  INT  iReturn;
  INT  iNewReturn;

  // this routine returns the entrance into the expression.  The exits from the expression are all 
  //  recursively set up.
  
  iReturn = RegExFactor (bNegativeMatchIn);

  // Factors are always set up so that the next state is the exit from this state.  So factors
  //  that exist one after the other should all line up correctly.

  while ((iNewReturn = RegExFactor (bNegativeMatchIn)) != -1)
    {
    };

  // return the entrance of the first factor.
  return (iReturn);  
  };

         
//------------------------------------------------------------------------
INT  RegEx::RegExFactor  (BOOL  bNegativeMatchIn)
  {
  BOOL  bNegativeMatch;
  
  INT   iBranchOne = iState;
  INT   iBranchTwo = -1;
  INT   iReturn = -1;

  // this routine returns the entrance into the expression.  The exits from the expression are all 
  //  recursively set up.
  
  // handle groups
  if (strPattern [iPatternPos] == '(')
    {
    ++iPatternPos;
    // Note:  You need to handle beginning sub-strings here.  To be implemented
    iBranchTwo = RegExExpression (bNegativeMatchIn);
    // close group
    if (strPattern [iPatternPos] == ')')
      {
      // Note:  You need to handle ending sub-strings here.  To be implemented
      ++iPatternPos;
      }
    else
      {
      // Error.  Group not closed
      return (-1);
      };
    }
  // handle lists  
  else if (strPattern [iPatternPos] == '[')
    {
    ++iPatternPos;
    bNegativeMatch = FALSE;

    // check to see if this is a "not match" list
    if (strPattern [iPatternPos] == '^')
      {
      ++iPatternPos;
      bNegativeMatch = TRUE;
      };

    iBranchTwo = RegExList (bNegativeMatch);
    // close list
    if (strPattern [iPatternPos] == ']')
      {
      ++iPatternPos;
      }
    else
      {
      // Error.  List not closed
      return (-1);
      };
    }
  // we have a character to match if we are not closing a 
  // group, or have a repetition character, or are at the 
  // end of the string
  else if ( (strPattern [iPatternPos] != ')') &&
            (strPattern [iPatternPos] != '\0') &&
            (strPattern [iPatternPos] != '{') &&
            (strPattern [iPatternPos] != '?') &&
            (strPattern [iPatternPos] != '+') &&
            (strPattern [iPatternPos] != '*'))
    {
    // we have a character.

    INT            iMatchSize;
    RegExMatch    matchNew;

    if (matchNew.GetType  (strPattern.AsChar (iPatternPos), false, iMatchSize))
      {
      // found a new character

      matchNew.SetNext (iState + 1, iState + 1);
      matchNew.InvertMatch (bNegativeMatchIn);
      arrayStates [iState] = matchNew;

      iBranchTwo = iState;
      iPatternPos += iMatchSize;
      ++iState;
      }
    else
      {
      // error:  not a character, and there were no other options
      return (-1);
      };
    }
  else
    {
    // error:  no factor
    return (-1);
    };
  
  // check for repetition
  
  if ((strPattern [iPatternPos] == '{') ||
      (strPattern [iPatternPos] == '?') ||
      (strPattern [iPatternPos] == '+') ||
      (strPattern [iPatternPos] == '*'))
    {
    RegExMatch    matchNew (RegExMatch::kRepeat, 0, 0, iState + 1, iBranchTwo);
                                            
    if (strPattern [iPatternPos] == '?')  {matchNew.SetNumMatches (0, 1);};
    if (strPattern [iPatternPos] == '+')  {matchNew.SetNumMatches (1, -1);};
    if (strPattern [iPatternPos] == '*')  {matchNew.SetNumMatches (0, -1);};
    
    if (strPattern [iPatternPos] == '{')
      {
      const char *  pchStartOne = strPattern.AsChar (iPatternPos + 1);
      const char *  pchStartTwo;
      const char *  pchEnd;

      long int lStartRange = strtol (pchStartOne, (char **)&pchStartTwo, 10);
      matchNew.SetMatchMin (lStartRange);
      

      if (*pchStartTwo == ',')
        {
        // Might be a second number.
        ++pchStartTwo;

        if (*pchStartTwo == '}')
          {
          // open ended range
          matchNew.SetMatchMax (-1);
          pchEnd = pchStartTwo;
          }
        else
          {
          long int lEndRange = strtol (pchStartTwo, (char **)&pchEnd, 10);
          matchNew.SetMatchMax (lEndRange);
          };
        }
      else if (*pchStartTwo == '}')
        {
        // exact count
        matchNew.SetMatchMax (lStartRange);
        pchEnd = pchStartTwo;
        }
      else 
        {
        // error: invalid character in range
        };

      if (*pchEnd == '}')
        {
        iPatternPos += pchEnd - pchStartOne + 1;
        }
      else
        {
        // not able to find the end 
        };
      };

    // Now save the repeat.

    arrayStates [iState] = matchNew;
    arrayStates [iBranchOne - 1].SetNextOne (iState);

    ++iPatternPos;
    ++iState;
    return (iState - 1);
    }
  else
    {
    return (iBranchTwo);
    };
  return (iReturn);
  };




//------------------------------------------------------------------------
INT  RegEx::RegExList (BOOL  bNegativeMatchIn)
  {
  INT  iLastAddedOr;
  INT  iNewReturn;
  INT  iStartState = iState;
  INT  iMatchState = iState + 1;
  INT  iChainStart = iState;


  // create a null for the entrance situation (to skip the match null)
  arrayStates [iStartState] = RegExMatch  (RegExMatch::kNull, 0, 0, iMatchState + 1, -1);
  ++iState;

  // create a null that we can target for the "match" situation
  arrayStates [iMatchState] = RegExMatch  (RegExMatch::kNull, 0, 0, -1, -1, bNegativeMatchIn);
  ++iState;
  
  iLastAddedOr = iChainStart = RegExListItem (bNegativeMatchIn, iMatchState);

  while ((iNewReturn = RegExListItem (bNegativeMatchIn, iMatchState)) != -1)
    {
    iLastAddedOr = iNewReturn;
    };

  // point the "match" null to the state after the chain
  arrayStates [iMatchState].SetNextOne (iState);

  // and turn the last Or into a null
  arrayStates [iLastAddedOr].SetType    (RegExMatch::kNull);
  arrayStates [iLastAddedOr].SetNextTwo (-1);

  return (iChainStart);  
  };


//------------------------------------------------------------------------
INT  RegEx::RegExListItem (BOOL  bNegativeMatchIn,
                            INT   iListStartState)
  {
  INT            iMatchSize;
  RegExMatch    matchNew;

  if (matchNew.GetType  (strPattern.AsChar (iPatternPos), true, iMatchSize))
    {
    // found a new character


    arrayStates [iState] = RegExMatch  (RegExMatch::kOr,   0, 0, iState + 1, iState + 2, bNegativeMatchIn);
    ++iState;

    matchNew.SetNext  (iListStartState, -1);
    matchNew.InvertMatch (bNegativeMatchIn);
    arrayStates [iState] = matchNew;
    iPatternPos += iMatchSize;
    ++iState;

    return (iState - 2);
    };

  return (-1);
  };


//-----------------------------------------------------------------------------
//  RegExMatchArray
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
RegExMatchArray::RegExMatchArray  ()
  {
  Init ();
  };


//-----------------------------------------------------------------------------
RegExMatchArray::RegExMatchArray  (const RegExMatchArray &  arrayIn)
  {
  Init ();
  Copy (arrayIn);
  };


//-----------------------------------------------------------------------------
RegExMatchArray::RegExMatchArray  (INT   iInitialSize)
  {
  Init ();
  SetLength (iInitialSize);
  InitValues (0, iAllocSize);
  };


//-----------------------------------------------------------------------------
RegExMatchArray::~RegExMatchArray  ()
  {
  DeleteArray (&pArray);
  };


//-----------------------------------------------------------------------------
VOID *  RegExMatchArray::AllocArray  (INT  iSizeIn)
  {
  return new RegExMatch [iSizeIn];
  };


//-----------------------------------------------------------------------------
VOID  RegExMatchArray::DeleteArray  (PVOID *  pvArrayIn)
  {
  delete [] ((RegExMatch *) *pvArrayIn);
  *pvArrayIn = NULL;
  };


//-----------------------------------------------------------------------------
VOID  RegExMatchArray::InitValues  (INT    iStartOffset,
                                     INT    iNumToInit)
  {
  // zero out the requested range of the array
  RegExMatch    remZero;
  
  INT iEndOffset = iStartOffset + iNumToInit;

  for (INT  iIndex = iStartOffset; iIndex < iEndOffset; ++iIndex)
    {
    ((RegExMatch *) pArray) [iIndex] = remZero;
    };
  };


//-----------------------------------------------------------------------------
VOID  RegExMatchArray::CopyValues  (PVOID  pvSourceDataIn,
                                     INT    iSourceOffsetIn,
                                     INT    iStartOffsetIn,
                                     INT    iNumToCopyIn)
  {
  for (INT  iIndex = 0; iIndex < iNumToCopyIn; ++iIndex)
    {
    ((RegExMatch *) pArray) [iIndex + iStartOffsetIn] = ((RegExMatch *) pvSourceDataIn) [iIndex + iSourceOffsetIn];
    };
  };


//-----------------------------------------------------------------------------
VOID  RegExMatchArray::CopyValuesRev  (PVOID  pvSourceDataIn,
                                        INT    iSourceOffsetIn,
                                        INT    iStartOffsetIn,
                                        INT    iNumToCopyIn)
  {
  // this routine is the opposite of CopyValues, and is used for
  //  properly shifting values to the right in the array

  for (INT  iIndex = iNumToCopyIn - 1; iIndex >= 0; --iIndex)
    {
    ((RegExMatch *) pArray) [iStartOffsetIn + iIndex] = ((RegExMatch *) pvSourceDataIn) [iIndex + iSourceOffsetIn];
    };
  };


//-----------------------------------------------------------------------------
VOID  RegExMatchArray::AverageValues  (INT    iIndexAverage,
                                        INT    iIndexValueOne,
                                        INT    iIndexValueTwo,
                                        FLOAT  fBias)
  {
  // note:  You can't average reg ex match entries.
  };


//-----------------------------------------------------------------------------
VOID  RegExMatchArray::SwapIndexes  (INT  iIndexOne,
                                      INT  iIndexTwo)
  {
  RegExMatch                    fTemp = ((RegExMatch *) pArray) [iIndexOne];
  ((RegExMatch *) pArray) [iIndexOne] = ((RegExMatch *) pArray) [iIndexTwo];
  ((RegExMatch *) pArray) [iIndexTwo] = fTemp;
  };
  
  
//-----------------------------------------------------------------------------
INT  RegExMatchArray::CompareIndexes  (INT  iIndexOne,
                                        INT  iIndexTwo)
  {
  // you can't compare reg ex matche entries
  return (0);
  };


//-----------------------------------------------------------------------------
EStatus RegExMatchArray::Set (RegExMatch   remElement,
                               INT           iIndex)
  {
  if (iIndex >= iLength) return (EStatus::kFailure);
  ((RegExMatch *) pArray) [iIndex] = remElement;
  return (EStatus::kSuccess);
  };


//-----------------------------------------------------------------------------
EStatus RegExMatchArray::Append (RegExMatch  remElement)
  {
  INT  iOldLength = iLength;

  if (SetLength (iOldLength + 1) == EStatus::kFailure) {return EStatus::kFailure;};

  ((RegExMatch *) pArray) [iOldLength] = remElement;
  return (EStatus::kSuccess);
  };

//-----------------------------------------------------------------------------
EStatus RegExMatchArray::GetArray (RegExMatch  aOut []) const
  {
  if (iLength == 0) return (EStatus::kFailure);

  for (INT  iIndex = 0; iIndex < iLength; ++iIndex)
    {
    aOut [iIndex] = ((RegExMatch *) pArray) [iIndex];
    };
  return (EStatus::kSuccess);
  };


//-----------------------------------------------------------------------------
EStatus RegExMatchArray::Copy (const RegExMatchArray &  arraySource)
  {
  if (SetLength (arraySource.iLength) == EStatus::kFailure) {return EStatus::kFailure;};

  CopyValues (arraySource.pArray, 0, 0, arraySource.Length ());

  return (EStatus::kSuccess);
  };


//-----------------------------------------------------------------------------
RegExMatch RegExMatchArray::operator[]  (INT  iIndex) const
  {
  return ((RegExMatch *) pArray) [iIndex];
  };


//-----------------------------------------------------------------------------
RegExMatch &  RegExMatchArray::operator[]  (INT  iIndex)
  {
  if (iIndex >= Length ())
    {
    // expand the size of the array so the index is valid
    
    SetLength (iIndex + 1);
    };

  
  return ((RegExMatch *) pArray) [iIndex];
  };


//-----------------------------------------------------------------------------
RegExMatchArray &  RegExMatchArray::operator=  (const RegExMatchArray &  arrayIn)
  {
  Copy (arrayIn);
  return (*this);
  };



