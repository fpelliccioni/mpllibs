// Copyright Abel Sinkovics (abel@sinkovics.hu) 2011.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_DYN_LINK

#include <mpllibs/metamonad/either.hpp>
#include <mpllibs/metamonad/return_.hpp>
#include <mpllibs/metamonad/bind.hpp>
#include <mpllibs/metamonad/fail.hpp>

#include <mpllibs/metatest/boost_test.hpp>
#include <boost/test/unit_test.hpp>

#include <boost/mpl/not.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/plus.hpp>

using mpllibs::metamonad::left;
using mpllibs::metamonad::right;

using boost::mpl::plus;
using boost::mpl::_1;
using boost::mpl::int_;

namespace
{
  typedef int_<2> int2;
  typedef int_<11> int11;
  typedef int_<13> int13;
  
  typedef right<int13> right13;
  typedef right<int11> right11;
  typedef left<int11> left11;
  
  typedef right<plus<_1, int2> > either_add_2;
  
}

BOOST_AUTO_TEST_CASE(test_either)
{
  using mpllibs::metatest::meta_require;

  using mpllibs::metamonad::either_tag;
  using mpllibs::metamonad::return_;
  using mpllibs::metamonad::bind;
  using mpllibs::metamonad::fail;

  using boost::mpl::equal_to;

  typedef either_tag<> et;

  typedef int_<13>::tag int_tag;

  meta_require<
    equal_to<return_<et, int13>, right13>
  >(MPLLIBS_HERE, "test_return");

  meta_require<
    equal_to<bind<et, right11, either_add_2>::type, right13>
  >(MPLLIBS_HERE, "test_bind_with_right");

  meta_require<
    equal_to<bind<et, left11, either_add_2>::type, left11>
  >(MPLLIBS_HERE, "test_bind_with_left");

  meta_require<
    equal_to<bind<et, fail<et, int13>, either_add_2>::type, left<int13> >
  >(MPLLIBS_HERE, "test_fail");
}


