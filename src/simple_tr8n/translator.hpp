// SPDX-FileCopyrightText: 2022 Eric Barndollar
//
// SPDX-License-Identifier: Apache-2.0

#ifndef SIMPLE_TR8N_TRANSLATOR_HPP
#define SIMPLE_TR8N_TRANSLATOR_HPP

#include <algorithm>
#include <initializer_list>
#include <string>
#include <utility>
#include <vector>

#include <gsl/gsl>

#include "simple_tr8n/string_view.hpp"

namespace simple_tr8n {

/**
 * Represents a set of named arguments to be substituted into a user-visible
 * string. All added string values must have lifetimes longer than this
 * TransArgs object.
 */
template<typename CharT>
class TransArgs {
public:
  using keyval_type = std::pair<basic_string_view<CharT>, basic_string_view<CharT>>;

  TransArgs() = default;
  TransArgs(std::initializer_list<keyval_type> args) : args_{std::move(args)} {}

  /** Returns true if an argument with the given key has been provided. */
  bool has(basic_string_view<CharT> key) const {
    const auto itr = std::find_if(
        args_.begin(), args_.end(), [&](const keyval_type& keyval) { return keyval.first == key; });
    return itr != args_.end();
  }

  /** Returns value of argument with given key, or emptry string if none was set. */
  basic_string_view<CharT> get(basic_string_view<CharT> key) const {
    auto itr = std::find_if(
        args_.begin(), args_.end(), [&](const keyval_type& keyval) { return keyval.first == key; });
    return (itr != args_.end()) ? itr->second : basic_string_view<CharT>{};
  }

  /** Adds an argument with the given key and value. */
  TransArgs& add(basic_string_view<CharT> key, basic_string_view<CharT> value) {
    Expects(!has(key));  // No replace support.
    args_.emplace_back(key, value);
    return *this;
  }

private:
  // Most argument lists should be short, so just use a vector with linear search.
  std::vector<keyval_type> args_;
};

/**
 * Interface that can translate user-visible strings, with optional argument
 * interpolation and plurals selection.
 */
template<typename CharT>
class Translator {
public:
  using string_type = std::basic_string<CharT>;

  Translator() = default;
  virtual ~Translator() noexcept = default;

  Translator(const Translator&) = delete;
  Translator& operator=(const Translator&) = delete;

  Translator(Translator&&) = delete;
  Translator& operator=(Translator&&) = delete;

  /**
   * Translates the given non-plural message type, with no argument
   * interpolation.
   *
   * Error handling behavior depends on the implementation. For
   * SimpleTranslator, it will throw an exception if this message type is not
   * found or if any required arguments weren't provided (unless the library is
   * compiled with SIMPLE_TR8N_ENABLE_EXCEPTIONS off, in which case it will
   * return an empty string).
   */
  virtual string_type translate(basic_string_view<CharT> msgType) const = 0;

  /**
   * Translates the given non-plural message type, including argument
   * interpolation.
   *
   * Error handling behavior depends on the implementation. For
   * SimpleTranslator, it will throw an exception if this message type is not
   * found or if any required arguments weren't provided (unless the library is
   * compiled with SIMPLE_TR8N_ENABLE_EXCEPTIONS off, in which case it will
   * return an empty string).
   *
   * Unused arguments are okay and should not result in an error.
   */
  virtual string_type translate(
      basic_string_view<CharT> msgType, const TransArgs<CharT>& args) const = 0;

  /**
   * Translates the given plural message type, including argument interpolation.
   * Given pluralCount must be non-negative.
   *
   * Error handling behavior depends on the implementation. For
   * SimpleTranslator, it will throw an exception if this message type is not
   * found or if any required arguments weren't provided (unless the library is
   * compiled with SIMPLE_TR8N_ENABLE_EXCEPTIONS off, in which case it will
   * return an empty string).
   *
   * Unused arguments are okay and should not result in an error.
   */
  virtual string_type translatePlural(
      basic_string_view<CharT> msgType, int pluralCount, const TransArgs<CharT>& args) const = 0;
};

}  // namespace simple_tr8n

#endif  // SIMPLE_TR8N_TRANSLATOR_HPP
