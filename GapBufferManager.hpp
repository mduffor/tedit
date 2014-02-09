//
// GapBufferManager.hpp
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

#ifndef GAPBUFFERMANAGER_HPP
#define GAPBUFFERMANAGER_HPP

#include "Types.hpp"
#include "GapBuffer.hpp"

//=============================================================================
class GapBufferManager
{
  private:
    GapBuffer *  pBuffers;
    GapBuffer *  pCurrent;
    
    
  public:
                  GapBufferManager     ();
                  ~GapBufferManager    ();
    VOID          FreeAllBuffers       (VOID);
    VOID          SetCurrent           (const char *  szNameIn);
    VOID          SetCurrent           (GapBuffer *  pBufferIn);
    GapBuffer *   GetCurrent           (VOID);
    GapBuffer *   GetBuffer            (const char *  szNameIn);
    GapBuffer *   GetBufferByFileName  (const char *  szFileNameIn);
    VOID          SetNextToCurrent     (VOID);
    GapBuffer *   CreateBuffer         (const char *  szNameIn);
    VOID          ClearBuffer          (const char *  szNameIn);
    VOID          DeleteBuffer         (const char *  szNameIn);
    const char *  GetCurrentBufferName (VOID);
    GapBuffer *   OpenFile             (const char *  pszFileNameIn);
    
    GapBuffer *   GetFirstBuffer       (VOID);
    GapBuffer *   GetNextBuffer        (GapBuffer *  pCurrBuffer);
    
    
};

#endif // GAPBUFFERMANAGER_HPP
