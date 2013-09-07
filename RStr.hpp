/* -----------------------------------------------------------------
                            TEdit String

    This module implements a string class.

                            Michael T. Duffy
   ----------------------------------------------------------------- */

// RStr.hpp
// Copyright 2004, 2008, Michael T. Duffy.  
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


#ifndef RSTR_HPP
#define RSTR_HPP


#include <ctype.h>
#include <stdarg.h>   // for variable argument functions (va_*)
#include "Types.hpp"

/**

  Overall description of the class goes here. <b> test1 </b>  <B> test1 </B>


  */
class RStr;

                  /** @brief Concatenation operator.  Concatenates string Two to the end of string One.
                      @param strOne The first string in the operation.
                      @param strTwo The second string in the operation.
                      @return The concatenated string.
                  */
RStr   operator+  (const RStr & strOne, const RStr & strTwo);

                  /** @brief Concatenation operator.  Concatenates string Two to the end of string One.
                      @param strOne The first string in the operation.
                      @param pszTwo The second string in the operation.
                      @return The concatenated string.
                  */
RStr   operator+  (const RStr & strOne, const char pszTwo []);

                  /** @brief Concatenation operator.  Concatenates string Two to the end of string One.
                      @param pszOne The first string in the operation.
                      @param strTwo The second string in the operation.
                      @return The concatenated string.
                  */
RStr   operator+  (const char * pszOne, const RStr & strTwo);

                  /** @brief Concatenation operator.  Concatenates character Two to the end of string One.
                      @param strOne The first string in the operation.
                      @param uTwo The second character in the operation.
                      @return The concatenated string.
                  */
RStr   operator+  (const RStr & strOne, const UINT32   uTwo);

                  /** @brief Concatenation operator.  Concatenates string Two to the end of character One.
                      @param uOne The first character in the operation.
                      @param strTwo The second string in the operation.
                      @return The concatenated string.
                  */
RStr   operator+  (const UINT32   uOne,  const RStr & strTwo);





                  /** @brief Equality operator.
                      @param strOne The first string to compare
                      @param strTwo The second string to compare
                      @return True if the two strings match (case sensitive), False if they don't.
                  */
BOOL operator== (const RStr & strOne, const RStr & strTwo);

                 /** @brief Equality operator.
                     @param strOne The first string to compare
                     @param pszTwo The second string to compare
                     @return True if the two strings match (case sensitive), False if they don't.
                 */
//BOOL operator== (const RStr & strOne, const char *      pszTwo);
//BOOL operator== (RStr & strOne, const char * pszTwo);
BOOL operator== (const RStr & strOne, const char *  pszTwo);
                 /** @brief Equality operator.
                     @param pszOne The first string to compare
                     @param strTwo The second string to compare
                     @return True if the two strings match (case sensitive), False if they don't.
                 */
BOOL operator== (const char *      pszOne, const RStr & strTwo);

                 /** @brief Equality operator.
                     @param strOne The first string to compare
                     @param strTwo The second string to compare
                     @return True if the two strings match (case sensitive), False if they don't.
                 */
BOOL operator!= (const RStr & strOne, const RStr & strTwo);

                 /** @brief Equality operator.
                     @param strOne The first string to compare
                     @param pszTwo The second string to compare
                     @return True if the two strings match (case sensitive), False if they don't.
                 */
BOOL operator!= (const RStr & strOne, const char *      pszTwo);

                 /** @brief Equality operator.
                     @param pszOne The first string to compare
                     @param strTwo The second string to compare
                     @return True if the two strings match (case sensitive), False if they don't.
                 */
BOOL operator!= (const char *      pszOne, const RStr & strTwo);




                  /** @brief Less than operator.
                      @param strOne The first string to compare
                      @param strTwo The second string to compare
                      @return True if the first string is less than the second (case sensitive), False otherwise.
                  */
BOOL operator< (const RStr & strOne, const RStr & strTwo);

                  /** @brief Less than operator.
                      @param strOne The first string to compare
                      @param pszTwo The second string to compare
                      @return True if the first string is less than the second (case sensitive), False otherwise.
                  */
BOOL operator< (const RStr & strOne, const char *      pszTwo);

                  /** @brief Less than operator.
                      @param pszOne The first string to compare
                      @param strTwo The second string to compare
                      @return True if the first string is less than the second (case sensitive), False otherwise.
                  */
BOOL operator< (const char *      pszOne, const RStr & strTwo);


                  /** @brief Greater than operator.
                      @param strOne The first string to compare
                      @param strTwo The second string to compare
                      @return True if the first string is greater than the second (case sensitive), False otherwise.
                  */
BOOL operator> (const RStr & strOne, const RStr & strTwo);

                  /** @brief Greater than operator.
                      @param strOne The first string to compare
                      @param pszTwo The second string to compare
                      @return True if the first string is greater than the second (case sensitive), False otherwise.
                  */
BOOL operator> (const RStr & strOne, const char *      pszTwo);

                  /** @brief Greater than operator.
                      @param pszOne The first string to compare
                      @param strTwo The second string to compare
                      @return True if the first string is greater than the second (case sensitive), False otherwise.
                  */
BOOL operator> (const char *      pszOne, const RStr & strTwo);


BOOL operator<= (const RStr & strOne, const RStr & strTwo);

BOOL operator<= (const RStr &  strOne, const char *  pszTwo);

BOOL operator<= (const char *  pszOne, const RStr &  strTwo);

BOOL operator>= (const RStr & strOne, const RStr & strTwo);

BOOL operator>= (const RStr &  strOne, const char *  pszTwo);

BOOL operator>= (const char *  pszOne, const RStr &  strTwo);


/// A String Class
//------------------------------------------------------------------------
class RStr
  {
  protected:
                   /// pointer to the string, always zero terminated
    char *         pszBuffer;
                   /// number of characters in the string, before the terminating zero.
    UINT32         uStringLength;
                   /// allocated size of the buffer
    UINT32         uBufferSize;

    UINT32         uGrowIncrement;

    static const UINT32   kESCAPE;        ///< The escape character 0x5c

  public:

    static       RStr     kEmpty;
    static       const char  szEmpty [1];

                                 /** @brief  Constructor
                                     @return None
                                 */
                  RStr           ();

                                 /** @brief  Constructor
                                     @param strIn the string used to initialize the class
                                     @return None
                                 */
                  RStr           (const RStr &  strIn);

                                 /** @brief  Constructor
                                     @param pszIn the null terminated character array used to initialize the class
                                     @return None
                                 */
                  RStr           (const char *  pszIn);
                  //RStr           (const char * szFormatString, ...);
                 
                                 /** @brief  Constructor
                                     @param uSizeIn The size to initialize the string buffer to.
                                     @return None
                                 */
                  RStr           (UINT32  uSizeIn);

                  
                  
                                 /** @brief  Destructor
                                     @return None
                                 */
    virtual       ~RStr          ();

                                 /** @brief  Initializes internal variables to their default values.
                                     @param uInitialSizeIn The size to preallocate the string buffer to.
                                     @return None
                                 */
    VOID          Init           (UINT32  uInitialSizeIn = 0);

                                 /** @brief Returns the length of the string
                                     @return Length of the string, not including the terminating null.
                                 */
    UINT32        GetLength      () const      {return uStringLength;};

                                 /** @brief Returns the length of the memory allocated to hold the string.  This will be larger or equal to the actual string.
                                     @return Length of the string, not including the terminating null.
                                 */
    UINT32        GetAllocSize   () const      {return uBufferSize;};


                                 /** @brief Returns the null terminated string.
                                     @return The string, terminated by a null character.
                                 */
    operator      const char *   () const   {return pszBuffer;};

                                 /** @brief Returns the null terminated string.
                                     @param iPosIn The zero-based offset into the buffer where the returned string pointer will start.  Defaults to 0.
                                     @return The string, terminated by a null character.
                                 */
    const char *  GetString      (INT32  iPosIn = 0) const   {return &pszBuffer [iPosIn];};

                                 /** @brief Returns a non-const pointer into the buffer.  Use very, very carefully since this circumvents all of RStr's buffer overflow checks.
                                     @param iPosIn The zero-based offset into the buffer where the returned string pointer will start.
                                     @return The string, terminated by a null character.
                                 */
    char *        GetBufferPtr   (INT32  iPosIn = 0) const   {return &pszBuffer [iPosIn];};

                                 /** @brief Returns the null terminated string.
                                     @param iPosIn The zero-based offset into the buffer where the returned string pointer will start.  Defaults to 0.
                                     @return The string, terminated by a null character.
                                 */
    const char *  AsChar         (INT32  iPosIn = 0) const   {return &pszBuffer [iPosIn];};

                                 /** @brief Returns the null terminated string.
                                     @param iPosIn The zero-based offset into the buffer where the returned string pointer will start.  Defaults to 0.
                                     @return The string, terminated by a null character.
                                 */
    const unsigned char *  AsUChar  (INT32  iPosIn = 0) const   {return (unsigned char *) &pszBuffer [iPosIn];};

                                 /** @brief Returns a character at the given zero based index.
                                     @param uPosIn The zero-based index of the desired character
                                     @return The character located at the given index.
                                 */
    UINT32        operator []    (UINT32  uPosIn)  {return ((UCHAR *) pszBuffer) [uPosIn];};

                                 /** @brief Returns a character at the given zero based index.
                                     @param iPosIn The zero-based index of the desired character
                                     @return The character located at the given index.
                                 */
    UINT32        GetAt          (INT32  iPosIn) const  {if (iPosIn >= 0) return ((UCHAR *) pszBuffer) [iPosIn]; else return '\0';};

                                 /** @brief Sets the character at the given zero based index.  If the value is past the end of the string, the string size will be increased accordingly so that the index given to SetAt is valid.
                                     @param uPosIn The zero-based index of the desired character
                                     @param uIn The value to set at the given index.
                                     @return None
                                 */
    VOID          SetAt          (UINT32  uPosIn,
                                  UINT32  uIn);


                                 /** @brief Extracts uCount number of characters from the left of the string.
                                     @param uCount The number of characters to read
                                     @param strResult The returned string containing the requested characters, or the entire string if uCount is greater than the number of characters in the string.
                                     @return None
                                 */
    VOID          GetLeft        (UINT32  uCount,
                                  RStr &  strResult) const;

                                 /** @brief Extracts uCount number of characters from the right of the string.
                                     @param uCount The number of characters to read
                                     @param strResult The returned string containing the requested characters, or the entire string if uCount is greater than the number of characters in the string.
                                     @return None
                                 */
    VOID          GetRight       (UINT32  uCount,
                                  RStr &  strResult) const;

                                 /** @brief Extracts uCount number of characters from the string, starting at zero-based position uStart.
                                     @param uStart The zero-based index of the first character to read
                                     @param uCount The number of characters to read
                                     @param strResult The returned string containing the requested characters.  If uCount would go beyond the end of the string, reading is stopped at the end of the string.
                                     @return None
                                 */
    VOID          GetMiddle      (UINT32  uStart,
                                  UINT32  uCount,
                                  RStr &  strResult) const;

                                 /** @brief Locate the first occurance of the given character, searching forwards.
                                     @param uIn The character to search for.
                                     @param uStartAt The inclusive zero-based index from where the search starts.
                                     @return The zero-based index of the found character, or -1 if the character is not found.
                                 */
    INT32         FindChar       (UINT32  uIn,
                                  UINT32  uStartAt = 0) const;

                                 /** @brief Locate the first occurance of the given character, searching backwards.
                                     @param uIn The character to search for.
                                     @param uStartAt The inclusive zero-based index from where the search starts.  A value of -1 (default) signifies that the search should start from the end of the string.
                                     @return The zero-based index of the found character, or -1 if the character is not found.
                                 */
    INT32         ReverseFindChar  (UINT32  uIn,
                                    UINT32  uStartAt = (UINT32) -1) const;

                                 /** @brief Locate the first occurance of the given string, searching forwards.
                                     @param strIn The string to search for.
                                     @param uStartAt The inclusive zero-based index from where the search starts.
                                     @return The zero-based index of the found character, or -1 if the character is not found.
                                 */
    INT32         Find           (const RStr &  strIn,
                                  UINT32        uStartAt = 0) const;
                                  
                                 /** @brief Locate the first occurance of the given string, searching forwards.
                                     @param szIn The string to search for.
                                     @param uStartAt The inclusive zero-based index from where the search starts.
                                     @return The zero-based index of the found character, or -1 if the character is not found.
                                 */
    INT32         Find           (const char *  szIn,
                                  UINT32        uStartAt = 0) const;
                                  
                                 /** @brief Replaces the search string with the replace string.
                                     @param strSearchString The string to search for.
                                     @param strReplaceString The string to replace the search string with
                                     @param bReplaceAll Whether to replace all occurances of the search string or just the first one.  Default is false (replace only first occurance.)
                                     @return Number of strings replaced by this operation.
                                 */
    INT           Replace        (RStr  strSearchString,
                                  RStr  strReplaceString,
                                  BOOL  bReplaceAll        = FALSE);
                                  
                                 /** @brief Compare this string with the given one.
                                     @param pszMatchIn The string to compare against.
                                     @param iNumCharsToMatch  The number of characters that need to match. A value of -1 matches the entire string.
                                     @param iStartPosition The position in this string to start the compare from.  Meant to support substring matching.
                                     @return 0 if the strings match, or a -1 or 1 if pszMatchIn is less than or greater than this string respectively.
                                 */
    INT32         Compare        (const char *  pszMatchIn,
                                  INT32         iNumCharsToMatch = -1,
                                  INT32         iStartPosition = 0) const;    // -1, 0 or 1

                                 /** @brief Compare this string with the given one.
                                     @param strMatchIn The string to compare against.
                                     @param iNumCharsToMatch  The number of characters that need to match. A value of -1 matches the entire string.
                                     @param iStartPosition The position in this string to start the compare from.  Meant to support substring matching.
                                     @return 0 if the strings match, or a -1 or 1 if strMatchIn is less than or greater than this string respectively.
                                 */
    INT32         Compare        (RStr          strMatchIn,
                                  INT32         iNumCharsToMatch = -1,
                                  INT32         iStartPosition = 0) const;    // -1, 0 or 1

                                 /** @brief Compare this string with the given one, ignoring case sensitivity.
                                     @param pszMatchIn The string to compare against.
                                     @param iNumCharsToMatch  The number of characters that need to match. A value of -1 matches the entire string.
                                     @param iStartPosition The position in this string to start the compare from.  Meant to support substring matching.
                                     @return 0 if the strings match, or a -1 or 1 if pszMatchIn is less than or greater than this string respectively.
                                 */
    INT32         CompareNoCase  (const char *  pszMatchIn,
                                  INT32         iNumCharsToMatch = -1,
                                  INT32         iStartPosition = 0) const;    // -1, 0 or 1

                                 /** @brief Compare this string with the given one, ignoring case sensitivity.
                                     @param strMatchIn The string to compare against.
                                     @param iNumCharsToMatch  The number of characters that need to match. A value of -1 matches the entire string.
                                     @param iStartPosition The position in this string to start the compare from.  Meant to support substring matching.
                                     @return 0 if the strings match, or a -1 or 1 if strMatchIn is less than or greater than this string respectively.
                                 */
    INT32         CompareNoCase  (RStr          strMatchIn,
                                  INT32         iNumCharsToMatch = -1,
                                  INT32         iStartPosition = 0) const;    // -1, 0 or 1

                                 /** @brief Replaces all occurances of the Search character with the Replace character.
                                     @param uSearchIn  The character to search for.
                                     @param uReplaceIn  The character that will replace the searched for character.
                                     @param uStartIn  The inclusive offset into the string where the search and replace operation starts.
                                     @param uEndIn  The exclusive offset into the string where the search and replace operation ends.
                                     @return Number of replacements performed.
                                 */
    INT32         ReplaceChar    (UINT32        uSearchIn,
                                  UINT32        uReplaceIn,
                                  UINT32        uStartIn = 0,
                                  UINT32        uEndIn   = 0);

                                 /** @brief Returns the number of times the given search character appears in the string.
                                     @param uSearchIn  The character to look for.
                                     @param uStartIn   The index to start searching from
                                     @return The number of times the uSearchIn character appears in the string.
                                 */    
    INT32         CountChar      (UINT32        uSearchIn,
                                  UINT32        uStartIn = 0);


    INT32         TranslateEscapedChars  (UINT32        uStartIn = 0,
                                          UINT32        uEndIn   = 0);


                                 /** @brief Check to see if this string has any characters in it.
                                     @return returns true if there aren't any characters in this string, false if there are.
                                 */
    bool          IsEmpty        (VOID) const   {return (uStringLength == 0);};

                                 /** @brief Deletes the string, setting its length to zero.  The internal buffer remains allocated.
                                     @return None
                                 */
    VOID          Empty          (VOID);

                                 /** @brief Deletes the string, setting its length to zero.  The internal buffer is also discarded.
                                     @return None
                                 */
    VOID          Reset          (VOID);

                                 /** @brief Increases the size of the internal buffer accept a string of the given length, if needed.  If the buffer is currently large enough, no change is made.
                                     @param uSizeIn Number of additional characters past the current string length the buffer should be allocated to hold, not including the terminating null.
                                     @return None
                                 */
    virtual VOID  Grow           (UINT32  uSizeIn);

                                 /** @brief Increases the size of the internal buffer accept a string of the given length, if needed.  If the buffer is currently large enough, no change is made.
                                     @param uSizeIn Absolute number of characters the buffer should be allocated to hold, not including the terminating null.
                                     @return None
                                 */
    virtual VOID  GrowAbsolute   (UINT32  uSizeIn);


    VOID          SetGrowIncrement  (UINT32  uIncrementIn)      {uGrowIncrement = TMax (1, uIncrementIn);};

    UINT32        GetGrowIncrement  (VOID)                      {return uGrowIncrement;};


                                 /** @brief Assignment operator.  Copies the contents of the given string to this string.
                                     @param strIn The string that will be copied.
                                     @return A reference to this string.
                                 */
    const RStr &  Set            (const RStr & strIn);

                                 /** @brief Assignment operator.  Copies the contents of the given zero terminated character array to this string.
                                     @param pszIn The character array that will be copied.
                                     @return A reference to this string.
                                 */
    const RStr &  Set            (const char *  pszIn);

// request: add start and end offsets to the Set commands to allow for setting from substrings directly


                                 /** @brief Assignment operator.  Copies the contents of the given string to this string.
                                     @param strIn The string that will be copied.
                                     @return A reference to this string.
                                 */
    const RStr &  operator =     (const RStr & strIn);

                                 /** @brief Assignment operator.  Copies the contents of the given zero terminated character array to this string.
                                     @param pszIn The character array that will be copied.
                                     @return A reference to this string.
                                 */
    const RStr &  operator =     (const char *  pszIn);

                                 /** @brief Append operator.  Appends the contents of the given string to this string.
                                     @param strIn The string that will be appended.
                                     @return None
                                 */
    VOID          operator +=    (const RStr &  strIn);

                                 /** @brief Append operator.  Appends the contents of the given zero terminated character array to this string.
                                     @param pszIn The character array that will be appended.
                                     @return None
                                 */
    VOID          operator +=    (const char *  pszIn);

                                 /** @brief Append operator.  Appends the given character to this string.
                                     @param uIn The character that will be appended.
                                     @return None
                                 */
    VOID          operator +=    (const UINT32  uIn);

                                 /** @brief Appends the contents of the given string to this string.
                                     @param strIn The string that will be appended.
                                     @return None
                                 */
    VOID          AppendString   (const RStr &  strIn);

                                 /** @brief Appends the contents of the given zero terminated character array to this string.
                                     @param pszIn The character array that will be appended.
                                     @return None
                                 */
    VOID          AppendString   (const char *  pszIn);

                                 /** @brief Appends the given character to this string.
                                     @param uIn The character that will be appended.
                                     @return None
                                 */
    VOID          AppendChar     (UINT32  uIn);

                                 /** @brief Appends the the first uCopyLengthIn characters of the given zero terminated character array to this string.
                                     @param pszIn The character array from which  characters to append will be read.
                                     @param iCopyLengthIn The number of characters to be copied
                                     @return None
                                 */
    VOID          AppendChars    (const char *   pszIn,
                                  INT32          iCopyLengthIn = -1);


                                 /** @brief Prepends the contents of the given string to this string.
                                     @param strIn The string that will be prepended.
                                     @return None
                                 */
    VOID          PrependString  (const RStr &  strIn);

                                 /** @brief Prepends the contents of the given zero terminated character array to this string.
                                     @param pszIn The character array that will be prepended.
                                     @return None
                                 */
    VOID          PrependString  (const char *  pszIn);

                                 /** @brief Prepends the given character to this string.
                                     @param uIn The character that will be prepended.
                                     @return None
                                 */
    VOID          PrependChar    (UINT32  uIn);

                                 /** @brief Inserts one or more copies of a character into a string
                                     @param uCharIn The character that will be inserted.
                                     @param uCount The number of copies of the character that will be inserted.
                                     @param uPosIn The zero-based index for the character before which the string will be inserted.
                                     @return None
                                 */
    VOID          InsertChars    (UINT32        uCharIn,
                                  UINT32        uCount,
                                  UINT32        uPosIn);

                                 /** @brief Inserts the contents of the given string to this string.
                                     @param strIn The string that will be inserted.
                                     @param uPosIn The zero-based index for the character before which the string will be inserted.
                                     @return None
                                 */
    VOID          InsertString   (const RStr &  strIn,
                                  UINT32        uPosIn);

                                 /** @brief Inserts the contents of the given zero terminated character array to this string.
                                     @param pszIn The zero terminated character array that will be prepended.
                                     @param uPosIn The zero-based index for the character before which the string will be inserted.
                                     @return None
                                 */
    VOID          InsertString   (const char *  pszIn,
                                  UINT32        uPosIn);

                                 /** @brief Removes characters from the left of the string.
                                     @param uCountIn The number of characters to remove.
                                     @return None
                                 */
    VOID          ClipLeft       (UINT32  uCountIn);     // removes count chars from left of string

                                 /** @brief Removes characters from the right of the string.
                                     @param uCountIn The number of characters to remove.
                                     @return None
                                 */
    VOID          ClipRight      (UINT32  uCountIn);     // removes count chars from right of string

                                 /** @brief Removes characters from the left of the string.
                                     @param uStartCharIn The zero-based index of the first character that will be removed.
                                     @param uCountIn The number of characters to remove.
                                     @return None
                                 */
    VOID          ClipMiddle     (UINT32  uStartCharIn,  // removes count chars from the middle of string
                                  UINT32  uCountIn);

                                 /** @brief Removes characters from the left of the string, and returns them.
                                     @param uCountIn The number of characters to remove.
                                     @return The characters that were removed.
                                 */
    RStr          ChompLeft      (UINT32  uCountIn);     // removes count chars from left of string

                                 /** @brief Removes characters from the right of the string, and returns them.
                                     @param uCountIn The number of characters to remove.
                                     @return The characters that were removed.
                                 */
    RStr          ChompRight     (UINT32  uCountIn);     // removes count chars from right of string


                                 /** @brief Returns characters from the middle of the string, without removing them.
                                     @param uStartCharIn The first character of the substring to return.
                                     @param uCountIn The number of characters to return.
                                     @return The characters that were removed.
                                 */
    RStr          SubString     (UINT32  uStartCharIn,
                                 UINT32  uCountIn);     // returns count chars from middle of string


                                 /** @brief Removes characters from the string before the given position
                                     @param uPosIn Exclusive zero-based index of the character before which characters will be removed.
                                     @return None
                                 */
    VOID          TruncateLeft   (UINT32  uPosIn);       // removes chars before position

                                 /** @brief Removes characters from the string after the given position
                                     @param uPosIn Exclusive zero-based index of the character after which characters will be removed.
                                     @return None
                                 */
    VOID          TruncateRight  (UINT32  uPosIn);       // removes chars after position

                                 /** @brief Sets contents to the given formatted string.  Formatting is similar to the C library printf command.
                                     @param szFormatString A printf type formatted string, followed by arguments to be used in the formatting
                                     @return None
                                 */
    VOID          Format         (const char * szFormatString, ...);


                                 /** @brief Appends the given formatted string to the current contents.  Formatting is similar to the C library printf command.
                                     @param szFormatString A printf type formatted string, followed by arguments to be used in the formatting
                                     @return None
                                 */
    VOID          AppendFormat   (const char * szFormatString, ...);

                                 /** @brief Concatenation operator.  Concatenates string Two to the end of string One.
                                     @param strOne The first string in the operation.
                                     @param strTwo The second string in the operation.
                                     @return The concatenated string.
                                 */
    friend RStr   operator+      (const RStr & strOne, const RStr & strTwo);

                                 /** @brief Concatenation operator.  Concatenates string Two to the end of string One.
                                     @param strOne The first string in the operation.
                                     @param pszTwo The second string in the operation.
                                     @return The concatenated string.
                                 */
    friend RStr   operator+      (const RStr & strOne, const char pszTwo []);

                                 /** @brief Concatenation operator.  Concatenates string Two to the end of string One.
                                     @param pszOne The first string in the operation.
                                     @param strTwo The second string in the operation.
                                     @return The concatenated string.
                                 */
    friend RStr   operator+      (const char * pszOne, const RStr & strTwo);

                                 /** @brief Concatenation operator.  Concatenates character Two to the end of string One.
                                     @param strOne The first string in the operation.
                                     @param uTwo The second character in the operation.
                                     @return The concatenated string.
                                 */
    friend RStr   operator+      (const RStr & strOne, const UINT32   uTwo);

                                 /** @brief Concatenation operator.  Concatenates string Two to the end of character One.
                                     @param uOne The first character in the operation.
                                     @param strTwo The second string in the operation.
                                     @return The concatenated string.
                                 */
    friend RStr   operator+      (const UINT32   uOne,  const RStr & strTwo);


                                 /** @brief Converts the string to all lower case
                                     @return None
                                 */
    VOID          ToLower        (VOID);

                                 /** @brief Converts the string to all upper case
                                     @return None
                                 */
    VOID          ToUpper        (VOID);


                                 /** @brief Converts the digit character at the given position to an integer.
                                     @param uPosIn The position in the string to test
                                     @return The value of the specified character converted to an int.  If the character is not a digit, a -1 is returned.
                                 */
    INT32         CharToInt      (UINT32  uPosIn)       {if (!isdigit (((UCHAR *) pszBuffer) [uPosIn])) return -1; return (((UCHAR *) pszBuffer) [uPosIn] - '0');};


                                 /** @brief Removes any number of the given character from the end of the string.
                                     @param uCharIn The character to remove.
                                     @return The number of characters removed.
                                 */
    INT32         StripTrailingChar (UINT32  uCharIn);

                                 /** @brief Remove any whitespace characters from the end of the string.
                                     @param bStripEOL Signals whether newline and carriage return characters are considered whitespace and removed.
                                     @return The number of characters removed.
                                 */
    INT32         StripTrailingWhitespace  (BOOL  bStripEOL = false);

    BOOL          StartsWith     (const char *  szIn);
    
    BOOL          EndsWith       (const char *  szIn);
    
  protected:

                                 /** @brief Sets the size of the internal buffer.  If the buffer is allocated smaller than the current string, the string is clipped.
                                     @param uSizeIn Minimum number of characters the buffer should be allocated to hold, not including the terminating null.
                                     @return None
                                 */
    VOID          SetBufferSize  (UINT32   uSizeIn);

                                 /** @brief Called internally by Format for each argument after the format string.
                                     @param pszFormatStringIn The formatted string passed to the Format command.
                                     @param vaArgListIn The list of arguments passed to Format
                                     @return None
                                 */
    VOID          FormatProcessArg  (const char *  pszFormatStringIn,
                                     va_list &     vaArgListIn);


                                 /** @brief Called internally by FormatProcessArg in order to process a single format marker
                                     @param pszFormatStringIn The current position in the format string passeed to the Format command.
                                     @param vaArgListIn The list of arguments passed to Format
                                     @return None
                                 */
    BOOL          FormatMarker      (const char * &  pszFormatStringIn,
                                     va_list &       vaArgListIn);

                                     

/*

// Miscellaneous implementation methods
protected:
    VOID NewFromString(const char* s, UINT32 slen, UINT32 prealloc);
    VOID _fastcall Buffer (UINT32 newlength);
    VOID _fastcall CoreAddChars(const char* s, UINT32 howmany);
    VOID FormatCore (const char* x, va_list& marker);
    BOOL FmtOneValue (const char*& x, va_list& marker);
public:
    // These are public to aVOID friend function definitions...
    static BYTE*  m_Cache[];            // Of size CSTR_DITEMS >> 2
    static SDesc  m_Null;

    static CRITICAL_SECTION m_CacheArbiter;
    static bool m_CacheArbiterInited;
    static UINT m_ArbiterWithinCS;

    // These may be reimplemented, see below
    static VOID ThrowIfNull(VOID* p);
    static VOID ThrowPgmError();
    static VOID ThrowNoUnicode();
    static VOID ThrowBadIndex();
#ifndef CSTR_LARGE_STRINGS
    static VOID ThrowTooLarge();
#endif

*/
  };


#endif // RSTR_HPP

