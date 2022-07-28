//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_RELATIVE_REF_RULE_IPP
#define BOOST_URL_IMPL_RELATIVE_REF_RULE_IPP

#include <boost/url/rfc/relative_ref_rule.hpp>
#include <boost/url/rfc/query_rule.hpp>
#include <boost/url/rfc/relative_part_rule.hpp>
#include <boost/url/rfc/detail/fragment_rule.hpp>
#include <boost/url/grammar/char_rule.hpp>
#include <boost/url/grammar/sequence_rule.hpp>
#include <boost/url/grammar/optional_rule.hpp>
#include <boost/url/grammar/parse.hpp>

namespace boost {
namespace urls {

auto
relative_ref_rule_t::
parse(
    char const*& it,
    char const* const end
        ) const noexcept ->
    result<value_type>
{
    detail::url_impl u;
    u.cs_ = it;

    // relative-part
    auto r0 = grammar::parse(
        it, end, relative_part_rule);
    if(! r0)
        return r0.error();

    // [ "?" query ]
    auto r1 = grammar::parse(
        it, end,
        grammar::optional_rule(
            grammar::sequence_rule(
                grammar::char_rule('?'),
                query_rule)));
    if(! r1)
        return r1.error();

    // [ "#" fragment ]
    auto r2 = grammar::parse(
        it, end, detail::fragment_part_rule);
    if(! r2)
        return r2.error();

    if(r0->has_authority)
        u.apply(r0->authority);
    u.apply(r0->path);
    if(r1->has_value())
    {
        auto const& v =
            std::get<1>(**r1);
        u.apply_query(
            v.encoded_string(),
            v.size());
    }
    if(r2->has_value())
        u.apply_frag(
            std::get<1>(**r2));
    return u.construct();
}

} // urls
} // boost

#endif
