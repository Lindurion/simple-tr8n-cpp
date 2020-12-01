#ifndef SIMPLE_TR8N_SIMPLE_TRANSLATOR_HPP
#define SIMPLE_TR8N_SIMPLE_TRANSLATOR_HPP

#include <initializer_list>
#include <memory>
#include <regex>
#include <string>
#include <unordered_map>
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

}  // namespace internal

/**
 * User-visible message value, optionally containing %{argKey} format argument
 * placeholders to be interpolated.
 */
template<typename CharT>
struct Message {
  std::basic_string<CharT> value;
};

/** User-visible message value configured for a particular plural count case. */
template<typename CharT>
class PluralCase {
public:
  // Note: Intentionally allowing implicit type conversion syntax.
  PluralCase(int count, Message<CharT> msg) : count(count), msg(msg) {
    Expects(count >= 0);
  }

  int count() const { return count_; }
  const Message<CharT>& msg() const { return msg_; }

private:
  /**
   * Minimum non-negative count for which this case will be selected. If a given
   * count matches no cases exactly, the nearest case with lower count value
   * will be selected.
   */
  int count_;

  /** User-visible message value. */
  Message<CharT> msg_;
};

/**
 * Configuration for a single particular message type, which can either be
 * specified as a single case Message or a list of PluralCase values.
 */
template<typename CharT>
class MsgConfig {
public:
  // Note: Intentionally allowing implicit type conversion syntax.
  /** Configures a message case without any plurals. */
  MsgConfig(string_view<CharT> msg) {
    cases_.emplace_back(kNoCount, msg);
  }

  // Note: Intentionally allowing implicit type conversion syntax.
  /**
   * Configures a message with (potentially multiple) plural cases. Input cases
   * must be in ascending count order.
   */
  MsgConfig(std::initializer_list<PluralCase> cases) : cases_{std::move(cases)} {
    Expects(cases_.size() >= 1);
  }

  ~MsgConfig() = default;

  MsgConfig(const MsgConfig&) = delete;
  MsgConfig& operator=(const MsgConfig&) = delete;

  MsgConfig(MsgConfig&&) = default;
  MsgConfig& operator=(MsgConfig&&) = default;

  /** Returns true if this message was configured with 1+ plural cases. */
  bool hasPluralCases() const {
    return (cases_.size() >= 2) || (cases_[0].count() != kNoCount);
  }

  /** Returns message value for the only case configured. */
  const Message<CharT>& onlyCase() const {
    Expects(!hasPluralCases());
    return cases_[0].msg();
  }

  /** Returns best matching message case value for the given plural count. */
  const Message<CharT>& pluralCase(int count) const {
    Expects(count >= 0);
    Expects(hasPluralCases());

    int bestMatchIndex = 0;
    for (int i = 0; i < cases_.size(); ++i) {
      if (count < cases_[i].count()) {
        break;  // Too high, so stop searching.
      }
      bestMatchIndex = i;
    }

    return cases_[bestMatchIndex].msg();
  }

private:
  static constexpr int kNoCount = -1;

  std::vector<PluralCase<CharT>> cases_;  // Invariant: cases_.size() >= 1.
};

/** Complete set of translated message configurations for a given locale. */
template<typename CharT>
class MsgConfigs {
public:
  using string_type = std::basic_string<CharT>;
  using entry_type = std::pair<string_type, MsgConfig>;

  // Note: Intentionally allowing implicit type conversion syntax.
  MsgConfigs(std::initializer_list<entry_type> configs)
      : configs_{std::move(configs)} {}

  ~MsgConfigs() = default;

  MsgConfigs(const MsgConfigs&) = delete;
  MsgConfigs& operator=(const MsgConfigs&) = delete;

  MsgConfigs(MsgConfigs&&) = delete;
  MsgConfigs& operator=(MsgConfigs&&) = delete;

  const MsgConfig<CharT>& get(string_view<CharT> msgType) const {
    const auto itr = configs_.find(msgType);

    if (itr == configs_.end()) {
      // This message type was not configured.
#ifdef SIMPLE_TR8N_ENABLE_EXCEPTIONS
      throw MissingMsgTypeException{msgType};
#else
      return emptyConfig_;
#endif
    }
  }

private:
  std::unordered_map<string_type, MsgConfig<CharT>> configs_;
  MsgConfig<CharT> emptyConfig_;
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

  string_type translate(string_view<CharT> msgType) const override {
    const auto& config = configs_->get(msgType);

    if (config.hasPluralCases()) {
      return invalidArgs(msgType);  // Mismatch: no args given, but args are required.
    }

    // TODO: Validate that there are no %{} arguments to substitute.
  }

  string_type translate(string_view<CharT> msgType, const TransArgs<CharT>& args) const override {
    const auto& config = configs_->get(msgType);
    // TODO: Argument substitution/plurals.
  }

private:
  static string_type invalidArgs(string_view<CharT> msgType) {
#ifdef SIMPLE_TR8N_ENABLE_EXCEPTIONS
    throw InvalidArgsException{msgType};
#else
    return {};
#endif
  }

  std::unique_ptr<MsgConfigs<CharT>> configs_;
  std::basic_regex<CharT> argPattern_ = internal::argPattern<CharT>();
};

}  // namespace simple_tr8n

#endif  // SIMPLE_TR8N_SIMPLE_TRANSLATOR_HPP
