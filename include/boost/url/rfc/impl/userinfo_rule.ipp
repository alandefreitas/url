//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_USERINFO_RULE_IPP
#define BOOST_URL_IMPL_USERINFO_RULE_IPP

#include <boost/url/rfc/userinfo_rule.hpp>
#include <boost/url/string_view.hpp>
#include <boost/url/rfc/charsets.hpp>
#include <boost/url/rfc/pct_encoded_rule.hpp>
#include <boost/url/grammar/lut_chars.hpp>
#include <boost/url/grammar/parse.hpp>

namespace boost {
namespace urls {

void
userinfo_rule::
parse(
    char const*& it,
    char const* const end,
    error_code& ec,
    userinfo_rule& t) noexcept
{
    static constexpr auto uchars =
        unreserved_chars +
        subdelim_chars;
    static constexpr auto pwchars =
        uchars + ':';

    auto rv = grammar::parse_(
        it, end, pct_encoded_rule(uchars));
    if(! rv)
    {
        ec = rv.error();
        return;
    }
    t.user = rv.value();

    if( it == end ||
        *it != ':')
    {
        t.has_password = false;
        t.password = pct_encoded_view("");
        return;
    }
    ++it;
    rv = grammar::parse_(
        it, end,
        pct_encoded_rule(pwchars));
    if(! rv)
    {
        ec = rv.error();
        return;
    }
    t.has_password = true;
    t.password = rv.value();
}

} // urls
} // boost

#endif
