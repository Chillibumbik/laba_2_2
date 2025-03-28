# === Компилятор и флаги ===
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude

# === Папки и пути ===
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
BIN = main

# === Исходники и объектные файлы ===
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# === Цель по умолчанию ===
all: $(BIN)

# Сборка object-файлов из src
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Сборка main
$(BIN): $(OBJS) main.cpp
	$(CXX) $(CXXFLAGS) main.cpp $(OBJS) -o $(BIN)

# === Команды пользователя ===
run: $(BIN)
	./$(BIN)

rebuild: del all

del:
	rm -rf $(OBJ_DIR)/*.o $(BIN)

# === Дополнительно ===
.PHONY: all run rebuild del
