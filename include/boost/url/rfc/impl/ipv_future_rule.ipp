//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_IPV_FUTURE_RULE_IPP
#define BOOST_URL_IMPL_IPV_FUTURE_RULE_IPP

#include <boost/url/rfc/ipv_future_rule.hpp>
#include <boost/url/rfc/charsets.hpp>
#include <boost/url/grammar/charset.hpp>
#include <boost/url/grammar/char_rule.hpp>
#include <boost/url/grammar/parse.hpp>
#include <boost/url/grammar/token_rule.hpp>
#include <boost/url/grammar/sequence_rule.hpp>

namespace boost {
namespace urls {

void
ipv_future_rule::
parse(
    char const*& it,
    char const* const end,
    error_code& ec,
    ipv_future_rule& t) noexcept
{
    static constexpr auto
        minor_chars = 
            unreserved_chars +
            subdelim_chars + ':';
    auto const it0 = it;
    auto rv = grammar::parse_(
        it, end,
        grammar::sequence_rule(
            grammar::char_rule('v'),
            grammar::token_rule(
                grammar::hexdig_chars),
            grammar::char_rule('.'),
            grammar::token_rule(minor_chars)));
    if(! rv)
    {
        ec = rv.error();
        return;
    }
    t.major = std::get<1>(rv.value());
    t.minor = std::get<3>(rv.value());
    if(t.major.empty())
    {
        // can't be empty
        ec = error::bad_empty_element;
        return;
    }
    if(t.minor.empty())
    {
        // can't be empty
        ec = error::bad_empty_element;
        return;
    }
    t.str = string_view(
        it0, it - it0);
}

} // urls
} // boost

#endif
