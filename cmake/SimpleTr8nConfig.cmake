# SPDX-FileCopyrightText: 2022 Eric Barndollar
#
# SPDX-License-Identifier: Apache-2.0

# Configure build based on defaults or client specified options.

# C++ exceptions support:
option(SIMPLE_TR8N_ENABLE_EXCEPTIONS "Enables C++ exceptions for SimpleTr8n" ON)
mark_as_advanced(SIMPLE_TR8N_ENABLE_EXCEPTIONS)

# simple_tr8n::string_view type:
set(SIMPLE_TR8N_STRING_VIEW_TYPE "std" CACHE STRING
    "SimpleTr8n: string_view implementation to use (std, lite, or custom)")
set_property(CACHE SIMPLE_TR8N_STRING_VIEW_TYPE PROPERTY STRINGS "std" "lite" "custom")

set(SIMPLE_TR8N_STRING_VIEW_CUSTOM_TYPE "" CACHE STRING
    "Custom type for simple_tr8n::basic_string_view")
mark_as_advanced(SIMPLE_TR8N_STRING_VIEW_CUSTOM_TYPE)

set(SIMPLE_TR8N_STRING_VIEW_CUSTOM_INCLUDE "" CACHE STRING
    "Custom include for simple_tr8n::basic_string_view")
mark_as_advanced(SIMPLE_TR8N_STRING_VIEW_CUSTOM_INCLUDE)

set(SIMPLE_TR8N_STRING_VIEW_CUSTOM_TARGET "" CACHE STRING
    "Custom library target to link for simple_tr8n::basic_string_view")
mark_as_advanced(SIMPLE_TR8N_STRING_VIEW_CUSTOM_TARGET)

if(SIMPLE_TR8N_STRING_VIEW_TYPE STREQUAL "std")
  set(SIMPLE_TR8N_STRING_VIEW_INCLUDE "<string_view>")
  set(SIMPLE_TR8N_BASIC_STRING_VIEW_TYPE "std::basic_string_view")
elseif(SIMPLE_TR8N_STRING_VIEW_TYPE STREQUAL "lite")
  set(SIMPLE_TR8N_REQUIRES_STRING_VIEW_LITE TRUE)

  set(SIMPLE_TR8N_STRING_VIEW_INCLUDE "<nonstd/string_view.hpp>")
  set(SIMPLE_TR8N_BASIC_STRING_VIEW_TYPE "nonstd::basic_string_view")
  set(SIMPLE_TR8N_STRING_VIEW_TARGET "nonstd::string-view-lite")
elseif(SIMPLE_TR8N_STRING_VIEW_TYPE STREQUAL "custom")
  if(NOT SIMPLE_TR8N_STRING_VIEW_CUSTOM_TYPE)
    message(FATAL_ERROR "SIMPLE_TR8N_STRING_VIEW_TYPE custom requires at least SIMPLE_TR8N_STRING_VIEW_CUSTOM_TYPE")
  endif()

  set(SIMPLE_TR8N_STRING_VIEW_INCLUDE SIMPLE_TR8N_STRING_VIEW_CUSTOM_INCLUDE)
  set(SIMPLE_TR8N_BASIC_STRING_VIEW_TYPE SIMPLE_TR8N_STRING_VIEW_CUSTOM_TYPE)
  set(SIMPLE_TR8N_STRING_VIEW_TARGET SIMPLE_TR8N_STRING_VIEW_CUSTOM_TARGET)
else()
  message(FATAL_ERROR "SIMPLE_TR8N_STRING_VIEW_TYPE must be std, lite, or custom")
endif()
