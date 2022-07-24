//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_FRAGMENT_RULE_IPP
#define BOOST_URL_IMPL_FRAGMENT_RULE_IPP

#include <boost/url/rfc/fragment_rule.hpp>
#include <boost/url/rfc/charsets.hpp>
#include <boost/url/rfc/pct_encoded_rule.hpp>
#include <boost/url/grammar/parse.hpp>

namespace boost {
namespace urls {

auto
fragment_part_rule_t::
parse(
    char const*& it,
    char const* const end) const noexcept ->
        result<value_type>
{
    value_type t;
    if(it == end)
    {
        t.has_fragment = false;
        return t;
    }
    if(*it != '#')
    {
        t.has_fragment = false;
        return t;
    }
    ++it;
    auto rv = grammar::parse_(
        it, end, fragment_rule);
    if(! rv)
        return rv.error();
    t.fragment = rv.value();
    t.has_fragment = true;
    return t;
}

} // urls
} // boost

#endif
