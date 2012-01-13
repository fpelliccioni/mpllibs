// Copyright Abel Sinkovics (abel@sinkovics.hu)  2012.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <mpllibs/metaparse/any.hpp>
#include <mpllibs/metaparse/sequence.hpp>
#include <mpllibs/metaparse/lit_c.hpp>
#include <mpllibs/metaparse/last_of.hpp>
#include <mpllibs/metaparse/space.hpp>
#include <mpllibs/metaparse/int.hpp>
#include <mpllibs/metaparse/foldr.hpp>
#include <mpllibs/metaparse/one_of.hpp>
#include <mpllibs/metaparse/parser_monad.hpp>
#include <mpllibs/metaparse/get_result.hpp>
#include <mpllibs/metaparse/token.hpp>
#include <mpllibs/metaparse/entire_input.hpp>
#include <mpllibs/metaparse/string.hpp>
#include <mpllibs/metaparse/transform.hpp>
#include <mpllibs/metaparse/always.hpp>

#include <mpllibs/metaparse/build_parser.hpp>

#include <mpllibs/metamonad/do.hpp>
#include <mpllibs/metamonad/do_try.hpp>
#include <mpllibs/metamonad/tag_tag.hpp>
#include <mpllibs/metamonad/meta_atom.hpp>

#include <mpllibs/metatest/to_stream_argument_list.hpp>

#include <boost/mpl/apply_wrap.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/back.hpp>
#include <boost/mpl/plus.hpp>
#include <boost/mpl/minus.hpp>
#include <boost/mpl/times.hpp>
#include <boost/mpl/divides.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/if.hpp>

using mpllibs::metaparse::sequence;
using mpllibs::metaparse::lit_c;
using mpllibs::metaparse::last_of;
using mpllibs::metaparse::space;
using mpllibs::metaparse::any;
using mpllibs::metaparse::build_parser;
using mpllibs::metaparse::int_;
using mpllibs::metaparse::foldr;
using mpllibs::metaparse::parser_tag;
using mpllibs::metaparse::get_result;
using mpllibs::metaparse::one_of;
using mpllibs::metaparse::token;
using mpllibs::metaparse::entire_input;
using mpllibs::metaparse::transform;
using mpllibs::metaparse::always;

using mpllibs::metamonad::do_try;
using mpllibs::metamonad::throw_;
using mpllibs::metamonad::do_;
using mpllibs::metamonad::set;

using boost::mpl::apply_wrap1;
using boost::mpl::front;
using boost::mpl::back;
using boost::mpl::plus;
using boost::mpl::minus;
using boost::mpl::times;
using boost::mpl::divides;
using boost::mpl::if_;
using boost::mpl::bool_;
using boost::mpl::equal_to;

/*
 * The grammar
 *
 * expression ::= plus_exp
 * plus_exp ::= prod_exp ((plus_token | minus_token) prod_exp)*
 * prod_exp ::= value_exp ((mult_token | div_token) value_exp)*
 * value_exp ::= int_token | '_'
 */

typedef token<lit_c<'+'> > plus_token;
typedef token<lit_c<'-'> > minus_token;
typedef token<lit_c<'*'> > mult_token;
typedef token<lit_c<'/'> > div_token;
 
typedef token<int_> int_token;
typedef token<lit_c<'_'> > arg_token;

struct x;

MPLLIBS_DEFINE_TAG(division_by_zero_tag)
MPLLIBS_DEFINE_META_ATOM(division_by_zero_tag, division_by_zero)

template <class T, char C>
struct is_c : bool_<T::type::value == C>
{
  struct to_stream
  {
    static std::ostream& run(std::ostream& o)
    {
      o << "is_c";
      mpllibs::metatest::to_stream_argument_list<T>::run(o);
      return o << ", " << C << ">";
    }
  };
};

struct build_plus
{
  template <class A, class B>
  struct _plus
  {
    typedef _plus type;

    template <class T>
    struct apply :
      plus<typename apply_wrap1<A, T>::type, typename apply_wrap1<B, T>::type>
    {};
  };

  template <class A, class B>
  struct _minus
  {
    typedef _minus type;

    template <class T>
    struct apply :
      minus<typename apply_wrap1<A, T>::type, typename apply_wrap1<B, T>::type>
    {};
  };

  template <class C, class State>
  struct apply :
    if_<
      typename is_c<front<C>, '+'>::type,
      _plus<State, typename back<C>::type>,
      _minus<State, typename back<C>::type>
    >
  {};

  struct to_stream
  {
    static std::ostream& run(std::ostream& o)
    {
      return o << "build_plus";
    }
  };
};

struct build_mult
{
  template <class A, class B>
  struct _mult
  {
    typedef _mult type;

    template <class T>
    struct apply :
      times<typename apply_wrap1<A, T>::type, typename apply_wrap1<B, T>::type>
    {};
  };

  template <class A, class B>
  struct _div
  {
    typedef _div type;

    template <class T>
    struct apply :
      divides<
        typename apply_wrap1<A, T>::type,
        typename apply_wrap1<B, T>::type
      >
    {};
  };

  template <class C, class State>
  struct apply :
    if_<
      typename is_c<front<C>, '*'>::type,
      _mult<State, typename back<C>::type>,
      _div<State, typename back<C>::type>
    >
  {};

  struct to_stream
  {
    static std::ostream& run(std::ostream& o)
    {
      return o << "build_mult";
    }
  };
};

class build_value
{
private:
  template <class V>
  struct impl
  {
    typedef impl type;

    template <class T>
    struct apply : V {};
  };

public:
  typedef build_value type;

  template <class V>
  struct apply : impl<typename V::type> {};

  struct to_stream
  {
    static std::ostream& run(std::ostream& o)
    {
      return o << "build_value";
    }
  };
};

struct arg
{
  typedef arg type;

  template <class T>
  struct apply
  {
    typedef T type;
  };

  struct to_stream
  {
    static std::ostream& run(std::ostream& o)
    {
      return o << "arg";
    }
  };
};

typedef
  one_of<transform<int_token, build_value>, always<arg_token, arg>>
  value_exp;

typedef
  do_<parser_tag>::apply<
    set<x, value_exp>,
    foldr<
      sequence<one_of<mult_token, div_token>, value_exp>,
      get_result<x>,
      build_mult
    >
  >::type
  prod_exp;
  
typedef
  do_<parser_tag>::apply<
    set<x, prod_exp>,
    foldr<
      sequence<one_of<plus_token, minus_token>, prod_exp>,
      get_result<x>,
      build_plus
    >
  >::type
  plus_exp;

typedef last_of<any<space>, plus_exp> expression;

typedef build_parser<entire_input<expression> > metafunction_parser;

#ifdef META_LAMBDA
  #error META_LAMBDA already defined
#endif
#define META_LAMBDA(exp) \
  apply_wrap1<metafunction_parser, MPLLIBS_STRING(#exp)>::type

int main()
{
  using std::cout;
  using std::endl;

  typedef META_LAMBDA(13) metafunction_class_1;
  typedef META_LAMBDA(2 + 3) metafunction_class_2;
  typedef META_LAMBDA(2 * 2) metafunction_class_3;
  typedef META_LAMBDA( 1+ 2*4-6/2) metafunction_class_4;
  typedef META_LAMBDA(2 * _) metafunction_class_5;

  typedef boost::mpl::int_<11> int11;

  cout
    << apply_wrap1<metafunction_class_1, int11>::type::value << endl
    << apply_wrap1<metafunction_class_2, int11>::type::value << endl
    << apply_wrap1<metafunction_class_3, int11>::type::value << endl
    << apply_wrap1<metafunction_class_4, int11>::type::value << endl
    << apply_wrap1<metafunction_class_5, int11>::type::value << endl
    ;
}

