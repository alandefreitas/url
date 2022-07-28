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
set_scheme(
    scheme id,
    std::size_t n) noexcept
{
    scheme_ = id;
    BOOST_ASSERT(
        (id == scheme::none) == (n == 0));
    set_size(id_scheme, n);
}
//------------------------------------------------

} // detail
} // urls
} // boost

#endif
