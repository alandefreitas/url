//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_HIER_PART_RULE_IPP
#define BOOST_URL_IMPL_HIER_PART_RULE_IPP

#include <boost/url/rfc/hier_part_rule.hpp>
#include <boost/url/grammar/parse.hpp>
#include <boost/url/rfc/paths_rule.hpp>
#include <boost/url/grammar/parse.hpp>

namespace boost {
namespace urls {

void
hier_part_rule::
parse(
    char const*& it,
    char const* const end,
    error_code& ec,
    hier_part_rule& t) noexcept
{
    if(it == end)
    {
        // path-empty
        t.path = {};
        t.has_authority = false;
        ec = {};
        return;
    }
    if(it[0] != '/')
    {
        // path-rootless
        auto rv = grammar::parse_(
            it, end,
            path_rootless_rule{});
        if(! rv)
        {
            ec = rv.error();
            return;
        }
        auto const& v = rv.value();
        t.path.path = v.string();
        t.path.count = v.size();
        t.has_authority = false;
        return;
    }
    if( end - it == 1 ||
        it[1] != '/')
    {
        // path-absolute
        auto rv = grammar::parse_(
            it, end,
            path_absolute_rule{});
        if(! rv)
        {
            ec = rv.error();
            return;
        }
        auto const& p = rv.value();
        t.path.path = p.string();
        t.path.count = p.size();
        t.has_authority = false;
        return;
    }
    // "//" authority path-abempty
    it += 2;
    // authority
    if(! grammar::parse(
        it, end, ec,
            t.authority))
        return;
    // path-abempty
    {
        auto rv = grammar::parse_(
            it, end, path_abempty_rule{});
        if(! rv)
        {
            ec = rv.error();
            return;
        }

        auto const& v = rv.value();
        t.path.path = v.string();
        t.path.count = v.size();
        t.has_authority = true;
    }
}

} // urls
} // boost

#endif
