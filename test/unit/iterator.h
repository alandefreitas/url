//
// Created by alandefreitas on 11/26/21.
//

#ifndef BOOST_URL_ITERATOR_H
#define BOOST_URL_ITERATOR_H

#include <iterator>

namespace boost {
namespace url {

template <class Iter> class reverse_iterator {
public:
  using value_type = typename std::iterator_traits<Iter>::value_type;
  using difference_type = typename std::iterator_traits<Iter>::difference_type;
  using pointer = typename std::iterator_traits<Iter>::pointer;
  using reference = typename std::iterator_traits<Iter>::reference;
  using iterator_category = typename std::iterator_traits<Iter>::iterator_category;
  using iterator_type = Iter;

  BOOST_CXX14_CONSTEXPR reverse_iterator() : base_() {}

  BOOST_CXX14_CONSTEXPR explicit reverse_iterator(Iter x) : base_(x) {}

  template <class Up>
  // NOLINTNEXTLINE(google-explicit-constructor)
  BOOST_CXX14_CONSTEXPR reverse_iterator(const reverse_iterator<Up> &u)
      : base_(u.base()) {}

  template <class Up>
  BOOST_CXX14_CONSTEXPR reverse_iterator &
  operator=(const reverse_iterator<Up> &u) {
    base_ = u.base();
    return *this;
  }

  BOOST_CXX14_CONSTEXPR Iter base() const { return base_; }

  BOOST_CXX14_CONSTEXPR reference operator*() const {
    Iter tmp = base_;
    return *--tmp;
  }

  BOOST_CXX14_CONSTEXPR pointer operator->() const {
    return std::addressof(operator*());
  }

  BOOST_CXX14_CONSTEXPR reverse_iterator &operator++() {
    --base_;
    return *this;
  }

  // NOLINTNEXTLINE(cert-dcl21-cpp)
  BOOST_CXX14_CONSTEXPR reverse_iterator operator++(int) {
    reverse_iterator tmp(*this);
    --base_;
    return tmp;
  }

  BOOST_CXX14_CONSTEXPR reverse_iterator &operator--() {
    ++base_;
    return *this;
  }

  // NOLINTNEXTLINE(cert-dcl21-cpp)
  BOOST_CXX14_CONSTEXPR reverse_iterator operator--(int) {
    reverse_iterator tmp(*this);
    ++base_;
    return tmp;
  }

  BOOST_CXX14_CONSTEXPR reverse_iterator operator+(difference_type n) const {
    return reverse_iterator(base_ - n);
  }

  BOOST_CXX14_CONSTEXPR reverse_iterator &operator+=(difference_type n) {
    base_ -= n;
    return *this;
  }

  BOOST_CXX14_CONSTEXPR reverse_iterator operator-(difference_type n) const {
    return reverse_iterator(base_ + n);
  }

  BOOST_CXX14_CONSTEXPR reverse_iterator &operator-=(difference_type n) {
    base_ += n;
    return *this;
  }

  BOOST_CXX14_CONSTEXPR reference operator[](difference_type n) const {
    return *(*this + n);
  }

protected:
  Iter base_;
};

template <class Iter1, class Iter2>
inline BOOST_CXX14_CONSTEXPR bool
operator==(const reverse_iterator<Iter1> &x,
           const reverse_iterator<Iter2> &y) {
  return x.base() == y.base();
}

template <class Iter1, class Iter2>
inline BOOST_CXX14_CONSTEXPR bool
operator<(const reverse_iterator<Iter1> &x,
          const reverse_iterator<Iter2> &y) {
  return x.base() > y.base();
}

template <class Iter1, class Iter2>
inline BOOST_CXX14_CONSTEXPR bool
operator!=(const reverse_iterator<Iter1> &x,
           const reverse_iterator<Iter2> &y) {
  return x.base() != y.base();
}

template <class Iter1, class Iter2>
inline BOOST_CXX14_CONSTEXPR bool
operator>(const reverse_iterator<Iter1> &x,
          const reverse_iterator<Iter2> &y) {
  return x.base() < y.base();
}

template <class Iter1, class Iter2>
inline BOOST_CXX14_CONSTEXPR bool
operator>=(const reverse_iterator<Iter1> &x,
           const reverse_iterator<Iter2> &y) {
  return x.base() <= y.base();
}

template <class Iter1, class Iter2>
inline BOOST_CXX14_CONSTEXPR bool
operator<=(const reverse_iterator<Iter1> &x,
           const reverse_iterator<Iter2> &y) {
  return x.base() >= y.base();
}

template <class Iter1, class Iter2>
inline BOOST_CXX14_CONSTEXPR auto operator-(const reverse_iterator<Iter1> &x,
                                            const reverse_iterator<Iter2> &y)
    -> decltype(y.base() - x.base()) {
  return y.base() - x.base();
}

template <class Iter>
inline BOOST_CXX14_CONSTEXPR reverse_iterator<Iter>
operator+(typename reverse_iterator<Iter>::difference_type n,
          const reverse_iterator<Iter> &x) {
  return reverse_iterator<Iter>(x.base() - n);
}

} // namespace url
} // namespace boost

#endif // BOOST_URL_ITERATOR_H
