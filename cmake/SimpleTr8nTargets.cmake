include(GNUInstallDirs)

## Adds header-only library with SimpleTr8n project defaults.
##
## Creates target named SimpleTr8n_${name} and alias of
## SimpleTr8n::${name}.
##
## All remaining arguments are passed as sources to target_sources().
function(simple_tr8n_header_library name)
  add_library(SimpleTr8n_${name} INTERFACE)
  add_library(SimpleTr8n::${name} ALIAS SimpleTr8n_${name})
  set_target_properties(SimpleTr8n_${name} PROPERTIES
      EXPORT_NAME SimpleTr8n_${name})

  target_sources(SimpleTr8n_${name} INTERFACE ${ARGN})

  # Always include the src/ dir as a base include path.
  target_include_directories(SimpleTr8n_${name}
      INTERFACE ${SimpleTr8n_SOURCE_DIR}/src)

  target_compile_features(SimpleTr8n_${name} INTERFACE cxx_std_14)
endfunction()

## Configures target compile options to enable this project's standard set of
## warnings for the SimpleTr8n_${name} target.
function(simple_tr8n_enable_warnings name)
  # Enable strict warnings.
  if (MSVC)
    target_compile_options(SimpleTr8n_${name}
        PRIVATE /W4)
  else()
    target_compile_options(SimpleTr8n_${name}
        PRIVATE -Wall -Wextra -pedantic)
  endif()
endfunction()

## Adds library with SimpleTr8n project defaults.
##
## Creates target named SimpleTr8n_${name} and alias of
## SimpleTr8n::${name}.
##
## All remaining arguments are passed to add_library().
function(simple_tr8n_library name)
  add_library(SimpleTr8n_${name} ${ARGN})
  add_library(SimpleTr8n::${name} ALIAS SimpleTr8n_${name})
  set_target_properties(SimpleTr8n_${name} PROPERTIES
      OUTPUT_NAME SimpleTr8n_${name}
      EXPORT_NAME SimpleTr8n_${name})

  # Always include the src/ dir as a base include path.
  target_include_directories(SimpleTr8n_${name}
      PUBLIC ${SimpleTr8n_SOURCE_DIR}/src)

  target_compile_features(SimpleTr8n_${name} PUBLIC cxx_std_14)
  simple_tr8n_enable_warnings(${name})

  # TODO: Configure installation, if necessary.
endfunction()

## If testing for this project is enabled, adds test executable and matching
## CTest test with SimpleTr8n project defaults. Creates executable named
## SimpleTr8n_${name}.
##
## All remaining arguments are passed to add_executable().
function(simple_tr8n_gtest name)
  if(NOT SIMPLE_TR8N_ENABLE_TESTS)
    # Project testing off, so simple_tr8n_gtest() should never have been invoked.
    message(FATAL_ERROR "Must guard test targets with SIMPLE_TR8N_ENABLE_TESTS")
  endif()

  include(GoogleTest)

  add_executable(SimpleTr8n_${name} ${ARGN})

  # Always include the src/ dir as a base include path.
  target_include_directories(SimpleTr8n_${name}
      PUBLIC ${SimpleTr8n_SOURCE_DIR}/src)

  target_compile_features(SimpleTr8n_${name} PUBLIC cxx_std_14)
  simple_tr8n_enable_warnings(${name})

  target_link_libraries(SimpleTr8n_${name}
      PRIVATE gtest gmock gtest_main)
  gtest_discover_tests(SimpleTr8n_${name}
      WORKING_DIRECTORY ${SimpleTr8n_BINARY_DIR}/stage/${CMAKE_INSTALL_BINDIR})
endfunction()
