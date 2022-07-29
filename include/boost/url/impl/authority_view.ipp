//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_AUTHORITY_VIEW_IPP
#define BOOST_URL_IMPL_AUTHORITY_VIEW_IPP

#include <boost/url/authority_view.hpp>
#include <boost/url/error.hpp>
#include <boost/url/detail/over_allocator.hpp>
#include <boost/url/grammar/parse.hpp>
#include <boost/url/rfc/authority_rule.hpp>
#include <boost/url/rfc/host_rule.hpp>
#include <boost/url/rfc/pct_encoded_rule.hpp>
#include <array>
#include <ostream>

namespace boost {
namespace urls {

authority_view::
authority_view(
    char const* cs) noexcept
    : u_(cs)
{
}

authority_view::
authority_view(
    authority_view const& u,
    char const* cs) noexcept
    : authority_view(u)
{
    u_.cs_ = cs;
}

//------------------------------------------------

authority_view::
~authority_view()
{
}

authority_view::
authority_view() noexcept = default;

authority_view::
authority_view(
    authority_view const&) noexcept = default;

authority_view&
authority_view::
operator=(
    authority_view const&) noexcept = default;

//----------------------------------------------------------
//
// Authority
//
//----------------------------------------------------------

// userinfo

bool
authority_view::
has_userinfo() const noexcept
{
    auto n = u_.len(id_pass);
    if(n == 0)
        return false;
    BOOST_ASSERT(u_.get(
        id_pass).ends_with('@'));
    return true;
}

string_view
authority_view::
encoded_userinfo() const noexcept
{
    auto s = u_.get(
        id_user, id_host);
    if(s.empty())
        return s;
    if(s.empty())
        return s;
    BOOST_ASSERT(
        s.ends_with('@'));
    s.remove_suffix(1);
    return s;
}

bool
authority_view::
has_password() const noexcept
{
    auto const n = u_.len(id_pass);
    if(n > 1)
    {
        BOOST_ASSERT(u_.get(id_pass
            ).starts_with(':'));
        BOOST_ASSERT(u_.get(id_pass
            ).ends_with('@'));
        return true;
    }
    BOOST_ASSERT(n == 0 || u_.get(
        id_pass).ends_with('@'));
    return false;
}

string_view
authority_view::
encoded_password() const noexcept
{
    auto s = u_.get(id_pass);
    switch(s.size())
    {
    case 1:
        BOOST_ASSERT(
            s.starts_with('@'));
        BOOST_FALLTHROUGH;
    case 0:
        return s.substr(0, 0);
    default:
        break;
    }
    BOOST_ASSERT(
        s.ends_with('@'));
    BOOST_ASSERT(
        s.starts_with(':'));
    return s.substr(1,
        s.size() - 2);
}

// host

string_view
authority_view::
encoded_host() const noexcept
{
    return u_.get(id_host);
}

urls::ipv4_address
authority_view::
ipv4_address() const noexcept
{
    if(u_.host_type_ !=
        urls::host_type::ipv4)
        return {};
    std::array<
        unsigned char, 4> bytes;
    std::memcpy(
        &bytes[0],
        &u_.ip_addr_[0], 4);
    return urls::ipv4_address(
        bytes);
}

urls::ipv6_address
authority_view::
ipv6_address() const noexcept
{
    if(u_.host_type_ ==
        urls::host_type::ipv6)
    {
        std::array<
            unsigned char, 16> bytes;
        std::memcpy(
            &bytes[0],
            &u_.ip_addr_[0], 16);
        return urls::ipv6_address(
            bytes);
    }
    return urls::ipv6_address();
}

string_view
authority_view::
ipvfuture() const noexcept
{
    if(u_.host_type_ ==
        urls::host_type::ipvfuture)
        return u_.get(id_host);
    return {};
}

// port

bool
authority_view::
has_port() const noexcept
{
    auto const n = u_.len(id_port);
    if(n == 0)
        return false;
    BOOST_ASSERT(
        u_.get(id_port).starts_with(':'));
    return true;
}

string_view
authority_view::
port() const noexcept
{
    auto s = u_.get(id_port);
    if(s.empty())
        return s;
    BOOST_ASSERT(has_port());
    return s.substr(1);
}

std::uint16_t
authority_view::
port_number() const noexcept
{
    BOOST_ASSERT(
        has_port() ||
        u_.port_number_ == 0);
    return u_.port_number_;
}

string_view
authority_view::
encoded_host_and_port() const noexcept
{
    return u_.get(id_host, id_end);
}

//------------------------------------------------
//
// Parsing
//
//------------------------------------------------

void
authority_view::
apply(
    decltype(host_rule)::value_type const& t) noexcept
{
    u_.host_type_ = t.host_type;
    switch(t.host_type)
    {
    default:
    case urls::host_type::none:
    {
        break;
    }
    case urls::host_type::name:
    {
        u_.decoded_[id_host] =
            t.name.size();
        break;
    }
    case urls::host_type::ipv4:
    {
        auto const bytes =
            t.ipv4.to_bytes();
        std::memcpy(
            &u_.ip_addr_[0],
            bytes.data(), 4);
        u_.decoded_[id_host] =
            t.host_part.size();
        break;
    }
    case urls::host_type::ipv6:
    {
        auto const bytes =
            t.ipv6.to_bytes();
        std::memcpy(
            &u_.ip_addr_[0],
            bytes.data(), 16);
        u_.decoded_[id_host] =
            t.host_part.size();
        break;
    }
    case urls::host_type::ipvfuture:
    {
        u_.decoded_[id_host] =
            t.host_part.size();
        break;
    }
    }

    if(t.host_type !=
        urls::host_type::none)
    {
        u_.set_size(
            id_host,
            t.host_part.size());
    }
}

void
authority_view::
apply(
    decltype(authority_rule)::value_type const& t) noexcept
{
    if(t.has_userinfo)
    {
        auto const& u = t.userinfo;

        u_.set_size(
            id_user,
            u.user.encoded().size());
        u_.decoded_[id_user] = u.user.size();

        if(u.has_password)
        {
            // leading ':' for password,
            // trailing '@' for userinfo
            u_.set_size(
                id_pass,
                u.password.encoded().size() + 2);
            u_.decoded_[id_pass] =
                u.password.size();
        }
        else
        {
            // trailing '@' for userinfo
            u_.set_size(id_pass, 1);
            u_.decoded_[id_pass] = 0;
        }
    }
    else
    {
        u_.set_size(id_user, 0);
        u_.decoded_[id_user] = 0;
    }

    // host
    apply(t.host);

    // port
    if(t.port.has_port)
    {
        u_.set_size(
            id_port,
            t.port.port.size() + 1);

        if(t.port.has_number)
            u_.port_number_ =
                t.port.port_number;
    }
}

//------------------------------------------------
//
// Parsing
//
//------------------------------------------------

result<authority_view>
parse_authority(
    string_view s) noexcept
{
    if(s.size() > authority_view::max_size())
        detail::throw_length_error(
            "authority_view::max_size exceeded",
            BOOST_CURRENT_LOCATION);

    auto rv = grammar::parse(
        s, authority_rule);
    if(! rv)
        return rv.error();

    authority_view a(s.data());

    // authority
    a.apply(*rv);

    return a;
}

} // urls
} // boost

#endif
