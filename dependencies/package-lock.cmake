# CPM Package Lock
# This file should be committed to version control

# gsl
CPMDeclarePackage(gsl
  NAME gsl
  VERSION 4.0.0
  GIT_TAG a3534567187d2edc428efd3f13466ff75fe5805c
  GIT_REPOSITORY https://github.com/microsoft/GSL
  EXCLUDE_FROM_ALL
)
# string_view_lite
CPMDeclarePackage(string_view_lite
  NAME string_view_lite
  VERSION 1.7.0
  GIT_TAG 7b1867b7c08f9ac010fb8b9dae79ff8c8359bafb
  GIT_REPOSITORY https://github.com/martinmoene/string-view-lite
  EXCLUDE_FROM_ALL
)
# googletest
CPMDeclarePackage(googletest
  NAME googletest
  VERSION 1.12.1
  GIT_TAG 58d77fa8070e8cec2dc1ed015d66b454c8d78850
  GIT_REPOSITORY https://github.com/google/googletest
  EXCLUDE_FROM_ALL
)
