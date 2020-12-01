#ifndef SIMPLE_TR8N_EXCEPTIONS_HPP
#define SIMPLE_TR8N_EXCEPTIONS_HPP

#ifndef SIMPLE_TR8N_ENABLE_EXCEPTIONS
#error "should not include simple_tr8n/exceptions.hpp without SIMPLE_TR8N_ENABLE_EXCEPTIONS"
#endif  // SIMPLE_TR8N_ENABLE_EXCEPTIONS

#include <exception>
#include <string>

#include "simple_tr8n/string_view.hpp"

namespace simple_tr8n {

/** Exception thrown if a requested message type was not configured. */
template<typename CharT>
struct MissingMsgTypeException : public std::exception {
public:
  MissingMsgTypeException(string_view<CharT> msgType);
  ~MissingMsgTypeException() override = default;
  const char* what() const noexcept override { return what_.c_str(); }

private:
  std::string what_;
};

// Default: just include exception name in what() explanation.
template<typename CharT>
MissingMsgTypeException<CharT>::MissingMsgTypeException(string_view<CharT>)
    : what_("simple_tr8n::MissingMsgTypeException") {}

// TODO: Specialize for other character types if needed.
template<>
MissingMsgTypeException<char>::MissingMsgTypeException(string_view<char> msgType)
    : what_("simple_tr8n::MissingMsgTypeException: ") {
  what_ += msgType;
}

/** Exception thrown if a required message argument was not provided. */
template<typename CharT>
struct MissingArgException : public std::exception {
public:
  MissingArgException(string_view<CharT> msgType, string_view<CharT> argKey);
  ~MissingArgException() override = default;
  const char* what() const noexcept override { return what_.c_str(); }

private:
  std::string what_;
};

// Default: just include exception name in what() explanation.
template<typename CharT>
MissingArgException<CharT>::MissingArgException(string_view<CharT>, string_view<CharT>)
    : what_("simple_tr8n::MissingArgException") {}

// TODO: Specialize for other character types if needed.
template<>
MissingArgException<char>::MissingArgException(string_view<char> msgType, string_view<char> argKey)
    : what_("simple_tr8n::MissingArgException: (msgType) ") {
  what_ += msgType;
  what_ += ": (argKey) ";
  what_ += argKey;
}

/**
 * Exception thrown if arguments given for a particular message type are invalid
 * (e.g. specified plural count when configured message has no plural cases).
 */
template<typename CharT>
struct InvalidArgsException : public std::exception {
public:
  InvalidArgsException(string_view<CharT> msgType);
  ~InvalidArgsException() override = default;
  const char* what() const noexcept override { return what_.c_str(); }

private:
  std::string what_;
};

// Default: just include exception name in what() explanation.
template<typename CharT>
InvalidArgsException<CharT>::InvalidArgsException(string_view<CharT>)
    : what_("simple_tr8n::InvalidArgsException") {}

// TODO: Specialize for other character types if needed.
template<>
InvalidArgsException<char>::InvalidArgsException(string_view<char> msgType, string_view<char> argKey)
    : what_("simple_tr8n::InvalidArgsException: ") {
  what_ += msgType;
}

}  // namespace simple_tr8n

#endif  // SIMPLE_TR8N_EXCEPTIONS_HPP
