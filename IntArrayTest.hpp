
#ifndef INTARRAYTEST_HPP
#define INTARRAYTEST_HPP

#include <cppunit/extensions/HelperMacros.h>

class IntArrayTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( IntArrayTest );
  CPPUNIT_TEST( testIntArray );
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp();
  void tearDown();

  void testIntArray();
};


#endif // INTARRAYTEST_HPP