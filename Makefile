LIBNAME := D1316Uuid

ifeq ($(OS),Windows_NT)
    TARGET := D1316Uuid.dll
    IMPORT_LIB := libD1316Uuid.a
    TEST_EXE := D1316Uuid_test_exec.exe
    RUN_TEST := .\$(TEST_EXE)
    LIB_TEST_EXE := D1316Uuid_lib_test_exec.exe
    RUN_LIB_TEST := .\$(LIB_TEST_EXE)
    LDFLAGS := -shared -Wl,--out-implib,$(IMPORT_LIB)
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Darwin)
        TARGET := libD1316Uuid.dylib
        LDFLAGS := -shared -fPIC -install_name @rpath/$(TARGET)
    else
        TARGET := libD1316Uuid.so
        LDFLAGS := -shared -fPIC
    endif
    TEST_EXE := D1316Uuid_test_exec
    RUN_TEST := ./$(TEST_EXE)
    LIB_TEST_EXE := D1316Uuid_lib_test_exec
    RUN_LIB_TEST := env LD_LIBRARY_PATH=. DYLD_LIBRARY_PATH=. ./$(LIB_TEST_EXE)
endif

CXX ?= g++
CXXFLAGS ?= -pipe -O2 -std=c++17 -Wall -Werror -Wpointer-arith -Wno-unused-parameter
INC := -I.
SRC := D1316Uuid.cpp
TEST_SRC := D1316Uuid_test.cpp
LIB_TEST_SRC := D1316Uuid_test.cpp

.PHONY: all build test test-unit test-library clean info

build: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(INC) $(LDFLAGS) $^ -o $@

OBJ := $(SRC:.cpp=.o) $(TEST_SRC:.cpp=.o)

$(TEST_EXE): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(TEST_EXE)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -DD1316UUID_TEST_UNIT $(INC) -c $< -o $@

$(LIB_TEST_EXE): $(LIB_TEST_SRC) $(TARGET)
	$(CXX) $(CXXFLAGS) $(INC) $< -o $@ -L. -l$(LIBNAME)

test: test-unit test-library

test-unit: $(TEST_EXE)
	$(RUN_TEST)
	rm -f $(OBJ) $(TEST_EXE)

test-library: $(LIB_TEST_EXE)
ifdef IMPORT_LIB
	rm -f $(IMPORT_LIB)
endif
	$(RUN_LIB_TEST)
	rm -f $(LIB_TEST_EXE)

clean:
	rm -f $(TARGET) $(IMPORT_LIB) $(LIB_TEST_EXE) $(TEST_EXE) $(OBJ)

info:
	@echo "OS       : $(if $(OS),Windows,$(UNAME_S))"
	@echo "Compiler : $(CXX)"
	@echo "Target   : $(TARGET)"
	@echo "LDFLAGS  : $(LDFLAGS)"