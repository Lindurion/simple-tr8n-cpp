<!--
SPDX-FileCopyrightText: 2022 Eric Barndollar

SPDX-License-Identifier: Apache-2.0
-->

# simple-tr8n-cpp

Really simple C++ header-only library for translating user-facing messages.
Designed for C++ libraries that don't want to require their client applications
to use any specific heavyweight internationalization (i18n) framework.

Requires C++17 at minimum.

Places all translation operations behind the `Translator` interface (declared
in [translator.hpp](src/simple_tr8n/translator.hpp)). Clients of your library then
have two options:

1. Use the provided `SimpleTranslator` implementation.
2. Provide their own implementation of `Translator`, which can then delegate to
   whichever i18n framework they choose.

The simple implementation supports argument substitution with `%{argName}`
syntax as well as plurals (varying messages based on one numerical input
argument).

It does NOT currently support gender or other more advanced i18n framework
features.

## How to Use in Your Library

This project uses the [CMake](https://cmake.org/) build system. Any standard
mechanism for depending on other "modern" CMake projects should work. If you aren't
already using another tool for dependency managment, see
[Downloading And Building From Source With FetchContent](https://cmake.org/cmake/help/latest/guide/using-dependencies/index.html#downloading-and-building-from-source-with-fetchcontent)
or
[FetchContent And find_package() Integration](https://cmake.org/cmake/help/latest/guide/using-dependencies/index.html#fetchcontent-and-find-package-integration).

You can then depend on the provided CMake targets:

```cmake
# Depend on API only (providing own implementation):
target_link_libraries(YourTarget PRIVATE SimpleTr8n::API)

# Or depend on API and SimpleTranslator implementation:
target_link_libraries(YourTarget PRIVATE SimpleTr8n::API SimpleTr8n::SimpleTranslator)
```

All classes are templates parameterized with a `CharT` character type. In most
cases, this will be `char` (*e.g.* for ASCII or UTF-8 strings), though you could
also use `wchar_t` (*e.g.* for UTF-16 strings for use with Windows).

Every string to be translated needs its own unique *message type* string
identifier. Placing string constants within a message-specific namespace (or
sub-namespace) in their own header file is one way to do this. This is also a
good place to document arguments for interpolation.

```cpp
// Within your_project/msgs.hpp:
namespace your_project {
namespace msgs {

// Args: userFirstName, userAge.
inline constexpr char kExampleMsgA[] = "your_project.a";

// Args: emailCount (plural).
inline constexpr char kExampleMsgB[] = "your_project.b";
// ...

}  // namespace msgs
}  // namespace your_project
```

Define default language translations (or configure a tool to generate them),
each with a simple string template or a list of plural cases:

```cpp
auto enConfig = std::make_unique<simple_tr8n::MsgConfigs<char>>();
enConfig->add(msgs::kExampleMsgA, "%{userFirstName} is %{userAge} years old")
    .add(msgs::kExampleMsgB, {
      {0, "You have no new email messages"},
      {1, "You have 1 new email message"},

      // Covers count >= 2 cases:
      {2, "You have %{emailCount} new email messages"},
    });
    // ...
```

<u>Tip</u>: If you want to allow clients of your library to use their own
implementation, keep all of your uses of `SimpleTranslator` and its
configuration (like the `enConfig` example above) in a separate CMake target.

For each translated language you support, define (or generate) a similar
`MsgConfigs` object. Note that different languages can have fewer or more plural
cases as needed.

Parameterize your library so that it can be configured with an implementation
of the `Translator` interface for your message type. For example:

```cpp
class MyLibContext {
  MyLibContext(std::unique_ptr<simple_tr8n::Translator<char>> translator) { ... }
  // ...
};
```

Optionally configure with your primary language translations (*e.g.*
`enTranslations`) as a default value:

```cpp
auto translator =
    std::make_unique<simple_tr8n::SimpleTranslator<char>>(std::move(enConfig));
```

When your library needs to produce translated messages, use the configured
implementation:

```cpp
const auto msgA = translator->translate(msgs::kExampleMsgA, {
  {"userFirstName", "Alice"},
  {"userAge", "34"},
});
  
// Second arg selects the plural case.
const msgB = translator->translatePlural(msgs::kExampleMsgB, 3, {
  {"emailCount", "3"},
});
```

## Dependencies

Libraries this one depends on (by default downloaded and built via `FetchContent`,
but consuming projects can provide these dependencies in other ways; see
[CMake Using Dependencies Guide](https://cmake.org/cmake/help/latest/guide/using-dependencies/index.html)):

* [microsoft/GSL: C++ Guidelines Support Library](https://github.com/microsoft/GSL)
* [google/googletest](https://github.com/google/googletest): for tests only.

## CMake Build Customization

See [SimpleTr8nConfig.cmake](src/cmake/SimpleTr8nConfig.cmake) for CMake variables
that can be customized.

* `SIMPLE_TR8N_ENABLE_EXCEPTIONS`: Enable C++ exceptions support?
  * If disabled, library will return empty strings instead of throwing exceptions.

## Licenses

This is free open source software. The code in this project is made available
under the [Apache-2.0](LICENSES/Apache-2.0.txt) license. See
[ThirdPartyNotices.txt](ThirdPartyNotices.txt) for license information about the
software libraries, assets, and tools this project depends upon.

Source files and `LICENSES/` are annotated and checked using
[reuse.software](https://reuse.software/) tools (run `reuse lint` to check this project).
