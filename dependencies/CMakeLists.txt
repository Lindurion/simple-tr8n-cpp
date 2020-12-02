include(FetchContent)

FetchContent_Declare(gsl
    GIT_REPOSITORY https://github.com/microsoft/GSL
    GIT_TAG 0140ab1d73d833d2b1eca37d1b3b91e52dda4176)  # 2020-11-23
FetchContent_MakeAvailable(gsl)

if(SIMPLE_TR8N_REQUIRES_STRING_VIEW_LITE)
  FetchContent_Declare(string_view_lite
      GIT_REPOSITORY https://github.com/martinmoene/string-view-lite
      GIT_TAG 31934cc97d387fd59d170ac76a3581b7d81f879d)  # 2020-11-16
  FetchContent_MakeAvailable(string_view_lite)
endif()

if(SIMPLE_TR8N_ENABLE_TESTS)
  FetchContent_Declare(googletest
      GIT_REPOSITORY https://github.com/google/googletest
      GIT_TAG b1fbd33c06cdb0024c67733c6fdec2009d17b384)  # 2020-11-24
  FetchContent_MakeAvailable(googletest)
endif()
