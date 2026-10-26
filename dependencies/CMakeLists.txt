# SPDX-FileCopyrightText: 2022 Eric Barndollar
#
# SPDX-License-Identifier: Apache-2.0

include(FetchContent)

FetchContent_Declare(
    GSL
    GIT_REPOSITORY https://github.com/microsoft/GSL
    GIT_TAG fbd20b8e8c52cba04dbc869cc9068a7800a5665d  # 2026-09-22 (v5.0.1)
    GIT_SHALLOW ON
    SYSTEM
    EXCLUDE_FROM_ALL
    FIND_PACKAGE_ARGS 5)
FetchContent_MakeAvailable(GSL)

if(SIMPLE_TR8N_ENABLE_TESTS)
  FetchContent_Declare(
      googletest
      GIT_REPOSITORY https://github.com/google/googletest
      GIT_TAG 063de7e9578f82b369302001269680b4b1553359  # 2026-08-10 (v1.18.0)
      EXCLUDE_FROM_ALL)
  FetchContent_MakeAvailable(googletest)
endif()
