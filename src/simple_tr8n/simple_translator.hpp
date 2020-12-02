#ifndef SIMPLE_TR8N_SIMPLE_TRANSLATOR_HPP
#define SIMPLE_TR8N_SIMPLE_TRANSLATOR_HPP

#include <initializer_list>
#include <functional>
#include <map>
#include <memory>
#include <regex>
#include <string>
#include <utility>
#include <vector>

#include <gsl/gsl>

#include "simple_tr8n/string_view.hpp"
#include "simple_tr8n/translator.hpp"

#ifdef SIMPLE_TR8N_ENABLE_EXCEPTIONS
#include "simple_tr8n/exceptions.hpp"
#endif

namespace simple_tr8n {
namespace internal {

/** Count value used to represent a non-plural case. */
constexpr int kNoCount = -1;

template<typename CharT>
std::basic_regex<CharT> argPattern();

template<>
std::basic_regex<char> argPattern() {
  return std::basic_regex<char>("%\\{(.*?)\\}");
}

template<>
std::basic_regex<wchar_t> argPattern() {
  return std::basic_regex<wchar_t>(L"%\\{(.*?)\\}");
}

// TODO: Provide specializations for additional char types (int16_t, int32_t),
// if needed.

template<typename CharT>
const std::basic_string<CharT>& emptyStr() {
  static std::basic_string<CharT> empty{};
  return empty;
}

}  // namespace internal

/** User-visible message value configured for a particular plural count case. */
template<typename CharT>
class PluralCase {
public:
  // Note: Intentionally allowing implicit type conversion syntax.
  /**
   * Configures plural case with the given minimum count. The highest matching
   * (<= actual count) case will be selected.
   */
  PluralCase(int count, std::basic_string<CharT> msg) : count_{count}, msg_{std::move(msg)} {}

  /** Minimum count for which this case will be selected. */
  int count() const { return count_; }

  /**
   * User-visible message, possibly containing %{argKey} tokens for
   * interpolation.
   */
  const std::basic_string<CharT>& msg() const { return msg_; }

private:
  int count_;
  std::basic_string<CharT> msg_;
};

/**
 * Configuration for a single particular message type, which can either be
 * specified as a single case message or a list of PluralCase values.
 */
template<typename CharT>
class MsgConfig {
public:
  // Note: Intentionally allowing implicit type conversion syntax.
  /** Configures a message case without any plurals. */
  MsgConfig(basic_string_view<CharT> msg) {
    cases_.emplace_back(internal::kNoCount, std::basic_string<CharT>{msg});
  }

  // Note: Intentionally allowing implicit type conversion syntax.
  /**
   * Configures a message with (potentially multiple) plural cases. Input cases
   * must be in ascending count order.
   */
  MsgConfig(std::initializer_list<PluralCase<CharT>> cases) : cases_{std::move(cases)} {
    Expects(cases_.size() >= 1);
  }

  ~MsgConfig() = default;

  MsgConfig(const MsgConfig&) = delete;
  MsgConfig& operator=(const MsgConfig&) = delete;

  MsgConfig(MsgConfig&&) = default;
  MsgConfig& operator=(MsgConfig&&) = default;

  /** Returns true if this message was configured with 1+ plural cases. */
  bool hasPluralCases() const {
    return (cases_.size() >= 2) || (cases_[0].count() != internal::kNoCount);
  }

  /** Returns message value for the only case configured. */
  const std::basic_string<CharT>& onlyCase() const {
    Expects(!hasPluralCases());
    return cases_[0].msg();
  }

  /** Returns best matching message case value for the given plural count. */
  const std::basic_string<CharT>& pluralCase(basic_string_view<CharT> msgType, int count) const {
    Expects(count >= 0);
    Expects(hasPluralCases());

    for (int i = gsl::narrow_cast<int>(cases_.size()) - 1; i >= 0; --i) {
      if (cases_[i].count() <= count) {
        return cases_[i].msg();
      }
    }

    // No configured plural case.
#ifdef SIMPLE_TR8N_ENABLE_EXCEPTIONS
    throw InvalidArgsException<CharT>{msgType};
#else
    msgType;  // Suppress unreferenced parameter warning.
    return internal::emptyStr<CharT>();
#endif
  }

private:
  std::vector<PluralCase<CharT>> cases_;  // Invariant: cases_.size() >= 1.
};

/** Complete set of translated message configurations for a given locale. */
template<typename CharT>
class MsgConfigs {
public:
  using string_type = std::basic_string<CharT>;

  MsgConfigs() = default;
  ~MsgConfigs() = default;

  MsgConfigs(const MsgConfigs&) = delete;
  MsgConfigs& operator=(const MsgConfigs&) = delete;

  MsgConfigs(MsgConfigs&&) = delete;
  MsgConfigs& operator=(MsgConfigs&&) = delete;

  /** Adds message with just a single non-plural case. */
  MsgConfigs& add(basic_string_view<CharT> msgType, basic_string_view<CharT> msg) {
    configs_.emplace(msgType, MsgConfig<CharT>{msg});
    return *this;
  }

  /** Adds message with (potentially) multiple plural cases. */
  MsgConfigs& add(
      basic_string_view<CharT> msgType, std::initializer_list<PluralCase<CharT>> cases) {
    configs_.emplace(msgType, MsgConfig<CharT>{std::move(cases)});
    return *this;
  }

  /** Accesses the configuration for the given message type. */
  const MsgConfig<CharT>& get(basic_string_view<CharT> msgType) const {
    const auto itr = configs_.find(msgType);

    if (itr == configs_.end()) {
      // This message type was not configured.
#ifdef SIMPLE_TR8N_ENABLE_EXCEPTIONS
      throw MissingMsgTypeException<CharT>{msgType};
#else
      return emptyConfig_;
#endif
    }

    return itr->second;
  }

private:
  // Note: Using transparent comparator std::less<> to support heterogeneous
  // lookup by string_view without key type conversion.
  std::map<string_type, MsgConfig<CharT>, std::less<>> configs_;
  MsgConfig<CharT> emptyConfig_ {string_type{}};
};

/**
 * A very simple Translator implementation that is configured at construction
 * time by passing all translations for the desired locale in a single
 * configuration object.
 */
template<typename CharT>
class SimpleTranslator : public Translator<CharT> {
public:
  SimpleTranslator(std::unique_ptr<MsgConfigs<CharT>> configs)
      : configs_{std::move(configs)} {}

  ~SimpleTranslator() override = default;

  SimpleTranslator(const SimpleTranslator&) = delete;
  SimpleTranslator& operator=(const SimpleTranslator&) = delete;

  SimpleTranslator(SimpleTranslator&&) = delete;
  SimpleTranslator& operator=(SimpleTranslator&&) = delete;

  string_type translate(basic_string_view<CharT> msgType) const override {
    const auto& config = configs_->get(msgType);

    if (config.hasPluralCases()) {
      return invalidArgs(msgType);  // Mismatch: must use translatePlural().
    }

    const auto& msg = config.onlyCase();

    match_type match;
    if (std::regex_search(msg, match, argPattern_)) {
      const std::basic_string<CharT> argKey = match[1];
      return missingArg(msgType, argKey);
    }

    return msg;
  }

  string_type translate(
      basic_string_view<CharT> msgType, const TransArgs<CharT>& args) const override {
    const auto& config = configs_->get(msgType);

    if (config.hasPluralCases()) {
      return invalidArgs(msgType);  // Mismatch: must use translatePlural().
    }

    return substituteArgs(msgType, config.onlyCase(), args);
  }

  string_type translatePlural(
      basic_string_view<CharT> msgType, int pluralCount,
      const TransArgs<CharT>& args) const override {
    Expects(pluralCount >= 0);
    const auto& config = configs_->get(msgType);

    if (!config.hasPluralCases()) {
      return invalidArgs(msgType);  // Mismatch: must use translate().
    }

    return substituteArgs(msgType, config.pluralCase(msgType, pluralCount), args);
  }

private:
  using match_type = std::match_results<typename std::basic_string<CharT>::const_iterator>;

  static string_type invalidArgs(basic_string_view<CharT> msgType) {
#ifdef SIMPLE_TR8N_ENABLE_EXCEPTIONS
    throw InvalidArgsException<CharT>{msgType};
#else
    msgType;  // Suppress unreferenced parameter warning.
    return {};
#endif
  }

  static string_type missingArg(basic_string_view<CharT> msgType, basic_string_view<CharT> argKey) {
#ifdef SIMPLE_TR8N_ENABLE_EXCEPTIONS
    throw MissingArgException<CharT>{msgType, argKey};
#else
    msgType;  // Suppress unreferenced parameter warning.
    argKey;  // Suppress unreferenced parameter warning.
    return {};
#endif
  }

  string_type substituteArgs(
      basic_string_view<CharT> msgType, const std::basic_string<CharT>& msg,
      const TransArgs<CharT>& args) const {
    // TODO: If needed, could improve efficiency here by reserving capacity
    // necessary to fit msg and argument values (TransArgs could track sum of lengths).
    string_type result;

    auto start = msg.begin();
    const auto end = msg.end();

    match_type match;
    while (std::regex_search(start, end, match, argPattern_)) {
      const std::basic_string<CharT> argKey = match[1];

      if (!args.has(argKey)) {
        return missingArg(msgType, argKey);  
      }

      result.append(match.prefix());
      result.append(std::basic_string<CharT>{args.get(argKey)});

      start += match.position() + match.length();  // Advance past %{argKey} token.
    }

    // No more arguments matches. Append rest of message.
    result.append(start, end);
    return result;
  }

  std::unique_ptr<MsgConfigs<CharT>> configs_;
  const std::basic_regex<CharT> argPattern_ = internal::argPattern<CharT>();
};

}  // namespace simple_tr8n

#endif  // SIMPLE_TR8N_SIMPLE_TRANSLATOR_HPP
