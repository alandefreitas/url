//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_GRAMMAR_CHAR_RULE_HPP
#define BOOST_URL_GRAMMAR_CHAR_RULE_HPP

#include <boost/url/detail/config.hpp>
#include <boost/url/string_view.hpp>
#include <boost/url/grammar/error.hpp>
#include <boost/url/grammar/parse_tag.hpp>

namespace boost {
namespace urls {
namespace grammar {

/** Rule to match a single character literal.
*/
struct char_rule
{
    using value_type = string_view;

    /** Constructor

        @param c The character to match
    */
    constexpr
    char_rule(
        char c) noexcept
        : c_(c)
    {
    }

#ifndef BOOST_URL_DOCS
    friend
    void
    tag_invoke(
        grammar::parse_tag const&,
        char const*& it,
        char const* end,
        error_code& ec,
        char_rule const& r,
        value_type& t) noexcept
    {
        if( it != end &&
            *it == r.c_)
        {
            t = { it, 1 };
            ++it;
            ec = {};
            return;
        }
        ec = error::syntax;
    }
#endif

private:
    char c_;
};

} // grammar
} // urls
} // boost

#endif
