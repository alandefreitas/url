//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_RFC_PCT_ENCODED_RULE_HPP
#define BOOST_URL_RFC_PCT_ENCODED_RULE_HPP

#include <boost/url/detail/config.hpp>
#include <boost/url/string_view.hpp>
#include <boost/url/pct_encoded_view.hpp>
#include <boost/url/grammar/charset.hpp>
#include <boost/url/grammar/error.hpp>
#include <boost/url/grammar/parse_tag.hpp>
#include <boost/static_assert.hpp>

#include <boost/url/pct_encoding.hpp>
#include <boost/url/grammar/detail/charset.hpp>

namespace boost {
namespace urls {

/** Rule for a string of percent-encoded characters from a character set

    @tparam CharSet The character set indicating
    which characters are unreserved. Any character
    not in this set will be considered reserved.

    @par Specification
    @li <a href="https://datatracker.ietf.org/doc/html/rfc3986#section-2.1">
        2.1. Percent-Encoding (rfc3986)</a>
*/
template<class CharSet>
struct pct_encoded_rule_t
{
    using type = pct_encoded_view;

    template<class CharSet_>
    friend
    auto
    pct_encoded_rule(
        CharSet_ const& cs) noexcept ->
            pct_encoded_rule_t<CharSet_>;

    friend
    void
    tag_invoke(
        grammar::parse_tag const&,
        char const*& it,
        char const* end,
        error_code& ec,
        pct_encoded_rule_t const& r,
        type& t) noexcept
    {
        r.parse(it, end, ec, t);
    }

private:
    CharSet cs_;

    pct_encoded_rule_t(
        CharSet const& cs) noexcept
        : cs_(cs)
    {
    }

    void
    parse(
        char const*& it,
        char const* end,
        error_code& ec,
        type& t) const noexcept;
};

template<class CharSet>
auto
pct_encoded_rule(
    CharSet const& cs) noexcept ->
        pct_encoded_rule_t<CharSet>
{
    return pct_encoded_rule_t<CharSet>(cs);
}

} // urls
} // boost

#include <boost/url/rfc/impl/pct_encoded_rule.hpp>

#endif
