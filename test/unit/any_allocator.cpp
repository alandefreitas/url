//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/CPPAlliance/url
//

// Test that header file is self-contained.
#include <boost/url/any_allocator.hpp>
#include <boost/core/allocator_access.hpp>
#include <boost/core/allocator_traits.hpp>
#include <boost/type_traits/make_void.hpp>
#include <boost/utility/result_of.hpp>
#include "test_suite.hpp"

namespace boost {
namespace urls {

template <typename T>
T fixed_arbitrary_value()
{
    return T{};
}

template <>
char fixed_arbitrary_value<char>()
        {
    return '_';
}

template <>
int fixed_arbitrary_value<int>()
        {
    return 42;
}

template <>
long fixed_arbitrary_value<long>()
        {
    return 42;
}

class any_allocator_test
{
public:
    template<class T, class = void>
    struct has_value_type : std::false_type {};

    template<class T>
    struct has_value_type<T, boost::void_t<typename T::value_type>> : std::true_type {};

    template<class T, class = void>
    struct has_pointer : std::false_type {};

    template<class T>
    struct has_pointer<T, boost::void_t<typename T::pointer>> : std::true_type {};

    template<class T, class = void>
    struct has_const_pointer : std::false_type {};

    template<class T>
    struct has_const_pointer<T, boost::void_t<typename T::const_pointer>> : std::true_type {};

    template<class T, class = void>
    struct has_void_pointer : std::false_type {};

    template<class T>
    struct has_void_pointer<T, boost::void_t<typename T::void_pointer>> : std::true_type {};

    template<class T, class = void>
    struct has_const_void_pointer : std::false_type {};

    template<class T>
    struct has_const_void_pointer<T, boost::void_t<typename T::const_void_pointer>> : std::true_type {};

    template<class T, class = void>
    struct has_size_type : std::false_type {};

    template<class T>
    struct has_size_type<T, boost::void_t<typename T::size_type>> : std::true_type {};

    template<class T, class = void>
    struct has_difference_type : std::false_type {};

    template<class T>
    struct has_difference_type<T, boost::void_t<typename T::difference_type>> : std::true_type {};

    template<class T, class = void>
    struct has_propagate_on_container_copy_assignment : std::false_type {};

    template<class T>
    struct has_propagate_on_container_copy_assignment<T, boost::void_t<typename T::propagate_on_container_copy_assignment>> : std::true_type {};

    template<class T, class = void>
    struct has_propagate_on_container_move_assignment : std::false_type {};

    template<class T>
    struct has_propagate_on_container_move_assignment<T, boost::void_t<typename T::propagate_on_container_move_assignment>> : std::true_type {};

    template<class T, class = void>
    struct has_propagate_on_container_swap : std::false_type {};

    template<class T>
    struct has_propagate_on_container_swap<T, boost::void_t<typename T::propagate_on_container_swap>> : std::true_type {};

    template<class T, class = void>
    struct has_is_always_equal : std::false_type {};

    template<class T>
    struct has_is_always_equal<T, boost::void_t<typename T::is_always_equal>> : std::true_type {};

    template<class T, class U, class = void>
    struct has_rebind : std::false_type {};

    template<class T, class U>
    struct has_rebind<T, U, boost::void_t<typename T::template rebind<U>>> : std::true_type {};

    template<class T, class = void>
    struct has_dereference_operator : std::false_type {};

    template<class T>
    struct has_dereference_operator<T, boost::void_t<decltype(*(boost::declval<T>()))>> : std::true_type {};

    template<class T, class = void>
    struct has_equal_operator : std::false_type {};

    template<class T>
    struct has_equal_operator<T, boost::void_t<decltype(boost::declval<T>() == boost::declval<T>())>> : std::true_type {};

    template <typename T>
    static
    typename std::enable_if<has_equal_operator<T>::value, bool>::type
    is_equal_if_equal_exists(const T& a, const T& b)
    {
        return a == b;
    }

    template <typename T>
    static
    typename std::enable_if<!has_equal_operator<T>::value, bool>::type
    is_equal_if_equal_exists(const T&, const T&)
    {
        return true;
    }

    template <typename T>
    static void testTypeRequirements()
    {
        BOOST_STATIC_ASSERT(has_pointer<boost::allocator_traits<any_allocator<T>>>::value);
        BOOST_STATIC_ASSERT(has_const_pointer<boost::allocator_traits<any_allocator<T>>>::value);
        BOOST_STATIC_ASSERT(has_void_pointer<boost::allocator_traits<any_allocator<T>>>::value);
        BOOST_STATIC_ASSERT(has_const_void_pointer<boost::allocator_traits<any_allocator<T>>>::value);
        BOOST_STATIC_ASSERT(has_value_type<any_allocator<T>>::value);
        BOOST_STATIC_ASSERT(has_size_type<boost::allocator_traits<any_allocator<T>>>::value);
        BOOST_STATIC_ASSERT(has_difference_type<boost::allocator_traits<any_allocator<T>>>::value);
        BOOST_STATIC_ASSERT(has_propagate_on_container_copy_assignment<boost::allocator_traits<any_allocator<T>>>::value);
        BOOST_STATIC_ASSERT(has_propagate_on_container_move_assignment<boost::allocator_traits<any_allocator<T>>>::value);
        BOOST_STATIC_ASSERT(has_propagate_on_container_swap<boost::allocator_traits<any_allocator<T>>>::value);
        BOOST_STATIC_ASSERT(has_is_always_equal<any_allocator<T>>::value);
        BOOST_STATIC_ASSERT(has_rebind<any_allocator<T>, int>::value);
        BOOST_STATIC_ASSERT(has_rebind<any_allocator<T>, unsigned long>::value);
        BOOST_STATIC_ASSERT(has_rebind<any_allocator<T>, long[40]>::value);
    }

    template <typename T>
    static void testPointerNamedRequirements()
    {
        using value_type = typename boost::allocator_traits<any_allocator<T>>::value_type;
        using pointer_type = typename boost::allocator_traits<any_allocator<T>>::pointer;
        using const_pointer_type = typename boost::allocator_traits<any_allocator<T>>::const_pointer;
        BOOST_STATIC_ASSERT(std::is_same<
                typename boost::result_of<
                    decltype(&any_allocator<T>::allocate)(any_allocator<T>*, std::size_t)
                >::type, pointer_type>::value);
        BOOST_STATIC_ASSERT(has_dereference_operator<pointer_type>::value);
        BOOST_STATIC_ASSERT(std::is_same<
                            decltype(*std::declval<pointer_type>()),
                            typename boost::allocator_traits<any_allocator<T>>::value_type&
                            >::value);
        BOOST_STATIC_ASSERT(std::is_same<
                            decltype(*std::declval<const_pointer_type>()),
                            const typename boost::allocator_traits<any_allocator<T>>::value_type&
                            >::value);
        BOOST_STATIC_ASSERT(std::is_same<
                            decltype(std::pointer_traits<pointer_type>::pointer_to(boost::declval<value_type&>())),
                            pointer_type
                            >::value);
        BOOST_STATIC_ASSERT(std::is_copy_constructible<any_allocator<T>>::value);
    }

    template <typename T>
    static void testRelationships()
    {
        BOOST_STATIC_ASSERT(std::is_same<
                              typename any_allocator<T>::is_always_equal,
                              std::false_type
                            >::value);
        std::allocator<T> base1;
        std::allocator<T> base2;
        any_allocator<T> a1(base1);
        any_allocator<T> a2(base2);
        BOOST_TEST(a1 == a2);
        BOOST_TEST_NOT(a1 != a2);

        // A a1(a): Copy-constructs a1 such that a1 == a
        any_allocator<T> a3(a1);
        BOOST_TEST(a3 == a1);

        // A a1 = a: Copy-constructs a1 such that a1 == a
        any_allocator<T> a4 = a1;
        BOOST_TEST(a4 == a1);

        // A a(b): Constructs a such that B(a)==b and A(b)==a
        std::allocator<int> base3;
        any_allocator<int> b1(base3);
        any_allocator<T> a5(b1);
        BOOST_TEST(any_allocator<int>(a5) == b1);
        BOOST_TEST(any_allocator<T>(b1) == a5);

        // A a1(std::move(a)): Constructs a1 such that it equals the prior value of a
        std::allocator<T> base4;
        any_allocator<T> a6(base4);
        any_allocator<T> prior_a6(a6);
        any_allocator<T> a7(std::move(a6));
        BOOST_TEST(a7 == prior_a6);

        // A a1 = std::move(a): Constructs a1 such that it equals the prior value of a
        std::allocator<T> base5;
        any_allocator<T> a8(base5);
        any_allocator<T> prior_a8(a8);
        any_allocator<T> a9 = std::move(a8);
        BOOST_TEST(a9 == prior_a8);

        // A a(std::move(b)): Constructs a such that it equals the prior value of A(b)
        std::allocator<int> base6;
        any_allocator<int> b2(base6);
        any_allocator<T> prior_ab(b2);
        any_allocator<T> a10(std::move(b2));
        BOOST_TEST(a10 == prior_ab);
    }

    template <typename T>
    static void testLifetimeOperations()
    {
        using value_type = typename boost::allocator_traits<any_allocator<T>>::value_type;
        using pointer_type = typename boost::allocator_traits<any_allocator<T>>::pointer;
        using size_type = typename boost::allocator_traits<any_allocator<T>>::size_type;

        // a.allocate(n) - A::pointer - Allocates storage suitable for an array
        // object of type T[n] and creates the array, but does not construct
        // array elements.
        BOOST_STATIC_ASSERT(std::is_same<
                            typename boost::result_of<
                                decltype(&any_allocator<T>::allocate)(any_allocator<T>*, std::size_t)
                                >::type, pointer_type>::value);
        std::allocator<T> base1;
        any_allocator<T> a(base1);
        T* p1 = a.allocate(1);
        *p1 = fixed_arbitrary_value<T>();

        T* p2 = a.allocate(3);
        p2[0] = fixed_arbitrary_value<T>();
        p2[1] = fixed_arbitrary_value<T>();
        p2[2] = fixed_arbitrary_value<T>();

        // a.allocate(n, cvp) - A::pointer - Same as a.allocate(n), but may use cvp
        // (nullptr or a pointer obtained from a.allocate()) in unspecified manner
        // to aid locality.
        T* p3 = std::allocator_traits<any_allocator<T>>::allocate(a, 1, p2);
        *p3 = fixed_arbitrary_value<T>();

        T* p4 = std::allocator_traits<any_allocator<T>>::allocate(a, 3, p2);
        p4[0] = fixed_arbitrary_value<T>();
        p4[1] = fixed_arbitrary_value<T>();
        p4[2] = fixed_arbitrary_value<T>();

        // Deallocates storage pointed to p, which must be a value returned by a
        // previous call to allocate or allocate_at_least (since C++23) that has
        // not been invalidated by an intervening call to deallocate.
        // n must match the value previously passed to allocate or be between
        // the request and returned number of elements via allocate_at_least
        // (may be equal to either bound) (since C++23).
        BOOST_STATIC_ASSERT(std::is_same<
                            typename boost::result_of<
                                decltype(&any_allocator<T>::deallocate)(any_allocator<T>*, std::size_t)
                                >::type, void>::value);
        a.deallocate(p4, 3);
        a.deallocate(p3, 1);
        a.deallocate(p2, 3);
        a.deallocate(p1, 1);

        // a.max_size() - A::size_type - The largest value that can be passed to A::allocate()
        BOOST_STATIC_ASSERT(std::is_same<
                            typename boost::result_of<
                                decltype(&static_cast<size_type (&)(const any_allocator<T>&)>(boost::allocator_traits<any_allocator<T>>::max_size))(any_allocator<T>&)
                                >::type, size_type>::value);
        BOOST_TEST((std::numeric_limits<size_type>::max)() / sizeof(value_type) == boost::allocator_traits<any_allocator<T>>::max_size(a));

        // a.construct(xp, args) (optional) - Constructs an object of type X in
        // previously-allocated storage at the address pointed to by xp, using
        // args as the constructor arguments.
        p1 = std::allocator_traits<any_allocator<T>>::allocate(a, 1);
        std::allocator_traits<any_allocator<T>>::construct(a, p1, fixed_arbitrary_value<T>());
        BOOST_TEST(is_equal_if_equal_exists(*p1, fixed_arbitrary_value<T>()));

        // Destructs an object of type X pointed to by xp, but does not deallocate
        // any storage.
        std::allocator_traits<any_allocator<T>>::destroy(a, p1);
        std::allocator_traits<any_allocator<T>>::deallocate(a, p1, 1);
    }

    template <typename T>
    static void testContainerOperations()
    {
        // Provides an instance of A to be used by the container that is copy-constructed from the one that uses `a`
        // currently. Usually returns either a copy of a or a default-constructed A.
        BOOST_STATIC_ASSERT(std::is_same<
                typename boost::result_of<
                        decltype(&static_cast<any_allocator<T> (&)(const any_allocator<T>&)>(std::allocator_traits<any_allocator<T>>::select_on_container_copy_construction))(any_allocator<T>&)
                >::type, any_allocator<T>>::value);
        std::allocator<T> b1;
        any_allocator<T> a1(b1);
        any_allocator<T> a2 = std::allocator_traits<any_allocator<T>>::select_on_container_copy_construction(a1);
        BOOST_TEST(a1 == a2);

        // std::true_type or derived from it if the allocator of type A needs to be copied when the container that uses
        // it is copy-assigned. Like, polymorphic_allocator, any_allocator does not propagate.
        BOOST_STATIC_ASSERT(std::is_same<
                typename std::allocator_traits<any_allocator<T>>::propagate_on_container_copy_assignment,
                std::false_type>::value);

        // std::true_type or derived from it if the allocator of type A needs to be moved when the container that uses
        // it is move-assigned.
        BOOST_STATIC_ASSERT(std::is_same<
                typename std::allocator_traits<any_allocator<T>>::propagate_on_container_move_assignment,
                std::false_type>::value);

        // std::true_type or derived from it if the allocators of type A need to be swapped when two containers that
        // use them are swapped.
        BOOST_STATIC_ASSERT(std::is_same<
                typename std::allocator_traits<any_allocator<T>>::propagate_on_container_swap,
                std::false_type>::value);
    }

    template <typename T>
    static void testAllocatorNamedRequirements()
    {
        testTypeRequirements<T>();
        testPointerNamedRequirements<T>();
        testRelationships<T>();
        testLifetimeOperations<T>();
        testContainerOperations<T>();
    };

    template <std::size_t N>
    static typename std::enable_if<alignof(max_align_t) <= N, void>::type
    testTryAlignedNamedRequirements()
    {
        testAllocatorNamedRequirements<max_align_t>();
    }

    template <std::size_t N>
    static typename std::enable_if<N < alignof(max_align_t), void>::type
    testTryAlignedNamedRequirements()
    {
        testAllocatorNamedRequirements<boost::type_with_alignment<2>::type>();
        testTryAlignedNamedRequirements<N * 2>();
    }

    static void testAlignedNamedRequirements()
    {
        testTryAlignedNamedRequirements<1>();
    }

    struct clumsy_struct
    {
        long a{1};
        std::size_t b{1};
        long c{1};
        int d{1};
        char e{'a'};
    };

    void
    run()
    {
        testAllocatorNamedRequirements<char>();
        testAllocatorNamedRequirements<int>();
        testAllocatorNamedRequirements<long>();
        testAllocatorNamedRequirements<max_align_t>();
        testAlignedNamedRequirements();
        testAllocatorNamedRequirements<clumsy_struct>();
    }
};

TEST_SUITE(
    any_allocator_test,
    "boost.url.any_allocator");

} // urls
} // boost
