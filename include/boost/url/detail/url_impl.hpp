//
// Copyright (c) 2022 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_DETAIL_URL_IMPL_HPP
#define BOOST_URL_DETAIL_URL_IMPL_HPP

#include <boost/url/host_type.hpp>
#include <boost/url/scheme.hpp>
#include <boost/url/string_view.hpp>
#include <boost/url/detail/parts_base.hpp>
#include <boost/assert.hpp>
#include <cstdint>

// VFALCO These structs used to be forward
// declared, but the parsers now use a
// nested type.
#include <boost/url/rfc/authority_rule.hpp>
#include <boost/url/rfc/fragment_rule.hpp>
#include <boost/url/rfc/host_rule.hpp>
#include <boost/url/rfc/paths_rule.hpp>
#include <boost/url/rfc/query_rule.hpp>
#include <boost/url/rfc/scheme_rule.hpp>

namespace boost {
namespace urls {

class url_view;

namespace detail {

// This is the private 'guts' of a
// url_view, exposed so different parts
// of the implementation can work on it.
struct url_impl : parts_base
{
    static
    constexpr
    char const* const empty_ = "";
 
    static
    constexpr
    pos_t const zero_ = 0;

    char const* cs_ = empty_;
    pos_t offset_[id_end + 1] = {};
    pos_t decoded_[id_end] = {};
    pos_t nseg_ = 0;
    pos_t nparam_ = 0;
    unsigned char ip_addr_[16] = {};
    // VFALCO don't we need a bool?
    std::uint16_t port_number_ = 0;
    host_type host_type_ =
        urls::host_type::none;
    scheme scheme_ =
        urls::scheme::none;

    url_impl() = default;

    // VFALCO This should no longer be needed
    url_impl(
        char const* cs) noexcept
        : cs_(cs)
    {
    }

    url_view construct() const noexcept;

    pos_t len(
        int first, int last) const noexcept;
    pos_t len(
        int id) const noexcept;
    pos_t offset(
        int id) const noexcept;
    string_view get(
        int id) const noexcept;
    string_view get(
        int first, int last) const noexcept;

    void set_size(
        int id, pos_t n) noexcept;
    void split(
        int id, std::size_t n) noexcept;
    void adjust(
        int first, int last,
        std::size_t n) noexcept;
    void collapse(
        int first, int last,
        std::size_t n) noexcept;

    void apply_scheme(
        string_view s) noexcept;
    void apply(scheme_part_rule::value_type const& t) noexcept;
    void apply(decltype(host_rule)::value_type const& h) noexcept;
    void apply(decltype(authority_rule)::value_type const& t) noexcept;
    void apply(parsed_path const& path) noexcept;
    void apply(decltype(query_part_rule)::value_type const& t) noexcept;
    void apply(decltype(fragment_part_rule)::value_type const& t) noexcept;
};

//------------------------------------------------

// return length of [first, last)
inline
auto
url_impl::
len(
    int first,
    int last) const noexcept ->
        pos_t
{
    BOOST_ASSERT(first <= last);
    BOOST_ASSERT(last <= id_end);
    return offset(last) - offset(first);
}

// return length of part
inline
auto
url_impl::
len(int id) const noexcept ->
    pos_t
{
    return id == id_end
        ? zero_
        : ( offset(id + 1) -
            offset(id) );
}

// return offset of id
inline
auto
url_impl::
offset(int id) const noexcept ->
    pos_t
{
    return
        id == id_scheme
        ? zero_
        : offset_[id];
}

// return id as string
inline
string_view
url_impl::
get(int id) const noexcept
{
    return {
        cs_ + offset(id), len(id) };
}

// return [first, last) as string
inline
string_view
url_impl::
get(int first,
    int last) const noexcept
{
    return { cs_ + offset(first),
        offset(last) - offset(first) };
}

//------------------------------------------------

// change id to size n
inline
void
url_impl::
set_size(
    int id,
    pos_t n) noexcept
{
    auto d = n - len(id);
    for(auto i = id + 1;
        i <= id_end; ++i)
        offset_[i] += d;
}

// trim id to size n,
// moving excess into id+1
inline
void
url_impl::
split(
    int id,
    std::size_t n) noexcept
{
    BOOST_ASSERT(id < id_end - 1);
    //BOOST_ASSERT(n <= len(id));
    offset_[id + 1] = offset(id) + n;
}

// add n to [first, last]
inline
void
url_impl::
adjust(
    int first,
    int last,
    std::size_t n) noexcept
{
    for(int i = first;
            i <= last; ++i)
        offset_[i] += n;
}

// set [first, last) offset
inline
void
url_impl::
collapse(
    int first,
    int last,
    std::size_t n) noexcept
{
    for(int i = first + 1;
            i < last; ++i)
        offset_[i] = n;
}

} // detail
} // urls
} // boost

#endif
