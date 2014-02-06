//
// GapBufferManager.cpp
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

#include "GapBuffer.hpp"
#include "GapBufferManager.hpp"


//-----------------------------------------------------------------------------
GapBufferManager::GapBufferManager ()
  {
  pBuffers = NULL;
  pCurrent = NULL;
  };

//-----------------------------------------------------------------------------
GapBufferManager::~GapBufferManager ()
  {
  FreeAllBuffers();
  };

//-----------------------------------------------------------------------------
VOID GapBufferManager::FreeAllBuffers()
  {
  while (pBuffers != NULL)
    {
    GapBuffer *  pBufferToDelete = pBuffers;
    pBuffers = pBuffers->pNext;
    delete pBufferToDelete;
    }
  };

//-----------------------------------------------------------------------------
VOID GapBufferManager::SetCurrent (const char *  szNameIn)
  {
  pCurrent = GetBuffer (szNameIn);
  };
  
//-----------------------------------------------------------------------------
VOID GapBufferManager::SetCurrent (GapBuffer*  pBufferIn)
  {
  GapBuffer *  pSearch = pBuffers;
  while (pSearch != NULL)
    {
    if (pSearch == pBufferIn)
      {
      pCurrent = pBufferIn;
      return;
      }
    pSearch = pSearch->pNext;
    }
  };

//-----------------------------------------------------------------------------
GapBuffer *  GapBufferManager::GetCurrent (VOID)
  {
  return pCurrent;
  };

//-----------------------------------------------------------------------------
GapBuffer *  GapBufferManager::GetBuffer (const char *  szNameIn)
  {
  GapBuffer *  pSearch = pBuffers;
  while (pSearch != NULL)
    {
    if (strcmp (pSearch->GetName(), szNameIn) == 0)
      {
      return (pSearch);
      }
    pSearch = pSearch->pNext;
    }
  return (NULL);
  };

//-----------------------------------------------------------------------------
GapBuffer *  GapBufferManager::GetBufferByFileName (const char *  szFileNameIn)
  {
  GapBuffer *  pSearch = pBuffers;
  while (pSearch != NULL)
    {
    if (strcmp (pSearch->GetFileName(), szFileNameIn) == 0)
      {
      return (pSearch);
      }
    pSearch = pSearch->pNext;
    }
  return (NULL);
  };
  
//-----------------------------------------------------------------------------
VOID GapBufferManager::SetNextToCurrent (VOID)
  {
  if (pCurrent != NULL)
    {
    pCurrent = pCurrent->pNext;
    }
  // if we reached the end of the list, loop around.  
  if (pCurrent == NULL)
    {
    pCurrent = pBuffers;
    }
  };

//-----------------------------------------------------------------------------
GapBuffer *  GapBufferManager::CreateBuffer (const char *  szNameIn)
  {
  GapBuffer *  pBufferNew = new GapBuffer();
  if (pBufferNew == NULL) 
    {
    return (NULL);
    }
  pBufferNew->SetName (szNameIn);
  // add to the end of the list
  GapBuffer * pBufferLast = NULL;
  GapBuffer * pBufferSearch = pBuffers;
  while (pBufferSearch != NULL)
    {
    pBufferLast = pBufferSearch;
    pBufferSearch = pBufferSearch->pNext;
    }
  if (pBufferLast == NULL)  
    {
    pBuffers = pBufferNew;
    pBuffers->pPrev = NULL;
    }
  else
    {
    pBufferLast->pNext = pBufferNew;
    pBufferNew->pPrev = pBufferLast;
    };
  pBufferNew->pNext = NULL;  
  SetCurrent (pBufferNew);
  return (pBufferNew);
  };

//-----------------------------------------------------------------------------
VOID GapBufferManager::ClearBuffer (const char *  szNameIn)
  {
  GapBuffer * pToClear = GetBuffer (szNameIn);
  if (pToClear != NULL)
    {
    pToClear->Clear();
    };
  };

//-----------------------------------------------------------------------------
VOID GapBufferManager::DeleteBuffer (const char *  szNameIn)
  {
  GapBuffer * pToDelete = GetBuffer (szNameIn);
  if (pToDelete != NULL)
    {
    if (pToDelete->pPrev != NULL)
      {
      pToDelete->pPrev->pNext = pToDelete->pNext;
      }
    else
      {
      pBuffers = pToDelete->pNext;
      }
    if (pToDelete->pNext != NULL)
      {
      pToDelete->pNext->pPrev = pToDelete->pPrev;
      };
    delete pToDelete;
    };
  };

//-----------------------------------------------------------------------------
const char *  GapBufferManager::GetCurrentBufferName ()
  {
  if (pCurrent != NULL)
    {
    return pCurrent->GetName ();
    }
  return NULL;
  };

  
//-----------------------------------------------------------------------------
GapBuffer *  GapBufferManager::OpenFile (const char *  pszFileNameIn)
  {
  // TODO:  Reduce name to just the file name instead of the entire path.
  
  GapBuffer *  pBuffer = CreateBuffer (pszFileNameIn);
  pBuffer->AllocBuffer (256);
  pBuffer->SetFileName (pszFileNameIn);
  pBuffer->Load ();
  pBuffer->FillGap ();
  pBuffer->SetCursor(1, 0);  
  return (pBuffer);
  }
  