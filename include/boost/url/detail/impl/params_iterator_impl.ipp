//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
// Copyright (c) 2022 Alan de Freitas (alandefreitas@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

#ifndef BOOST_URL_DETAIL_IMPL_PARAMS_ITERATOR_IMPL_IPP
#define BOOST_URL_DETAIL_IMPL_PARAMS_ITERATOR_IMPL_IPP

#include <boost/url/detail/params_iterator_impl.hpp>
#include <boost/assert.hpp>

namespace boost {
namespace urls {
namespace detail {

void
params_iterator_impl::
scan() noexcept
{
    string_view s(pos_, end_ - pos_);
    std::size_t i;
    if(pos_ != begin_ || i_ != 0)
    {
        BOOST_ASSERT(
            s.starts_with('&'));
        i = s.find_first_of('&', 1);
    }
    else
    {
        i = s.find_first_of('&');
    }
    if( i == string_view::npos)
        i = s.size();
    nk_ = string_view(
        pos_, i).find_first_of('=');
    if(nk_ != string_view::npos)
    {
        nv_ = i - nk_;
    }
    else
    {
        // has_value==false
        nk_ = i;
        nv_ = 0;
    }
}

params_iterator_impl::
params_iterator_impl(
    string_view s,
    const_string::factory a) noexcept
    : begin_(s.data())
    , pos_(s.data())
    , end_(s.data() + s.size())
    , a_(a)
{
    scan();
}

params_iterator_impl::
params_iterator_impl(
    string_view s,
    std::size_t nparam,
    int,
    const_string::factory a) noexcept
    : i_(nparam)
    , begin_(s.data())
    , pos_(s.data() + s.size())
    , end_(s.data() + s.size())
    , a_(a)
{
}



params_view::reference
params_iterator_impl::
dereference() const noexcept
{
    // valid domain
    BOOST_ASSERT(begin_ != nullptr);
    BOOST_ASSERT(end_ != nullptr);

    // cannot dereference one past end
    BOOST_ASSERT(pos_ != end_);

    // cannot dereference one before begin
    BOOST_ASSERT(pos_ != nullptr);

    using reference = params_view::reference;
    int prefix = pos_ != begin_ || i_ != 0;
    if(nv_ > 0)
        return reference{
            pos_ + prefix,
            nk_ - prefix,
            nv_,
            a_};
    return reference{
        pos_ + prefix,
        nk_ - prefix,
        nv_,
        a_};
}


void
params_iterator_impl::
increment() noexcept
{
    // valid domain
    BOOST_ASSERT(begin_ != nullptr);
    BOOST_ASSERT(end_ != nullptr);

    // cannot increment one past end
    BOOST_ASSERT(pos_ != end_);

    ++i_;
    if (pos_ != nullptr)
    {
        pos_ += nk_ + nv_;
        if(pos_ == end_)
        {
            nk_ = 0;
            nv_ = 0;
            return;
        }
        scan();
        return;
    }
    else
    {
        pos_ = begin_;
    }
    scan();
}

void
params_iterator_impl::
decrement() noexcept
{
    // valid domain
    BOOST_ASSERT(begin_ != nullptr);
    BOOST_ASSERT(end_ != nullptr);

    // cannot decrement one before begin
    BOOST_ASSERT(pos_ != nullptr);

    --i_;
    if (begin_ != pos_)
    {
        string_view s(begin_, pos_ - begin_);
        std::size_t i = s.find_last_of('&');
        pos_ = begin_;
        if (i != string_view::npos)
            pos_ = begin_ + i;
        scan();
    }
    else
    {
        // one before begin
        pos_ = nullptr;
    }
}

string_view
params_iterator_impl::
encoded_key() const noexcept
{
    // valid domain
    BOOST_ASSERT(begin_ != nullptr);
    BOOST_ASSERT(end_ != nullptr);

    // cannot dereference one past end
    BOOST_ASSERT(pos_ != end_);

    // cannot dereference one before begin
    BOOST_ASSERT(pos_ != nullptr);

    if(pos_ != begin_)
        return string_view(
            pos_ + 1, nk_ - 1);
    return string_view{ pos_, nk_ };
}


bool
params_iterator_impl::
equal(params_iterator_impl const& other) const noexcept
{
    BOOST_ASSERT(end_ == other.end_);
    return
        pos_ == other.pos_ &&
        i_ == other.i_;
}


} // detail
} // url
} // boost

#endif
