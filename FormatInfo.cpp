//
// FormatInfo.cpp
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

#include "FormatInfo.hpp"

static const INT LINE_FEED_CHAR = '\n';
static const INT NULL_CHAR = '\0';


//-----------------------------------------------------------------------------
FormatInfo::FormatInfo ()
  {
  };
  
//-----------------------------------------------------------------------------
FormatInfo::~FormatInfo ()
  {
  };
    
//-----------------------------------------------------------------------------
BOOL  FormatInfo::IsWordChar    (INT iCharIn)
  {
  // Note:  Eventually this should probably be set by configuration
  if (((iCharIn >= 'a') && (iCharIn <= 'z')) ||
      ((iCharIn >= 'A') && (iCharIn <= 'Z')) ||
      ((iCharIn >= '0') && (iCharIn <= '9')) ||
      (iCharIn == '_'))
    {
    return (TRUE);
    };
  return (FALSE);
  };

//-----------------------------------------------------------------------------
BOOL  FormatInfo::IsWhitespace  (INT iCharIn)
  {
  if ((iCharIn == ' ') || (iCharIn == '\t')) 
    {
    return (TRUE);
    };
  return (FALSE);
  }; 


//-----------------------------------------------------------------------------
BOOL  FormatInfo::IsEOL  (INT iCharIn)
  {
  if ((iCharIn == LINE_FEED_CHAR) || (iCharIn == NULL_CHAR)) 
    {
    return (TRUE);
    };
  return (FALSE);
  }; 
  
  