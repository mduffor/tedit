/* -----------------------------------------------------------------
                            TEdit Standard Types

     This module contains type definitions for platform independence,
     and error reporting code.

                            Michael T. Duffy
   ----------------------------------------------------------------- */

// Types.cpp
// Copyright 2004, 2008, Michael T. Duffy.  
// contact:  mduffor@users.sourceforge.net

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


#include "Types.hpp"
#include "RStr.hpp"


//------------------------------------------------------------------------------
EStatus::EStatus  ()
  {
  eStatusCode     = kFailure;
  pstrDescription = NULL;
  };


//------------------------------------------------------------------------------
EStatus::EStatus  (ECode         codeIn)
  {
  eStatusCode     = codeIn;
  pstrDescription = NULL;
  };


//------------------------------------------------------------------------------
EStatus::EStatus  (const EStatus &  statIn)
  {
  eStatusCode     = statIn.eStatusCode;
  pstrDescription = NULL;

  SetDescription  (statIn.pstrDescription == NULL ? NULL : statIn.pstrDescription->AsChar ());
  };


//------------------------------------------------------------------------------
EStatus::EStatus  (ECode         codeIn,
                   const char *  pszDescriptionIn)
  {
  eStatusCode     = codeIn;
  pstrDescription = NULL;
  SetDescription  (pszDescriptionIn);
  };


//------------------------------------------------------------------------------
EStatus::~EStatus ()
  {
  if (pstrDescription != NULL)
    {
    delete (pstrDescription);
    };
  };


//------------------------------------------------------------------------------
EStatus &  EStatus::operator=  (const EStatus &  statIn)
  {
  eStatusCode = statIn.eStatusCode;

  SetDescription  (statIn.pstrDescription == NULL ? NULL : statIn.pstrDescription->AsChar ());

  return *this;
  };


//------------------------------------------------------------------------------
VOID  EStatus::SetDescription  (const char *  pszDescriptionIn)
  {

  if (pstrDescription != NULL)
    {
    *pstrDescription = "";
    };

  if (pszDescriptionIn != NULL)
    {
    if (pstrDescription == NULL)
      {
      pstrDescription = new RStr;
      };
    *pstrDescription = pszDescriptionIn;
    };
  };

//------------------------------------------------------------------------------
const char *  EStatus::GetDescription  (VOID)
  {
  if (pstrDescription != NULL)
    {
    return (pstrDescription->AsChar ());
    };
  return (RStr::kEmpty.AsChar ());
  };





