<!--
SPDX-FileCopyrightText: 2022 Eric Barndollar

SPDX-License-Identifier: Apache-2.0
-->

# simple-tr8n-cpp

Really simple C++ header-only library for translating user-facing messages.
Designed for C++ libraries that don't want to require their client applications
to use any specific heavyweight i18n framework.

Places all translation operations behind the `Translator` interface (declared
[here](src/simple_tr8n/translator.hpp)). Clients of your library then have two
options:

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
mechanism for depending on other CMake projects should work. The easiest
technique for most projects will likely be using
[FetchContent](https://cmake.org/cmake/help/latest/module/FetchContent.html),
which will download this library at a particular git commit and add its targets
directly to your CMake project.

```cmake
FetchContent_Declare(SimpleTr8n
    GIT_REPOSITORY https://github.com/Lindurion/simple-tr8n-cpp
    GIT_TAG <REPLACE DESIRED GIT COMMIT HASH HERE>)
FetchContent_MakeAvailable(SimpleTr8n)
```

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

// (With C++17 or higher, all of these can also be declared inline).

// Args: userFirstName, userAge.
constexpr char kExampleMsgA[] = "your_project.a";

// Args: emailCount (plural).
constexpr char kExampleMsgB[] = "your_project.b";
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

## Dependencies and C++ Language Version Support

This library supports C++14 and above. By default, however, it requires C++17
to support `std::basic_string_view`.

Libraries this one depends on (automatically downloaded during build via
[CPM.cmake](https://github.com/cpm-cmake/CPM.cmake), which is a wrapper around `FetchContent`):

* [microsoft/GSL: C++ Guidelines Support Library](https://github.com/microsoft/GSL)
* [martinmoene/string-view-lite](https://github.com/martinmoene/string-view-lite):
    optional dependency for a C++14 compatible version of `std::basic_string_view`.
* [google/googletest](https://github.com/google/googletest): for tests only.

If you need to manage dependencies another way (*e.g.* using `vcpkg`, `conan`, or manually
downloading all 3rd party libraries and checking them into your own company repositories), you can
disable automatic dependency downloads by defining `CPM_USE_LOCAL_PACKAGES` when running `cmake`.

## CMake Build Customization

See [SimpleTr8nConfig.cmake](src/cmake/SimpleTr8nConfig.cmake) for CMake variables
that can be customized.

* `SIMPLE_TR8N_ENABLE_EXCEPTIONS`: Enable C++ exceptions support?
  * If disabled, library will return empty strings instead of throwing exceptions.
* `SIMPLE_TR8N_STRING_VIEW_TYPE`: Controls version of `basic_string_view`.
  * `std` (default): Use `std::basic_string_view`, which requires C++17.
  * `lite`: Use `string-view-lite` library for a C++14 compatible version.
  * `custom`: Manually specify a different implementation (see below).
* `SIMPLE_TR8N_STRING_VIEW_CUSTOM_TYPE`: *e.g.* `your_library::basic_string_view`
* `SIMPLE_TR8N_STRING_VIEW_CUSTOM_INCLUDE`: *e.g.* `<your_library/basic_string_view.hpp>`
* `SIMPLE_TR8N_STRING_VIEW_CUSTOM_TARGET`: CMake target, *e.g.* `your_library::string_view`

## Licenses

This is free open source software. The code in this project is made available
under the [Apache-2.0](LICENSES/Apache-2.0.txt) license. See
[ThirdPartyNotices.txt](ThirdPartyNotices.txt) for license information about the
software libraries, assets, and tools this project depends upon.

Source files and `LICENSES/` are annotated and checked using
[reuse.software](https://reuse.software/) tools.
