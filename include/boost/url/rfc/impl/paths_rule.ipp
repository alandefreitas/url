//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_RFC_IMPL_PATHS_RULE_IPP
#define BOOST_URL_RFC_IMPL_PATHS_RULE_IPP

#include <boost/url/rfc/paths_rule.hpp>
#include <boost/url/rfc/charsets.hpp>
#include <boost/url/rfc/pct_encoded_rule.hpp>
#include <boost/url/grammar/parse.hpp>

namespace boost {
namespace urls {

void
segment_rule::
parse(
    char const*& it,
    char const* const end,
    error_code& ec,
    segment_rule const& t) noexcept
{
    auto rv = grammar::parse_(
        it, end,
        pct_encoded_rule(pchars));
    if(! rv)
    {
        ec = rv.error();
        return;
    }
    t.v = rv.value();
}

//------------------------------------------------

void
segment_nz_rule::
parse(
    char const*& it,
    char const* const end,
    error_code& ec,
    segment_nz_rule const& t) noexcept
{
    auto rv = grammar::parse_(
        it, end,
        pct_encoded_rule(pchars));
    if(! rv)
    {
        ec = rv.error();
        return;
    }
    t.v = rv.value();
    if(t.v.empty())
    {
        // can't be empty
        ec = grammar::error::syntax;
        return;
    }
}

//------------------------------------------------

void
segment_nz_nc_rule::
parse(
    char const*& it,
    char const* const end,
    error_code& ec,
    segment_nz_nc_rule const& t) noexcept
{
    static constexpr auto seg_chars =
        pchars - ':';
    auto rv = grammar::parse_(it, end,
        pct_encoded_rule(seg_chars));
    if(! rv)
    {
        ec = rv.error();
        return;
    }
    t.v = rv.value();
    if(t.v.empty())
    {
        // can't be empty
        ec = error::empty_path_segment;
        return;
    }
}

//------------------------------------------------

auto
path_abempty_rule::
parse(
    char const*& it,
    char const* end
        ) const noexcept ->
    result<value_type>
{
    return grammar::parse_range(
        it, end, *this,
        &path_abempty_rule::begin,
        &path_abempty_rule::increment);
}

auto
path_abempty_rule::
begin(
    char const*& it,
    char const* end
        ) const noexcept ->
    result<pct_encoded_view>
{
    return increment(it, end);
}

auto
path_abempty_rule::
increment(
    char const*& it,
    char const* end
        ) const noexcept ->
    result<pct_encoded_view>
{
    error_code ec;
    pct_encoded_view t;
    auto it0 = it;
    if(grammar::parse(
        it, end, ec,
        '/', segment_rule{t}))
    {
        return t;
    }
    it = it0;
    return BOOST_URL_ERR(
        grammar::error::end);
}

//------------------------------------------------

auto
path_absolute_rule::
parse(
    char const*& it,
    char const* end
        ) const noexcept ->
    result<value_type>
{
    return grammar::parse_range(
        it, end, *this,
        &path_absolute_rule::begin,
        &path_absolute_rule::increment);
}

auto
path_absolute_rule::
begin(
    char const*& it,
    char const* const end
        ) const noexcept ->
    result<pct_encoded_view>
{
    if(it == end)
    {
        // expected '/'
        return error::missing_path_segment;
    }
    if(*it != '/')
    {
        // expected '/'
        return error::missing_path_separator;
    }
    ++it;
    if(it == end)
        return pct_encoded_view{};
    if(*it == '/')
    {
        // can't begin with "//"
        return error::empty_path_segment;
    }
    error_code ec;
    pct_encoded_view t;
    if(! grammar::parse(
        it, end, ec,
        segment_rule{t}))
        return ec;
    return t;
}

auto
path_absolute_rule::
increment(
    char const*& it,
    char const* const end
        ) const noexcept ->
    result<pct_encoded_view>
{
    auto const start = it;
    error_code ec;
    pct_encoded_view t;
    if(grammar::parse(
        it, end, ec,
        '/', segment_rule{t}))
        return t;
    it = start;
    return grammar::error::end;
}

//------------------------------------------------

auto
path_noscheme_rule::
parse(
    char const*& it,
    char const* end
        ) const noexcept ->
    result<value_type>
{
    return grammar::parse_range(
        it, end, *this,
        &path_noscheme_rule::begin,
        &path_noscheme_rule::increment);
}

auto
path_noscheme_rule::
begin(
    char const*& it,
    char const* end
        ) const noexcept ->
    result<pct_encoded_view>
{
    error_code ec;
    pct_encoded_view t;
    if(grammar::parse(
        it, end, ec,
        segment_nz_nc_rule{t}))
        return t;
    // bad segment-nz-nc
    return error::bad_schemeless_path_segment;
}

auto
path_noscheme_rule::
increment(
    char const*& it,
    char const* end
        ) const noexcept ->
    result<pct_encoded_view>
{
    error_code ec;
    pct_encoded_view t;
    auto const it0 = it;
    if(grammar::parse(
        it, end, ec,
        '/', segment_rule{t}))
        return t;
    it = it0;
    return grammar::error::end;
}

//------------------------------------------------

auto
path_rootless_rule::
parse(
    char const*& it,
    char const* end
        ) const noexcept ->
    result<value_type>
{
    return grammar::parse_range(
        it, end, *this,
        &path_rootless_rule::begin,
        &path_rootless_rule::increment);
}

auto
path_rootless_rule::
begin(
    char const*& it,
    char const* end
        ) const noexcept ->
     result<pct_encoded_view>
{
    error_code ec;
    pct_encoded_view t;
    if(! grammar::parse(
        it, end, ec,
        segment_nz_rule{t}))
        return ec;
    return t;
}

auto
path_rootless_rule::
increment(
    char const*& it,
    char const* end
        ) const noexcept ->
    result<pct_encoded_view>
{
    error_code ec;
    pct_encoded_view t;
    auto const it0 = it;
    if(grammar::parse(
        it, end, ec,
        '/', segment_rule{t}))
        return t;
    it = it0;
    return grammar::error::end;
}

} // urls
} // boost

#endif
