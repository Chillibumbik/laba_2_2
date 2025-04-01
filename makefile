# Компилятор и флаги
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude -Itest

# Каталоги
SRC_DIR = src
INC_DIR = include
BIN_DIR = bin
OBJ_DIR = obj
TEST_DIR = test

# Основные файлы
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
SRC_OBJ_FILES = $(SRC_FILES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Тестовые файлы
TEST_FILES = $(wildcard $(TEST_DIR)/*.cpp)
TEST_OBJ_FILES = $(TEST_FILES:$(TEST_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Цели
TARGET = $(BIN_DIR)/program
TEST_TARGET = $(BIN_DIR)/test_program

# Основные цели
.PHONY: all run clean rebuild test

all: $(TARGET)

# Сборка основной программы
$(TARGET): $(SRC_OBJ_FILES) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Сборка объектников из исходников
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Сборка объектников из тестов
$(OBJ_DIR)/%.o: $(TEST_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Каталоги
$(BIN_DIR):
	@mkdir $(BIN_DIR) 2>nul || exit 0

$(OBJ_DIR):
	@mkdir $(OBJ_DIR) 2>nul || exit 0

# Запуск основной программы
run: $(TARGET)
	./$(TARGET)

# Сборка и запуск тестов
test: $(TEST_TARGET)
	./$(TEST_TARGET)

# Исключаем main.o из линковки тестов
SRC_OBJ_TEST = $(filter-out $(OBJ_DIR)/main.o, $(SRC_OBJ_FILES))

$(TEST_TARGET): $(SRC_OBJ_TEST) $(TEST_OBJ_FILES) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Очистка
clean:
	@if exist $(OBJ_DIR) rmdir /S /Q $(OBJ_DIR)
	@if exist $(BIN_DIR) rmdir /S /Q $(BIN_DIR)

# Пересборка
rebuild: clean all
