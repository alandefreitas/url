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
    typename std::enable_if<
        I < 1 + sizeof...(Rn)
            >::type const* = nullptr)
{
    std::get<I>(tn) =
        parse_(
            it, end, ec,
            std::get<I>(rn));
    if(ec.failed())
        return;
    parse_element(
        it, end, ec, rn, tn,
        std::integral_constant<
            std::size_t, I + 1>{});
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
    typename std::enable_if<
        I >= 1 + sizeof...(Rn)
            >::type const* = nullptr)
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
            std::size_t, 0>{});
}

//------------------------------------------------

} // grammar
} // urls
} // boost

#endif
