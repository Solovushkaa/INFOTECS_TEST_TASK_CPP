ifeq ($(origin CXX),default)
  CXX = g++
endif

ROOT_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

CXXFLAGS ?= -std=c++17 -Wall -Wpedantic -O2
LDFLAGS := -L$(ROOT_DIR)../liblogger/build -llogger
LDFLAGS += -Wl,-rpath,'$(ROOT_DIR)../liblogger/build'

BUILD_DIR ?= $(ROOT_DIR)build/
OBJ_DIR ?= $(BUILD_DIR)obj/

HEADERS := $(ROOT_DIR)include
CPPFLAGS := \
	-I$(HEADERS) \
	-I$(ROOT_DIR)../liblogger/include
SRC_DIR := $(ROOT_DIR)src/

RAW_SRC := \
	renderer.cpp \
	application.cpp \
	argsvalidator.cpp \
	worker.cpp \

SRC := $(addprefix $(SRC_DIR),$(RAW_SRC))
MAIN := main.cpp
LIB := ../liblogger/build/liblogger.so

OBJ := $(addprefix $(OBJ_DIR),$(RAW_SRC:.cpp=.o))
DEPS := $(OBJ:.o=.d)

TARGET ?= logger

.PHONY: all
all: $(TARGET)

$(TARGET): | $(BUILD_DIR) $(LIB) $(OBJ)
	@echo "=== Start of linking ==="
	$(CXX) $(OBJ) $(MAIN) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) -o $@
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

UNIT_CPP = $(TEST_DIR)unit.cpp
UNIT_OUT = $(TEST_DIR)unit

.PHONY: test
test: $(UNIT_OUT)

$(UNIT_OUT): $(UNIT_CPP) $(TARGET)
	$(CXX) $(OBJ) $< $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) -o $@

.PHONY: testrun
testrun: test
	$(UNIT_OUT) $(TEST_DIR)


.PHONY: testclean
testclean:
	@echo "=== Removing all test resources ==="
	rm -rf $(TEST_DIR)*.txt $(UNIT_OUT)
	@echo "=== Done ==="

.PHONY: clean
clean:
	@echo "=== Removing all temporary resources ==="
	rm -rf $(OBJ_DIR)
	@echo "=== Done ==="

.PHONY: distclean
distclean: testclean
	@echo "=== Removing all non-initial resources ==="
	rm -rf $(BUILD_DIR) $(TARGET) $(ROOT_DIR)*.txt
	@echo "=== Done ==="

-include $(DEPS)