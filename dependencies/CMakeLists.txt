# SPDX-FileCopyrightText: 2022 Eric Barndollar
#
# SPDX-License-Identifier: Apache-2.0

include(GetCPM)
include(SimpleTr8nDeps)

CPMAddPackage(
    NAME gsl
    GIT_REPOSITORY https://github.com/microsoft/GSL
    GIT_TAG 10df83d292bf5bbdc487e57dc8c2dc8c7a01f4d1  # 2022-08-18
    EXCLUDE_FROM_ALL)
simple_tr8n_mark_dir_targets_as_external(${gsl_SOURCE_DIR})

if(SIMPLE_TR8N_REQUIRES_STRING_VIEW_LITE)
  CPMAddPackage(
      NAME string_view_lite
      GIT_REPOSITORY https://github.com/martinmoene/string-view-lite
      GIT_TAG 6e90d372626effce61053044f18516c3b96870f9  # 2022-08-14
      EXCLUDE_FROM_ALL)
  simple_tr8n_mark_dir_targets_as_external(${string_view_lite_SOURCE_DIR})
endif()

if(SIMPLE_TR8N_ENABLE_TESTS)
  CPMAddPackage(
      NAME googletest
      GIT_REPOSITORY https://github.com/google/googletest
      GIT_TAG 0e0d9feefab1b51aaab9dfd70132e93c0b6964e5  # 2022-09-01
      EXCLUDE_FROM_ALL)
endif()
