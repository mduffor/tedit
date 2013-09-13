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


#ifndef EDITORSETTINGS_HPP
#define EDITORSETTINGS_HPP

#include "Types.hpp"
#include "RStr.hpp"

class EditorSettings 
  {
  private:
    RStr  strClipBuffer;
    
  public:
  
    EditorSettings ();
    ~EditorSettings ();
    
    VOID          SaveClip  (const char *  szClipIn);
    VOID          ClearClip (VOID);
    RStr &        GetClip   (VOID);
    
    
  };




#endif // FORMATINFO_HPP