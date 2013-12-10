//
// SyntaxParser.hpp
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

#ifndef SYNTAXPARSER_HPP
#define SYNTAXPARSER_HPP

#include "Types.hpp"
#include "RStr.hpp"
#include "RegEx.hpp"



/*
enum SyntaxType
{
  kNormalText
  kKeyWord = 2,
  kDataType
  
  kCharacter = 0,
  kString = 5,

  kInteger = 1,
  kFloatingPoint
  kOctal
  
  kPunctuation = 3,
  kComment = 4,
  kPreProcessor
}
*/

//=============================================================================
class SyntaxElement
  {
  public:
    RStr             strName;
    RegEx            rexPattern;
    INT              iColor;
    SyntaxElement *  pNext;
  };

//=============================================================================
class SyntaxGroup
  {
  private:
    RStr             strName;
    SyntaxElement *  pElements;
    SyntaxGroup *    pNext;
  
  public:
                 SyntaxGroup   (const char *  szNameIn);
                 ~SyntaxGroup  ();
    
    VOID         AddElement    (const char *  szName,
                                const char *  szRexPatternIn,
                                INT   iColorIn);
                       
    VOID         ClearElements (VOID);                   
    
    RStr &       Name          (VOID)  {return strName;};
                       
    VOID         Push          (SyntaxGroup * *  ppList);

    static VOID  FreeList      (SyntaxGroup * *  ppList);

    INT          Parse         (const char *  pszStringIn, 
                                INT           iStringLengthIn,
                                INT &         iNumCharsReadOut);
    
  };

//=============================================================================
class SyntaxParser
  {
  private:
    SyntaxGroup *  pGroups;
    SyntaxGroup *  pActiveGroup;
  
  public:
                   SyntaxParser  ();
                   ~SyntaxParser ();

    BOOL           ActivateGroup (const char *  szNameIn);

    VOID           ClearGroups   (VOID);
    
    INT            Parse         (const char *  pszStringIn, 
                                  INT           iStringLengthIn,
                                  INT &         iNumCharsReadOut);
    
    SyntaxGroup *  CreateTestCPPGroup (VOID);

    VOID           MarkupLine         (const char * szLine,
                                       int          iLineLength,
                                       int *        aiMarkupOut)    ;
    
  };  
  

#endif // SYNTAXPARSER_HPP