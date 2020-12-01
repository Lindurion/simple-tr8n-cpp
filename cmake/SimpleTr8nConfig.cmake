# Configure build based on defaults or client specified options.

# C++ exceptions support:
option(SIMPLE_TR8N_ENABLE_EXCEPTIONS "Enables C++ exceptions for SimpleTr8n" ON)
mark_as_advanced(SIMPLE_TR8N_ENABLE_EXCEPTIONS)

# simple_tr8n::string_view type:
set(SIMPLE_TR8N_STRING_VIEW_TYPE "std" CACHE STRING "simple_tr8n::string_view type to use")
set_property(CACHE SIMPLE_TR8N_STRING_VIEW_TYPE PROPERTY STRINGS "std" "absl" "custom")

option(SIMPLE_TR8N_STRING_VIEW_CUSTOM_TYPE "Custom type for simple_tr8n::string_view")
mark_as_advanced(SIMPLE_TR8N_STRING_VIEW_CUSTOM_TYPE)

option(SIMPLE_TR8N_STRING_VIEW_CUSTOM_INCLUDES "Custom includes for simple_tr8n::string_view")
mark_as_advanced(SIMPLE_TR8N_STRING_VIEW_CUSTOM_INCLUDES)

option(SIMPLE_TR8N_STRING_VIEW_CUSTOM_TARGET "Custom library target to link for simple_tr8n::string_view")
mark_as_advanced(SIMPLE_TR8N_STRING_VIEW_CUSTOM_TARGET)

if(SIMPLE_TR8N_STRING_VIEW_TYPE STREQUAL "std")
  set(SIMPLE_TR8N_STRING_VIEW_INCLUDES "#include <string_view>")
  set(SIMPLE_TR8N_STRING_VIEW_TYPE "std::string_view")
elseif(SIMPLE_TR8N_STRING_VIEW_TYPE STREQUAL "absl")
  set(SIMPLE_TR8N_REQUIRES_ABSL TRUE)

  set(SIMPLE_TR8N_STRING_VIEW_INCLUDES "#include <absl/strings/string_view.h>")
  set(SIMPLE_TR8N_STRING_VIEW_TYPE "absl::string_view")
  set(SIMPLE_TR8N_STRING_VIEW_TARGET "absl::strings")
elseif(SIMPLE_TR8N_STRING_VIEW_TYPE STREQUAL "custom")
  if(NOT SIMPLE_TR8N_STRING_VIEW_CUSTOM_TYPE)
    message(FATAL_ERROR "SIMPLE_TR8N_STRING_VIEW_TYPE custom requires at least SIMPLE_TR8N_STRING_VIEW_CUSTOM_TYPE")
  endif()

  set(SIMPLE_TR8N_STRING_VIEW_INCLUDES SIMPLE_TR8N_STRING_VIEW_CUSTOM_INCLUDES)
  set(SIMPLE_TR8N_STRING_VIEW_TYPE SIMPLE_TR8N_STRING_VIEW_CUSTOM_TYPE)
  set(SIMPLE_TR8N_STRING_VIEW_TARGET SIMPLE_TR8N_STRING_VIEW_CUSTOM_TARGET)
else()
  message(FATAL_ERROR "SIMPLE_TR8N_STRING_VIEW_TYPE must be std, absl, or custom")
endif()
