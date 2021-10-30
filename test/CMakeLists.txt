find_package(Catch2 3 REQUIRED)

include(CTest)
include(Catch)

add_executable(tests lru_cache_tests.cpp)
target_link_libraries(tests PRIVATE project_warnings project_options Catch2::Catch2WithMain)

catch_discover_tests(
  tests
  TEST_PREFIX "tests."
  REPORTER xml
  OUTPUT_DIR .
  OUTPUT_PREFIX "tests."
  OUTPUT_SUFFIX .xml)
