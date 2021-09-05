//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_RFC_IMPL_RELATIVE_PART_IPP
#define BOOST_URL_RFC_IMPL_RELATIVE_PART_IPP

#include <boost/url/rfc/relative_part.hpp>
#include <boost/url/bnf/parse.hpp>
#include <boost/url/rfc/detail/paths.hpp>

namespace boost {
namespace urls {
namespace rfc {

bool
parse(
    char const*& it,
    char const* const end,
    error_code& ec,
    relative_part& t)
{
    using bnf::parse;
    if(it == end)
    {
        // path-empty
        t.authority.reset();
        parse(it, end, ec,
            detail::path_empty{t.path});
        ec = {};
        return true;
    }
    if(it[0] != '/')
    {
        // path-noscheme
        if(! parse(it, end, ec,
            detail::path_noscheme{
                t.path}))
            return false;
        t.authority.reset();
        return true;
    }
    if( end - it == 1 ||
        it[1] != '/')
    {
        // path-absolute
        parse(it, end, ec,
            detail::path_absolute{
                t.path});
        t.authority.reset();
        ec = {};
        return true;
    }
    // "//" authority path-abempty
    it += 2;
    // authority
    t.authority.emplace();
    if(! parse(it, end, ec,
            *t.authority))
        return false;
    // path-abempty
    if(! parse(it, end, ec,
        detail::path_abempty{
            t.path}))
        return false;
    return true;
}

} // rfc
} // urls
} // boost

#endif
