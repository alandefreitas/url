//
// Copyright (c) 2022 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

// Test that header file is self-contained.
#include <boost/url/grammar/char_rule.hpp>

#include <boost/url/grammar/parse.hpp>
#include "test_suite.hpp"

namespace boost {
namespace urls {
namespace grammar {

struct char_rule_test
{
    template<class R>
    static
    void
    ok( string_view s,
        R const& r)
    {
        error_code ec;
        grammar::parse_(s, ec, r);
        BOOST_TEST(! ec.failed());
    }

    template<class R>
    static
    void
    bad(string_view s,
        R const& r)
    {
        error_code ec;
        grammar::parse_(s, ec, r);
        BOOST_TEST(ec.failed());
    }

    void
    testRule()
    {
        ok("$", char_rule('$'));
        
        bad("~", char_rule('$'));
    }

    void
    run()
    {
        testRule();
    }
};

TEST_SUITE(
    char_rule_test,
    "boost.url.grammar.char_rule");

} // grammar
} // urls
} // boost
