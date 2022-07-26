//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_SCHEME_RULE_IPP
#define BOOST_URL_IMPL_SCHEME_RULE_IPP

#include <boost/url/rfc/scheme_rule.hpp>
#include <boost/url/rfc/charsets.hpp>
#include <boost/url/grammar/char_rule.hpp>
#include <boost/url/grammar/sequence_rule.hpp>
#include <boost/url/grammar/parse.hpp>

namespace boost {
namespace urls {

auto
scheme_rule::
parse(
    char const*& it,
    char const* end) const noexcept ->
        result<value_type>
{
    auto const start = it;
    if(it == end)
    {
        // expected alpha
        return grammar::error::incomplete;
    }
    if(! grammar::alpha_chars(*it))
    {
        // expected alpha
        return error::bad_alpha;
    }

    static
    constexpr
    grammar::lut_chars scheme_chars(
        "0123456789" "+-."
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz");
    it = grammar::find_if_not(
        it + 1, end, scheme_chars);
    value_type t;
    t.scheme = string_view(
        start, it - start);
    t.scheme_id = string_to_scheme(
        t.scheme);
    return t;
}

auto
scheme_part_rule::
parse(
    char const*& it,
    char const* end) const noexcept ->
        result<value_type>
{
    auto const it0 = it;
    auto rv = grammar::parse_(
        it, end,
        grammar::sequence_rule(
            scheme_rule(),
            grammar::char_rule(':')));
    if(! rv)
        return rv.error();
    value_type t;
    t.scheme = get<0>(
        rv.value()).scheme;
    t.scheme_id = get<0>(
        rv.value()).scheme_id;
    t.scheme_part = string_view(
        it0, it - it0);
    return t;
}

} // urls
} // boost

#endif
