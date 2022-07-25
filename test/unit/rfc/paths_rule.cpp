//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

// Test that header file is self-contained.
#include <boost/url/rfc/paths_rule.hpp>

#include <boost/url/grammar/parse.hpp>
#include <boost/url/grammar/range.hpp>
#include <boost/static_assert.hpp>
#include "test_suite.hpp"
#include "test_rule.hpp"

namespace boost {
namespace urls {

class paths_rule_test
{
public:
    template<class R>
    void
    ok(string_view s, R const& r)
    {
        BOOST_TEST(grammar::parse_(
            s, r).has_value());
    }

    template<class R>
    void
    bad_(string_view s, R const& r)
    {
        BOOST_TEST(grammar::parse_(
            s, r).has_error());
    }

    template<class T>
    void
    bad(string_view s)
    {
        T t;
        error_code ec;
        if(! BOOST_TEST(
            ! grammar::parse_string(s,
                ec, t)))
            BOOST_TEST(! ec.failed());
        else
            BOOST_TEST(ec.failed());
    }

    /* ASCII HEX

        %   25
        .   2e
        /   2f
        :   3a
    */
    void
    test_path_abempty_rule()
    {
        // path-abempty  = *( "/" segment )
        using T = path_abempty_rule;

        ok("", T{});
        ok("/", T{});
        ok("//", T{});
        ok("/x", T{});
        ok("/:", T{});
        ok("/x/", T{});
        ok("/%3a/", T{});
        ok("/%20", T{});
        ok("/%20", T{});
        ok("/%25", T{});
        ok("/%25%2e", T{});

        bad_(".", T{});
        bad_(":", T{});
        bad_("x", T{});
        bad_("%20", T{});
        bad_("%2f", T{});
        bad_("a/", T{});
        bad_(" ", T{});
    }

    void
    test_path_absolute_rule()
    {
        // path-absolute = "/" [ segment-nz *( "/" segment ) ]
        using T = path_absolute_rule;

        ok("/", T{});
        ok("/x", T{});
        ok("/x/", T{});
        ok("/:/", T{});
        ok("/x//", T{});
        ok("/%20", T{});
        ok("/:%20", T{});
        ok("/%20", T{});
        ok("/%25", T{});
        ok("/%25%2e", T{});

        bad_("", T{});
        bad_("//", T{});
        bad_(".", T{});
        bad_(":", T{});
        bad_("x", T{});
        bad_("%20", T{});
        bad_("%2f", T{});
        bad_("a/", T{});
        bad_(" ", T{});
    }

    void
    test_path_noscheme_rule()
    {
        // path-noscheme = segment-nz-nc *( "/" segment )
        using T = path_noscheme_rule;

        ok(".", T{});
        ok("x", T{});
        ok("%20", T{});
        ok("%2f", T{});
        ok("a/", T{});
        ok("a//", T{});
        ok("a/x", T{});
        ok("a/x/", T{});
        ok("a/x//", T{});
        ok("a///", T{});

        bad_("", T{});
        bad_(" ", T{});
        bad_(":", T{});
        bad_("/", T{});
        bad_("/x", T{});
        bad_("//", T{});
        bad_("/x/", T{});
        bad_("/:/", T{});
        bad_("/x//", T{});
        bad_("/%20", T{});
        bad_("/:%20", T{});
        bad_("/%20", T{});
        bad_("/%25", T{});
        bad_("/%25%2e", T{});
    }

    void
    test_path_rootless_rule()
    {
        // path-rootless = segment-nz *( "/" segment )
        using T = path_rootless_rule;

        ok(".", T{});
        ok(":", T{});
        ok(":/", T{});
        ok("::/", T{});
        ok("://", T{});
        ok(":/:/", T{});
        ok("x", T{});
        ok("%20", T{});
        ok("%2f", T{});
        ok("a/", T{});
        ok("a//", T{});
        ok("a/x", T{});
        ok("a/x/", T{});
        ok("a/x//", T{});
        ok("a///", T{});

        bad_("", T{});
        bad_(" ", T{});
        bad_("/", T{});
        bad_("/x", T{});
        bad_("//", T{});
        bad_("/x/", T{});
        bad_("/:/", T{});
        bad_("/x//", T{});
        bad_("/%20", T{});
        bad_("/:%20", T{});
        bad_("/%20", T{});
        bad_("/%25", T{});
        bad_("/%25%2e", T{});
    }

    void
    run()
    {
        test_path_abempty_rule();
        test_path_absolute_rule();
        test_path_noscheme_rule();
        test_path_rootless_rule();
    }
};

TEST_SUITE(
    paths_rule_test,
    "boost.url.paths_rule");

} // urls
} // boost
