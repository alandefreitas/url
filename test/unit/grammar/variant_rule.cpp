//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

// Test that header file is self-contained.
#include <boost/url/grammar/variant_rule.hpp>

#include <boost/url/grammar/parse.hpp>

#include "test_suite.hpp"

namespace boost {
namespace urls {
namespace grammar {

struct variant_rule_test
{
    void
    run()
    {
    }
};

TEST_SUITE(
    variant_rule_test,
    "boost.url.grammar.variant_rule");

} // grammar
} // urls
} // boost
