include(FetchContent)

FetchContent_Declare(gsl
    GIT_REPOSITORY https://github.com/microsoft/GSL
    GIT_TAG 0140ab1d73d833d2b1eca37d1b3b91e52dda4176)  # 2020-11-23
FetchContent_MakeAvailable(gsl)

if(SIMPLE_TR8N_REQUIRES_ABSL)
  FetchContent_Declare(absl
      GIT_REPOSITORY https://github.com/abseil/abseil-cpp
      GIT_TAG 592924480acf034aec0454160492a20bccdbdf3e)  # 2020-12-01
  FetchContent_MakeAvailable(absl)
endif()

if(SIMPLE_TR8N_ENABLE_TESTS)
  FetchContent_Declare(googletest
      GIT_REPOSITORY https://github.com/google/googletest
      GIT_TAG b1fbd33c06cdb0024c67733c6fdec2009d17b384)  # 2020-11-24
  FetchContent_MakeAvailable(googletest)
endif()
