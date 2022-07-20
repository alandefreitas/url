//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
// Copyright (c) 2022 Alan de Freitas (alandefreitas@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_IMPL_QUERY_PARAM_IPP
#define BOOST_URL_IMPL_QUERY_PARAM_IPP

#include <boost/url/query_param.hpp>
#include <boost/url/pct_encoding.hpp>

namespace boost {
namespace urls {

query_param::
query_param(
    char const* s,
    std::size_t nk,
    std::size_t const nv)
{
    if(nk + nv == 0)
    {
        has_value = false;
        return;
    }
    // key
    string_view ek{s, nk};
    key = pct_encoded_view(ek);
    if(nv > 0)
    {
        // value
        BOOST_ASSERT(s[nk] == '=');
        has_value = true;
        string_view ev{
            s + nk + 1, nv - 1 };
        value = pct_encoded_view(ev);
    }
    else
    {
        has_value = false;
    }
}

} // urls
} // boost

#endif
