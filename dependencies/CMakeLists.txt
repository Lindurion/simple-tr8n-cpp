include(FetchContent)

FetchContent_Declare(gsl
    GIT_REPOSITORY https://github.com/microsoft/GSL
    GIT_TAG f09b24970dfed112c57c3d21cf3b90273c9f7ff2)  # 2021-09-15
FetchContent_MakeAvailable(gsl)

if(SIMPLE_TR8N_REQUIRES_STRING_VIEW_LITE)
  FetchContent_Declare(string_view_lite
      GIT_REPOSITORY https://github.com/martinmoene/string-view-lite
      GIT_TAG e644cfd1889ef31a821bfc06c87a34caa26a0b45)  # 2021-08-31
  FetchContent_MakeAvailable(string_view_lite)
endif()

if(SIMPLE_TR8N_ENABLE_TESTS)
  FetchContent_Declare(googletest
      GIT_REPOSITORY https://github.com/google/googletest
      GIT_TAG 40dfd4b775a66979ad1bd19321cdfd0feadfea27)  # 2021-09-16
  FetchContent_MakeAvailable(googletest)
endif()
