ifeq ($(origin CXX),default)
  CXX = g++
endif

CXXFLAGS ?= -std=c++17 -Wall -Wpedantic -fPIC -O2
LDFLAGS := -shared

ROOT_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
BUILD_DIR := $(ROOT_DIR)build/
OBJ_DIR := $(BUILD_DIR)obj/

HEADERS := $(ROOT_DIR)include
CPPFLAGS := -I$(HEADERS)
SRC_DIR := $(ROOT_DIR)src/

RAW_SRC := \
	logger.cpp \
	fileverification.cpp \
	loglevel.cpp
SRC := $(addprefix $(SRC_DIR),$(RAW_SRC))

OBJ := $(addprefix $(OBJ_DIR),$(RAW_SRC:.cpp=.o))
DEPS := $(OBJ:.o=.d)

TARGET := $(BUILD_DIR)liblogger.so

.PHONY: all
all: $(TARGET)

$(TARGET): | $(BUILD_DIR) $(OBJ) 
	@echo "=== Start of linking ==="
	$(CXX) $(OBJ) $(CPPFLAGS) $(LDFLAGS) -o $@
	@echo "=== Done ==="

$(BUILD_DIR):
	@mkdir -p $(@D)

$(OBJ): $(OBJ_DIR)%.o : $(SRC_DIR)%.cpp | $(OBJ_DIR)
	@echo "=== Compiling $(@F) ==="
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -MMD -c $< -o $@
	@echo "=== Done ==="

$(OBJ_DIR):
	@mkdir -p $(@D)

TEST_DIR := $(ROOT_DIR)test/
TEST_FILES_DIR := $(TEST_DIR)files/

PR_TEST_FILE := $(TEST_FILES_DIR)prFile.txt
PUB_TEST_FILE := $(TEST_FILES_DIR)pubFile.txt

UNIT_CPP = $(TEST_DIR)unit.cpp
UNIT_OUT = $(TEST_DIR)unit

.PHONY: test
test: $(TEST_FILES_DIR) $(UNIT_OUT) 
	
$(UNIT_OUT): $(UNIT_CPP) $(TARGET)
	$(CXX) $< $(CXXFLAGS) $(CPPFLAGS) $(TARGET) -o $@

.PHONY: testrun
testrun: test
	$(UNIT_OUT) $(PR_TEST_FILE) $(PUB_TEST_FILE)

$(TEST_FILES_DIR):
	@mkdir -p $(@D)
	@touch $(PR_TEST_FILE)
	@chmod 006 $(PR_TEST_FILE)
	@touch $(PUB_TEST_FILE)
	@chmod 666 $(PUB_TEST_FILE)

.PHONY: clean
clean:
	@echo "=== Removing all temporary resources ==="
	rm -rf $(OBJ_DIR)
	@echo "=== Done ==="

.PHONY: testclean
testclean:
	@echo "=== Removing all test resources ==="
	rm -rf $(TEST_FILES_DIR) $(UNIT_OUT)
	@echo "=== Done ==="

.PHONY: distclean
distclean: testclean
	@echo "=== Removing all non-initial resources ==="
	rm -rf $(BUILD_DIR) 
	@echo "=== Done ==="


-include $(DEPS)