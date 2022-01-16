# SPDX-FileCopyrightText: 2022 Eric Barndollar
#
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.18)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

project(SimpleTr8n
    VERSION 0.1
    DESCRIPTION "Really simple C++ library for translating user-facing messages."
    HOMEPAGE_URL "https://github.com/Lindurion/simple-tr8n-cpp"
    LANGUAGES CXX)

if(CMAKE_SOURCE_DIR STREQUAL SimpleTr8n_SOURCE_DIR)
  set(SIMPLE_TR8N_IS_TOP_LEVEL_PROJECT YES)
  set(SimpleTr8nTestingDefault ON)
else()
  set(SimpleTr8nTestingDefault OFF)
endif()
  
option(SIMPLE_TR8N_ENABLE_TESTS "Enable testing for the SimpleTr8n project"
    ${SimpleTr8nTestingDefault})
if(SIMPLE_TR8N_ENABLE_TESTS)
  enable_testing()
endif()

include(SimpleTr8nDefaults)
include(SimpleTr8nConfig)

add_subdirectory(dependencies)
add_subdirectory(src)
