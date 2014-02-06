/* -----------------------------------------------------------------
                            TEdit File Path

     This module implements cross-platform routines for dealing with
     file and directory paths

                            Michael T. Duffy
   ----------------------------------------------------------------- */

// FilePath.cpp
// Copyright 2004, 2008, 2013, Michael T. Duffy.  
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


#include "Debug.hpp"
ASSERTFILE (__FILE__);

#ifdef ANDROID_NDK
  #define LINUX
  #include <assert.h>
  #include <jni.h>
  #include <string.h>
  #include <sys/types.h>
  #include <android/asset_manager.h>
  #include <android/asset_manager_jni.h>
  #include <android/log.h> 
  
  // global variables needed for interfacing with the java native interface 
  //   under Android.
  //JNIEnv*  filePath_jniEnv; 
  //jobject  filePath_jobjAssetManager;
  AAssetManager*  filePath_assetManager;

#endif // ANDROID_NDK

#ifdef LINUX
  #include <sys/stat.h>
  #include <unistd.h>
  #include <dirent.h>
  #include <stdio.h>
  #include <string.h>
#endif // LINUX


#include "RegEx.hpp"
#include "FilePath.hpp"


//-----------------------------------------------------------------------------
//  RAttrList
//-----------------------------------------------------------------------------

//------------------------------------------------------------------------
FilePath::FilePath ()
  {
  strCurrPath = "";
  };


//------------------------------------------------------------------------
FilePath::~FilePath ()
  {
  };


//------------------------------------------------------------------------
RStrArray  FilePath::lsf  (const char *  pszFullPathSpec,
                            BOOL         bFullPaths,
                            EStatus *    pStatusOut)
  {
  return (ls  (pszFullPathSpec, true, false, bFullPaths, pStatusOut));
  };

    
//------------------------------------------------------------------------
RStrArray  FilePath::lsd  (const char *  pszFullPathSpec,
                           BOOL          bFullPaths,
                           EStatus *     pStatusOut)
  {
  return (ls  (pszFullPathSpec, false, true, bFullPaths, pStatusOut));
  };


//------------------------------------------------------------------------
const RStr  FilePath::operator =  (const RStr & strIn)
  {
  cd (strIn.AsChar());
  return strCurrPath;
  };


//------------------------------------------------------------------------
const RStr  FilePath::operator =  (const char *  pszIn)
  {
  cd (pszIn);
  return strCurrPath;
  };

//------------------------------------------------------------------------------
UINT32 FilePath::SplitPath (const char *  pszPathIn,
                             UINT32       uSeparatorIn,
                             RStrArray &  arrayOut)
  {
  INT32      iCopyStart;
  UINT32     uNumStringsFound = 0;

  INT  iLength = strlen (pszPathIn);
  INT  iCurrPos = 0;
  
  while (iCurrPos < iLength)
    {
    // search for the separator
    iCopyStart = iCurrPos;
    while (uSeparatorIn != (unsigned char) pszPathIn [iCurrPos])
      {
      ++iCurrPos;
      if (iCurrPos >= iLength) break;
      };

    INT32  iStringLength = iCurrPos - iCopyStart;

    if (iStringLength > 0)
      {
      arrayOut.Append (RStr (""));
      RStr &  strOut = arrayOut [arrayOut.Length () - 1];

      if (iStringLength > 0)
        {
        RStr strPathIn (pszPathIn);
        strPathIn.GetMiddle (iCopyStart, iStringLength, strOut);
        };
      ++uNumStringsFound;
      };

    // skip the separator
    ++iCurrPos;
    };

  return (uNumStringsFound);
  };

  
  
#ifdef LINUX



#ifdef ANDROID_NDK  
//------------------------------------------------------------------------
RStrArray  FilePath::ls  (const char *  pszFullPathSpec,
                          BOOL          bShowFiles,
                          BOOL          bShowDirs,
                          BOOL          bFullPaths,
                          EStatus *     pStatusOut)
  {
  struct dirent *  pDirEntry;
  BOOL             bDone = false;
  RStrArray        arrayOut;
  struct stat      statINode;

  if (pStatusOut)  *pStatusOut = EStatus::kSuccess;

  // use the given path, or the current path, or the current directory
  
  RStr  strPathOut = pszFullPathSpec;
  if (strPathOut.IsEmpty ())
    {
    strPathOut = strCurrPath;
    };
  if (strPathOut.IsEmpty ())
    {
    strPathOut = ".";
    };
  // Note:  Extract any file spec for glob matching.

  RegEx  rexFileSpec ("*", RegEx::kGlob);
  INT32  iFinalSlash = strPathOut.ReverseFindChar ('/');
  if ((iFinalSlash != -1) && (iFinalSlash != (INT32 (strPathOut.GetLength ()) - 1)))
    {
    RStr  strFileSpec = strPathOut.ChompRight (strPathOut.GetLength () - iFinalSlash - 1);

    rexFileSpec.Set (strFileSpec, RegEx::kGlob);
    };

  // make sure the directory does NOT end in a slash
  strPathOut.StripTrailingChar ('/');

  //DBG_INFO ("Contents of directory: %s", strPathOut.AsChar ());
  
  AAssetDir *assetDir = AAssetManager_openDir (filePath_assetManager, strPathOut.AsChar ());
  
  if (assetDir == NULL)
    {
    // Error.  Couldn't open directory.
    RStr  strOut;
    strOut.Format ("FilePath::ls - Unable to open directory %s\n", strPathOut.AsChar ());
    DBG_INFO (strOut.AsChar ());
    if (pStatusOut)  *pStatusOut = EStatus (EStatus::kFailure, strOut.AsChar ());
    }
  else
    {
    while (!bDone)
      {
      const char * fName = AAssetDir_getNextFileName (assetDir);
      
      if (fName != NULL)
        {
        //DBG_INFO ("ls found file %s", fName);
        if (fName [0] != '.')
          {
          
          RStr  strFullPath = strPathOut + "/" + fName;

          //DBG_INFO (strFullPath.AsChar ());
          if ((bShowFiles) && (rexFileSpec.Match (fName) == fName))
            {
            if (bFullPaths)
              {
              arrayOut.Append (strFullPath);
              }
            else
              {
              arrayOut.Append (fName);
              };
            };
          // Note:  android NDK does not currently allow showing of directories.
            
          /*
          if (stat (strFullPath.AsChar (), &statINode) != -1)
            {
            if (bShowDirs && S_ISDIR (statINode.st_mode))
              {
              if (bFullPaths)
                {
                arrayOut.Append (strFullPath);
                }
              else
                {
                arrayOut.Append (fName);
                };
              };
            if (bShowFiles && S_ISREG (statINode.st_mode))
              {
              // Perform filename glob matching.
              if (rexFileSpec.Match (fName) == fName)
                {
                // we have a match.
                if (bFullPaths)
                  {
                  arrayOut.Append (strFullPath);
                  }
                else
                  {
                  arrayOut.Append (fName);
                  };
                };
              };
            };
          */
          }
        else
          {
          /* bypass hidden files and . and .. */;
          };
        }
      else
        {
        bDone = true;
        };
      }
    };
  //DBG_INFO ("Done reading directory");
  AAssetDir_close (assetDir);  
  return (arrayOut);
  };

#else
//------------------------------------------------------------------------
RStrArray  FilePath::ls  (const char *  pszFullPathSpec,
                          BOOL          bShowFiles,
                          BOOL          bShowDirs,
                          BOOL          bFullPaths,
                          EStatus *     pStatusOut)
  {
  DIR *            dirHandle;
  struct dirent *  pDirEntry;
  BOOL             bDone = false;
  RStrArray        arrayOut;
  struct stat      statINode;

  if (pStatusOut)  *pStatusOut = EStatus::kSuccess;

  // use the given path, or the current path, or the current directory
  
  RStr  strPathOut = pszFullPathSpec;
  if (strPathOut.IsEmpty ())
    {
    strPathOut = strCurrPath;
    };
  if (strPathOut.IsEmpty ())
    {
    strPathOut = ".";
    };
  // Note:  Extract any file spec for glob matching.

  RegEx  rexFileSpec ("*", RegEx::kGlob);
  INT32  iFinalSlash = strPathOut.ReverseFindChar ('/');
  if ((iFinalSlash != -1) && (iFinalSlash != (INT32 (strPathOut.GetLength ()) - 1)))
    {
    RStr  strFileSpec = strPathOut.ChompRight (strPathOut.GetLength () - iFinalSlash - 1);

    rexFileSpec.Set (strFileSpec, RegEx::kGlob);
    };

  // make sure the directory ends in a slash
  if (strPathOut [strPathOut.GetLength () -1] != '/')
    {
    strPathOut += '/';
    };

  
  if ((dirHandle = opendir (strPathOut.AsChar ())) == NULL)
    {
    // Error.  Couldn't open directory.
    RStr  strOut;
    strOut.Format ("FilePath::ls - Unable to open directory %s\n", strPathOut.AsChar ());
    if (pStatusOut)  *pStatusOut = EStatus (EStatus::kFailure, strOut.AsChar ());
    }
  else
    {  
    while (!bDone)
      {
      pDirEntry = readdir (dirHandle);
      if (pDirEntry != NULL)
        {
        if (pDirEntry->d_name [0] != '.')
          {
          RStr  strFullPath = strPathOut + pDirEntry->d_name;
          
          if (stat (strFullPath.AsChar (), &statINode) != -1)
            {
            if (bShowDirs && S_ISDIR (statINode.st_mode))
              {
              if (bFullPaths)
                {
                arrayOut.Append (strFullPath);
                }
              else
                {
                arrayOut.Append (pDirEntry->d_name);
                };
              };
            if (bShowFiles && S_ISREG (statINode.st_mode))
              {
              // Perform filename glob matching.
              if (rexFileSpec.Match (pDirEntry->d_name) == pDirEntry->d_name)
                {
                // we have a match.
                if (bFullPaths)
                  {
                  arrayOut.Append (strFullPath);
                  }
                else
                  {
                  arrayOut.Append (pDirEntry->d_name);
                  };
                };
              };
            };
          }
        else
          {
          /* bypass hidden files and . and .. */;
          };
        }
      else
        {
        bDone = true;
        };
      }
    };
  closedir (dirHandle);  

  return (arrayOut);
  };
#endif //ANDROID_NDK

//------------------------------------------------------------------------
EStatus  FilePath::cd  (const char *  pszDirIn,
                        BOOL          bChangeShellDir)
  {
  DIR *            dirHandle;
  RStr             strNewPath;
  
  if (pszDirIn == NULL) {return (EStatus::kFailure);};
  if ((unsigned char) pszDirIn [0] == UINT32 ('/'))
    {
    // absolute directory
    strNewPath = pszDirIn;
    }
  else
    {
    // relative path
    strNewPath = strCurrPath + pszDirIn;
    };
  
  // make sure the path ends in a slash
  
  if (strNewPath [strNewPath.GetLength () - 1] != UINT32 ('/'))
    {
    strNewPath += '/';
    };

  if ((dirHandle = opendir (strNewPath.AsChar ())) == NULL)
    {
    // Error.  Couldn't open directory.
    return (EStatus::kFailure);
    }                                                            

  // valid directory
  closedir (dirHandle);  

  strCurrPath = strNewPath;

  if (bChangeShellDir)
    {
    if (chdir (strCurrPath.AsChar ()) != 0)
      {
      return (EStatus::kFailure);
      };
    };

  return (EStatus::kSuccess);
  };


//------------------------------------------------------------------------------
bool FilePath::DirExists (const char *  pszPathIn)
  {
  struct stat  statInfo;
 
  // Note: May want to strip out double slashes here.
  // to be implemented
 
  if (stat (pszPathIn, &statInfo) == 0)
    {
    if (S_ISDIR (statInfo.st_mode))
      {
      return (true);
      };
    };
  return (false);
  };


#ifdef ANDROID_NDK  
//------------------------------------------------------------------------------
bool FilePath::FileExists (const char *  pszPathIn)
  {
  AAsset* asset = AAssetManager_open(filePath_assetManager, pszPathIn, AASSET_MODE_UNKNOWN);
  if (asset == NULL) 
    {
    return (false);
    }
  AAsset_close(asset); 
  return (true);
  };
#else
//------------------------------------------------------------------------------
bool FilePath::FileExists (const char *  pszPathIn)
  {
  struct stat  statInfo;
 
  if (stat (pszPathIn, &statInfo) == 0)
    {
    if (S_ISREG (statInfo.st_mode))
      {
      return (true);
      };
    };
  return (false);
  };
#endif

#ifdef ANDROID_NDK  
//------------------------------------------------------------------------------
UINT  FilePath::GetFileSize  (const char *  pszPathIn)
  {
  UINT     ulFileSize = 0;
  
  // read from the asset director via the asset manager.

  //AAssetManager* mgr = AAssetManager_fromJava (filePath_jniEnv, filePath_jobjAssetManager);
 
  AAsset* asset = AAssetManager_open(filePath_assetManager, pszPathIn, AASSET_MODE_UNKNOWN);
  if (asset == NULL) 
    {
    DBG_ERROR ("_ASSET_NOT_FOUND_ : %s", pszPathIn);
    return (0);
    }
    
  ulFileSize = AAsset_getLength (asset);
  AAsset_close(asset); 
  return (ulFileSize);
  };
#else // ANDROID_NDK
//------------------------------------------------------------------------------
UINT  FilePath::GetFileSize  (const char *  pszPathIn)
  {
  FILE *   fp;
  UINT     ulFileSize = 0;
  
  if ((fp = fopen (pszPathIn, "rb")) != NULL)
    {
    fseek (fp, 0, SEEK_END);
    ulFileSize = ftell (fp);
    };
  return (ulFileSize);
  };
#endif // ANDROID_NDK
  
  
  
//------------------------------------------------------------------------------
VOID FilePath::MakeDir (const char *  pszPathIn)
  {
  if (! DirExists (pszPathIn))
    {
    RStrArray   arrayPath;
    RStr        strCurrPath;

    INT  iNumDirs = SplitPath (pszPathIn, '/', arrayPath);
   
    for (INT  iDirIndex = 0; iDirIndex < iNumDirs; ++iDirIndex)
      {
      if ((iDirIndex == 0) && (pszPathIn[0] != '/'))
        {
        // path is relative, so don't start it with a slash.
        strCurrPath += arrayPath [iDirIndex];
        }
      else
        {
        strCurrPath += "/" + arrayPath [iDirIndex];
        };
      if (! DirExists (strCurrPath))
        {
        mkdir (strCurrPath.AsChar (), 0777);
        //printf ("Does not exist %s\n", strCurrPath.AsChar ());
        }
      else
        {
        //printf ("Exists %s\n", strCurrPath.AsChar ());
        };
      };
    };
  };


//------------------------------------------------------------------------------
const char *  FilePath::GetCwd  (VOID)
  {
  static CHAR   szBuffer [512];
  
  memset (szBuffer, 0, 512);
  if (getcwd (szBuffer, 511) == NULL)
    {
    return (RStr::kEmpty);
    };

  return (szBuffer);
  };



#endif // LINUX

  

















#ifdef WIN32

//------------------------------------------------------------------------
RStrArray  FilePath::ls  (const char *  pszFullPathSpec,
                          BOOL          bShowFiles,
                          BOOL          bShowDirs,
                          BOOL          bFullPaths,
                          EStatus *     pStatusOut)
  {
  BOOL             bDone = false;
  RStrArray        arrayOut;

  // Note:  Anytime you store or use a path in TEdit, use UNIX style forward slashes.
  //   However anytime a string is passed to windows, make sure you reverse the slashes
  //   to back-slashes.  You must do any regular expression compares as forward slashes
  //   otherwise you'll wind up escaping characters.


  if (pStatusOut)  *pStatusOut = EStatus::kSuccess;

  // use the given path, or the current path, or the current directory
  
  RStr  strPathOut = pszFullPathSpec;
  if (strPathOut.IsEmpty ())
    {
    strPathOut = strCurrPath;
    };
  if (strPathOut.IsEmpty ())
    {
    strPathOut = ".";
    };

  // Note:  Extract any file spec for glob matching.

  RegEx  rexFileSpec ("*", RegEx::kGlob);
  INT32  iFinalSlash = strPathOut.ReverseFindChar ('/');
  if ((iFinalSlash != -1) && (iFinalSlash != (INT32 (strPathOut.GetLength ()) - 1)))
    {
    RStr  strFileSpec = strPathOut.ChompRight (strPathOut.GetLength () - iFinalSlash - 1);

    rexFileSpec.Set (strFileSpec, RegEx::kGlob);
    };

  // make sure the directory ends in a slash
  if (strPathOut [strPathOut.GetLength () -1] != '/')
    {
    strPathOut += '/';
    };

  /*
  // if the first part of the filename is a drive letter, remove the opening slash.
  RStr  strDosPathOut = strPathOut;
  
  if (strDosPathOut * RegEx ("^/?[[:alpha:]]:") != "")
    {
    // found a drive letter
    if (strDosPathOut.GetAt (0) == UINT32 ('/'))
      {
      strDosPathOut.ClipLeft (1);
      };
    };
  */

  // convert to forward slashes so Windows can handle it.
  strPathOut.ReplaceChar ('/', '\\');
  
  // set a wildcard to search the entire directory.
  RStr  strPathOutWild = strPathOut + "*";


  WIN32_FIND_DATA fdFindFileData;


  HANDLE hFile = FindFirstFile (strPathOutWild.AsChar (), &fdFindFileData);

  if (hFile == INVALID_HANDLE_VALUE) 
    {
    // unable to find the first file.
    if (pStatusOut)  *pStatusOut = EStatus (EStatus::kFailure, RStr ("Unable to find first file for ") + strPathOutWild);
    }
  else
    {
    do 
      {
      if (fdFindFileData.cFileName [0] != '.')
        {
        RStr  strFullPath = strPathOut + fdFindFileData.cFileName;

        if (GetFileAttributes (strFullPath.AsChar ()) & FILE_ATTRIBUTE_DIRECTORY) 
          {
          // directory bit set.  This is a directory
          if (bShowDirs)
            {
            RStr  strPathReturned;
            
            if (bFullPaths)
              {
              strPathReturned = strFullPath;
              }
            else
              {
              strPathReturned = fdFindFileData.cFileName;
              };
            strPathReturned.ReplaceChar ('\\', '/');
            arrayOut.Append (strPathReturned);
            };
          }
        else
          {
          // this is a file
          if (bShowFiles)
            {
            // Perform filename glob matching.
            RStr  strFileNameReturned = fdFindFileData.cFileName;
            strFileNameReturned.ReplaceChar ('\\', '/');

            
            if (rexFileSpec.Match (strFileNameReturned) == strFileNameReturned)
              {
              // we have a match.
              RStr  strPathReturned;
              
              if (bFullPaths)
                {
                strPathReturned = strFullPath;
                }
              else
                {
                strPathReturned = strFileNameReturned;
                };
              strPathReturned.ReplaceChar ('\\', '/');
              arrayOut.Append (strPathReturned);
              };
            };
          };
        }
      else
        {
        /* bypass hidden files and . and .. */;
        };
      } while (FindNextFile (hFile, &fdFindFileData) != 0);
    
    FindClose (hFile);
    };

  return (arrayOut);
  };


//------------------------------------------------------------------------
EStatus  FilePath::cd  (const RStr &  strDirIn,
                         BOOL          bChangeShellDir)
  {
  RStr             strNewPath;
  
  if (strDirIn [0] == UINT32 ('/'))
    {
    // absolute directory
    strNewPath = strDirIn;
    }
  else
    {
    // relative path
    strNewPath = strCurrPath + strDirIn;
    };
  
  // make sure the path ends in a slash
  
  if (strNewPath [strNewPath.GetLength () - 1] != UINT32 ('/'))
    {
    strNewPath += '/';
    };

  // if the first part of the filename is a drive letter, remove the opening slash.
  RStr  strDosPathOut = strNewPath;
  
  if (strDosPathOut * RegEx ("^/?[[:alpha:]]:") != "")
    {
    // found a drive letter
    if (strDosPathOut.GetAt (0) == UINT32 ('/'))
      {
      strDosPathOut.ClipLeft (1);
      };
    };
  

  // convert to forward slashes so Windows can handle it.
  strDosPathOut.ReplaceChar ('/', '\\');

  if (GetFileAttributes (strDosPathOut.AsChar ()) & FILE_ATTRIBUTE_DIRECTORY) 
    {
    // directory bit set.  This is a directory
    strCurrPath = strNewPath;

    strCurrPath.ReplaceChar ('\\', '/');


    return (EStatus::kSuccess);
    };
  return (EStatus::kFailure);
  };


//------------------------------------------------------------------------------
bool FilePath::DirExists (const char *  pszPathIn)
  {
  // Note: May want to strip out double slashes here.
  // to be implemented

  if (GetFileAttributes (pszPathIn) & FILE_ATTRIBUTE_DIRECTORY)
    {
    return (true);
    };
  return (false);
  };


//------------------------------------------------------------------------------
bool FilePath::FileExists (const char *  pszPathIn) const
  {
  // Note:  Ideally we should be using GetFileAttributes to check and see if this
  //   is a file or not, but apparently Windows is broken and can't find the file
  //   on a Linux server.  So we will try to open the file to see if exists.
  
  
  FILE *  fp;
  if ((fp = fopen (pszPathIn, "rb")) == NULL)
    {
    return (false);
    };
  return (true);

  //if (GetFileAttributes (pszPathIn) & FILE_ATTRIBUTE_NORMAL)
  //  {
  //  return (true);
  //  };
  //return (false);
  };



//------------------------------------------------------------------------------
VOID FilePath::MakeDir (const char *  pszPathIn)
  {
  if (! DirExists (pszPathIn))
    {
    RStrArray   arrayPath;
    RStr        strCurrPath;

    INT  iNumDirs = SplitPath (pszPathIn, '/', arrayPath);
   
    for (INT  iDirIndex = 0; iDirIndex < iNumDirs; ++iDirIndex)
      {
      if ((iDirIndex == 0) && (pszPathIn[0] != '/'))
        {
        // path is relative, so don't start it with a slash.
        strCurrPath += arrayPath [iDirIndex];
        }
      else
        {
        strCurrPath += "/" + arrayPath [iDirIndex];
        };
      if (! DirExists (strCurrPath))
        {
        CreateDirectory (strCurrPath.AsChar (), NULL);
        //printf ("Does not exist %s\n", strCurrPath.AsChar ());
        }
      else
        {
        //printf ("Exists %s\n", strCurrPath.AsChar ());
        };
      };
    };
  };


//------------------------------------------------------------------------------
RStr  FilePath::GetCwd  (VOID)
  {
  // to be implemented
  return ("");
  };




#endif // WIN32


//------------------------------------------------------------------------------
VOID  FilePath::DirTreeSearch  (const char *  pszStartingPath,
                                RegEx         rexSearchOne,
                                RegEx         rexSearchTwo,
                                RStrArray &   arrayAllPathsOut)
  {
  // recursively search the directory tree, starting at the directory passed as 
  //  the starting path.  If you find a directory containing both rexSearchOne
  //  and rexSearchTwo (either in the files or directories), then add this directory
  //  to arrayAllPathsOut.  We search all sub-directories, except the sub directories
  //  whose parent matched the search parameters.  So leaves cannot contain more leaves.

  RStrArray  arrayToSearch;
  INT        iSearchPos = 0;
  RStrArray  arraySubDirs;
  EStatus    statusDirs;
  INT        iIndex;
  FilePath  filePath;
  INT        iFoundCount = 0;
  

  //printf ("Start path %s\n", pszStartingPath);  
  // initialize search parameters
  arrayToSearch.Append (pszStartingPath);
  
  while (arrayToSearch.Length () > iSearchPos)
    {
    RStr   strCurrPath = arrayToSearch [iSearchPos];
    ++iSearchPos;

    //printf ("Searching path %s\n", strCurrPath.AsChar ());

    if (DirTreeMatch  (strCurrPath, rexSearchOne, rexSearchTwo))
      {
      arrayAllPathsOut.Append (strCurrPath);
      ++iFoundCount;
      }
    else
      {
      // still no match.  Add the child directories to the search path.
      arraySubDirs = filePath.lsd (strCurrPath, true, &statusDirs);
      INT  iNumDirs = arraySubDirs.Length ();

      for (iIndex = 0; iIndex < iNumDirs; ++iIndex)
        {
        // make sure the directory ends in a slash
        if (arraySubDirs [iIndex].GetAt (arraySubDirs [iIndex].GetLength () - 1) != '/')
          {
          arraySubDirs [iIndex] += "/";
          };

        arrayToSearch.Append (arraySubDirs [iIndex]);
        //printf ("Adding sub dir %s\n", arraySubDirs [iIndex].AsChar ());
        };
      };
    };
    
  // if we didn't find anything, we may be in a subdirectory of a desired directory.
  //  search directly back up the path for a matching directory, but don't consider
  //  searching down subdirectories for matches.
  
  if (iFoundCount == 0)
    {
    RStr  strCurrPath = pszStartingPath;
    
    do 
      {
      INT  iSlashPos = strCurrPath.ReverseFindChar ('/', strCurrPath.GetLength () - 2);
      
      if (iSlashPos == -1) break;
      
      strCurrPath.TruncateRight (iSlashPos);

      //printf ("Tail Searching path %s\n", strCurrPath.AsChar ());

      if (DirTreeMatch  (strCurrPath, rexSearchOne, rexSearchTwo))
        {
        arrayAllPathsOut.Append (strCurrPath);
        break;
        }
      } while (strCurrPath.GetLength () > 2);
    };
  };


//------------------------------------------------------------------------------
BOOL  FilePath::DirTreeMatch  (const char *  pszPathIn,
                               RegEx         rexSearchOne,
                               RegEx         rexSearchTwo)
  {
  // this routine is called by DirTreeSearch.  It checks the given path for the existance
  //  of the two search expressions in either files or subdirectories.

  RStrArray  arraySubDirs;
  RStrArray  arrayFiles;
  EStatus    statusFiles;
  EStatus    statusDirs;
  FilePath  filePath;
  INT        iIndex;
  BOOL       bMatchOne = (rexSearchOne.Pattern () == "") ? true : false;
  BOOL       bMatchTwo = (rexSearchTwo.Pattern () == "") ? true : false;
  
  // get the directory and file listings
  arrayFiles   = filePath.lsf (pszPathIn, true, &statusFiles);
  arraySubDirs = filePath.lsd (pszPathIn, true, &statusDirs);

  // check the files for a match  
  if (statusFiles == EStatus::kSuccess)
    {
    INT  iNumFiles = arrayFiles.Length ();
      
    for (iIndex = 0; iIndex < iNumFiles; ++iIndex)
      {
      //printf ("Considering file %s\n", arrayFiles [iIndex].AsChar ());
      if (arrayFiles [iIndex] * rexSearchOne != "") 
        {
        // match
        bMatchOne = true;
        }
      else if (arrayFiles [iIndex] * rexSearchTwo != "") 
        {
        // match
        bMatchTwo = true;
        };
      
      if ((bMatchOne) && (bMatchTwo))  
        {
        // match
        //printf ("Found file %s\n", arrayFiles [iIndex].AsChar ());
        break;
        };
      };
    };

  if ((! bMatchOne) || (! bMatchTwo))
    {
    // didn't match on the files.  Try the directories.

    // check the directories for a match
    if (statusDirs == EStatus::kSuccess)
      {
      INT  iNumDirs = arraySubDirs.Length ();
        
      for (iIndex = 0; iIndex < iNumDirs; ++iIndex)
        {
        // make sure the directory ends in a slash
        if (arraySubDirs [iIndex].GetAt (arraySubDirs [iIndex].GetLength () - 1) != '/')
          {
          arraySubDirs [iIndex] += "/";
          };

        if (arraySubDirs [iIndex] * rexSearchOne != "")
          {
          bMatchOne = true;
          }
        else if (arraySubDirs [iIndex] * rexSearchTwo != "")
          {
          bMatchTwo = true;
          };
        
        if ((bMatchOne) && (bMatchTwo))  
          {
          // match
          //printf ("Found dir %s %s\n", arraySubDirs [iIndex].AsChar (), pszPathIn);
          break;
          };
        };
      };
    };
  return (bMatchOne && bMatchTwo);
  };


#ifndef ANDROID_NDK
//------------------------------------------------------------------------------
EStatus  FilePath::ReadFromFile  (const char *     szFilenameIn,
                                   INT              iStartIndexIn,
                                   INT &            iBufferSizeInOut,
                                   unsigned char *  pbyBufferOut)
  {
  /// Reads up to iBufferSize bytes into pbyBufferOut from szFilenameIn starting at offset iStartIndexIn.
  /// Does not do any of its own allocation; only uses the buffer provided.
  
  FILE *             fp;
  RStr               strFilenameFinal = szFilenameIn;


  // Make sure input values are valid
  //ASSERT (strFilenameFinal != "");
  if (strFilenameFinal == RStr("")) return EStatus (EStatus::kFailure, "FilePath::ReadFromFile () :  Empty filename passed");
  if (iBufferSizeInOut == 0) return EStatus (EStatus::kFailure, "FilePath::ReadFromFile () : Zero length buffer passed.");

  #ifdef WIN32
    strFilenameFinal.ReplaceChar ('/', '\\');
  #endif

  fp = fopen (strFilenameFinal.AsChar (), "rb");
  if (fp == NULL)
    {
    RStr  strOut;
    strOut.Format ("FilePath::ReadFromFile - Unable to open file %s", strFilenameFinal.AsChar ());
    return (EStatus (EStatus::kFailure, strOut.AsChar ()));
    };

  fseek (fp, 0, SEEK_END);
  UINT32  uSize = ftell (fp);
  fseek (fp, iStartIndexIn, SEEK_SET);

  iBufferSizeInOut = TMin (iBufferSizeInOut, (INT) uSize);

  if (fread (pbyBufferOut, 1, (UINT) iBufferSizeInOut, fp) != (UINT) iBufferSizeInOut)
    {
    fclose (fp);
    return (EStatus (EStatus::kFailure, "FilePath::ReadFromFile () : Unable to read entire file"));
    };

  fclose (fp);
  return (EStatus::kSuccess);
  };
#endif // NOT ANDROID_NDK


#ifdef ANDROID_NDK
//------------------------------------------------------------------------------
EStatus  FilePath::ReadFromFile  (const char *     szFilenameIn,
                                   INT              iStartIndexIn,
                                   INT &            iBufferSizeInOut,
                                   unsigned char *  pbyBufferOut)
  {
  /// Reads up to iBufferSize bytes into pbyBufferOut from szFilenameIn starting at offset iStartIndexIn.
  /// Does not do any of its own allocation; only uses the buffer provided.
  
  if ((szFilenameIn == NULL) || (szFilenameIn[0] == '\0'))  return EStatus (EStatus::kFailure, "FilePath::ReadFromFile () :  Empty filename passed");
  if (iBufferSizeInOut == 0) return EStatus (EStatus::kFailure, "FilePath::ReadFromFile () : Zero length buffer passed.");
  
  // read from the asset director via the asset manager.

  //AAssetManager* mgr = AAssetManager_fromJava (filePath_jniEnv, filePath_jobjAssetManager);
 
  AAsset* asset = AAssetManager_open(filePath_assetManager, szFilenameIn, AASSET_MODE_UNKNOWN);
  if (asset == NULL) 
    {
    __android_log_print(ANDROID_LOG_ERROR, "FilePath", "_ASSET_NOT_FOUND_");
    RStr  strOut ("FilePath::ReadFromFile () : Unable to find asset ");
    strOut += szFilenameIn;
    return EStatus (EStatus::kFailure, strOut.AsChar ());
    }
    
  long lSize = AAsset_getLength (asset);
  iBufferSizeInOut = TMin (iBufferSizeInOut, lSize);
  
  AAsset_read (asset, pbyBufferOut, iBufferSizeInOut);
  AAsset_close(asset); 
  
  return (EStatus::kSuccess);
  };
#endif // ANDROID_NDK  
  

//------------------------------------------------------------------------------
EStatus  FilePath::WriteToFile  (const char *     szFilenameIn,
                                 BOOL             bAppend,
                                 INT              iBytesToWriteIn,
                                 unsigned char *  pbyBufferIn)
  {
  FILE *             fp;
  INT                iBytesToWrite = iBytesToWriteIn;
  RStr               strFilenameFinal = szFilenameIn;


  #ifdef WIN32
    strFilenameFinal.ReplaceChar ('/', '\\');
  #endif

  // Make sure input values are valid
  //ASSERT (strFilenameFinal != "");
  if (strFilenameFinal == RStr("")) return EStatus (EStatus::kFailure, "FilePath::WriteToFile () : Empty Filename");
  if (bAppend)
    {
    fp = fopen (strFilenameFinal.AsChar (), "ab");
    }
  else
    {
    fp = fopen (strFilenameFinal.AsChar (), "wb");
    }
  if (fp == NULL)
    {
    //sprintf (szDbgBuffer, "Unable to open file %s", szFilename); DebugMessage (szDbgBuffer);
    RStr  strOut ("FilePath::WriteToFile () : Unable to open file ");
    strOut += strFilenameFinal;
    return EStatus (EStatus::kFailure, strOut.AsChar ());
    };
  if (fwrite (pbyBufferIn, 1, iBytesToWrite, fp) != (unsigned int) iBytesToWrite)
    {
    fclose (fp);

    RStr  strOut ("FilePath::WriteToFile () : Unable to write to file ");
    strOut += strFilenameFinal;
    return EStatus (EStatus::kFailure, strOut.AsChar ());
    };

  fclose (fp);
  return (EStatus::kSuccess);
  };








