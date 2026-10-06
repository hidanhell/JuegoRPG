CXX ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra
BUILD_DIR := build/Debug
OBJECT_DIR := $(BUILD_DIR)/obj
TARGET := $(BUILD_DIR)/outDebug
SOURCES := $(filter-out test_save.cpp,$(wildcard *.cpp))
OBJECTS := $(patsubst %.cpp,$(OBJECT_DIR)/%.o,$(SOURCES))
DEPS := $(OBJECTS:.o=.d)

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(LDFLAGS) $^ -o $@

$(OBJECT_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -c $< -o $@

-include $(DEPS)
