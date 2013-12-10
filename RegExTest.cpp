

#include "Types.hpp"
#include "Debug.hpp"
ASSERTFILE (__FILE__)

#include "RegEx.hpp"
#include "RegExTest.hpp"
#include <string.h>

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION (RegExTest);


//-----------------------------------------------------------------------------
void RegExTest::setUp ()
{
}


//-----------------------------------------------------------------------------
void RegExTest::tearDown ()
{
}

//-----------------------------------------------------------------------------
void RegExTest::testRegEx ()
  {
  RegEx        rexPattern;
  
  //rexPattern.Set("(int|float|char)");
  //rexPattern.Set("abc[^def]g");
  //rexPattern.Debug();

  //rexPattern.Set("(if|pragma)");
  //rexPattern.Debug();
  
  
  //CPPUNIT_ASSERT (TestMatch ("[+-]*[0-9]+", "norm !", "norm"));
  
  //CPPUNIT_ASSERT (TestMatch ("(int|float|char)", " float ", "float"));

  // many tests taken from http://hg.python.org/cpython/file/178075fbff3a/Lib/test/re_tests.py#l60
  
  // Test \0 is handled everywhere
  // Will revisit this later.  \0 denotes end of line, so I don't know if this functionality is practical
  //CPPUNIT_ASSERT (TestMatch ("\0", "\0", "\0"));
  //CPPUNIT_ASSERT (TestMatch ("[\0a]", "\0", "\0"));
  //CPPUNIT_ASSERT (TestMatch ("[a\0]", "\0", "\0"));
  //CPPUNIT_ASSERT (TestMatch ("[^a\0]", "\0", "", TRUE));
  
  
  // Test various letter escapes
  CPPUNIT_ASSERT (TestMatch ("\a[\b]\f\n\r\t\v", "\a\b\f\n\r\t\v","\a\b\f\n\r\t\v"));
  CPPUNIT_ASSERT (TestMatch ("[\a][\b][\f][\n][\r][\t][\v]", "\a\b\f\n\r\t\v", "\a\b\f\n\r\t\v"));
  
  CPPUNIT_ASSERT (TestMatch ("a.b", "acb", "acb"));
  //CPPUNIT_ASSERT (TestMatch ("a.b", "a\nb", "", TRUE)); // We don't match \n with '.'
  //CPPUNIT_ASSERT (TestMatch ("a.*b", "acc\nccb", "", TRUE)); // We don't match \n with '.'
  //CPPUNIT_ASSERT (TestMatch ("a.{4,5}b", "acc\nccb", "", TRUE)); // We don't match \n with '.'
  CPPUNIT_ASSERT (TestMatch ("a.b", "a\rb", "a\rb"));
  //CPPUNIT_ASSERT (TestMatch ("a.b(?s)", "a\nb", "a\nb")); // Python Extended Notation
  //CPPUNIT_ASSERT (TestMatch ("a.*(?s)b", "acc\nccb", "acc\nccb")); // Python Extended Notation
  //CPPUNIT_ASSERT (TestMatch ("(?s)a.{4,5}b", "acc\nccb", "acc\nccb")); // Python Extended Notation
  //CPPUNIT_ASSERT (TestMatch ("(?s)a.b", "a\nb", "a\nb")); // Python Extended Notation

  CPPUNIT_ASSERT (rexPattern.Set(")") == EStatus::kFailure); // Unmatched right bracket
  CPPUNIT_ASSERT (TestMatch ("", "", ""));                      // Empty pattern
  
  CPPUNIT_ASSERT (TestMatch ("abc", "abc", "abc"));
  CPPUNIT_ASSERT (TestMatch ("abc", "xbc", "", TRUE));
  CPPUNIT_ASSERT (TestMatch ("abc", "axc", "", TRUE));
  CPPUNIT_ASSERT (TestMatch ("abc", "abx", "", TRUE));
  CPPUNIT_ASSERT (TestMatch ("abc", "xabcy", "abc"));
  CPPUNIT_ASSERT (TestMatch ("abc", "ababc", "abc"));
  CPPUNIT_ASSERT (TestMatch ("ab*c", "abc", "abc"));
  CPPUNIT_ASSERT (TestMatch ("ab*bc", "abc", "abc"));
  CPPUNIT_ASSERT (TestMatch ("ab*bc", "abbc", "abbc"));
  CPPUNIT_ASSERT (TestMatch ("ab*bc", "abbbbc", "abbbbc"));
  CPPUNIT_ASSERT (TestMatch ("ab+bc", "abbc", "abbc"));
  CPPUNIT_ASSERT (TestMatch ("ab+bc", "abc", "", TRUE));
  CPPUNIT_ASSERT (TestMatch ("ab+bc", "abq", "", TRUE));
  CPPUNIT_ASSERT (TestMatch ("ab+bc", "abbbbc", "abbbbc"));
  CPPUNIT_ASSERT (TestMatch ("ab?bc", "abbc", "abbc"));
  CPPUNIT_ASSERT (TestMatch ("ab?bc", "abc", "abc"));
  CPPUNIT_ASSERT (TestMatch ("ab?bc", "abbbbc", "", TRUE));
  CPPUNIT_ASSERT (TestMatch ("ab?c", "abc", "abc"));
  CPPUNIT_ASSERT (TestMatch ("^abc$", "abc", "abc"));
  CPPUNIT_ASSERT (TestMatch ("^abc$", "abcc", "", TRUE));
  CPPUNIT_ASSERT (TestMatch ("^abc", "abcc", "abc"));
  CPPUNIT_ASSERT (TestMatch ("^abc$", "aabc", "", TRUE));
  CPPUNIT_ASSERT (TestMatch ("abc$", "aabc", "abc"));
  //TestMatch ("^", "abc+"-"", "-");
  //TestMatch ("$", "abc+"-"", "-");
  CPPUNIT_ASSERT (TestMatch ("a.c", "abc", "abc"));
  CPPUNIT_ASSERT (TestMatch ("a.c", "axc", "axc"));
  CPPUNIT_ASSERT (TestMatch ("a.*c", "axyzc", "axyzc"));
  CPPUNIT_ASSERT (TestMatch ("a.*c", "axyzd", "", TRUE));
  CPPUNIT_ASSERT (TestMatch ("a[bc]d", "abc", "", TRUE));
  CPPUNIT_ASSERT (TestMatch ("a[bc]d", "abd", "abd"));
  CPPUNIT_ASSERT (TestMatch ("a[b-d]e", "abd", "", TRUE));
  CPPUNIT_ASSERT (TestMatch ("a[b-d]e", "ace", "ace"));
  CPPUNIT_ASSERT (TestMatch ("a[b-d]", "aac", "ac"));
  CPPUNIT_ASSERT (TestMatch ("a[-b]", "a-", "a-"));
  CPPUNIT_ASSERT (TestMatch ("a[\\-b]", "a-", "a-"));

  CPPUNIT_ASSERT (TestMatch ("abc", "abc", "abc"));
  CPPUNIT_ASSERT (TestMatch ("abc", "xbc", "", TRUE));
  CPPUNIT_ASSERT (TestMatch ("abc", "axc", "", TRUE));
  CPPUNIT_ASSERT (TestMatch ("abc", "abx", "", TRUE));
  CPPUNIT_ASSERT (TestMatch ("abc", "xabcy", "abc"));
  CPPUNIT_ASSERT (TestMatch ("abc", "ababc", "abc"));
  CPPUNIT_ASSERT (TestMatch ("ab*c", "abc", "abc"));
  CPPUNIT_ASSERT (TestMatch ("ab*bc", "abc", "abc"));
  CPPUNIT_ASSERT (TestMatch ("ab*bc", "abbc", "abbc"));
  CPPUNIT_ASSERT (TestMatch ("ab*bc", "abbbbc", "abbbbc"));
  CPPUNIT_ASSERT (TestMatch ("ab{0,}bc", "abbbbc", "abbbbc"));
  CPPUNIT_ASSERT (TestMatch ("ab+bc", "abbc", "abbc"));
  CPPUNIT_ASSERT (TestMatch ("ab+bc", "abc", "", TRUE));
  CPPUNIT_ASSERT (TestMatch ("ab+bc", "abq", "", TRUE));
  CPPUNIT_ASSERT (TestMatch ("ab{1,}bc", "abq", "", TRUE));
  CPPUNIT_ASSERT (TestMatch ("ab+bc", "abbbbc", "abbbbc"));
  CPPUNIT_ASSERT (TestMatch ("ab{1,}bc", "abbbbc", "abbbbc"));
  CPPUNIT_ASSERT (TestMatch ("ab{1,3}bc", "abbbbc", "abbbbc"));
  CPPUNIT_ASSERT (TestMatch ("ab{3,4}bc", "abbbbc", "abbbbc"));
  CPPUNIT_ASSERT (TestMatch ("ab{4,5}bc", "abbbbc", "", TRUE));
  CPPUNIT_ASSERT (TestMatch ("ab?bc", "abbc", "abbc"));
  CPPUNIT_ASSERT (TestMatch ("ab?bc", "abc", "abc"));
  CPPUNIT_ASSERT (TestMatch ("ab{0,1}bc", "abc", "abc"));
  CPPUNIT_ASSERT (TestMatch ("ab?bc", "abbbbc", "", TRUE));
  CPPUNIT_ASSERT (TestMatch ("ab?c", "abc", "abc"));
  CPPUNIT_ASSERT (TestMatch ("ab{0,1}c", "abc", "abc"));
  CPPUNIT_ASSERT (TestMatch ("^abc$", "abc", "abc"));
  CPPUNIT_ASSERT (TestMatch ("^abc$", "abcc", "", TRUE));
  CPPUNIT_ASSERT (TestMatch ("^abc", "abcc", "abc"));
  CPPUNIT_ASSERT (TestMatch ("^abc$", "aabc", "", TRUE));
  CPPUNIT_ASSERT (TestMatch ("abc$", "aabc", "abc"));
  CPPUNIT_ASSERT (TestMatch ("^", "abc", ""));
  CPPUNIT_ASSERT (TestMatch ("$", "abc", ""));
  CPPUNIT_ASSERT (TestMatch ("a.c", "abc", "abc"));
  CPPUNIT_ASSERT (TestMatch ("a.c", "axc", "axc"));
  CPPUNIT_ASSERT (TestMatch ("a.*c", "axyzc", "axyzc"));
  CPPUNIT_ASSERT (TestMatch ("a.*c", "axyzd", "", TRUE));
  CPPUNIT_ASSERT (TestMatch ("a[bc]d", "abc", "", TRUE));
  CPPUNIT_ASSERT (TestMatch ("a[bc]d", "abd", "abd"));
  CPPUNIT_ASSERT (TestMatch ("a[b-d]e", "abd", "", TRUE));
  CPPUNIT_ASSERT (TestMatch ("a[b-d]e", "ace", "ace"));
  CPPUNIT_ASSERT (TestMatch ("a[b-d]", "aac", "ac"));
  CPPUNIT_ASSERT (TestMatch ("a[-b]", "a-", "a-"));
  CPPUNIT_ASSERT (TestMatch ("a[b-]", "a-", "a-"));
  
  CPPUNIT_ASSERT (rexPattern.Set("a[b-a]") == EStatus::kFailure);
  CPPUNIT_ASSERT (rexPattern.Set("a[]b") == EStatus::kFailure);
  CPPUNIT_ASSERT (rexPattern.Set("a[") == EStatus::kFailure);

  //CPPUNIT_ASSERT (TestMatch ("a]", "a]", "a]"));  // Currently we require escaping brackets, to detect incomplete lists.
  //CPPUNIT_ASSERT (TestMatch ("a[]]b", "a]b", "a]b"));  // Currently we require escaping brackets, to detect incomplete lists.
  CPPUNIT_ASSERT (TestMatch ("a[^bc]d", "aed", "aed"));
  CPPUNIT_ASSERT (TestMatch ("a[^bc]d", "abd", "", TRUE)); 
  CPPUNIT_ASSERT (TestMatch ("a[^-b]c", "adc", "adc")); 
  CPPUNIT_ASSERT (TestMatch ("a[^-b]c", "a-c", "", TRUE)); 
  //CPPUNIT_ASSERT (TestMatch ("a[^]b]c", "a]c", "", TRUE)); // Currently we require escaping brackets, to detect incomplete lists.
  //CPPUNIT_ASSERT (TestMatch ("a[^]b]c", "adc", "adc")); // Currently we require escaping brackets, to detect incomplete lists.
  CPPUNIT_ASSERT (TestMatch ("ab|cd", "abc", "ab"));
  CPPUNIT_ASSERT (TestMatch ("ab|cd", "abcd", "ab"));

  CPPUNIT_ASSERT (rexPattern.Set("abc)") == EStatus::kFailure);
  CPPUNIT_ASSERT (rexPattern.Set("(abc") == EStatus::kFailure);
  CPPUNIT_ASSERT (TestMatch ("a+b+c", "aabbabc", "abc"));
  CPPUNIT_ASSERT (TestMatch ("a{1,}b{1,}c", "aabbabc", "abc"));
  CPPUNIT_ASSERT (rexPattern.Set("a**") == EStatus::kFailure);
  //CPPUNIT_ASSERT (TestMatch ("a.+?c", "abcabc", "abc")); // Weird case we don't handle correctly.

  CPPUNIT_ASSERT (rexPattern.Set(")(") == EStatus::kFailure);
  CPPUNIT_ASSERT (TestMatch ("[^ab]*", "cde", "cde"));
  CPPUNIT_ASSERT (TestMatch ("abc", "", "", TRUE));
  CPPUNIT_ASSERT (TestMatch ("a|b|c|d|e", "e", "e"));
  CPPUNIT_ASSERT (TestMatch ("abcd*efg", "abcdefg", "abcdefg"));
  CPPUNIT_ASSERT (TestMatch ("ab*", "xabyabbbz", "ab")); 
  CPPUNIT_ASSERT (TestMatch ("ab*", "xayabbbz", "a"));
  CPPUNIT_ASSERT (TestMatch ("[abhgefdc]ij", "hij", "hij"));
  CPPUNIT_ASSERT (TestMatch ("^(ab|cd)e", "abcde", "", TRUE));
  CPPUNIT_ASSERT (TestMatch ("a[bcd]*dcdcde", "adcdcde", "adcdcde"));
  CPPUNIT_ASSERT (TestMatch ("a[bcd]+dcdcde", "adcdcde", "", TRUE));
  CPPUNIT_ASSERT (TestMatch ("[a-zA-Z_][a-zA-Z0-9_]*", "alpha", "alpha"));
  CPPUNIT_ASSERT (TestMatch ("(bc+d$|ef*g.|h?i(j|k))", "effg", "", TRUE));
  CPPUNIT_ASSERT (TestMatch ("(bc+d$|ef*g.|h?i(j|k))", "bcdd", "", TRUE));
  CPPUNIT_ASSERT (TestMatch ("multiple words of text", "uh-uh", "", TRUE));
  CPPUNIT_ASSERT (TestMatch ("multiple words", "multiple words, yeah", "multiple words"));
  
  
  // Most of the below tests deal with groups, which is not implemented yet.
  /*
  CPPUNIT_ASSERT (TestMatch ("()ef", "def+"-"+g1", "ef-"));
  CPPUNIT_ASSERT (TestMatch ("*a", "-', SYNTAX_ERROR),
  CPPUNIT_ASSERT (TestMatch ("(*)b", "-', SYNTAX_ERROR),
  CPPUNIT_ASSERT (TestMatch ("$b", "b", "", TRUE));
  CPPUNIT_ASSERT (TestMatch ("a\\", "-', SYNTAX_ERROR),
  CPPUNIT_ASSERT (TestMatch ("a\\(b", "a(b+"-"+g1", "a(b-Error"));
  CPPUNIT_ASSERT (TestMatch ("a\\(*b", "ab", "ab"));
  CPPUNIT_ASSERT (TestMatch ("a\\(*b", "a((b", "a((b"));
  CPPUNIT_ASSERT (TestMatch ("a\\\\b", "a\\b", "a\\b"));
  CPPUNIT_ASSERT (TestMatch ("((a))", "abc+"-"+g1+"-"+g2", "a-a-a"));
  CPPUNIT_ASSERT (TestMatch ("(a)b(c)", "abc+"-"+g1+"-"+g2", "abc-a-c"));
  CPPUNIT_ASSERT (TestMatch ("(a+|b)*", "ab+"-"+g1", "ab-b"));
  CPPUNIT_ASSERT (TestMatch ("(a+|b){0,}", "ab+"-"+g1", "ab-b"));
  CPPUNIT_ASSERT (TestMatch ("(a+|b)+", "ab+"-"+g1", "ab-b"));
  CPPUNIT_ASSERT (TestMatch ("(a+|b){1,}", "ab+"-"+g1", "ab-b"));
  CPPUNIT_ASSERT (TestMatch ("(a+|b)?", "ab+"-"+g1", "a-a"));
  CPPUNIT_ASSERT (TestMatch ("(a+|b){0,1}", "ab+"-"+g1", "a-a"));
  
  CPPUNIT_ASSERT (TestMatch ("a*", "", ""));
  CPPUNIT_ASSERT (TestMatch ("([abc])*d", "abbbcd+"-"+g1", "abbbcd-c"));
  CPPUNIT_ASSERT (TestMatch ("([abc])*bcd", "abcd+"-"+g1", "abcd-a"));
  
  CPPUNIT_ASSERT (TestMatch ("(a|b|c|d|e)f", "ef+"-"+g1", "ef-e"));
  CPPUNIT_ASSERT (TestMatch ("(ab|cd)e", "abcde+"-"+g1", "cde-cd"));
  CPPUNIT_ASSERT (TestMatch ("(abc|)ef", "abcdef+"-"+g1", "ef-"));
  CPPUNIT_ASSERT (TestMatch ("(a|b)c*d", "abcd+"-"+g1", "bcd-b"));
  CPPUNIT_ASSERT (TestMatch ("(ab|ab*)bc", "abc+"-"+g1", "abc-a"));
  CPPUNIT_ASSERT (TestMatch ("a([bc]*)c*", "abc+"-"+g1", "abc-bc"));
  CPPUNIT_ASSERT (TestMatch ("a([bc]*)(c*d)", "abcd+"-"+g1+"-"+g2", "abcd-bc-d"));
  CPPUNIT_ASSERT (TestMatch ("a([bc]+)(c*d)", "abcd+"-"+g1+"-"+g2", "abcd-bc-d"));
  CPPUNIT_ASSERT (TestMatch ("a([bc]*)(c+d)", "abcd+"-"+g1+"-"+g2", "abcd-b-cd"));
  
  CPPUNIT_ASSERT (TestMatch ("(ab|a)b*c", "abc+"-"+g1", "abc-ab"));
  CPPUNIT_ASSERT (TestMatch ("((a)(b)c)(d)", "abcd', SUCCEED, 'g1+"-"+g2+"-"+g3+"-"+g4", "abc-a-b-d"));
  CPPUNIT_ASSERT (TestMatch ("^a(bc+|b[eh])g|.h$", "abh+"-"+g1", "bh-None"));
  CPPUNIT_ASSERT (TestMatch ("(bc+d$|ef*g.|h?i(j|k))", "effgz+"-"+g1+"-"+g2", "effgz-effgz-None"));
  CPPUNIT_ASSERT (TestMatch ("(bc+d$|ef*g.|h?i(j|k))", "ij+"-"+g1+"-"+g2", "ij-ij-j"));
  CPPUNIT_ASSERT (TestMatch ("(bc+d$|ef*g.|h?i(j|k))", "reffgz+"-"+g1+"-"+g2", "effgz-effgz-None"));
  CPPUNIT_ASSERT (TestMatch ("((((((((((a))))))))))", "a', SUCCEED, 'g10", "a"));
  CPPUNIT_ASSERT (TestMatch ("((((((((((a))))))))))\\10", "aa", "aa"));
# Python does not have the same rules for \\41 so this is a syntax error
#  CPPUNIT_ASSERT (TestMatch ("((((((((((a))))))))))\\41", "aa", "", TRUE));
#  CPPUNIT_ASSERT (TestMatch ("((((((((((a))))))))))\\41", "a!", "a!"));
  CPPUNIT_ASSERT (TestMatch ("((((((((((a))))))))))\\41", "', SYNTAX_ERROR),
  CPPUNIT_ASSERT (TestMatch ("(?i)((((((((((a))))))))))\\41", "', SYNTAX_ERROR),  // Python Extended Notation
  CPPUNIT_ASSERT (TestMatch ("(((((((((a)))))))))", "a", "a"));
  
  CPPUNIT_ASSERT (TestMatch ("(.*)c(.*)", "abcde+"-"+g1+"-"+g2", "abcde-ab-de"));
  CPPUNIT_ASSERT (TestMatch ("\\((.*), (.*)\\)", "(a, b)', SUCCEED, 'g2+"-"+g1", "b-a"));
  CPPUNIT_ASSERT (TestMatch ("[k]", "ab", "", TRUE));
  CPPUNIT_ASSERT (TestMatch ("a[-]?c", "ac", "ac"));
  CPPUNIT_ASSERT (TestMatch ("(abc)\\1", "abcabc', SUCCEED, 'g1", "abc"));
  CPPUNIT_ASSERT (TestMatch ("([a-c]*)\\1", "abcabc', SUCCEED, 'g1", "abc"));  
  */
  };

//-----------------------------------------------------------------------------
BOOL RegExTest::TestMatch (const char *  szPattern,
                           const char *  szString,
                           const char *  szResult,
                           BOOL          bShouldFail)
  {                          
  const char * pszMatchStart;
  INT32        iMatchSize;
  RegEx        rexPattern;
  
  rexPattern.Set (szPattern);
  //rexPattern.Debug ();
  rexPattern.Match (szString, 
                    strlen(szString), 
                    0,
                    &pszMatchStart,
                    iMatchSize);
  if (bShouldFail)
    {
    return (iMatchSize == 0);
    }
  else
    {
    //printf ("RegEx Match: %d %s\n\n", (int)iMatchSize, pszMatchStart);
    if ((int)strlen (szResult) != iMatchSize) {return (FALSE);};
    return (strncmp (pszMatchStart, szResult, iMatchSize) == 0);
    }
  };