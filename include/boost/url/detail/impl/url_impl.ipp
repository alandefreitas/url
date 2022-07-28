//
// Copyright (c) 2022 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_DETAIL_IMPL_URL_IMPL_IPP
#define BOOST_URL_DETAIL_IMPL_URL_IMPL_IPP

#include <boost/url/detail/url_impl.hpp>
#include <boost/assert.hpp>

namespace boost {
namespace urls {
namespace detail {

void
url_impl::
apply_scheme(
    string_view s) noexcept
{
    if(! s.empty())
    {
        scheme_ = string_to_scheme(s);
        set_size(id_scheme, s.size() + 1);
    }
    else
    {
        scheme_ = scheme::none;
    }
}

//------------------------------------------------

void
url_impl::
apply(
    scheme_part_rule::value_type const& t) noexcept
{
    scheme_ = t.scheme_id;
    if(t.scheme_id !=
        urls::scheme::none)
    set_size(
        id_scheme,
        t.scheme_part.size());
}

void
url_impl::
apply(
    decltype(host_rule)::value_type const& t) noexcept
{
    host_type_ = t.host_type;
    if(t.host_type ==
        urls::host_type::name)
    {
        decoded_[id_host] =
            t.name.size();
    }
    else if(t.host_type ==
        urls::host_type::ipv4)
    {
        auto const bytes =
            t.ipv4.to_bytes();
        decoded_[id_host] = t.host_part.size();
        std::memcpy(
            &ip_addr_[0],
            bytes.data(), 4);
    }
    else if(t.host_type ==
        urls::host_type::ipv6)
    {
        auto const bytes =
            t.ipv6.to_bytes();
        decoded_[id_host] = t.host_part.size();
        std::memcpy(
            &ip_addr_[0],
            bytes.data(), 16);
    }
    else
    {
        decoded_[id_host] = t.host_part.size();
    }

    if(t.host_type !=
        urls::host_type::none)
        set_size(
            id_host,
            t.host_part.size());
}

void
url_impl::
apply(
    decltype(authority_rule)::value_type const& t) noexcept
{
    if(t.has_userinfo)
    {
        auto const& u = t.userinfo;

        // leading "//" for authority
        set_size(
            id_user,
            u.user.encoded().size() + 2);
        decoded_[id_user] = u.user.size();

        if(u.has_password)
        {
            // leading ':' for password,
            // trailing '@' for userinfo
            set_size(
                id_pass,
                u.password.encoded().size() + 2);
            decoded_[id_pass] =
                u.password.size();
        }
        else
        {
            // trailing '@' for userinfo
            set_size(id_pass, 1);
            decoded_[id_pass] = 0;
        }
    }
    else
    {
        // leading "//" for authority
        set_size(id_user, 2);
        decoded_[id_user] = 0;
    }

    // host
    apply(t.host);

    // port
    if(t.port.has_port)
    {
        set_size(
            id_port,
            t.port.port.size() + 1);
        if(t.port.has_number)
            port_number_ =
                t.port.port_number;
    }
}

void
url_impl::
apply(
    parsed_path const& t) noexcept
{
    auto s = t.path;
    set_size(id_path, s.size());
    decoded_[id_path] =
        pct_decode_bytes_unchecked(t.path);
    nseg_ = detail::path_segments(
        t.path, t.count);
}

void
url_impl::
apply_query(
    string_view s,
    std::size_t n) noexcept
{
    nparam_ = n;
    set_size(id_query, 1 + s.size());

    // VFALCO we are doing two passes over
    // the string. once for the range and
    // again for the decoded size.
    decoded_[id_query] =
        pct_decode_bytes_unchecked(s);
}

void
url_impl::
apply_frag(
    pct_encoded_view s) noexcept
{
    set_size(id_frag,
        s.encoded().size() + 1);
    decoded_[id_frag] = s.size();
}

} // detail
} // urls
} // boost

#endif
