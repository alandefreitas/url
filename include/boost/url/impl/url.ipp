//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
// Copyright (c) 2022 Alan Freitas (alandefreitas@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/url
//

#ifndef BOOST_URL_IMPL_URL_IPP
#define BOOST_URL_IMPL_URL_IPP

#include <boost/url/url.hpp>
#include <boost/assert.hpp>

namespace boost {
namespace urls {

//------------------------------------------------

url::
~url()
{
    if(s_)
    {
        BOOST_ASSERT(
            cap_ != 0);
        deallocate(s_);
    }
}

// construct empty
url::
url() noexcept = default;

url::
url(string_view s)
    : url(parse_uri_reference(s
        ).value(BOOST_URL_POS))
{
}

url::
url(url&& u) noexcept
    : url_base(u.impl_)
{
    s_ = u.s_;
    cap_ = u.cap_;
    u.s_ = nullptr;
    u.cap_ = 0;
    u.impl_ = {from::url};
}

url&
url::
operator=(url&& u) noexcept
{
    if(s_)
        deallocate(s_);
    impl_ = u.impl_;
    s_ = u.s_;
    cap_ = u.cap_;
    u.s_ = nullptr;
    u.cap_ = 0;
    u.impl_ = {from::url};
    return *this;
}

//------------------------------------------------

char*
url::
allocate(std::size_t n)
{
    auto s = new char[n + 1];
    cap_ = n;
    return s;
}

void
url::
deallocate(char* s)
{
    delete[] s;
}

void
url::
clear_impl() noexcept
{
    if(s_)
    {
        // preserve capacity
        impl_ = {from::url};
        s_[0] = '\0';
        impl_.cs_ = s_;
    }
    else
    {
        BOOST_ASSERT(impl_.cs_ ==
            detail::empty_c_str_);
    }
}

void
url::
reserve_impl(
    std::size_t n,
    op_t& op)
{
    if(n > max_size())
        detail::throw_length_error();
    if(n <= cap_)
        return;
    char* s;
    if(s_ != nullptr)
    {
        // 50% growth policy
        auto const h = cap_ / 2;
        std::size_t new_cap;
        if(cap_ <= max_size() - h)
            new_cap = cap_ + h;
        else
            new_cap = max_size();
        if( new_cap < n)
            new_cap = n;
        s = allocate(new_cap);
        std::memcpy(s, s_, size() + 1);
        BOOST_ASSERT(! op.old);
        op.old = s_;
        s_ = s;
    }
    else
    {
        s_ = allocate(n);
        s_[0] = '\0';
    }
    impl_.cs_ = s_;
}

result<void>
relative(
    url_view_base const& base,
    url_view_base const& href,
    url_base& dest)
{
    // AFREITAS:
    // - filesystem functions use
    //   (const path&, const path& base)
    // - this function name is probably still bad
    // - the function in URI.js behaves slightly
    //   differently from their own examples
    //   see https://medialize.github.io/URI.js/docs.html#relativeto
    // - we have a bug where u.set_path("") or
    //   u.segments() = {} crash, so this function
    //   uses u.segments() = {"."}, which gives
    //   us slightly different results.
    // - The basic exception guarantee is not
    //   satisfied in case of allocation errors.

    BOOST_ASSERT(&dest != &base);
    BOOST_ASSERT(&dest != &href);

    // Validate input
    if (!href.is_path_absolute())
    {
        // href is already relative
        return error::not_a_base;
    }
    if (!base.is_path_absolute())
    {
        // cannot calculate a URI relative to another relative URI
        return error::not_a_base;
    }

    // Resolve scheme
    if (href.scheme() == base.scheme() ||
        !href.has_scheme())
        dest.remove_scheme();
    else
        dest.set_scheme(href.scheme());

    // Resolve authority
    if (dest.has_scheme() ||
        href.has_authority() != base.has_authority() ||
        href.authority() != base.authority() ||
        href.has_userinfo() ||
        href.has_password())
    {
        // Otherwise, copy all but scheme from href
        if (href.has_authority())
            dest.set_encoded_authority(href.encoded_authority());
        else
            dest.remove_authority();
        dest.set_encoded_path(href.encoded_path());
        dest.normalize_path();
        if (href.has_query())
            dest.set_encoded_query(href.encoded_query());
        else
            dest.remove_query();
        if (href.has_fragment())
            dest.set_encoded_fragment(href.encoded_fragment());
        else
            dest.remove_fragment();
        return {};
    }
    dest.remove_authority();

    // Resolve path
    // 0. Get segments
    auto segs0 = base.segments();
    auto segs1 = href.segments();
    auto begin0 = segs0.begin();
    auto it0 = begin0;
    auto end0 = segs0.end();
    auto last0 = begin0 != end0 ? std::prev(end0) : end0;
    auto begin1 = segs1.begin();
    auto it1 = begin1;
    auto end1 = segs1.end();
    auto last1 = begin0 != end1 ? std::prev(end1) : end1;
    pct_encoded_view const dotdot("..");
    pct_encoded_view const dot(".");

    // 1. Find the longest common path
    while (
        it0 != last0 &&
        it1 != last1)
    {
        if (*it0 == *it1)
        {
            ++it0;
            ++it1;
        }
        else if (*it0 == dot)
        {
            ++it0;
        }
        else if (*it1 == dot)
        {
            ++it1;
        }
        else if (*it0 == dotdot)
        {
            ++it0;
            if (it1 != begin1)
                --it1;
        }
        else if (*it1 == dotdot)
        {
            if (it0 != begin0)
                --it0;
            ++it1;
        }
        else
        {
            // Check if *it0 will be consumed by a dotdot
            auto it2 = std::next(it0);
            std::size_t l = 1;
            while (it2 != last0)
            {
                if (*it2 == dotdot)
                {
                    if (--l == 0)
                    {
                        ++it2;
                        it0 = it2;
                        break;
                    }
                }
                else if (*it2 != dot)
                {
                    ++l;
                }
                ++it2;
            }
            if (it0 == it2)
                continue;

            // Check if *it1 will be consumed by a dotdot
            auto it3 = std::next(it1);
            l = 1;
            while (it3 != last1)
            {
                if (*it3 == dotdot)
                {
                    if (--l == 0)
                    {
                        ++it3;
                        it1 = it3;
                        break;
                    }
                }
                else if (*it3 != dot)
                {
                    ++l;
                }
                ++it3;
            }
            if (it1 == it3)
                continue;

            break;
        }
    }

    // 1.b Check if paths are the same
    if (it0 == last0 &&
        it1 == last1 &&
        it0 != end0 &&
        it1 != end1 &&
        *it0 == *it1)
    {
        // Return empty path
        dest.segments() = {dot.encoded()};
        if (href.has_query())
            dest.set_encoded_query(href.encoded_query());
        else
            dest.remove_query();
        if (href.has_fragment())
            dest.set_encoded_fragment(href.encoded_fragment());
        else
            dest.remove_fragment();
        return {};
    }

    // 2. replace each path component in the
    //    base path with ../
    segments_encoded segs = dest.encoded_segments();
    segs = {dot.encoded()};
    if (it0 != end0)
    {
        dest.set_path_absolute(false);
        auto last0 = std::prev(end0);
        while (it0 != last0)
        {
            if (*it0 == dotdot)
            {
                if (segs.size() > 1)
                    segs.pop_back();
                else
                    segs = {dot.encoded()};
            }
            else if (*it0 != dot)
            {
                if (dest.path() == dot)
                    segs = {dotdot.encoded()};
                else
                    segs.push_back(dotdot.encoded());
            }
            ++it0;
        }
    }

    // 3. Append the reference path
    while (it1 != end1)
    {
        if (*it1 == dotdot)
        {
            if (segs.size() > 1)
                segs.pop_back();
            else
                segs = {dot.encoded()};
        }
        else if (*it1 != dot)
        {
            string_view v = (*it1).encoded();
            if (dest.path() == dot)
                segs = {v};
            else
                segs.push_back(v);
        }
        ++it1;
    }
    if (href.has_query())
        dest.set_encoded_query(href.encoded_query());
    else
        dest.remove_query();

    if (href.has_fragment())
        dest.set_encoded_fragment(href.encoded_fragment());
    else
        dest.remove_fragment();

    return {};
}


void
url::
cleanup(
    op_t& op)
{
    if(op.old)
        deallocate(op.old);
}

//------------------------------------------------

void
url::
swap(url& other) noexcept
{
    if (this == &other)
        return;
    std::swap(s_, other.s_);
    std::swap(cap_, other.cap_);
    std::swap(impl_, other.impl_);
    std::swap(pi_, other.pi_);
    if (pi_ == &other.impl_)
        pi_ = &impl_;
    if (other.pi_ == &impl_)
        other.pi_ = &other.impl_;
}

} // urls
} // boost

#endif
