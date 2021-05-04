include(FetchContent)

FetchContent_Declare(gsl
    GIT_REPOSITORY https://github.com/microsoft/GSL
    GIT_TAG ef0ffefe525a6219ff245d19a832ce06f3fd3504)  # 2021-02-24
FetchContent_MakeAvailable(gsl)

if(SIMPLE_TR8N_REQUIRES_STRING_VIEW_LITE)
  FetchContent_Declare(string_view_lite
      GIT_REPOSITORY https://github.com/martinmoene/string-view-lite
      GIT_TAG ad19de8fa2e37c4fd6fb9d0112866ad9a717b790)  # 2021-05-02
  FetchContent_MakeAvailable(string_view_lite)
endif()

if(SIMPLE_TR8N_ENABLE_TESTS)
  FetchContent_Declare(googletest
      GIT_REPOSITORY https://github.com/google/googletest
      GIT_TAG f5e592d8ee5ffb1d9af5be7f715ce3576b8bf9c4)  # 2021-04-29
  FetchContent_MakeAvailable(googletest)
endif()
