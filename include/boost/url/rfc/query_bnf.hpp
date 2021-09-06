//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_RFC_QUERY_BNF_HPP
#define BOOST_URL_RFC_QUERY_BNF_HPP

#include <boost/url/detail/config.hpp>
#include <boost/url/error.hpp>
#include <boost/url/optional.hpp>
#include <boost/url/bnf/range.hpp>
#include <boost/url/rfc/pct_encoded_bnf.hpp>

namespace boost {
namespace urls {
namespace rfc {

struct query_param
{
    pct_encoded_str key;
    optional<
        pct_encoded_str> value;
};

/** BNF for query

    @par BNF
    @code
    query           = *( pchar / "/" / "?" )
                    / path-absolute
                    / path-rootless
                    / path-empty
    @endcode

    @see
        https://datatracker.ietf.org/doc/html/rfc3986#section-3.4
*/
struct query_bnf
{
    bnf::range<query_param>& v;

    BOOST_URL_DECL
    friend
    bool
    parse(
        char const*& it,
        char const* const end,
        error_code& ec,
        query_bnf const& t);
};

} // rfc
} // urls
} // boost

#endif
