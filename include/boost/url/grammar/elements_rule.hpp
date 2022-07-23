//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_GRAMMAR_ELEMENTS_RULE_HPP
#define BOOST_URL_GRAMMAR_ELEMENTS_RULE_HPP

#include <boost/url/detail/config.hpp>
#include <boost/url/grammar/error.hpp>
#include <boost/url/grammar/parse_tag.hpp>
#include <tuple>

namespace boost {
namespace urls {
namespace grammar {

template<
    class R0,
    class... Rn>
class elements_rule_t
{
public:
    using value_type = std::tuple<
        typename R0::value_type,
        typename Rn::value_type...>;

    template<
        class R0_,
        class... Rn_>
    friend
    auto
    elements_rule(
        R0_ const& r0,
        Rn_ const&... rn) noexcept ->
            elements_rule_t<R0_, Rn_...>;

    template<
        class R0_,
        class... Rn_>
    friend
    void
    tag_invoke(
        grammar::parse_tag const&,
        char const*& it,
        char const* end,
        error_code& ec,
        elements_rule_t<
            R0_, Rn_...> const& r,
        value_type& t) noexcept
    {
        r.parse(it, end, ec, t);
    }

private:
    elements_rule_t(
        R0 const& r0,
        Rn const&... rn) noexcept
        : rn_(r0, rn...)
    {
    }

    void
    parse(
        char const*& it,
        char const* end,
        error_code& ec,
        value_type& t) const;

    std::tuple<R0, Rn...> rn_;
};

//------------------------------------------------

template<
    class R0,
    class... Rn>
auto
elements_rule(
    R0 const& r0,
    Rn const&... rn) noexcept ->
        elements_rule_t<
            R0, Rn...>
{
    return {r0, rn...};
}

} // grammar
} // urls
} // boost

#include <boost/url/grammar/impl/elements_rule.hpp>

#endif
