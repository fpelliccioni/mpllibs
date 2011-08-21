#ifndef MPLLIBS_METAPARSE_KEYWORD_HPP
#define MPLLIBS_METAPARSE_KEYWORD_HPP

// Copyright Abel Sinkovics (abel@sinkovics.hu)  2009 - 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <mpllibs/metaparse/lit.hpp>
#include <mpllibs/metaparse/return.hpp>
#include <mpllibs/metaparse/is_error.hpp>
#include <mpllibs/metaparse/get_remaining.hpp>
#include <mpllibs/metaparse/get_position.hpp>

#include <mpllibs/metamonad/meta_atom.hpp>
#include <mpllibs/metamonad/tag_tag.hpp>

#include <mpllibs/metatest/to_stream_argument_list.hpp>

#include <boost/mpl/if.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/empty.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/apply_wrap.hpp>

namespace mpllibs
{
  namespace metaparse
  {
    MPLLIBS_DEFINE_TAG(accepted_keyword_tag)
    
    MPLLIBS_DEFINE_META_ATOM(accepted_keyword_tag, accepted_keyword)

    // Does not consume/check anything after the keyword
    template <class Kw, class ResultType = accepted_keyword>
    struct keyword
    {
    private:
      struct nonempty
      {
      private:
        typedef lit<typename boost::mpl::front<Kw>::type> next_char_parser;

        typedef
          keyword<typename boost::mpl::pop_front<Kw>::type, ResultType>
          rest_parser;
        
        template <class S, class Pos>
        struct apply_unchecked :
          boost::mpl::apply_wrap2<
            rest_parser,
            typename get_remaining<
              boost::mpl::apply_wrap2<next_char_parser, S, Pos>
            >::type,
            typename get_position<
              boost::mpl::apply_wrap2<next_char_parser, S, Pos>
            >::type
          >
        {};
      public:
        template <class S, class Pos>
        struct apply :
          boost::mpl::eval_if<
            typename is_error<
              boost::mpl::apply_wrap2<next_char_parser, S, Pos>
            >::type,
            boost::mpl::apply_wrap2<next_char_parser, S, Pos>,
            apply_unchecked<S, Pos>
          >
        {};
      };
    public:
      typedef keyword type;
      
      template <class S, class Pos>
      struct apply :
        boost::mpl::apply_wrap2<
          typename boost::mpl::if_<
            boost::mpl::empty<Kw>,
            return_<ResultType>,
            nonempty
          >::type,
          S,
          Pos
        >
      {};

      struct to_stream
      {
        static std::ostream& run(std::ostream& o)
        {
          o << "keyword<";
          mpllibs::metatest::to_stream_argument_list<Kw, ResultType>::run(o);
          return o << ">";
        }
      };
    };
  }
}

#endif

