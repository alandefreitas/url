//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
// Copyright (c) 2022 Alan de Freitas (alandefreitas@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_PARAMS_IPP
#define BOOST_URL_IMPL_PARAMS_IPP

#include <boost/url/params.hpp>
#include <boost/url/url.hpp>
#include <boost/url/detail/pct_encoding.hpp>
#include <boost/assert.hpp>

namespace boost {
namespace urls {

params::
reference::
reference(
    char const* const s,
    std::size_t const nk,
    std::size_t const nv,
    const_string::factory const& a)
{
    // key
    BOOST_ASSERT(nk > 0);
    BOOST_ASSERT(
        s[0] == '?' || s[0] == '&');
    string_view ek{s + 1, nk - 1};
    auto n =
        pct_decode_bytes_unchecked(ek);
    key = a(n, [nk, ek]
        (std::size_t, char* dest)
        {
            pct_decode_unchecked(
                dest, dest + nk, ek);
        });
    if(nv > 0)
    {
        // value
        BOOST_ASSERT(s[nk] == '=');
        has_value = true;
        string_view ev{
            s + nk + 1, nv - 1 };
        n = pct_decode_bytes_unchecked(ev);
        value = a(n, [ev]
            (std::size_t n, char* dest)
            {
                pct_decode_unchecked(
                    dest, dest + n, ev);
            });
    }
    else
    {
        has_value = false;
    }
}

params::
reference::
reference(
    const_string&& key0,
    const_string&& value0,
    bool has_value0)
    : key(std::move(key0))
    , value(std::move(value0))
    , has_value(has_value0)
{
}

//------------------------------------------------
//
// Modifiers
//
//------------------------------------------------

auto
params::
remove_value(
    iterator pos) ->
        iterator
{
    BOOST_ASSERT(pos.impl_.begin_ ==
        u_->encoded_query().data());
    BOOST_ASSERT(pos.impl_.end_ ==
        u_->encoded_query().data() +
        u_->encoded_query().size());
    auto r = u_->param(pos.impl_.i_);
    value_type v{
        string_view{
            u_->s_ + r.pos + 1,
            r.nk - 1},
        string_view{},
        false};
    u_->edit_params(
        pos.impl_.i_,
        pos.impl_.i_ + 1,
        detail::make_enc_params_iter(
            &v, &v + 1),
        detail::make_enc_params_iter(
            &v, &v + 1));
    return std::next(begin(), pos.impl_.i_);
}

auto
params::
replace_value(
    iterator pos,
    string_view value) ->
        iterator
{
    auto const r =
        u_->param(pos.impl_.i_);
    string_view key{
        u_->s_ + r.pos + 1,
        r.nk - 1};
    value_type v{
        key, value, true };
    BOOST_ASSERT(pos.impl_.begin_ ==
        u_->encoded_query().data());
    BOOST_ASSERT(pos.impl_.end_ ==
        u_->encoded_query().data() +
        u_->encoded_query().size());
    using detail::
        make_plain_value_iter;
    u_->edit_params(
        pos.impl_.i_,
        pos.impl_.i_ + 1,
        make_plain_value_iter(
            &v, &v + 1),
        make_plain_value_iter(
            &v, &v + 1));
    return std::next(begin(), pos.impl_.i_);
}

auto
params::
erase(
    iterator first,
    iterator last) ->
        iterator
{
    BOOST_ASSERT(first.impl_.begin_ ==
        u_->encoded_query().data());
    BOOST_ASSERT(first.impl_.end_ ==
        u_->encoded_query().data() +
        u_->encoded_query().size());
    BOOST_ASSERT(last.impl_.begin_ ==
        u_->encoded_query().data());
    BOOST_ASSERT(last.impl_.end_ ==
        u_->encoded_query().data() +
        u_->encoded_query().size());
    string_view s;
    u_->edit_params(
        first.impl_.i_,
        last.impl_.i_,
        detail::enc_query_iter(s),
        detail::enc_query_iter(s));
    return std::next(begin(), first.impl_.i_);
}

std::size_t
params::
erase(string_view key) noexcept
{
    std::size_t n = 0;
    auto it = find(key);
    while(it != end())
    {
        ++n;
        it = erase(it);
        it = find(it, key);
    }
    return n;
}

//------------------------------------------------
//
// Lookup
//
//------------------------------------------------

std::size_t
params::
count(string_view key) const noexcept
{
    std::size_t n = 0;
    auto it = find(key);
    auto const end_ = end();
    while(it != end_)
    {
        ++n;
        ++it;
        it = find(it, key);
    }
    return n;
}

auto
params::
find(
    iterator from,
    string_view key) const noexcept ->
        iterator
{
    BOOST_ASSERT(from.impl_.begin_ ==
        u_->encoded_query().data());
    BOOST_ASSERT(from.impl_.end_ ==
        u_->encoded_query().data() +
        u_->encoded_query().size());
    auto const end_ = end();
    while(from != end_)
    {
        auto r = u_->param(
            from.impl_.i_);
        if( detail::key_equal_encoded(
            key, string_view(u_->s_ +
            r.pos + 1, r.nk - 1)))
            break;
        ++from;
    }
    return from;
}

} // urls
} // boost

#endif
