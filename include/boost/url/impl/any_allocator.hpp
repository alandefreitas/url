//
// Copyright (c) 2022 Alan Freitas (alandefreitas@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_ANY_ALLOCATOR_HPP
#define BOOST_URL_IMPL_ANY_ALLOCATOR_HPP

#include <boost/assert.hpp>
#include <boost/core/allocator_access.hpp>
#include <boost/config/workaround.hpp>
#include <boost/type_traits/type_with_alignment.hpp>
#include <boost/type_traits/make_void.hpp>

#include <cstddef>

namespace boost {
namespace urls {

namespace detail {
#if BOOST_WORKAROUND(BOOST_GCC, < 40900)
    using max_align_t = ::max_align_t;
#else
    using max_align_t = std::max_align_t;
#endif

inline std::size_t ceil_div(std::size_t n, std::size_t d)
{
    return (n + d - 1) / d;
}

template <std::size_t N, class Allocator>
typename std::enable_if<alignof(max_align_t) <= N, void*>::type
try_allocate_aligned(const Allocator& a, std::size_t n, std::size_t size, std::size_t align)
{
    using aligned_t = max_align_t;
    using aligned_allocator_t = boost::allocator_rebind_t<Allocator, aligned_t>;
    return aligned_allocator_t(a).allocate(n * ceil_div(size, align));
}

template <std::size_t N, class Allocator>
typename std::enable_if<N < alignof(max_align_t), void*>::type
try_allocate_aligned(const Allocator& a, std::size_t n, std::size_t size, std::size_t align)
{
    if (align <= N)
    {
        using aligned_t = typename boost::type_with_alignment<N>::type;
        using aligned_allocator_t = boost::allocator_rebind_t<Allocator, aligned_t>;
        return aligned_allocator_t(a).allocate(n * ceil_div(size, align));
    }
    else
    {
        return try_allocate_aligned<2*N>(a, n, size, align);
    }
}

template <class Allocator>
void* allocate_aligned(const Allocator& a, std::size_t n, std::size_t size, std::size_t align)
{
    return try_allocate_aligned<2*alignof(char)>(a, n, size, align);
}

template <std::size_t N, class Allocator>
typename std::enable_if<alignof(max_align_t) <= N, void>::type
try_deallocate_aligned(const Allocator& a, void* p, std::size_t n, std::size_t size, std::size_t align)
{
    using aligned_t = max_align_t;
    using aligned_allocator_t = boost::allocator_rebind_t<Allocator, aligned_t>;
    aligned_allocator_t(a).deallocate(reinterpret_cast<aligned_t*>(p), n * ceil_div(size, align));
}

template <std::size_t N, class Allocator>
typename std::enable_if<N < alignof(max_align_t), void>::type
try_deallocate_aligned(const Allocator& a, void* p, std::size_t n, std::size_t size, std::size_t align)
{
    if (align <= N)
    {
        using aligned_t = typename boost::type_with_alignment<N>::type;
        using aligned_allocator_t = boost::allocator_rebind_t<Allocator, aligned_t>;
        aligned_allocator_t(a).deallocate(reinterpret_cast<aligned_t*>(p), n * ceil_div(size, align));
    } else {
        try_deallocate_aligned<2*N>(a, p, n, size, align);
    }
}

template <class Allocator>
void deallocate_aligned(const Allocator& a, void* p, std::size_t n, std::size_t size, std::size_t align)
{
    try_deallocate_aligned<2*alignof(char)>(a, p, n, size, align);
}
}

template <class T>
any_allocator<T>::
any_allocator() noexcept
: any_allocator(std::allocator<char>{})
{
}

template <class T>
template <class Allocator>
any_allocator<T>::
any_allocator(Allocator const& a) noexcept
{
    class impl;

    using char_allocator_type = boost::allocator_rebind_t<Allocator, char>;

    class impl
            : public detail::any_allocator
    {
         char_allocator_type a_;

    public:
        explicit
        impl(char_allocator_type const& a)
        : any_allocator(detail::get_type_id<char_allocator_type>()),
          a_(a)
        {
        }

        void*
        allocate(std::size_t n, std::size_t size, std::size_t align) override
        {
            if (align == alignof(char))
                return a_.allocate(n);
            else
                return detail::allocate_aligned(a_, n, size, align);
        }

        void
        deallocate(void* p, std::size_t n, std::size_t size, std::size_t align)
        {
            if (align == alignof(char))
                a_.deallocate(reinterpret_cast<char*>(p), n);
            else
                detail::deallocate_aligned(a_, p, n, size, align);
        }

        bool
        is_equal(const any_allocator& other) override
        {
            const impl* other_impl = static_cast<const impl*>(&other);
            BOOST_ASSERT(other_impl);
            return a_ == other_impl->a_;
        };
    };

    p_ = std::allocate_shared<impl>(a, char_allocator_type{});
}

template <class T>
template <class U>
any_allocator<T>::
any_allocator(
    any_allocator<U> const& other) noexcept
: p_(other.p_)
{
}

template <class T>
T*
any_allocator<T>::
allocate(
    std::size_t n) const
{
    BOOST_ASSERT(p_);
    return std::allocator<T>().allocate(n);
}

template <class T>
void
any_allocator<T>::
deallocate(
    T* p,
    std::size_t n)
{
    BOOST_ASSERT(p_);
    return std::allocator<T>().deallocate(p, n);
}

}
}


#endif
