SRC_DIR := src
SRCS    := $(wildcard $(SRC_DIR)/*.cpp)
HEADERS := $(wildcard $(SRC_DIR)/*.hpp)
BIN_DIR := bin
OBJS    := $(addprefix $(BIN_DIR)/, $(notdir $(SRCS:.cpp=.o)))

LIBS    := winmm
LINKS   := $(addprefix -l, $(LIBS))

CXX := g++
CXX_FLAGS := -Wall

MKDIR := mkdir

EXECUTABLE = test.exe

all : $(EXECUTABLE)

test :
	echo $(SRCS)
	echo $(OBJS)
	echo $(LINKS)

$(EXECUTABLE) : $(OBJS)
	$(CXX) $(CXX_FLAGS) $^ -o $@ $(LINKS)

$(BIN_DIR)/%.o : $(SRC_DIR)/%.cpp $(HEADERS) | $(BIN_DIR)
	$(CXX) -c $(CXX_FLAGS) $< -o $@
	
$(BIN_DIR) :
	$(MKDIR) $@