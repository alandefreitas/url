//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_GRAMMAR_IMPL_ELEMENTS_RULE_HPP
#define BOOST_URL_GRAMMAR_IMPL_ELEMENTS_RULE_HPP

#include <boost/url/grammar/parse.hpp>

namespace boost {
namespace urls {
namespace grammar {

namespace detail {

// must come first
template<
    class R0,
    class... Rn,
    std::size_t I>
void
parse_element(
    char const*&,
    char const*,
    error_code&,
    std::tuple<R0, Rn...> const&,
    std::tuple<
        typename R0::value_type,
        typename Rn::value_type...>&,
    std::integral_constant<
        std::size_t, I> const&,
    std::false_type const&)
{
    // end
}

template<
    class R0,
    class... Rn,
    std::size_t I>
void
parse_element(
    char const*& it,
    char const* end,
    error_code& ec,
    std::tuple<R0, Rn...> const& rn,
    std::tuple<
        typename R0::value_type,
        typename Rn::value_type...>& tn,
    std::integral_constant<
        std::size_t, I> const&,
    std::true_type const&)
{
    auto rv = grammar::parse_(
        it, end, std::get<I>(rn));
    if(! rv)
    {
        ec = rv.error();
        return;
    }
    else
    {
        ec = {}; // VFALCO REMOVE
    }
    std::get<I>(tn) = rv.value();
    parse_element(
        it, end, ec, rn, tn,
        std::integral_constant<
            std::size_t, I + 1>{},
        std::integral_constant<bool,
            ((I + 1) < (
                1 + sizeof...(Rn)))>{});
}

} // detail

template<
    class R0,
    class... Rn>
auto
elements_rule_t<R0, Rn...>::
parse(
    char const*& it,
    char const* end) const ->
        result<value_type>
{
    error_code ec;
    value_type t;
    detail::parse_element(
        it, end, ec, rn_, t,
        std::integral_constant<
            std::size_t, 0>{},
        std::true_type{});
    if(ec.failed())
        return ec;
    return t;
}

//------------------------------------------------

} // grammar
} // urls
} // boost

#endif
