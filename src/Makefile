GCC = g++
CFLAGS = -Wall -Werror -Wextra -std=c++20
TEST_FLAGS = -lgtest -lpthread -lm
TEST_SRCS = backend/backend.cc backend/controller.cc test/test.cc
TEST_TARGET = test/testing_exe
TARGET = build/3d_viewer
COVERAGE_FLAGS = -fprofile-arcs -ftest-coverage

UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
	OPEN = xdg-open
else
	OPEN = open
endif

all: $(TARGET)

$(TARGET): install

install:
	rm -rf build
	mkdir build
	cd build && cmake ..
	cd build && make preinstall
	rm -rf build/.qt build/Makefile build/CMakeFiles build/3d_viewer_autogen build/*.cmake build/*.txt

install_with_gif:
	rm -rf build
	mkdir build
	cd build && cmake -DWITH_GIF_SUPPORT=ON ..
	cd build && make
	rm -rf build/.qt build/Makefile build/CMakeFiles build/3d_viewer_autogen build/*.cmake build/*.txt

test:
	rm -rf $(TEST_TARGET) prefs.txt
	$(GCC) $(CFLAGS) $(TEST_SRCS) -o $(TEST_TARGET) $(TEST_FLAGS)
	./$(TEST_TARGET)
	rm -rf prefs.txt

valgrind_check:
	rm -rf $(TEST_TARGET) prefs.txt
	$(GCC) $(CFLAGS) $(TEST_SRCS) -o $(TEST_TARGET) $(TEST_FLAGS)
	CK_FORK=no valgrind --vgdb=no --leak-check=full \
	--show-leak-kinds=all --track-origins=yes -v -q ./$(TEST_TARGET)
	rm -rf prefs.txt

uninstall:
	rm -rf build prefs.txt

clean:
	rm -rf *.tar build $(TEST_TARGET) coverage_report prefs.txt html latex

run:
	./$(TARGET)

dist: clean
	tar -cvf viewer.tar *

dvi:
	@doxygen info/Doxyfile
	@$(OPEN) html/index.html

gcov_report:
	@rm -rf *.gcov test/*.gcno test/*.gcda $(TEST_TARGET) *.info coverage_report prefs.txt
	@echo "┏====================================┓"
	@echo "┃     Generating your report ...     ┃"
	@echo "┗====================================┛"
	@$(GCC) $(CFLAGS) $(TEST_SRCS) $(COVERAGE_FLAGS) -o $(TEST_TARGET) $(TEST_FLAGS)
	@./$(TEST_TARGET)
	@gcov test/testing_exe-backend.gcda
	@lcov --capture --directory . --output-file coverage.info --ignore-errors inconsistent --include "*/backend/backend.cc"
	@genhtml coverage.info --output-directory coverage_report
	@$(OPEN) coverage_report/index.html
	@rm -rf *.gcov test/*.gcno test/*.gcda $(TEST_TARGET) *.info prefs.txt

format_code:
	@echo "┏=========================================┓"
	@echo "┃  Formatting your code for Google Style  ┃"
	@echo "┗=========================================┛"
	@cp ../materials/linters/.clang-format .
	@clang-format -i backend/* frontend/* test/*.cc main.cc
	@rm -rf .clang-format

style_check:
	@echo "┏=======================================┓"
	@echo "┃  Checking your code for Google Style  ┃"
	@echo "┗=======================================┛"
	@cp ../materials/linters/.clang-format .
	@clang-format -n backend/* frontend/* test/*.cc main.cc
	@rm -rf .clang-format

.PHONY: all install uninstall test clean dist run gcov_report style_check format_code