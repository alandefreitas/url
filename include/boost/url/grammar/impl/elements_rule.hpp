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
    std::get<I>(tn) =
        grammar::parse_(
            it, end, ec,
            std::get<I>(rn));
    if(ec.failed())
        return;
    parse_element(
        it, end, ec, rn, tn,
        std::integral_constant<
            std::size_t, I + 1>{},
        std::integral_constant<bool,
            I + 1 < (1 + sizeof...(Rn))>{});
}

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

} // detail

template<
    class R0,
    class... Rn>
void
elements_rule_t<R0, Rn...>::
parse(
    char const*& it,
    char const* end,
    error_code& ec,
    value_type& t) const
{
    detail::parse_element(
        it, end, ec, rn_, t,
        std::integral_constant<
            std::size_t, 0>{},
        std::true_type{});
}

//------------------------------------------------

} // grammar
} // urls
} // boost

#endif
