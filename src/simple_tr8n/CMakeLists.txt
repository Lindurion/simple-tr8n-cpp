# SPDX-FileCopyrightText: 2022 Eric Barndollar
#
# SPDX-License-Identifier: Apache-2.0

include(SimpleTr8nTargets)

# StringView: alias for std::basic_string_view or an equivalent type.
simple_tr8n_header_library(StringView string_view.hpp)
if(SIMPLE_TR8N_STRING_VIEW_TYPE STREQUAL "std")
  target_compile_features(SimpleTr8n_StringView INTERFACE cxx_std_17)
endif()

target_compile_definitions(SimpleTr8n_StringView INTERFACE
    "SIMPLE_TR8N_BASIC_STRING_VIEW_TYPE=${SIMPLE_TR8N_BASIC_STRING_VIEW_TYPE}")
if(SIMPLE_TR8N_STRING_VIEW_INCLUDE)
  target_compile_definitions(SimpleTr8n_StringView INTERFACE
      "SIMPLE_TR8N_STRING_VIEW_INCLUDE=${SIMPLE_TR8N_STRING_VIEW_INCLUDE}")
endif()
if(SIMPLE_TR8N_STRING_VIEW_TARGET)
  target_link_libraries(SimpleTr8n_StringView INTERFACE "${SIMPLE_TR8N_STRING_VIEW_TARGET}")
endif()

# SimpleTr8n::API: interface for SimpleTr8n project.
simple_tr8n_header_library(API translator.hpp)
target_link_libraries(SimpleTr8n_API INTERFACE SimpleTr8n::StringView Microsoft.GSL::GSL)

# SimpleTr8n::SimpleTranslator: simple implementation of the API.
simple_tr8n_header_library(SimpleTranslator simple_translator.hpp)
if(SIMPLE_TR8N_ENABLE_EXCEPTIONS)
  target_sources(SimpleTr8n_SimpleTranslator INTERFACE exceptions.hpp)
  target_compile_definitions(SimpleTr8n_SimpleTranslator INTERFACE "SIMPLE_TR8N_ENABLE_EXCEPTIONS")
endif()
target_link_libraries(SimpleTr8n_SimpleTranslator
    INTERFACE SimpleTr8n::API SimpleTr8n::StringView)

if(SIMPLE_TR8N_ENABLE_TESTS)
  # Note: Only testing with C++17 std::basic_string_view and exceptions enabled
  # by default. Can manually test other configurations as needed.
  simple_tr8n_gtest(SimpleTranslatorTest simple_translator_test.cpp)
  target_link_libraries(SimpleTr8n_SimpleTranslatorTest
      PRIVATE SimpleTr8n::SimpleTranslator)
endif()
