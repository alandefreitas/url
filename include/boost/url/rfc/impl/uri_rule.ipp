//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_URI_RULE_IPP
#define BOOST_URL_IMPL_URI_RULE_IPP

#include <boost/url/rfc/uri_rule.hpp>
#include <boost/url/rfc/hier_part_rule.hpp>
#include <boost/url/rfc/query_rule.hpp>
#include <boost/url/rfc/scheme_rule.hpp>
#include <boost/url/rfc/detail/fragment_rule.hpp>
#include <boost/url/grammar/parse.hpp>

namespace boost {
namespace urls {

auto
uri_rule_t::
parse(
    char const*& it,
    char const* const end
        ) const noexcept ->
    result<value_type>
{
    detail::url_impl u;
    u.cs_ = it;

    // scheme ":"
    auto r0 = grammar::parse(
        it, end, scheme_part_rule());
    if(! r0)
        return r0.error();

    // hier_part
    auto r1 = grammar::parse(
        it, end, hier_part_rule);
    if(! r1)
        return r1.error();

    // [ "?" query ]
    auto r2 = grammar::parse(
        it, end, query_part_rule);
    if(! r2)
        return r2.error();

    // [ "#" fragment ]
    auto r3 = grammar::parse(
        it, end, detail::fragment_part_rule);
    if(! r3)
        return r3.error();

    u.apply(*r0);
    if(r1->has_authority)
        u.apply(r1->authority);
    u.apply(r1->path);
    u.apply(*r2);
    if(r3->has_value())
        u.apply_frag(
            std::get<1>(**r3));
    return u.construct();
}

} // urls
} // boost

#endif
