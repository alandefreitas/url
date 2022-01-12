//
// Copyright (c) 2022 Alan Freitas (alandefreitas@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//


#ifndef BOOST_URL_DETAIL_ANY_ALLOCATOR_HPP
#define BOOST_URL_DETAIL_ANY_ALLOCATOR_HPP

#include <boost/url/detail/type_id.hpp>

#include <cstddef>

namespace boost {
namespace urls {
namespace detail {

struct any_allocator
{
    type_id_t type;

    virtual
    ~any_allocator()
    {
    }

    explicit any_allocator(type_id_t t)
    : type(t)
    {
    }

    virtual
    void *
    allocate(std::size_t n, std::size_t size, std::size_t align) = 0;

    virtual
    void
    deallocate(void *p, std::size_t n, std::size_t size, std::size_t align) = 0;

    virtual
    bool
    is_equal(const any_allocator &p) = 0;

};

}
}
}


#endif
