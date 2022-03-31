//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
// Copyright (c) 2022 Alan de Freitas (alandefreitas@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_PARAMS_VIEW_IPP
#define BOOST_URL_IMPL_PARAMS_VIEW_IPP

#include <boost/url/params_view.hpp>
#include <boost/url/url.hpp>
#include <boost/url/detail/pct_encoding.hpp>
#include <boost/assert.hpp>

namespace boost {
namespace urls {

params_view::
value_type::
~value_type() noexcept = default;

params_view::
value_type::
value_type() noexcept = default;

params_view::
value_type::
value_type(
    value_type const& other) noexcept = default;

auto
params_view::
value_type::
operator=(
    value_type const& other) noexcept ->
        value_type& = default;

params_view::
value_type::
value_type(
    char const* s,
    std::size_t nk,
    std::size_t const nv,
    const_string::factory const& a)
{
    if(nk + nv == 0)
    {
        has_value = false;
        return;
    }
    // key
    string_view ek{s, nk};
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

//------------------------------------------------
//
// Element Access
//
//------------------------------------------------

auto
params_view::
at(string_view key) const ->
    const_string
{
    auto it = find(key);
    for(;;)
    {
        if(it == end())
            detail::throw_out_of_range(
                BOOST_CURRENT_LOCATION);
        if(it.impl_.nv_ != 0)
            break;
        ++it;
        it = find(it, key);
    }
    string_view ev{
        it.impl_.pos_ + it.impl_.nk_ + 1,
        it.impl_.nv_ - 1 };
    auto n =
        pct_decode_bytes_unchecked(ev);
    return a_(n, [ev]
        (std::size_t n, char* dest)
        {
            pct_decode_unchecked(
                dest, dest + n, ev);
        });
}

//------------------------------------------------
//
// Iterators
//
//------------------------------------------------

auto
params_view::
begin() const noexcept ->
    iterator
{
    if(n_ > 0)
        return { s_, a_ };
    return end();
}

auto
params_view::
end() const noexcept ->
    iterator
{
    return { s_, n_, 0, a_ };
}

//------------------------------------------------
//
// Lookup
//
//------------------------------------------------

std::size_t
params_view::
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
params_view::
find(
    iterator from,
    string_view key) const noexcept ->
        iterator
{
    BOOST_ASSERT(from.impl_.end_ ==
        s_.data() + s_.size());

    auto const end_ = end();
    while(from != end_)
    {
        if( detail::key_equal_encoded(
            key,
            from.encoded_key()))
            break;
        ++from;
    }
    return from;
}

} // urls
} // boost

#endif
