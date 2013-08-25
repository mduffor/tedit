/* -----------------------------------------------------------------
                            PolarFlow File Path

     This module implements cross-platform routines for dealing with
     file and directory paths

                            Michael T. Duffy
   ----------------------------------------------------------------- */

// FilePath.hpp
// Copyright 2004, 2008,  Michael T. Duffy.  
// contact:  mduffor@users.sourceforge.net

// This file is part of PolarFlow.
//
// PolarFlow is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 3 as
// published by the Free Software Foundation.
//
// PolarFlow is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with PolarFlow.  If not, see <http://www.gnu.org/licenses/>.

#ifndef FILEPATH_HPP
#define FILEPATH_HPP


#include "Types.hpp"
#include "RStr.hpp"
#include "RegEx.hpp"


#ifdef ANDROID_NDK
  #include <jni.h>
  #include <sys/types.h>
  #include <android/asset_manager.h>
  #include <android/asset_manager_jni.h>
  
  // global variables needed for interfacing with the java native interface 
  //   under Android.
  extern AAssetManager*  filePath_assetManager;
  
#endif // ANDROID_NDK


//------------------------------------------------------------------------
// Defines
//------------------------------------------------------------------------


//------------------------------------------------------------------------
// Forward Declarations
//------------------------------------------------------------------------



//------------------------------------------------------------------------
// Class Definitions
//------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class FilePath
  {
  private:
  
    RStr   strCurrPath;
  
  
  public:


                  FilePath      ();
                  
                  ~FilePath     ();

                                 /** @brief List Directories and Files.
                                 */
    RStrArray     ls             (RStr       strFullPathSpec = RStr (""),
                                  BOOL       bShowFiles  = true,
                                  BOOL       bShowDirs   = true,
                                  BOOL       bFullPaths  = true,
                                  EStatus *  statusOut   = NULL);

                                 /** @brief List Files Only.
                                 */
    RStrArray     lsf            (RStr       strFullPathSpec = RStr (""),
                                  BOOL       bFullPaths  = true,
                                  EStatus *  statusOut   = NULL);
    
                                 /** @brief List Directories Only.
                                 */
    RStrArray     lsd            (RStr       strFullPathSpec = RStr (""),
                                  BOOL       bFullPaths  = true, 
                                  EStatus *  statusOut   = NULL);

    
                                 /** @brief Change Directory.
                                 */
    EStatus       cd             (const RStr &  strDirIn,
                                  BOOL          bChangeShellDir = FALSE);

    static bool   DirExists      (RStr  strPathIn);
    
    static bool   FileExists     (RStr  strPathIn);

    static UINT   GetFileSize    (RStr  strPathIn);
    
    
    static VOID   MakeDir        (RStr  strPathIn);

    static RStr   GetCwd         (VOID);

    static VOID   DirTreeSearch  (RStr         strStartingPath,
                                  RegEx       rexSearchOne,
                                  RegEx       rexSearchTwo,
                                  RStrArray &  arrayAllPathsOut);

    static BOOL   DirTreeMatch   (RStr         strPathIn,
                                  RegEx       rexSearchOne,
                                  RegEx       rexSearchTwo);



    operator      RStr           () const   {return strCurrPath;};

    operator      const char *   () const   {return strCurrPath.AsChar ();};

                                 /** @brief Assignment operator.  Copies the contents of the given string to this string.
                                     @param strIn The string that will be copied.
                                     @return A reference to this string.
                                 */
    const RStr    operator =     (const RStr & strIn);

                                 /** @brief Assignment operator.  Copies the contents of the given zero terminated character array to this string.
                                     @param pszIn The character array that will be copied.
                                     @return A reference to this string.
                                 */
    const RStr    operator =     (const char *  pszIn);

    static UINT32  SplitPath     (RStr         strPathIn,
                                  UINT32       uSeparatorIn,
                                  RStrArray &  arrayOut);
    
    static EStatus  ReadFromFile (const char *     szFilenameIn,
                                  INT              iStartIndexIn,
                                  INT &            iBufferSizeInOut,
                                  unsigned char *  pbyBufferOut);

    static EStatus  WriteToFile  (const char *     szFilenameIn,
                                  INT              iStartOffsetIn,
                                  INT              iBytesToWriteIn,
                                  unsigned char *  pbyBufferIn);
  };

#endif // FILEPATH_HPP

