# SPDX-FileCopyrightText: 2022 Eric Barndollar
#
# SPDX-License-Identifier: Apache-2.0

if(NOT SIMPLE_TR8N_IS_TOP_LEVEL_PROJECT)
  if(CMAKE_CXX_STANDARD LESS 14)
    message(FATAL_ERROR
        "SimpleTr8n requires CMAKE_CXX_STANDARD >= 14 (got: ${CMAKE_CXX_STANDARD})")
  endif()

  return()  # Don't set these defaults if this isn't the top-level project.
endif()

# The defaults below apply only when SimpleTr8n is the top-level CMake project
# (i.e. when developing it directly as opposed to using it as a library within
# another project).

include(CheckCXXCompilerFlag)

# C++ standard is determined by string_view type.
if(SIMPLE_TR8N_STRING_VIEW_TYPE STREQUAL "std")
  set(CMAKE_CXX_STANDARD 17)  # std::string_view requires C++17 minimum.
else()
  set(CMAKE_CXX_STANDARD 14)
endif()

# Default all compilers to hiding symbols by default.
set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN TRUE)

# Prefer finding package configs to Find<pkg> modules, and isolate builds
# from user/system package registries.
set(CMAKE_FIND_PACKAGE_PREFER_CONFIG TRUE)
set(CMAKE_EXPORT_NO_PACKAGE_REGISTRY TRUE)
set(CMAKE_FIND_USE_PACKAGE_REGISTRY FALSE)
set(CMAKE_FIND_USE_SYSTEM_PACKAGE_REGISTRY FALSE)

# If available, split debug symbols into separate .dwo files for debug builds.
check_cxx_compiler_flag("-gsplit-dwarf" HAVE_SPLIT_DWARF)
if(HAVE_SPLIT_DWARF)
  add_compile_options("$<$<CONFIG:Debug>:-gsplit-dwarf>")
  add_compile_options("$<$<CONFIG:RelWithDebInfo>:-gsplit-dwarf>")
endif()

# Collect binaries under stage/ subdirectory, unless a parent project overrides
# this default.
include(GNUInstallDirs)
if(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
      ${SimpleTr8n_BINARY_DIR}/stage/${CMAKE_INSTALL_BINDIR})
endif()
if(NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY
      ${SimpleTr8n_BINARY_DIR}/stage/${CMAKE_INSTALL_LIBDIR})
endif()
if(NOT CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY
      ${SimpleTr8n_BINARY_DIR}/stage/${CMAKE_INSTALL_LIBDIR})
endif()

# Discover Google Test list of tests at test time, rather than post build.
if(SIMPLE_TR8N_ENABLE_TESTS)
  set(CMAKE_GTEST_DISCOVER_TESTS_DISCOVERY_MODE PRE_TEST)
endif()
