CMAKE=cmake
BUILD_SRC = .
BUILD_DIR = build
BUILD_DIR_TEST = $(BUILD_DIR)/test
BUILD_TYPE = debug
ENABLE_COVERAGE = ON
COVERAGE_REPORT_FILE_NAME = coverage.xml

build:
	$(CMAKE) -S $(BUILD_SRC) -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) -DENABLE_COVERAGE=$(ENABLE_COVERAGE)
	$(CMAKE) --build $(BUILD_DIR) --config $(BUILD_TYPE)

check:
	$(BUILD_DIR_TEST)/lru_cache_tests

check-with-coverage:
	cd $(BUILD_DIR) && ctest -C $(BUILD_TYPE)
	cd $(BUILD_DIR) && gcovr -r ../ --print-summary --xml-pretty --xml $(COVERAGE_REPORT_FILE_NAME) $(BUILD_SRC) -e ".*/test/.*"

clean:
	@rm -rf $(BUILD_DIR)
