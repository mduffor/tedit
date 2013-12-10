
#ifndef REGEXTEST_HPP
#define REGEXTEST_HPP

#include <cppunit/extensions/HelperMacros.h>

class RegExTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( RegExTest );
  CPPUNIT_TEST( testRegEx );
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp();
  void tearDown();

  void testRegEx();
  
  BOOL TestMatch (const char *  szPattern,
                  const char *  szString,
                  const char *  szResult,
                  BOOL          bShouldFail = FALSE);
};


#endif // REGEXTEST_HPP