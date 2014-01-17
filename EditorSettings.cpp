//
// EditorSettings.cpp
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

#include "EditorSettings.hpp"

//-----------------------------------------------------------------------------
EditorSettings::EditorSettings ()
  {
  iTabSize = 2;
  bIndentOn = TRUE;
  bIndentOnBracket = FALSE;
  };
  
//-----------------------------------------------------------------------------
EditorSettings::~EditorSettings ()
  {
  };
    
//-----------------------------------------------------------------------------
VOID  EditorSettings::SaveClip  (const char *  szClipIn)
  {
  strClipBuffer = szClipIn;
  };
  
//-----------------------------------------------------------------------------
VOID  EditorSettings::ClearClip (VOID)
  {
  strClipBuffer.Reset();
  };
  
//-----------------------------------------------------------------------------
RStr &  EditorSettings::GetClip   (VOID)
  {
  return (strClipBuffer);
  };

//-----------------------------------------------------------------------------
const char *  EditorSettings::GetTabString  (VOID) 
  {
  // TODO: Replace with tabs or spaces equal to tab size. 
  return ("  ");
  }
  
//-----------------------------------------------------------------------------
BOOL  EditorSettings::IsIndentOpenBracket  (INT  iCharIn)
  {
  // TODO: Make configurable
  return (iCharIn == '{');
  }

//-----------------------------------------------------------------------------
BOOL  EditorSettings::IsIndentCloseBracket (INT  iCharIn)
  {
  // TODO: Make configurable
  return (iCharIn == '}');
  }
  