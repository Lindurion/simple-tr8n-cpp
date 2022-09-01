# SPDX-FileCopyrightText: 2022 Eric Barndollar
#
# SPDX-License-Identifier: Apache-2.0

include(GetCPM)
include(SimpleTr8nDeps)

CPMUsePackageLock(package-lock.cmake)

#===================================================================================================
# IMPORTANT: Upon updating any dependencies below, regenerate package-lock.cmake with:
#
# # From simple-tr8n-cpp/ source directory (for existing CMake build):
# $ cmake -D "SIMPLE_TR8N_STRING_VIEW_TYPE=lite" -D "SIMPLE_TR8N_ENABLE_TESTS=ON" build
# $ cmake --build build --target cpm-update-package-lock
#===================================================================================================

CPMAddPackage(
    NAME gsl
    VERSION 4.0.0
    GIT_REPOSITORY https://github.com/microsoft/GSL
    GIT_TAG a3534567187d2edc428efd3f13466ff75fe5805c  # 2022-01-28
    EXCLUDE_FROM_ALL)
simple_tr8n_mark_dir_targets_as_external(${gsl_SOURCE_DIR})

if(SIMPLE_TR8N_REQUIRES_STRING_VIEW_LITE)
  CPMAddPackage(
      NAME string_view_lite
      VERSION 1.7.0
      GIT_REPOSITORY https://github.com/martinmoene/string-view-lite
      GIT_TAG 7b1867b7c08f9ac010fb8b9dae79ff8c8359bafb  # 2022-08-10
      EXCLUDE_FROM_ALL)
  simple_tr8n_mark_dir_targets_as_external(${string_view_lite_SOURCE_DIR})
endif()

if(SIMPLE_TR8N_ENABLE_TESTS)
  CPMAddPackage(
      NAME googletest
      VERSION 1.12.1
      GIT_REPOSITORY https://github.com/google/googletest
      GIT_TAG 58d77fa8070e8cec2dc1ed015d66b454c8d78850  # 2022-06-30
      EXCLUDE_FROM_ALL)
endif()
