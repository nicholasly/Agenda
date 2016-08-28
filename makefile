CC := g++
FLAGS := -std=c++11 -w -g -Wall
INC_DIR := include
SRC_DIR := src
BUILD_DIR := build
BIN_DIR := bin
INCLUDE := -I./$(INC_DIR)

$(BIN_DIR)/Agenda: $(BUILD_DIR)/Agenda.o $(BUILD_DIR)/AgendaUI.o $(BUILD_DIR)/AgendaService.o $(BUILD_DIR)/Storage.o $(BUILD_DIR)/Meeting.o $(BUILD_DIR)/Date.o $(BUILD_DIR)/User.o
	$(CC) $(FLAGS) $(INCLUDE) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(FLAGS) $(INCLUDE) -c -o $@ $<

clean:
	rm -rf $(BUILD_DIR)/*.o
	rm -rf $(BIN_DIR)/Agenda


