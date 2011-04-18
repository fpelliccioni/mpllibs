// Copyright Abel Sinkovics (abel@sinkovics.hu) 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <mpllibs/error/return_.h>

#include <mpllibs/metatest/test.h>
#include <mpllibs/metatest/TestSuite.h>

#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/identity.hpp>

#include "common.h"

using boost::mpl::identity;
using boost::mpl::equal_to;
using boost::mpl::apply;

using mpllibs::metatest::TestSuite;

using mpllibs::error::return_;

namespace
{
  struct test_tag
  {
    typedef test_tag type;
  };
}

namespace mpllibs
{
  namespace error
  {
    template <>
    struct return__impl<test_tag>
    {
      template <class t>
      struct apply : identity<int13> {};
    };
  }
}

namespace
{
  const TestSuite suite("return_");

  typedef
    equal_to<int13, apply<return_<test_tag>, int>::type>
    test_specialisation_is_called;  

  typedef
    equal_to<int13, return_<test_tag, int>::type>
    test_using_two_arguments;
}

MPLLIBS_ADD_TEST(suite, test_specialisation_is_called)
MPLLIBS_ADD_TEST(suite, test_using_two_arguments)


