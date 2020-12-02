#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>

#include "simple_tr8n/simple_translator.hpp"

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

TEST(SimpleTranslatorChar, ShouldTranslate) {
  auto enConfig = std::make_unique<simple_tr8n::MsgConfigs<char>>();
  enConfig->add(test_msgs::kNoArgs, "A simple message with no arguments")
      .add(test_msgs::kHelloName, "hello, %{personName}!")
      .add(test_msgs::kProgressPct, "progress: %{pct}%")
      .add(test_msgs::kCoupleFishCount, {
        {0, "%{person1Name} and %{person2Name}, you have no fish"},
        {1, "%{person1Name} and %{person2Name}, you have a fish"},
        {2, "%{person1Name} and %{person2Name}, you have two fish"},
        {3, "%{person1Name} and %{person2Name}, you have %{fishCount} fish"},
      })
      .add(test_msgs2::kAddlMsg, "An additional message with %{arg}");

  simple_tr8n::SimpleTranslator<char> enTranslator(std::move(enConfig));
  EXPECT_THAT(enTranslator.translate(test_msgs::kNoArgs),
      Eq("A simple message with no arguments"));
  EXPECT_THAT(
      enTranslator.translate(test_msgs::kHelloName, {{"personName", "Bob"}}),
      Eq("hello, Bob!"));
  EXPECT_THAT(
      enTranslator.translate(test_msgs::kProgressPct, {{"pct", "75"}}),
      Eq("progress: 75%"));

  EXPECT_THAT(
      enTranslator.translatePlural(test_msgs::kCoupleFishCount, 0, {
        {"person1Name", "Alice"},
        {"person2Name", "Bob"},
        {"fishCount", "0"},
      }),
      Eq("Alice and Bob, you have no fish"));
  EXPECT_THAT(
      enTranslator.translatePlural(test_msgs::kCoupleFishCount, 1, {
        {"person1Name", "Alice"},
        {"person2Name", "Bob"},
        {"fishCount", "1"},
      }),
      Eq("Alice and Bob, you have a fish"));
  EXPECT_THAT(
      enTranslator.translatePlural(test_msgs::kCoupleFishCount, 2, {
        {"person1Name", "Alice"},
        {"person2Name", "Bob"},
        {"fishCount", "2"},
      }),
      Eq("Alice and Bob, you have two fish"));
  EXPECT_THAT(
      enTranslator.translatePlural(test_msgs::kCoupleFishCount, 3, {
        {"person1Name", "Alice"},
        {"person2Name", "Bob"},
        {"fishCount", "3"},
      }),
      Eq("Alice and Bob, you have 3 fish"));
  EXPECT_THAT(
      enTranslator.translatePlural(test_msgs::kCoupleFishCount, 102, {
        {"person1Name", "Alice"},
        {"person2Name", "Bob"},
        {"fishCount", "102"},
      }),
      Eq("Alice and Bob, you have 102 fish"));
}
