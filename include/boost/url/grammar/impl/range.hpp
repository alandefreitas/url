//
// Copyright (c) 2016-2019 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_GRAMMAR_IMPL_RANGE_HPP
#define BOOST_URL_GRAMMAR_IMPL_RANGE_HPP

#include <boost/url/detail/except.hpp>
#include <boost/url/grammar/error.hpp>
#include <boost/assert.hpp>
#include <iterator>
#include <new>

namespace boost {
namespace urls {
namespace grammar {

template<class T>
void
range_::
parse_impl(
    char const*& it,
    char const* end,
    error_code& ec,
    range_& t)
{
    typename T::value_type t0;
    auto start = it;
    std::size_t n = 0;
    T::begin(it, end, ec, t0);
    if(ec.failed())
    {
        if(ec == error::end)
        {
            ec = {};
            goto finish;
        }
        return;
    }
    for(;;)
    {
        ++n;
        T::increment(
            it, end, ec, t0);
        if(ec.failed())
        {
            if(ec == error::end)
            {
                ec = {};
                break;
            }
            return;
        }
    }
finish:
    t.str = string_view(
        start, it - start);
    t.count = n;
}

template<class T>
range_::
range_(T const*) noexcept
    : fp_(&range_::parse_impl<T>)
{
    // Type requirements not met!
    BOOST_STATIC_ASSERT(
        is_range<T>::value);
}

//------------------------------------------------

template<class R>
class range_base<R>::iterator
{
    friend class range_base<R>;

    char const* next_ = nullptr;
    char const* end_ = nullptr;
    typename R::reference v_;

    explicit
    iterator(string_view s)
        : next_(s.data())
        , end_(s.data() + s.size())
    {
        error_code ec;
        R::begin(next_, end_, ec, v_);
        BOOST_ASSERT(! ec.failed());
    }

    iterator(
        string_view s,
        int) noexcept
        : next_(nullptr)
        , end_(s.data() + s.size())
    {
    }

public:
    using value_type = typename R::value_type;
    using reference = typename R::reference;
    using pointer = void const*;
    using difference_type = std::ptrdiff_t;
    using iterator_category =
        std::forward_iterator_tag;

    iterator() = default;
    iterator(
        iterator const&) noexcept = default;
    iterator& operator=(
        iterator const&) noexcept = default;

    reference
    operator*() const noexcept
    {
        return v_;
    }

    bool
    operator==(
        iterator other) const noexcept
    {
        return
            next_ == other.next_ &&
            end_ == other.end_;
    }

    bool
    operator!=(
        iterator other) const noexcept
    {
        return
            next_ != other.next_ ||
            end_ != other.end_;
    }

    iterator&
    operator++() noexcept
    {
        BOOST_ASSERT(next_ != nullptr);
        error_code ec;
        R::increment(next_, end_, ec, v_);
        if(ec == error::end)
            next_ = nullptr;
        else
            BOOST_ASSERT(! ec.failed());
        return *this;
    }

    iterator
    operator++(int) noexcept
    {
        auto tmp = *this;
        ++*this;
        return tmp;
    }
};

//------------------------------------------------

template<class R>
auto
range_base<R>::
begin() const ->
    iterator
{
    return iterator(s_);
}

template<class R>
auto
range_base<R>::
end() const ->
    iterator
{
    return iterator(s_, 1);
}

template<class R>
void
range_base<R>::
parse(
    char const*& it,
    char const* const end,
    error_code& ec,
    std::size_t N,
    std::size_t M)
{
    std::size_t n = 0;
    auto const start = it;
    typename range<R>::reference v;

    R::begin(it, end, ec, v);
    if(ec.failed())
    {
        if(ec != error::end)
            return;
        if(n < N)
        {
            // too few
            ec = error::syntax;
            return;
        }

        // good
        ec = {};
        n_ = n;
        s_ = string_view(
            start, it - start);
        return;
    }

    for(;;)
    {
        ++n;
        R::increment(it, end, ec, v);
        if(ec.failed())
        {
            if(ec != error::end)
                return;
            ec = {};
            break;
        }
        if(n > M)
        {
            // too many
            ec = error::syntax;
            return;
        }
    }

    // good
    n_ = n;
    s_ = string_view(start, it - start);
}

//------------------------------------------------
//
// range
//
//------------------------------------------------

// base class for the type-erased rule
template<class T>
struct range__<T>::
    any_rule
{
    virtual ~any_rule() = default;
    virtual void copy(
        void*) const noexcept = 0;
    virtual result<T> begin(
        char const*&, char const*)
            const noexcept = 0;
    virtual result<T> increment(
        char const*&, char const*)
            const noexcept = 0;
};

//------------------------------------------------

template<class T>
class range__<T>::
    iterator
{
public:
    using value_type = T;
    using reference = T const&;
    using pointer = void const*;
    using difference_type =
        std::ptrdiff_t;
    using iterator_category =
        std::forward_iterator_tag;

    iterator() = default;
    iterator(
        iterator const&) noexcept = default;
    iterator& operator=(
        iterator const&) noexcept = default;

    reference
    operator*() const noexcept
    {
        return rv_.value();
    }

    bool
    operator==(
        iterator other) const noexcept
    {
        // can't compare iterators
        // from different containers!
        BOOST_ASSERT(r_ == other.r_);

        return p_ == other.p_;
    }

    bool
    operator!=(
        iterator other) const noexcept
    {
        return !(*this == other);
    }

    iterator&
    operator++() noexcept
    {
        BOOST_ASSERT(
            p_ != nullptr);
        auto const end =
            r_->s_.data() +
            r_->s_.size();
        auto const p =
            reinterpret_cast<
                any_rule const*>(
                    r_->buf_);
        r_->get().increment(p_, end);
        if(rv_.has_error())
        {
            BOOST_ASSERT(
                rv_.error() ==
                    error::end);
            p_ = nullptr;
        }
        return *this;
    }

    iterator
    operator++(int) noexcept
    {
        auto tmp = *this;
        ++*this;
        return tmp;
    }

private:
    friend class range<T>;

    range__<T> const* r_ = nullptr;
    char const* p_ = nullptr;
    result<T> rv_;

    iterator(
        range__<T> const& r) noexcept
        : r_(&r)
        , p_(r.s_.data())
    {
        auto const end =
            r_->s_.data() +
            r_->s_.size();
        auto const p =
            reinterpret_cast<
                any_rule const*>(
                    r_->buf_);
        rv_ = r_->get().begin_(p_, end);
        if(rv_.has_error())
        {
            BOOST_ASSERT(
                rv_.error() ==
                    error::end);
            p_ = nullptr;
        }
    }

    constexpr
    iterator(
        range__<T> const& r,
        int) noexcept
        : p_(nullptr)
        , r_(&r)
    {
    }
};

//------------------------------------------------

template<class T>
auto
range__<T>::
get() const noexcept ->
    any_rule const&
{
    return *reinterpret_cast<
        any_rule const*>(buf_);
}

template<class T>
range__<T>::
~range__()
{
    get().~any_rule();
}

template<class T>
range__<T>::
range__(
    range__ const& other) noexcept
    : s_(other.s_)
    , n_(other.n_)
{
    other.get().copy(buf_);
}

template<class T>
auto
range__<T>::
begin() const noexcept ->
    iterator
{
    return { *this };
}

template<class T>
auto
range__<T>::
end() const noexcept ->
    iterator
{
    return { *this, 0 };
}

template<class T>
template<class R>
range__<T>::
range__(
    string_view s,
    std::size_t n,
    R const& r,
    range_fn<R, T> begin,
    range_fn<R, T> increment) noexcept
    : s_(s)
    , n_(n)
{
    static_assert(
        is_rule<R>::value,
        "Rule requirements not met");

    static_assert(
        sizeof(R) <= BufferSize,
        "sizeof(R) is unsupported");

    struct impl : any_rule
    {
        R const r_;
        range_fn<R, T> begin_;
        range_fn<R, T> increment_;

        impl(
            R const& r,
            range_fn<R, T> begin,
            range_fn<R, T> increment) noexcept
            : r_(r)
            , begin_(begin)
            , increment_(increment)
        {
        }

        void
        copy(void* dest)
            const noexcept override
        {
            ::new(dest) impl(*this);
        }

        result<T>
        begin(
            char const*& it,
            char const* end)
                const noexcept override
        {
            return (r_.*begin_)(it, end);
        }

        result<T>
        increment(
            char const*& it,
            char const* end)
                const noexcept override
        {
            return (r_.*increment_)(it, end);
        }
    };

    ::new(buf_) impl(
        r, begin, increment);
}

template<
    class R,
    class T>
auto
parse_range(
    char const*& it,
    char const* end,
    R const& r,
    range_fn<typename std::remove_cv<R>::type, T> begin,
    range_fn<typename std::remove_cv<R>::type, T> increment,
    std::size_t N = 0,
    std::size_t M = std::size_t(-1)) ->
        result<range__<T>>
{
    std::size_t n = 0;
    auto const it0 = it;
    auto rv = (r.*begin)(it, end);
    if(rv.has_error())
    {
        if(rv.error() != error::end)
            return rv.error();
        if(n < N)
        {
            // too few
            return error::syntax;
        }

        // good
        return range__<T>(
            string_view(it0, it - it0),
                n, r, begin, increment);
    }

    for(;;)
    {
        ++n;
        rv = (r.*increment)(it, end);
        if(rv.has_error())
        {
            if(rv.error() != error::end)
                return rv.error();
            break;
        }
        if(n > M)
        {
            // too many
            return error::syntax;
        }
    }

    // good
    return range__<T>(
        string_view(it0, it - it0),
            n, r, begin, increment);
}

} // grammar
} // urls
} // boost

#endif
