include(FetchContent)
include(SimpleTr8nDeps)

FetchContent_Declare(gsl
    GIT_REPOSITORY https://github.com/microsoft/GSL
    GIT_TAG f09b24970dfed112c57c3d21cf3b90273c9f7ff2)  # 2021-09-15
simple_tr8n_make_dep_available(gsl)
simple_tr8n_mark_dir_targets_as_external(${gsl_SOURCE_DIR})

if(SIMPLE_TR8N_REQUIRES_STRING_VIEW_LITE)
  FetchContent_Declare(string_view_lite
      GIT_REPOSITORY https://github.com/martinmoene/string-view-lite
      GIT_TAG e644cfd1889ef31a821bfc06c87a34caa26a0b45)  # 2021-08-31
  simple_tr8n_make_dep_available(string_view_lite)
  simple_tr8n_mark_dir_targets_as_external(${string_view_lite_SOURCE_DIR})
endif()

if(SIMPLE_TR8N_ENABLE_TESTS)
  FetchContent_Declare(googletest
      GIT_REPOSITORY https://github.com/google/googletest
      GIT_TAG e4717df71a4f45bf9f0ac88c6cd9846a0bc248dd)  # 2021-09-23
  simple_tr8n_make_dep_available(googletest)
endif()
