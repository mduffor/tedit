/* -----------------------------------------------------------------
                     TEdit Regular Expression Library

     This module implements regular expression and glob matching.

                            Michael T. Duffy
   ----------------------------------------------------------------- */

// RegEx.hpp
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

#ifndef RREGEX_HPP
#define RREGEX_HPP


#include "Types.hpp"
#include "RStr.hpp"
#include "RStrArray.hpp"
#include "BaseArray.hpp"
#include "IntArray.hpp"

//------------------------------------------------------------------------
// Defines
//------------------------------------------------------------------------

//------------------------------------------------------------------------
// Forward Declarations
//------------------------------------------------------------------------

//------------------------------------------------------------------------
// Class Definitions
//------------------------------------------------------------------------


//------------------------------------------------------------------------
class RegExMatch
  {
  public:

    enum  EType {kNull         = 0,     
                 kOr           = 1, 
                 kRepeat       = 2,
                 kAnyChar      = 3,
                 kChar         = 4, 
                 kCharRange    = 5, 
                 kAlnum        = 6, 
                 kAlpha        = 7, 
                 kBlank        = 8, 
                 kCntrl        = 9, 
                 kDigits       = 10, 
                 kGraph        = 11, 
                 kLower        = 12,
                 kPrint        = 13,
                 kPunct        = 14,
                 kSpace        = 15,
                 kUpper        = 16,
                 kXDigit       = 17,
                 kWordBoundary = 18,
                 kWordInside   = 19,
                 kWordBegin    = 20,
                 kWordEnd      = 21,
                 kWordChar     = 22,
                 kNotWordChar  = 23,
                 kBufferStart  = 24,
                 kBufferEnd    = 25,
                 kLineStart    = 26,
                 kLineEnd      = 27
                 };

  private:

    EType   eMatchType;
    UINT    uStartMatch;
    UINT    uEndMatch;
    BOOL    bInvertMatch;
    
    INT     iNextOne;
    INT     iNextTwo;

    INT     iMinMatches;
    INT     iMaxMatches;
    INT     iNumMatches;
    
    BOOL    bParsingError;
  
  public:
  
                   RegExMatch  ();

                   RegExMatch  (EType  eMatchTypeIn,
                                UINT   uMatchStartIn  = 0,
                                UINT   uMatchEndIn    = 0,
                                INT    iNextOneIn     = 0,
                                INT    iNextTwoIn     = 0,
                                BOOL   bInvertMatchIn = FALSE);
                    
                   RegExMatch (const RegExMatch &  matchIn); 
                    
                   ~RegExMatch ()                            {};
  
    RegExMatch &  operator=   (const RegExMatch &  remIn)   {eMatchType  = remIn.eMatchType;
                                                               uStartMatch = remIn.uStartMatch;
                                                               uEndMatch   = remIn.uEndMatch;
                                                               bInvertMatch= remIn.bInvertMatch;
                                                               iNextOne    = remIn.iNextOne;
                                                               iNextTwo    = remIn.iNextTwo;
                                                               iMinMatches = remIn.iMinMatches;
                                                               iMaxMatches = remIn.iMaxMatches;
                                                               iNumMatches = remIn.iNumMatches;
                                                               return *this;};

    BOOL           IsMatch     (const char *  pchIn,
                                const char *  pchBufferStart,
                                const char *  pchBufferEnd);

    BOOL           RawMatch    (const char *  pchIn,
                                const char *  pchBufferStart,
                                const char *  pchBufferEnd);
    
    VOID           SetType     (EType  eMatchTypeIn,
                                UINT   uStartMatchIn = 0x20,
                                UINT   uEndMatchIn   = 0x20)  {eMatchType = eMatchTypeIn; uStartMatch = uStartMatchIn; uEndMatch = uEndMatchIn;};

    UINT           GetStartMatch  (VOID)   {return uStartMatch;};

                   /** @brief Returns whether to increment to the next character if this matches.
                       @return True if the counter should be incremented, false if not.
                   */
    BOOL           IncOnMatch      (VOID);
                              
    BOOL           IsWordChar      (const char *  pchIn);
    
    VOID           InvertMatch     (BOOL  bInvertIn)          {bInvertMatch = bInvertIn;};

    BOOL           GetInvertMatch  (VOID)                     {return (bInvertMatch);};
    
    BOOL           IsNull          (VOID)                     {return (eMatchType == kNull);};

    BOOL           IsOr            (VOID)                     {return (eMatchType == kOr);};
    
    BOOL           IsRepeat        (VOID)                     {return (eMatchType == kRepeat);};

    BOOL           MatchOnLineEnd  (VOID);

    BOOL           IsError         (VOID)                     {return (bParsingError);};

    VOID           SetNextOne      (INT  iNextIn)             {iNextOne = iNextIn;};
    
    VOID           SetNextTwo      (INT  iNextIn)             {iNextTwo = iNextIn;};

    INT            GetNextOne      (VOID)                     {return (iNextOne);};
    
    INT            GetNextTwo      (VOID)                     {return (iNextTwo);};

    VOID           SetNext         (INT  iNextOneIn,
                                    INT  iNextTwoIn)          {iNextOne = iNextOneIn; iNextTwo = iNextTwoIn;};


    VOID           SetMatchMin     (INT  iMinIn)              {iMinMatches = iMinIn;};
    
    VOID           SetMatchMax     (INT  iMaxIn)              {iMaxMatches = iMaxIn;};

    INT            GetMatchMin     (VOID)                     {return iMinMatches;};
                                                    
    INT            GetMatchMax     (VOID)                     {return iMaxMatches;};

    VOID           SetNumMatches   (INT  iMinIn,
                                    INT  iMaxIn = -1)         {iMinMatches = iMinIn; iMaxMatches = iMaxIn;};

    VOID           ClearNumMatches (VOID)                     {iNumMatches = 0;};
    
    INT            GetNumMatches   (VOID)                     {return iNumMatches;};

    VOID           IncNumMatches   (VOID)                     {++iNumMatches;};

    BOOL           GetType         (const char *   pszPatternIn,
                                    BOOL           bInList,
                                    INT &          iSizeMatchedOut);


    const char *   GetTypeText     (VOID);
                              
  };



//-----------------------------------------------------------------------------
class RegExMatchArray : public BaseArray
  {
  private:

  public:

                 RegExMatchArray      ();
                 RegExMatchArray      (const RegExMatchArray &  arrayIn);
                 RegExMatchArray      (INT   iInitialSize);
                 ~RegExMatchArray     ();

    VOID *       AllocArray     (INT      iSizeIn);
    VOID         DeleteArray    (PVOID *  pvArrayIn);
    VOID         InitValues     (INT      iStartOffset,
                                 INT      iNumToInit);
    VOID         CopyValues     (PVOID    pvSourceData,
                                 INT      iSourceOffsetIn,
                                 INT      iStartOffset,
                                 INT      iNumToCopy);
    VOID         CopyValuesRev  (PVOID    pvData,
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

    EStatus      Copy           (const RegExMatchArray &  arraySource);
    EStatus      Set            (RegExMatch  remElement, INT  iIndex);
    EStatus      Append         (RegExMatch  remElement);
    EStatus      GetArray       (RegExMatch  []) const;

    RegExMatch         operator[]     (INT  iIndex) const;
    RegExMatch &       operator[]     (INT  iIndex);
    RegExMatchArray &  operator=      (const  RegExMatchArray & arrayIn);


  };


//------------------------------------------------------------------------
class RegExDeque
  {
  private:
  
    IntArray   aiValues;
    IntArray   aiNext;
    IntArray   aiPrev;
    
    INT         iPool;
    INT         iHead;
    INT         iTail;

  private:
  
    VOID  Grow          (INT   iGrowSizeIn);
    
    VOID  InitSentinels (VOID);
    
    INT   GetFromPool   (VOID);
    
    VOID  ReturnToPool  (INT   iIndexIn);

    
  public:

          RegExDeque  ();
          ~RegExDeque ();
    
    VOID  Push         (INT  iIn);
    INT   Pop          (VOID);
    VOID  Put          (INT  iIn);
   
    VOID  Clear        (VOID);
    
    BOOL  IsEmpty      (VOID)    {return (aiNext [iHead] == iTail);};

    BOOL  InQueueHead  (INT  iIn);

    BOOL  InQueueTail  (INT  iIn);

  
  };


//------------------------------------------------------------------------
class RegEx
  {
  public:
  
    enum EParser {kRegEx = 0,
                  kGlob  = 1} ;

  public:
    RegExMatchArray   arrayStates;
    RegExDeque        deque;

    RStr               strPattern;
    UINT32             iPatternPos;
    INT32              iState;
    
    BOOL               bParsingError;
    INT                iNumParenthesis;
                           
    
  private:

    INT   RegExExpression   (BOOL  bNegativeMatchIn = false);
         
    INT   RegExTerm         (BOOL  bNegativeMatchIn);
         
    INT   RegExFactor       (BOOL  bNegativeMatchIn);

    INT   RegExListInverted (VOID);
    
    INT   RegExListItemInverted (VOID);
    
    INT   RegExList         (VOID);

    INT   RegExListItem     (INT   iListStartState);

  public:
  
  
             RegEx      ();

             ~RegEx     ();

             RegEx      (const RegEx &  reIn);

             RegEx      (RStr          strExprIn,
                         EParser       eParseType = kRegEx);

             RegEx      (const char *  pszExprIn,
                         EParser       eParseType = kRegEx);

    EStatus  Set        (RStr          strExprIn,
                         EParser       eParseType = kRegEx);

    RStr     Match      (const RStr &  strSourceIn,
                         INT32         iSearchStartIn = 0,
                         RStrArray *   pstraSubstringsOut = NULL);

    VOID     Match      (const char *    szSourceIn,
                         INT32           iSourceLengthIn,
                         INT32           iSearchStartIn,
                         const char * *  szMatchStartOut,
                         INT32 &         iMatchSizeOut,          
                         RStrArray *     pstraSubstringsOut = NULL);
                   
    RStr     Substitute (RStr          strSourceIn,
                         RStr          strReplaceIn);


    EStatus  ParseRegEx (RStr          strExprIn);

    EStatus  ParseGlob  (RStr          strExprIn);

    VOID     Debug      (VOID);
    
    RStr     Pattern    (VOID)        {return (strPattern);};
  
  
  };

RStr  operator*  (const RStr &  strIn,   RegEx &      regexIn);
RStr  operator*  (RegEx &      regexIn, const RStr &  strIn);




#endif // REGEX_HPP
