//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_RFC_FRAGMENT_RULE_HPP
#define BOOST_URL_RFC_FRAGMENT_RULE_HPP

#include <boost/url/detail/config.hpp>
#include <boost/url/rfc/charsets.hpp>
#include <boost/url/rfc/pct_encoded_rule.hpp>
#include <boost/url/grammar/char_rule.hpp>
#include <boost/url/grammar/elements_rule.hpp>
#include <boost/url/grammar/optional_rule.hpp>

namespace boost {
namespace urls {

/** Rule for fragment

    @par BNF
    @code
    fragment        = *( pchar / "/" / "?" )
    @endcode

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.5"
        >3.5. Fragment (rfc3986)</a>
*/
constexpr auto fragment_rule =
    pct_encoded_rule(
        pchars + '/' + '?');

/** Rule for fragment-part

    @par BNF
    @code
    fragment-part   = [ "#" fragment ]

    fragment        = *( pchar / "/" / "?" )
    @endcode

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-3.5"
        >3.5. Fragment (rfc3986)</a>
*/
constexpr auto fragment_part_rule =
    grammar::optional_rule(
        grammar::elements_rule(
            grammar::char_rule('#'),
            fragment_rule));

} // urls
} // boost

#endif
