//
// Copyright (c) 2022 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_DETAIL_OPTIONAL_HPP
#define BOOST_URL_DETAIL_OPTIONAL_HPP

#include <boost/static_assert.hpp>
#include <type_traits>

namespace boost {
namespace urls {
namespace detail {

// this simple constexpr optional is
// used to enable rules and values
// which are not default-constructible.
template<class T>
class optional
{
    BOOST_STATIC_ASSERT(
        std::is_trivial<T>::value);

    union U
    {
        T t;

        constexpr U() = default;
        constexpr U(
            T const& t_) noexcept
            : t(t_)
        {
        }
    };
    U u_;
    bool b_ = false;

public:
    constexpr optional() = default;

    ~optional()
    {
        if(b_)
            u_.t.~T();
    }

    constexpr
    optional(
        T const& t) noexcept
        : u_(t)
        , b_(true)
    {
    }

    constexpr
    optional(
        optional const& other) noexcept = default;

    constexpr
    T const&
    operator*() const noexcept
    {
        return u_.t;
    }
};

} // detail
} // urls
} // boost

#endif
