//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

// Test that header file is self-contained.
#include <boost/url/bnf/userinfo.hpp>

#include <boost/url/detail/except.hpp>
#include "test_suite.hpp"
#include "test_bnf.hpp"

namespace boost {
namespace urls {
namespace bnf {

class userinfo_test
{
public:
    void
    check(
        string_view s,
        string_view s1,
        string_view s2,
        string_view s3)
    {
        userinfo p;
        error_code ec;
        auto const end =
            s.data() + s.size();
        auto it = p.parse(
            s.data(), end, ec);
        if(! ec && it != end)
            ec = error::syntax;
        if(ec)
            detail::throw_system_error(ec,
                BOOST_CURRENT_LOCATION);
        BOOST_TEST(p.value().encoded_userinfo() == s1);
        BOOST_TEST(p.value().user() == s2);
        BOOST_TEST(p.value().password() == s3);
    }

    void
    run()
    {
        test::bad <userinfo>("@");

        test::good<userinfo>("");
        test::good<userinfo>("x");
        test::good<userinfo>("xy");
        test::good<userinfo>("x:");
        test::good<userinfo>("x:y");
        test::good<userinfo>("x:y:");
        test::good<userinfo>("x:y:z");
        test::good<userinfo>("%41");

        check("x", "x", "x", "");
        check("x:", "x:", "x", "");
        check(":", ":", "", "");
        check("::", "::", "", ":");
        check(":x", ":x", "", "x");
        check("x:y", "x:y", "x", "y");
        check("xy:zz:", "xy:zz:", "xy", "zz:");
        check(
            "%41%42:%43%44",
            "%41%42:%43%44",
            "AB", "CD");
    }
};

TEST_SUITE(
    userinfo_test,
    "boost.url.userinfo");

} // bnf
} // urls
} // boost
