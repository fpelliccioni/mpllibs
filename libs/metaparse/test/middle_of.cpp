// Copyright Abel Sinkovics (abel@sinkovics.hu) 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_DYN_LINK

#include <mpllibs/metaparse/middle_of.hpp>
#include <mpllibs/metaparse/is_error.hpp>
#include <mpllibs/metaparse/source_position.hpp>
#include <mpllibs/metaparse/get_result.hpp>

#include "common.hpp"

#include <mpllibs/metatest/test.hpp>
#include <mpllibs/metatest/has_type.hpp>

#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/apply_wrap.hpp>

#include <mpllibs/metatest/boost_test.hpp>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_middle_of)
{
  using mpllibs::metatest::meta_require;
  using mpllibs::metatest::has_type;
  
  using mpllibs::metaparse::get_result;
  using mpllibs::metaparse::middle_of;
  using mpllibs::metaparse::start;
  using mpllibs::metaparse::is_error;
  
  using boost::mpl::equal_to;
  using boost::mpl::apply_wrap2;

  meta_require<
    has_type<middle_of<lit_h, lit_e, lit_l> >
  >(MPLLIBS_HERE, "test_has_type");

  meta_require<
    equal_to<
      get_result<
        apply_wrap2<middle_of<lit_h, lit_e, lit_l>, str_hello, start>
      >::type,
      char_e
    >
  >(MPLLIBS_HERE, "test_three_chars");

  meta_require<
    is_error<apply_wrap2<middle_of<lit_x, lit_e, lit_l>, str_hello, start> >
  >(MPLLIBS_HERE, "test_first_fails");

  meta_require<
    is_error<apply_wrap2<middle_of<lit_h, lit_x, lit_l>, str_hello, start> >
  >(MPLLIBS_HERE, "test_second_fails");

  meta_require<
    is_error<apply_wrap2<middle_of<lit_h, lit_e, lit_x>, str_hello, start> >
  >(MPLLIBS_HERE, "test_third_fails");

  meta_require<
    is_error<apply_wrap2<middle_of<lit_h, lit_e, lit_l>, str_, start> >
  >(MPLLIBS_HERE, "test_empty_input");
}


