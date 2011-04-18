// Copyright Abel Sinkovics (abel@sinkovics.hu) 2011.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <mpllibs/error/try_.h>
#include <mpllibs/error/throw.h>

#include <mpllibs/error/get_data.h>

#include <mpllibs/metatest/test.h>
#include <mpllibs/metatest/TestSuite.h>

#include <boost/mpl/int.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/always.hpp>
#include <boost/mpl/identity.hpp>

#include <boost/type_traits/is_same.hpp>

#include "common.h"

using boost::mpl::equal_to;
using boost::mpl::identity;
using boost::mpl::tag;

using mpllibs::metatest::TestSuite;

using mpllibs::error::exception;
using mpllibs::error::catch_any;

namespace
{
  const TestSuite suite("try_");
  
  struct tag1
  {
    typedef tag1 type;
  };
  
  struct e1
  {
    typedef tag1 tag;
    typedef e1 type;
  };
  
  struct tag2
  {
    typedef tag1 type;
  };
  
  struct e2
  {
    typedef tag2 tag;
    typedef e2 type;
  };
  
  typedef
    equal_to<
      int13,
      TRY<
        RETURN<int13>
      >
      ::catch_<tag1, x, int11>
      ::type
    >
    test_no_exception;

  typedef
    equal_to<
      int11,
      TRY<
        THROW<e1>
      >
      ::catch_<tag1, x, identity<int11> >
      ::type
    >
    test_catch;

  typedef
    equal_to<
      int13,
      TRY<
        THROW<int13>
      >
      ::catch_<tag<int13>::type, x, identity<x> >
      ::type
    >
    test_exception_value_in_catch;

  typedef
    equal_to<
      exception<int13>,
      TRY<
        THROW<int13>
      >
      ::catch_<tag2, x, identity<int11> >
      ::type
    >
    test_not_catching;

  typedef
    equal_to<
      int13,
      TRY<
        THROW<e2>
      >
      ::catch_<tag1, x, identity<int11> >
      ::catch_<tag2, x, identity<int13> >
      ::type
    >
    test_second_catch;

  typedef
    equal_to<
      int11,
      TRY<
        THROW<e1>,
        RETURN<int1>
      >
      ::catch_<tag1, x, identity<int11> >
      ::type
    >
    test_exception_propagation;


  typedef
    equal_to<
      int11,
      TRY<
        RETURN<int13>,
        RETURN<int11>
      >
      ::catch_<tag1, x, identity<int11> >
      ::type
    >
    test_execution_sequence;
  
  typedef
    equal_to<
      int11,
      TRY<
        SET<x, THROW<e1> >,
        RETURN<int1>
      >
      ::catch_<tag1, x, identity<int11> >
      ::type
    >
    test_exception_in_set;

  typedef
    equal_to<
      int13,
      TRY<
        THROW<e1>,
        RETURN<int1>
      >
      ::catch_<catch_any, x, identity<int13> >
      ::type
    >
    test_catch_any;
}

MPLLIBS_ADD_TEST(suite, test_no_exception)
MPLLIBS_ADD_TEST(suite, test_catch)
MPLLIBS_ADD_TEST(suite, test_exception_value_in_catch)
MPLLIBS_ADD_TEST(suite, test_not_catching)
MPLLIBS_ADD_TEST(suite, test_second_catch)
MPLLIBS_ADD_TEST(suite, test_exception_propagation)
MPLLIBS_ADD_TEST(suite, test_execution_sequence)
MPLLIBS_ADD_TEST(suite, test_exception_in_set)
MPLLIBS_ADD_TEST(suite, test_catch_any)


