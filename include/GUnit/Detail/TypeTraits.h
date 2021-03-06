//
// Copyright (c) 2016-2017 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include <memory>
#include <tuple>
#include <type_traits>

#define GUNIT_REQUIRES(...) typename std::enable_if<__VA_ARGS__, int>::type = 0

namespace testing {
inline namespace v1 {
namespace detail {

using byte = unsigned char;

template <class...>
using void_t = void;

template <class T>
struct identity {
  using type = T;
};

struct none_t {};

template <class T>
using identity_t = typename identity<T>::type;

template <class T, class = decltype(sizeof(T))>
std::true_type is_complete_impl(int);

template <class T>
std::false_type is_complete_impl(...);

template <class T>
using is_complete = decltype(is_complete_impl<T>(0));

template <class>
struct is_shared_ptr_impl : std::false_type {};

template <class T>
struct is_shared_ptr_impl<std::shared_ptr<T>> : std::true_type {};

template <class T>
using is_shared_ptr = typename is_shared_ptr_impl<std::remove_cv_t<T>>::type;

template <class T, class U, class>
struct is_complete_base_of_impl : std::is_base_of<T, U> {};

template <class T, class U>
struct is_complete_base_of_impl<T, U, std::false_type> : std::false_type {};

template <class T, class U>
using is_complete_base_of = typename is_complete_base_of_impl<T, U, typename is_complete<U>::type>::type;

template <class, class = void>
struct is_callable : std::false_type {};

template <class T, class... TArgs>
struct is_callable<T(TArgs...), void_t<decltype(std::declval<T>()(std::declval<TArgs>()...))>> : std::true_type {};

template <bool...>
struct bool_list {};

template <class>
struct always : std::true_type {};

template <class...>
struct type {
  static void id() {}
};

template <class T>
auto type_id() {
  return reinterpret_cast<std::size_t>(&type<std::remove_cv_t<T>>::id);
}

template <class, class>
struct function_type;

template <class T, class R, class... TArgs>
struct function_type<T, R(TArgs...)> {
  using type = R (T::*)(TArgs...);
};

template <class T, class R, class... TArgs>
struct function_type<T, R(TArgs...) const> {
  using type = R (T::*)(TArgs...) const;
};

template <class T, class U>
using function_type_t = typename function_type<T, U>::type;

template <class, class>
struct contains;

template <class T, class... TArgs>
struct contains<T, std::tuple<TArgs...>>
    : std::integral_constant<bool,
                             !std::is_same<std::integer_sequence<bool, false, std::is_same<T, TArgs>::value...>,
                                           std::integer_sequence<bool, std::is_same<T, TArgs>::value..., false>>::value> {};

}  // detail
}  // v1
}  // testing
