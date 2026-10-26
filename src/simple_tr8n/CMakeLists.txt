# SPDX-FileCopyrightText: 2022 Eric Barndollar
#
# SPDX-License-Identifier: Apache-2.0

include(SimpleTr8nTargets)

# SimpleTr8n::API: interface for SimpleTr8n project.
simple_tr8n_header_library(API translator.hpp)
target_link_libraries(SimpleTr8n_API INTERFACE Microsoft.GSL::GSL)

# SimpleTr8n::SimpleTranslator: simple implementation of the API.
simple_tr8n_header_library(SimpleTranslator simple_translator.hpp)
target_link_libraries(SimpleTr8n_SimpleTranslator INTERFACE Microsoft.GSL::GSL)
if(SIMPLE_TR8N_ENABLE_EXCEPTIONS)
  target_sources(SimpleTr8n_SimpleTranslator INTERFACE exceptions.hpp)
  target_compile_definitions(SimpleTr8n_SimpleTranslator INTERFACE "SIMPLE_TR8N_ENABLE_EXCEPTIONS")
endif()

if(SIMPLE_TR8N_ENABLE_TESTS)
  simple_tr8n_gtest(SimpleTranslatorTest simple_translator_test.cpp)
  target_link_libraries(SimpleTr8n_SimpleTranslatorTest
      PRIVATE SimpleTr8n::SimpleTranslator)
endif()
