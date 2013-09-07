

#include "Types.hpp"
#include "Debug.hpp"
ASSERTFILE (__FILE__)

#include "IntArray.hpp"
#include "IntArrayTest.hpp"
#include <string.h>

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION (IntArrayTest);


//-----------------------------------------------------------------------------
void IntArrayTest::setUp ()
{
}


//-----------------------------------------------------------------------------
void IntArrayTest::tearDown ()
{
}

//-----------------------------------------------------------------------------
void IntArrayTest::testIntArray ()
{
  // Test AllocArray and default size constructor
  IntArray *  pArray = new IntArray (10);
  
  CPPUNIT_ASSERT (pArray->Length () == 10);
  CPPUNIT_ASSERT (pArray->AllocSize () == 10);
  
  // Test assignment and equality
  (*pArray)[0] = 1;
  CPPUNIT_ASSERT ((*pArray)[0] == 1);
  CPPUNIT_ASSERT ((*pArray)[0] != 2);
  
  // Test insertion and deletion
  for (INT iIndex = 0; iIndex < pArray->Length (); ++iIndex)
    {
    (*pArray)[iIndex] = iIndex;
    }
  pArray->Remove (2, 1);
  CPPUNIT_ASSERT ((*pArray)[2] == 3);
  CPPUNIT_ASSERT (pArray->Length () == 9);
  
  pArray->Remove (2, 2);
  CPPUNIT_ASSERT ((*pArray)[2] == 5);
  CPPUNIT_ASSERT (pArray->Length () == 7);

  pArray->Insert (2, 1);
  CPPUNIT_ASSERT ((*pArray)[3] == 5);
  CPPUNIT_ASSERT (pArray->Length () == 8);

  pArray->Insert (2, 3);
  CPPUNIT_ASSERT ((*pArray)[6] == 5);
  CPPUNIT_ASSERT (pArray->Length () == 11);
  
  //CPPUNIT_FAIL( "not implemented" );
  
  delete pArray;
  pArray = NULL;
}

