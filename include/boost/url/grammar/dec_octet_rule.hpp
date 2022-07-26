//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_GRAMMAR_DEC_OCTET_RULE_HPP
#define BOOST_URL_GRAMMAR_DEC_OCTET_RULE_HPP

#include <boost/url/detail/config.hpp>
#include <boost/url/result.hpp>

namespace boost {
namespace urls {
namespace grammar {

struct dec_octet_rule
{
    using value_type =
        unsigned char;

    BOOST_URL_DECL
    auto
    parse(
        char const*& it,
        char const* end
            ) const noexcept ->
        result<value_type>;
};

} // grammar
} // urls
} // boost

#endif
