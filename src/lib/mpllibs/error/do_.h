#ifndef MPLLIBS_ERROR_DO__H
#define MPLLIBS_ERROR_DO__H

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <mpllibs/error/lambda.h>
#include <mpllibs/error/bind_.h>
#include <mpllibs/error/bind.h>
#include <mpllibs/error/let.h>
#include <mpllibs/error/return_.h>

#include <boost/mpl/apply.hpp>
#include <boost/preprocessor/repetition/enum_params_with_a_default.hpp>
#include <boost/preprocessor/arithmetic/dec.hpp>

namespace mpllibs
{
  namespace error
  {
    #ifndef DO_MAX_ARGUMENT
      #define DO_MAX_ARGUMENT 8
    #endif
    
    #if DO_MAX_ARGUMENT + 2 > LET_MAX_TEMPLATE_ARGUMENT
      #error LET_MAX_TEMPLATE_ARGUMENT not big enough
    #endif
    
    /*
     * Syntactic sugar
     */
    #ifdef DO
      #error DO already defined
    #endif
    #define DO mpllibs::error::do_
    
    #ifdef SET
      #error SET already defined
    #endif
    #define SET mpllibs::error::set
  
    #ifdef RETURN
      #error RETURN already defined
    #endif
    #define RETURN mpllibs::error::do_return

    struct unused_do_argument;

    template <
      class Monad,
      BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(
        DO_MAX_ARGUMENT,
        class E,
        mpllibs::error::unused_do_argument
      )
    >
    struct do_impl;
    
    /*
     * do_return
     */
    template <class>
    struct do_return;

    /*
     * do_return substitution
     */
    // I can't use let, because I should not stop at let expressions.
    // Nested do_'s are not important, since they should be used by
    // accessing a nested apply template, and do_substitute doesn't
    // recurse into that.
    // Using boost::mpl::apply and do_ together is not supported
    template <class Monad, class T>
    struct do_substitute : mpllibs::error::util::id<T> {};

    template <class Monad, class T>
    struct do_substitute<Monad, do_return<T> > :
      mpllibs::error::util::id<
        mpllibs::error::return_<
          Monad,
          typename mpllibs::error::do_substitute<Monad, T>::type
        >
      >
    {};
    
    // When an outer do_ has already substituted this do_return,
    // we need to re-substitute it. I couldn't find a way of preventing
    // substitution of inner do_'s do_returns.
    template <class Monad1, class Monad2, class T>
    struct do_substitute<Monad1, mpllibs::error::return_<Monad2, T> > :
      mpllibs::error::util::id<
        mpllibs::error::return_<
          Monad1,
          typename mpllibs::error::do_substitute<Monad1, T>::type
        >
      >
    {};

    #ifdef DO_CLASS
      #error DO_CLASS alread defined
    #endif
    #define DO_CLASS(z, n, unused) \
      BOOST_PP_COMMA_IF(n) class
    
    #ifdef DO_REC_CASE
      #error DO_REC_CASE alread defined
    #endif
    #define DO_REC_CASE(z, n, unused) \
      BOOST_PP_COMMA_IF(n) \
      typename mpllibs::error::do_substitute<Monad, X##n>::type

    #ifdef DO_TEMPLATE_CASE
      #error DO_TEMPLATE_CASE already defined
    #endif
    #define DO_TEMPLATE_CASE(z, n, unused) \
      template < \
        class Monad, \
        template<BOOST_PP_REPEAT(n, DO_CLASS, ~) > class T, \
        BOOST_PP_ENUM_PARAMS(n, class X) \
      > \
      struct do_substitute<Monad, T<BOOST_PP_ENUM_PARAMS(n, X)> > : \
        mpllibs::error::util::id< T< BOOST_PP_REPEAT(n, DO_REC_CASE, ~) > > \
      {};
    
    BOOST_PP_REPEAT_FROM_TO(1, LET_MAX_TEMPLATE_ARGUMENT, DO_TEMPLATE_CASE, ~)

    #undef DO_TEMPLATE_CASE
    #undef DO_CLASS
    #undef DO_REC_CASE

    /*
     * set
     */
    template <class Name, class F>
    struct set;

    /*
     * do1
     */
    // This case handles nullary metafunction elements of the do
    template <class Monad, class E>
    struct do1 : E {};

    template <class Monad, class Name, class F>
    struct do1<Monad, mpllibs::error::set<Name, F> >;
      // Error: last statement in a 'do' construct must be an expression.
      // Current way of error handling is not having an implementation.
      // It may be improved.
    
    /*
     * don
     */
     
    #ifdef DO_CLASS_CASE
      #error DO_CLASS_CASE already defined
    #endif
    #define DO_CLASS_CASE(z, n, unused) \
      , class E##n
    
    #ifdef DO_CLASS_USE_CASE
      #error DO_CLASS_USE_CASE already defined
    #endif
    #define DO_CLASS_USE_CASE(z, n, unused) \
      BOOST_PP_COMMA_IF(BOOST_PP_DEC(n)) E##n

    #ifdef DO_CASE
      #error DO_CASE already defined
    #endif
    // I need at least one template argument. The "n"th case handles n+1 args.
    #define DO_CASE(z, n, unused) \
      template < \
        class Monad, \
        class T \
        BOOST_PP_REPEAT_FROM_TO(1, n, DO_CLASS_CASE, ~) \
      > \
      struct do##n : \
        boost::mpl::apply< \
          mpllibs::error::bind__impl<Monad>, \
          typename T::type, \
          typename mpllibs::error::do_impl< \
            Monad, \
            BOOST_PP_REPEAT_FROM_TO(1, n, DO_CLASS_USE_CASE, ~) \
          >::type \
        > \
      {}; \
      \
      template < \
        class Monad, \
        class Name, \
        class Ex \
        BOOST_PP_REPEAT_FROM_TO(1, n, DO_CLASS_CASE, ~) \
      > \
      struct do##n< \
        Monad, \
        mpllibs::error::set<Name, Ex> \
        BOOST_PP_COMMA_IF(BOOST_PP_DEC(n)) \
        BOOST_PP_REPEAT_FROM_TO(1, n, DO_CLASS_USE_CASE, ~) \
      > : \
        boost::mpl::apply< \
          mpllibs::error::bind_impl<Monad>, \
          typename mpllibs::error::do1<Monad, Ex>::type, \
          typename mpllibs::error::lambda< \
            Name, \
            mpllibs::error::do_impl< \
              Monad, \
              BOOST_PP_REPEAT_FROM_TO(1, n, DO_CLASS_USE_CASE, ~) \
            > \
          >::type \
        > \
      {};
    
    BOOST_PP_REPEAT_FROM_TO(2, DO_MAX_ARGUMENT, DO_CASE, ~)
    
    #undef DO_CASE
    #undef DO_CLASS_USE_CASE
    #undef DO_CLASS_CASE
        
    /*
     * do_impl
     */
    #ifdef DO_UNUSED_PARAM
      #error DO_UNUSED_PARAM already defined
    #endif
    #define DO_UNUSED_PARAM(z, n, unused) \
      BOOST_PP_COMMA_IF(n) mpllibs::error::unused_do_argument
    
    #ifdef DO_CASE
      #error DO_CASE already defined
    #endif
    #define DO_CASE(z, n, unused) \
      template <class Monad, BOOST_PP_ENUM_PARAMS(n, class E)> \
      struct do_impl< \
        Monad, \
        BOOST_PP_ENUM_PARAMS(n, E) BOOST_PP_COMMA_IF(n) \
        BOOST_PP_REPEAT( \
          BOOST_PP_SUB(DO_MAX_ARGUMENT, n), \
          DO_UNUSED_PARAM, \
          ~ \
        ) \
      > : \
        mpllibs::error::do##n<Monad, BOOST_PP_ENUM_PARAMS(n, E)> \
      {};
  
    BOOST_PP_REPEAT_FROM_TO(1, DO_MAX_ARGUMENT, DO_CASE, ~)
    
    #undef DO_CASE
    #undef DO_UNUSED_PARAM
    
    /*
     * do_
     */
    #ifdef DO_ARG
      #error DO_ARG already defined
    #endif
    #define DO_ARG(z, n, unused) , typename do_substitute<Monad, E##n>::type
    
    template <class Monad>
    struct do_
    {
      template <
        BOOST_PP_ENUM_PARAMS_WITH_A_DEFAULT(
          DO_MAX_ARGUMENT,
          class E,
          mpllibs::error::unused_do_argument
        )
      >
      struct apply :
        mpllibs::error::do_impl<
          Monad
          BOOST_PP_REPEAT(DO_MAX_ARGUMENT, DO_ARG, ~)
        >
      {};
    };
    
    #undef DO_ARG
  }
}

#endif

