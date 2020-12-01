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
 * string. Also supports designation of a plural count, which may affect the
 * selected message case.
 */
template<typename CharT>
class TransArgs {
public:
  using string_type = std::basic_string<CharT>;
  using keyval_type = std::pair<string_type, string_type>;

  explicit TransArgs() {}
  explicit TransArgs(std::initializer_list<keyval_type> args)
      : args_{std::move(args)} {}

  /** Returns true if an argument with the given key has been provided. */
  bool has(string_view<CharT> key) const {
    const auto itr = std::find_if(args_.begin(), args_.end(),
        [](const keyval_type& keyval) { return keyval.first() == key; });
    return itr != args_.end();
  }

  /** Returns value of argument with given key, or emptry string if none was set. */
  const string_type& get(string_view<CharT> key) const {
    auto itr = std::find_if(args_.begin(), args_.end(),
        [](const keyval_type& keyval) { return keyval.first() == key; });
    return (itr != args_.end()) ? *itr : {};
  }

  /** Returns true if a plural count has been provided. */
  bool hasCount() const {
    return count_ != kNoCount;
  }

  /** Adds an argument with the given key and value. */
  TransArgs& add(string_view<CharT> key, string_view<CharT> value) {
    args_.emplace_back(key, value);
    return *this;
  }

  /** Sets the plural count, which must be non-negative. */
  TransArgs& setCount(int count) {
    Expects(count_ >= 0);
    count_ = count;
  }

private:
  static constexpr int kNoCount = -1;  // Valid counts must be >= 0.

  // Most argument lists should be short, so just use a vector with linear search.
  std::vector<keyval_type> args_;
  int count_ = kNoCount;
};

/**
 * Interface that can translate user-visible strings, with optional argument
 * interpolation and plurals selection.
 */
template<typename CharT>
class Translator {
public:
  using string_type = std::basic_string<CharT>;

  virtual ~Translator() noexcept = default;

  Translator(const Translator&) = delete;
  Translator& operator=(const Translator&) = delete;

  Translator(Translator&&) = delete;
  Translator& operator=(Translator&&) = delete;

  /**
   * Translates the given message type, with no argument interpolation.
   *
   * Error handling behavior depends on the implementation. For
   * SimpleTranslator, it will throw an exception if this message type is not
   * found or if any required arguments weren't provided (unless the library is
   * compiled with SIMPLE_TR8N_ENABLE_EXCEPTIONS off, in which case it will
   * return an empty string).
   */
  virtual string_type translate(string_view<CharT> msgType) const = 0;

  /**
   * Translates the given message type, including argument interpolation and
   * possible plural case selection.
   *
   * Error handling behavior depends on the implementation. For
   * SimpleTranslator, it will throw an exception if this message type is not
   * found or if any required arguments weren't provided (unless the library is
   * compiled with SIMPLE_TR8N_ENABLE_EXCEPTIONS off, in which case it will
   * return an empty string).
   *
   * Unused arguments are okay and should not result in an error.
   */
  virtual string_type translate(string_view<CharT> msgType, const TransArgs<CharT>& args) const = 0;
};

}  // namespace simple_tr8n

#endif  // SIMPLE_TR8N_TRANSLATOR_HPP
