//
// SyntaxParser.cpp
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

#include "SyntaxParser.hpp"
#include "ncurses.h"


  
//------------------------------------------------------------------------------
SyntaxGroup::SyntaxGroup   (const char *  szNameIn)
  {
  strName   = szNameIn;
  pElements = NULL;
  pNext     = NULL;
  };

//------------------------------------------------------------------------------
SyntaxGroup::~SyntaxGroup  ()
  {
  ClearElements();
  };
    
//------------------------------------------------------------------------------
VOID SyntaxGroup::AddElement (const char *  szName,
                              const char *  szRexPatternIn,
                              INT           iColorIn)
  {
  SyntaxElement *  pNewElem = new SyntaxElement ();
  if (pNewElem == NULL) return;
  pNewElem->strName = szName;
  pNewElem->rexPattern.Set (szRexPatternIn);
  pNewElem->iColor = iColorIn;
  pNewElem->pNext = pElements;
  }

//------------------------------------------------------------------------------
VOID SyntaxGroup::ClearElements (VOID)
  {
  while (pElements != NULL)
    {
    SyntaxElement *  pToDelete = pElements;
    pElements = pElements->pNext;
    delete (pToDelete);
    }
  };

//------------------------------------------------------------------------------
VOID SyntaxGroup::Push (SyntaxGroup * *  ppList)
  {
  SyntaxGroup *  pPrev = NULL;
  SyntaxGroup *  pCurr = *ppList;
  
  while (pCurr != NULL)
    {
    pPrev = pCurr;
    pCurr = pCurr->pNext;
    }
    
  if (pPrev == NULL)
    {
    *ppList = this;
    }
  else
    {
    pPrev->pNext = this;
    }
  };
  
//------------------------------------------------------------------------------
VOID SyntaxGroup::FreeList (SyntaxGroup * *  ppList)
  {
  SyntaxGroup *  pToDelete = *ppList;
  SyntaxGroup *  pNext = NULL;
  
  while (pToDelete != NULL)
    {
    pNext = pToDelete->pNext;
    delete pToDelete;
    pToDelete = pNext;
    }
  *ppList = NULL;  
  }

//------------------------------------------------------------------------------
INT  SyntaxGroup::Parse (const char *  pszStringIn,
                         INT           iStringLengthIn,
                         INT &         iNumCharsReadOut)
  {
  // step through each syntax element and see if it matches
  const char *  pszMatchStartOut;
  INT32         iNumCharsRead;
                      
                      
  SyntaxElement * pCurrElem = pElements;
  while (pCurrElem != NULL) 
    {
    pCurrElem->rexPattern.Match (pszStringIn, 
                                 iStringLengthIn, 
                                 0,
                                 &pszMatchStartOut,
                                 iNumCharsRead);
    if (iNumCharsRead != 0)
      {
      // found a match
      iNumCharsReadOut = iNumCharsRead;
      
      if (pszMatchStartOut != pszStringIn) 
        {
        // matched somewhere other than the string start
        DBG_ERROR("SyntaxParser: Pattern %s does not match from the start of the line!");
        };
      
      return (pCurrElem->iColor);
      }
    pCurrElem = pCurrElem->pNext;
    }
  iNumCharsReadOut = 0;  
  return (0);
  }
  
//------------------------------------------------------------------------------
SyntaxParser::SyntaxParser  ()
  {
  pActiveGroup = NULL;
  
  pActiveGroup = CreateTestCPPGroup ();
  };

//------------------------------------------------------------------------------
SyntaxParser::~SyntaxParser ()
  {
  SyntaxGroup::FreeList (&pGroups);
  };

//------------------------------------------------------------------------------
INT SyntaxParser::Parse (const char *  pszStringIn, 
                         INT           iStringLengthIn,
                         INT &         iNumCharsReadOut)
  {
  if (pActiveGroup == NULL)
    {
    iNumCharsReadOut = 0;
    return 0;
    }
  return (pActiveGroup->Parse (pszStringIn, iStringLengthIn, iNumCharsReadOut));
  };

//------------------------------------------------------------------------------
BOOL SyntaxParser::ActivateGroup (const char *  szNameIn)
  {
  SyntaxGroup *  pSearch = pGroups;
  while (pSearch != NULL)
    {
    if (pSearch->Name () == szNameIn)
      {
      pActiveGroup = pSearch;
      return (TRUE);
      }
    }
  return (FALSE);
  }

//------------------------------------------------------------------------------
SyntaxGroup * SyntaxParser::CreateTestCPPGroup (VOID)
  {
  SyntaxGroup *  pNewGroup = new SyntaxGroup ("C++");
  
  pNewGroup->AddElement ("KeyWord", "^and|and_eq|asm|auto|bitand|bitor|break|case|catch|class|compl|constexpr|cast|continue|decltype|default|delete|do|dynamic_cast|else|explicit|export|extern|false|float|for|friend|goto|if|inline|mutable|namespace|new|noexcept|not|not_eq|nullptr|operator|or|or_eq|private|protected|public|register|reinterpret_cast|return|sizeof|static_assert|static_cast|struct|switch|template|this|thread_local|throw|true|try|typedef|typeid|typename|union|using|virtual|void|volatile|while|xor|exor_eq", COLOR_WHITE | A_BOLD);
  pNewGroup->AddElement ("DataType", "^const|static|char|int|float|double|bool|short|long|char16_t|char32_t|enum|wchar_t|signed|unsigned", COLOR_CYAN | A_BOLD);
  pNewGroup->AddElement ("Character", "^\'\\+.\'", COLOR_RED);
  pNewGroup->AddElement ("String", "^\"[a-zA-Z_][a-zA-Z_0-9]*\"",COLOR_RED);
  pNewGroup->AddElement ("Integer", "^[+-]*[0-9]+", COLOR_GREEN);
  //pNewGroup->AddElement ("Float", "", COLOR_GREEN);
  //pNewGroup->AddElement ("Octal", "",);
  pNewGroup->AddElement ("Punctuation", "", COLOR_CYAN);
  pNewGroup->AddElement ("LineComment", "^//.*$", COLOR_WHITE);
  //pNewGroup->AddElement ("BlockComment", "",);
  pNewGroup->AddElement ("PreProcessor", "^#if|#ifdef|#ifndef|#define|#include|#warning|#error|#pragma|#line", COLOR_MAGENTA);
  pNewGroup->AddElement ("NormalText", "[0-9a-zA-Z_]+", COLOR_WHITE | A_BOLD);
  /*
  addch
  
   #define COLOR_BLACK 0
#define COLOR_RED   1
#define COLOR_GREEN 2
#define COLOR_YELLOW    3
#define COLOR_BLUE  4
#define COLOR_MAGENTA   5
#define COLOR_CYAN  6
#define COLOR_WHITE 7
  */
  
  return (pNewGroup);
  };
