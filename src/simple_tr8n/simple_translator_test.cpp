// SPDX-FileCopyrightText: 2022 Eric Barndollar
//
// SPDX-License-Identifier: Apache-2.0

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>

#include "simple_tr8n/simple_translator.hpp"
#include "simple_tr8n/translator.hpp"

#ifdef SIMPLE_TR8N_ENABLE_EXCEPTIONS
  #include "simple_tr8n/exceptions.hpp"
#endif

namespace test_msgs {

constexpr char kNoArgs[] = "test.no_args";
constexpr char kHelloName[] = "test.hello_name";
constexpr char kProgressPct[] = "test.progress_pct";
constexpr char kCoupleFishCount[] = "test.couple_fish_count";

}  // namespace test_msgs

namespace test_msgs2 {

constexpr char kAddlMsg[] = "test2.addl";

}  // namespace test_msgs2

using ::testing::Eq;
using ::testing::StrEq;
using ::testing::Test;

class SimpleTranslatorCharTest : public Test {
protected:
  void SetUp() override {
    auto enConfig = std::make_unique<simple_tr8n::MsgConfigs<char>>();
    enConfig->add(test_msgs::kNoArgs, "A simple message with no arguments")
        .add(test_msgs::kHelloName, "hello, %{personName}!")
        .add(test_msgs::kProgressPct, "progress: %{pct}%")
        .add(
            test_msgs::kCoupleFishCount,
            {
                {0, "%{person1Name} and %{person2Name}, you have no fish"},
                {1, "%{person1Name} and %{person2Name}, you have a fish"},
                {2, "%{person1Name} and %{person2Name}, you have two fish"},
                {3, "%{person1Name} and %{person2Name}, you have %{fishCount} fish"},
            })
        .add(test_msgs2::kAddlMsg, "An additional message with %{arg}");
    enTranslator = std::make_unique<simple_tr8n::SimpleTranslator<char>>(std::move(enConfig));

    auto esConfig = std::make_unique<simple_tr8n::MsgConfigs<char>>();
    esConfig->add(test_msgs::kNoArgs, "Un mensaje simple sin argumentos")
        .add(test_msgs::kHelloName, "hola, %{personName}!")
        .add(test_msgs::kProgressPct, "progreso: %{pct}%")
        .add(
            test_msgs::kCoupleFishCount,
            {
                // Not providing 0 case to test error handling.
                {1, "%{person1Name} y %{person2Name}, tienen un pez"},
                {2, "%{person1Name} y %{person2Name}, tienen %{fishCount} peces"},
                // Not providing separate cases for 2 and 3+.
            })
        .add(test_msgs2::kAddlMsg, "Un mensaje adicional con %{arg}");
    esTranslator = std::make_unique<simple_tr8n::SimpleTranslator<char>>(std::move(esConfig));
  }

  std::unique_ptr<simple_tr8n::SimpleTranslator<char>> enTranslator;
  std::unique_ptr<simple_tr8n::SimpleTranslator<char>> esTranslator;
};

TEST_F(SimpleTranslatorCharTest, ShouldTranslateWithNoArgs) {
  EXPECT_THAT(
      enTranslator->translate(test_msgs::kNoArgs), Eq("A simple message with no arguments"));
  EXPECT_THAT(esTranslator->translate(test_msgs::kNoArgs), Eq("Un mensaje simple sin argumentos"));
}

TEST_F(SimpleTranslatorCharTest, ShouldTranslateWithArgs) {
  EXPECT_THAT(
      enTranslator->translate(test_msgs::kHelloName, {{"personName", "Bob"}}), Eq("hello, Bob!"));
  EXPECT_THAT(
      esTranslator->translate(test_msgs::kHelloName, {{"personName", "Bob"}}), Eq("hola, Bob!"));

  EXPECT_THAT(
      enTranslator->translate(test_msgs::kProgressPct, {{"pct", "75"}}), Eq("progress: 75%"));
  EXPECT_THAT(
      esTranslator->translate(test_msgs::kProgressPct, {{"pct", "75"}}), Eq("progreso: 75%"));

  // Test that TransArgs can be built incrementally too.
  simple_tr8n::TransArgs<char> args;
  args.add("arg", "the argument");
  args.add("extraArg", "should be ignored with no problem");

  EXPECT_THAT(
      enTranslator->translate(test_msgs2::kAddlMsg, args),
      Eq("An additional message with the argument"));
  EXPECT_THAT(
      esTranslator->translate(test_msgs2::kAddlMsg, args),
      Eq("Un mensaje adicional con the argument"));
}

TEST_F(SimpleTranslatorCharTest, ShouldTranslatePlural) {
  EXPECT_THAT(
      enTranslator->translatePlural(
          test_msgs::kCoupleFishCount, 0,
          {
              {"person1Name", "Alice"},
              {"person2Name", "Bob"},
              {"fishCount", "0"},
          }),
      Eq("Alice and Bob, you have no fish"));
  EXPECT_THAT(
      enTranslator->translatePlural(
          test_msgs::kCoupleFishCount, 1,
          {
              {"person1Name", "Alice"},
              {"person2Name", "Bob"},
              {"fishCount", "1"},
          }),
      Eq("Alice and Bob, you have a fish"));
  EXPECT_THAT(
      enTranslator->translatePlural(
          test_msgs::kCoupleFishCount, 2,
          {
              {"person1Name", "Alice"},
              {"person2Name", "Bob"},
              {"fishCount", "2"},
          }),
      Eq("Alice and Bob, you have two fish"));
  EXPECT_THAT(
      enTranslator->translatePlural(
          test_msgs::kCoupleFishCount, 3,
          {
              {"person1Name", "Alice"},
              {"person2Name", "Bob"},
              {"fishCount", "3"},
          }),
      Eq("Alice and Bob, you have 3 fish"));
  EXPECT_THAT(
      enTranslator->translatePlural(
          test_msgs::kCoupleFishCount, 102,
          {
              {"person1Name", "Alice"},
              {"person2Name", "Bob"},
              {"fishCount", "102"},
          }),
      Eq("Alice and Bob, you have 102 fish"));

  EXPECT_THAT(
      esTranslator->translatePlural(
          test_msgs::kCoupleFishCount, 1,
          {
              {"person1Name", "Alice"},
              {"person2Name", "Bob"},
              {"fishCount", "1"},
          }),
      Eq("Alice y Bob, tienen un pez"));
  EXPECT_THAT(
      esTranslator->translatePlural(
          test_msgs::kCoupleFishCount, 2,
          {
              {"person1Name", "Alice"},
              {"person2Name", "Bob"},
              {"fishCount", "2"},
          }),
      Eq("Alice y Bob, tienen 2 peces"));
  EXPECT_THAT(
      esTranslator->translatePlural(
          test_msgs::kCoupleFishCount, 3,
          {
              {"person1Name", "Alice"},
              {"person2Name", "Bob"},
              {"fishCount", "3"},
          }),
      Eq("Alice y Bob, tienen 3 peces"));
  EXPECT_THAT(
      esTranslator->translatePlural(
          test_msgs::kCoupleFishCount, 102,
          {
              {"person1Name", "Alice"},
              {"person2Name", "Bob"},
              {"fishCount", "102"},
          }),
      Eq("Alice y Bob, tienen 102 peces"));
}

#ifdef SIMPLE_TR8N_ENABLE_EXCEPTIONS

TEST_F(SimpleTranslatorCharTest, ShouldHandleErrorsNoArgs) {
  // Missing message type:
  try {
    enTranslator->translate("not.configured_msg_type");
    FAIL() << "Expecting MissingMsgTypeException";
  } catch (const simple_tr8n::MissingMsgTypeException<char>& e) {
    EXPECT_THAT(e.what(), StrEq("simple_tr8n::MissingMsgTypeException: not.configured_msg_type"));
  }

  // Plural mismatch:
  try {
    enTranslator->translate(test_msgs::kCoupleFishCount);
    FAIL() << "Expecting InvalidArgsException";
  } catch (const simple_tr8n::InvalidArgsException<char>& e) {
    EXPECT_THAT(e.what(), StrEq("simple_tr8n::InvalidArgsException: test.couple_fish_count"));
  }

  // Missing argument:
  try {
    enTranslator->translate(test_msgs::kHelloName);
    FAIL() << "Expecting MissingArgException";
  } catch (const simple_tr8n::MissingArgException<char>& e) {
    EXPECT_THAT(
        e.what(),
        StrEq("simple_tr8n::MissingArgException: (msgType) test.hello_name: (argKey) personName"));
  }
}

TEST_F(SimpleTranslatorCharTest, ShouldHandleErrorsArgs) {
  // Missing message type:
  try {
    enTranslator->translate("not.configured_msg_type", {{"argKey", "argValue"}});
    FAIL() << "Expecting MissingMsgTypeException";
  } catch (const simple_tr8n::MissingMsgTypeException<char>& e) {
    EXPECT_THAT(e.what(), StrEq("simple_tr8n::MissingMsgTypeException: not.configured_msg_type"));
  }

  // Plural mismatch:
  try {
    enTranslator->translate(
        test_msgs::kCoupleFishCount, {
                                         {"person1Name", "Alice"},
                                         {"person2Name", "Bob"},
                                         {"fishCount", "2"},
                                     });
    FAIL() << "Expecting InvalidArgsException";
  } catch (const simple_tr8n::InvalidArgsException<char>& e) {
    EXPECT_THAT(e.what(), StrEq("simple_tr8n::InvalidArgsException: test.couple_fish_count"));
  }

  // Missing argument:
  try {
    enTranslator->translate(test_msgs::kHelloName, {{"wrongArg", "won't match"}});
    FAIL() << "Expecting MissingArgException";
  } catch (const simple_tr8n::MissingArgException<char>& e) {
    EXPECT_THAT(
        e.what(),
        StrEq("simple_tr8n::MissingArgException: (msgType) test.hello_name: (argKey) personName"));
  }
}

TEST_F(SimpleTranslatorCharTest, ShouldHandleErrorsPlural) {
  // Missing message type:
  try {
    enTranslator->translatePlural("not.configured_msg_type", 2, {{"argKey", "argValue"}});
    FAIL() << "Expecting MissingMsgTypeException";
  } catch (const simple_tr8n::MissingMsgTypeException<char>& e) {
    EXPECT_THAT(e.what(), StrEq("simple_tr8n::MissingMsgTypeException: not.configured_msg_type"));
  }

  // Non-plural mismatch:
  try {
    enTranslator->translatePlural(test_msgs::kHelloName, 1, {{"personName", "Alice"}});
    FAIL() << "Expecting InvalidArgsException";
  } catch (const simple_tr8n::InvalidArgsException<char>& e) {
    EXPECT_THAT(e.what(), StrEq("simple_tr8n::InvalidArgsException: test.hello_name"));
  }

  // Missing argument:
  try {
    enTranslator->translatePlural(
        test_msgs::kCoupleFishCount, 2,
        {
            {"person1Name", "Alice"},
            {"fishCount", "2"},
        });
    FAIL() << "Expecting MissingArgException";
  } catch (const simple_tr8n::MissingArgException<char>& e) {
    EXPECT_THAT(
        e.what(), StrEq("simple_tr8n::MissingArgException: (msgType) "
                        "test.couple_fish_count: (argKey) person2Name"));
  }

  // No matching plural case (<= count):
  try {
    esTranslator->translatePlural(
        test_msgs::kCoupleFishCount, 0,
        {
            {"person1Name", "Alice"},
            {"person2Name", "Bob"},
            {"fishCount", "0"},
        });
    FAIL() << "Expecting InvalidArgsException";
  } catch (const simple_tr8n::InvalidArgsException<char>& e) {
    EXPECT_THAT(e.what(), StrEq("simple_tr8n::InvalidArgsException: test.couple_fish_count"));
  }
}

#else  // SIMPLE_TR8N_ENABLE_EXCEPTIONS

// With exceptions disabled, all errors should just yield the empty string:

TEST_F(SimpleTranslatorCharTest, ShouldHandleErrorsNoArgs) {
  // Missing message type:
  EXPECT_THAT(enTranslator->translate("not.configured_msg_type"), Eq(""));

  // Plural mismatch:
  EXPECT_THAT(enTranslator->translate(test_msgs::kCoupleFishCount), Eq(""));

  // Missing argument:
  EXPECT_THAT(enTranslator->translate(test_msgs::kHelloName), Eq(""));
}

TEST_F(SimpleTranslatorCharTest, ShouldHandleErrorsArgs) {
  // Missing message type:
  EXPECT_THAT(enTranslator->translate("not.configured_msg_type", {{"argKey", "argValue"}}), Eq(""));

  // Plural mismatch:
  EXPECT_THAT(
      enTranslator->translate(
          test_msgs::kCoupleFishCount,
          {
              {"person1Name", "Alice"},
              {"person2Name", "Bob"},
              {"fishCount", "2"},
          }),
      Eq(""));

  // Missing argument:
  EXPECT_THAT(
      enTranslator->translate(test_msgs::kHelloName, {{"wrongArg", "won't match"}}), Eq(""));
}

TEST_F(SimpleTranslatorCharTest, ShouldHandleErrorsPlural) {
  // Missing message type:
  EXPECT_THAT(
      enTranslator->translatePlural("not.configured_msg_type", 2, {{"argKey", "argValue"}}),
      Eq(""));

  // Non-plural mismatch:
  EXPECT_THAT(
      enTranslator->translatePlural(test_msgs::kHelloName, 1, {{"personName", "Alice"}}), Eq(""));

  // Missing argument:
  EXPECT_THAT(
      enTranslator->translatePlural(
          test_msgs::kCoupleFishCount, 2,
          {
              {"person1Name", "Alice"},
              {"fishCount", "2"},
          }),
      Eq(""));

  // No matching plural case (<= count):
  EXPECT_THAT(
      esTranslator->translatePlural(
          test_msgs::kCoupleFishCount, 0,
          {
              {"person1Name", "Alice"},
              {"person2Name", "Bob"},
              {"fishCount", "0"},
          }),
      Eq(""));
}

#endif  // SIMPLE_TR8N_ENABLE_EXCEPTIONS

namespace test_wmsgs {

constexpr wchar_t kNoArgs[] = L"test.no_args";
constexpr wchar_t kHelloName[] = L"test.hello_name";
constexpr wchar_t kProgressPct[] = L"test.progress_pct";
constexpr wchar_t kCoupleFishCount[] = L"test.couple_fish_count";

}  // namespace test_wmsgs

class SimpleTranslatorWCharTest : public Test {
protected:
  void SetUp() override {
    auto enConfig = std::make_unique<simple_tr8n::MsgConfigs<wchar_t>>();
    enConfig->add(test_wmsgs::kNoArgs, L"A simple message with no arguments")
        .add(test_wmsgs::kHelloName, L"hello, %{personName}!")
        .add(test_wmsgs::kProgressPct, L"progress: %{pct}%")
        .add(
            test_wmsgs::kCoupleFishCount,
            {
                {0, L"%{person1Name} and %{person2Name}, you have no fish"},
                {1, L"%{person1Name} and %{person2Name}, you have a fish"},
                {2, L"%{person1Name} and %{person2Name}, you have two fish"},
                {3, L"%{person1Name} and %{person2Name}, you have %{fishCount} fish"},
            });
    enTranslator = std::make_unique<simple_tr8n::SimpleTranslator<wchar_t>>(std::move(enConfig));
  }

  std::unique_ptr<simple_tr8n::SimpleTranslator<wchar_t>> enTranslator;
};

TEST_F(SimpleTranslatorWCharTest, ShouldWorkWithWideChars) {
  // Success case:
  EXPECT_THAT(
      enTranslator->translatePlural(
          test_wmsgs::kCoupleFishCount, 6,
          {
              {L"person1Name", L"Alice"},
              {L"person2Name", L"Bob"},
              {L"fishCount", L"6"},
          }),
      Eq(L"Alice and Bob, you have 6 fish"));

#ifdef SIMPLE_TR8N_ENABLE_EXCEPTIONS

  // Missing message type:
  try {
    enTranslator->translatePlural(L"not.configured_msg_type", 2, {{L"argKey", L"argValue"}});
    FAIL() << "Expecting MissingMsgTypeException";
  } catch (const simple_tr8n::MissingMsgTypeException<wchar_t>& e) {
    EXPECT_THAT(e.what(), StrEq("simple_tr8n::MissingMsgTypeException"));
  }

#endif  // SIMPLE_TR8N_ENABLE_EXCEPTIONS
}
