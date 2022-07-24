//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_TEST_RULE_HPP
#define BOOST_URL_TEST_RULE_HPP

#include <boost/url/grammar/charset.hpp>
#include <boost/url/grammar/parse.hpp>
#include <boost/url/string_view.hpp>
#include <initializer_list>

#include "test_suite.hpp"

namespace boost {
namespace urls {

template<class F>
void
for_each_char(F const& f)
{
    unsigned char u = 0;
    do
    {
        f(static_cast<
            char>(u));
    }
    while(++u != 0);
}

template<class CharSet>
void
test_char_set(
    CharSet const& cs,
    string_view s) noexcept
{
    // each char in s is in the set.
    for(char c : s)
        BOOST_TEST(cs(c));

    // number of chars in
    // set equals s.size()
    std::size_t n = 0;
    for_each_char(
    [&cs, &n](char c)
    {
        if(cs(c))
            ++n;
    });
    BOOST_TEST_EQ(n, s.size());

    // test find_if and find_if_not
    for_each_char(
    [&cs](char c)
    {
        if(cs(0) || ! cs(c))
        {
            if(cs(c))
            {
                BOOST_TEST(grammar::find_if(
                    &c, &c+1, cs) == &c);
                BOOST_TEST(grammar::find_if_not(
                    &c, &c+1, cs) == &c+1);
            }
            else
            {
                BOOST_TEST(grammar::find_if(
                    &c, &c+1, cs) == &c+1);
                BOOST_TEST(grammar::find_if_not(
                    &c, &c+1, cs) == &c);
            }
            return;
        }

        char buf[40];
        std::memset(
            buf, 0, sizeof(buf));
        buf[19] = c;
        buf[22] = c;
        BOOST_TEST(grammar::find_if(
            buf, buf + sizeof(buf), cs) ==
                buf + 19);
        std::memset(
            buf, c, sizeof(buf));
        buf[19] = 0;
        buf[22] = 0;
        BOOST_TEST(grammar::find_if_not(
            buf, buf + sizeof(buf), cs) ==
                buf + 19);
    });
}

template<
    class T, class V>
struct test_ref
{
    V v;

    friend
    bool
    parse(
        char const*& it,
        char const* end,
        error_code& ec,
        test_ref& t)
    {
        return grammar::parse(
            it, end, ec,
            T{t.v});
    }
};

template<class T>
bool
is_valid(string_view s)
{
    T t;
    error_code ec;
    return grammar::parse_string(
        s, ec, t);
}

bool
is_valid(char) = delete;

template<class T>
void
validate(string_view s)
{
    if(! is_valid<T>(s))
        detail::throw_invalid_argument(
            BOOST_CURRENT_LOCATION);
}

template<class T>
void
bad(string_view s)
{
    BOOST_TEST_THROWS(
        validate<T>(s),
        std::exception);
    BOOST_TEST(! is_valid<T>(s));
}

template<class R>
void
bad(string_view s, R const& r)
{
    BOOST_TEST(grammar::parse_(
        s, r).has_error());
}

template<class T>
void
bad(std::initializer_list<
    string_view> init)
{
    for(auto s : init)
        bad<T>(s);
}

template<class T>
void
good(string_view s)
{
    BOOST_TEST_NO_THROW(
        validate<T>(s));
    BOOST_TEST(is_valid<T>(s));
}

template<class T>
void
good(std::initializer_list<
    string_view> init)
{
    for(auto s : init)
        good<T>(s);
}

} // urls
} // boost

#endif
