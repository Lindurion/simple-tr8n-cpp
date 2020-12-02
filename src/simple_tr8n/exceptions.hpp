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
  MissingMsgTypeException(basic_string_view<CharT> msgType);
  ~MissingMsgTypeException() override = default;
  const char* what() const noexcept override { return what_.c_str(); }

private:
  std::string what_;
};

// Default: just include exception name in what() explanation.
template<typename CharT>
MissingMsgTypeException<CharT>::MissingMsgTypeException(basic_string_view<CharT>)
    : what_("simple_tr8n::MissingMsgTypeException") {}

// TODO: Specialize for other character types if needed.
template<>
MissingMsgTypeException<char>::MissingMsgTypeException(basic_string_view<char> msgType)
    : what_("simple_tr8n::MissingMsgTypeException: ") {
  what_.append(std::basic_string<char>{msgType});
}

/** Exception thrown if a required message argument was not provided. */
template<typename CharT>
struct MissingArgException : public std::exception {
public:
  MissingArgException(basic_string_view<CharT> msgType, basic_string_view<CharT> argKey);
  ~MissingArgException() override = default;
  const char* what() const noexcept override { return what_.c_str(); }

private:
  std::string what_;
};

// Default: just include exception name in what() explanation.
template<typename CharT>
MissingArgException<CharT>::MissingArgException(basic_string_view<CharT>, basic_string_view<CharT>)
    : what_("simple_tr8n::MissingArgException") {}

// TODO: Specialize for other character types if needed.
template<>
MissingArgException<char>::MissingArgException(basic_string_view<char> msgType, basic_string_view<char> argKey)
    : what_("simple_tr8n::MissingArgException: (msgType) ") {
  what_.append(std::basic_string<char>{msgType});
  what_.append(": (argKey) ");
  what_.append(std::basic_string<char>{argKey});
}

/**
 * Exception thrown if arguments given for a particular message type are invalid
 * (e.g. specified plural count when configured message has no plural cases).
 */
template<typename CharT>
struct InvalidArgsException : public std::exception {
public:
  InvalidArgsException(basic_string_view<CharT> msgType);
  ~InvalidArgsException() override = default;
  const char* what() const noexcept override { return what_.c_str(); }

private:
  std::string what_;
};

// Default: just include exception name in what() explanation.
template<typename CharT>
InvalidArgsException<CharT>::InvalidArgsException(basic_string_view<CharT>)
    : what_("simple_tr8n::InvalidArgsException") {}

// TODO: Specialize for other character types if needed.
template<>
InvalidArgsException<char>::InvalidArgsException(basic_string_view<char> msgType)
    : what_("simple_tr8n::InvalidArgsException: ") {
  what_.append(std::basic_string<char>{msgType});
}

}  // namespace simple_tr8n

#endif  // SIMPLE_TR8N_EXCEPTIONS_HPP
