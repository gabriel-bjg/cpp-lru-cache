find_package(Catch2 3 REQUIRED)

include(CTest)
include(Catch)

add_executable(lru_cache_tests lru_cache_tests.cpp)
target_link_libraries(lru_cache_tests PRIVATE project_warnings project_options Catch2::Catch2WithMain)

catch_discover_tests(
  lru_cache_tests
  TEST_PREFIX "lru_cache_tests."
  REPORTER xml
  OUTPUT_DIR .
  OUTPUT_PREFIX "lru_cache_tests."
  OUTPUT_SUFFIX .xml)
