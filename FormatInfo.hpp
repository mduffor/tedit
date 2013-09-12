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


#ifndef FORMATINFO_HPP
#define FORMATINFO_HPP

#include "Types.hpp"

class FormatInfo 
  {
  private:
  
  public:
  
    FormatInfo ();
    ~FormatInfo ();
    
    BOOL  IsWordChar    (INT iCharIn); 
    BOOL  IsWhitespace  (INT iCharIn); 
    BOOL  IsEOL         (INT iCharIn);
    
  };




#endif // FORMATINFO_HPP