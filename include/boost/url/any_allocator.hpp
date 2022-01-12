//
// Copyright (c) 2022 Alan Freitas (alandefreitas@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_ANY_ALLOCATOR_HPP
#define BOOST_URL_ANY_ALLOCATOR_HPP

#include <boost/url/detail/config.hpp>
#include <boost/url/detail/any_allocator.hpp>

#include <memory>
#include <type_traits>

namespace boost {
namespace urls {

/** A type-erased allocator with shared ownership.

    This type satisfies the requirements for <em>Allocator</em>

    @par Specification
    @li <a href="https://en.cppreference.com/w/cpp/named_req/Allocator"
       >Allocator (cppreference.com)</a>

 */
#ifdef BOOST_URL_DOCS
template< class T >
using any_allocator = __see_below __;
#else
template <class T>
class any_allocator
{
    std::shared_ptr<detail::any_allocator> p_;

    template <class U>
    friend class any_allocator;

public:
    using value_type = T;

    template<class U>
    struct rebind
    {
        using other = any_allocator<U>;
    };

    using is_always_equal = std::false_type;

    any_allocator() noexcept;

    any_allocator(
        any_allocator const&) noexcept = default;

    template <class U>
    explicit
    any_allocator(
        any_allocator<U> const& other) noexcept;

    any_allocator&
    operator=(
        any_allocator const&) noexcept = default;

    template<class Allocator>
    explicit
    any_allocator(Allocator const& a) noexcept;

    T*
    allocate(
        std::size_t n) const;

    void
    deallocate(
        T* p,
        std::size_t n);

    friend
    bool
    operator==(
        const any_allocator& a,
        const any_allocator& b) noexcept
    {
        return (a.p_ == b.p_) || (a.p_->type == b.p_->type && a.p_->is_equal(*b.p_));
    }

    friend
    bool
    operator!=(
        const any_allocator& a,
        const any_allocator& b) noexcept
    {
        return !(a == b);
    }

};
#endif

} // urls
} // boost

#include <boost/url/impl/any_allocator.hpp>

#endif
