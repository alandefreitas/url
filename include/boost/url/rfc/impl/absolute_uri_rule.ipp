//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_ABSOLUTE_URI_RULE_IPP
#define BOOST_URL_IMPL_ABSOLUTE_URI_RULE_IPP

#include <boost/url/rfc/absolute_uri_rule.hpp>
#include <boost/url/grammar/parse.hpp>

namespace boost {
namespace urls {

void
absolute_uri_rule::
parse(
    char const*& it,
    char const* const end,
    error_code& ec,
    absolute_uri_rule& t) noexcept
{
    // scheme
    {
        auto rv = grammar::parse_(
            it, end, scheme_part_rule());
        if(! rv)
        {
            ec = rv.error();
            return;
        }
        t.scheme_part = *rv;
    }

    grammar::parse(
        it, end, ec,
        t.hier_part);
    if(ec.failed())
        return;

    // [ "?" query ]
    {
        auto rv = grammar::parse_(
            it, end, query_part_rule);
        if(! rv)
        {
            ec = rv.error();
            return;
        }
        t.query_part = rv.value();
    }
}

} // urls
} // boost

#endif
