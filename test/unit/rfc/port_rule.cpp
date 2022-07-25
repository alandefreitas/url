//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

// Test that header file is self-contained.
#include <boost/url/rfc/port_rule.hpp>

#include "test_suite.hpp"
#include "test_rule.hpp"

namespace boost {
namespace urls {

class port_rule_test
{
public:
    void
    check(
        string_view s,
        std::uint16_t v,
        bool has_number = true)
    {
        auto rv = grammar::parse_(
            s, port_rule{});
        if(! BOOST_TEST(rv.has_value()))
            return;
        auto t = rv.value();
        BOOST_TEST(
            t.has_number == has_number);
        BOOST_TEST_EQ(t.str, s);
        if(t.has_number)
            BOOST_TEST_EQ(t.number, v);
    }

    void
    run()
    {
        using T = port_rule;

        bad("x", T{});
        bad("80x", T{});
        bad(":443", T{});

        check("", 0, false);
        check("0", 0);
        check("00", 0);
        check("01", 1);
        check("1", 1);
        check("65535", 65535);
        check("65536", 0, false);
        check("123456789", 0, false);
    }
};

TEST_SUITE(
    port_rule_test,
    "boost.url.port_rule");

} // urls
} // boost
