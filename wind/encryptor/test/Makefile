LIB_DIR := . ../
INC_DIR := . ../

LIBS := crypto

SRC := $(wildcard *.cpp) $(wildcard ../*.cpp)
OBJS := $(patsubst %.cpp, %.o, $(SRC))

CFLAGS := -g -Wall
CXXFLAGS := $(CFLAGS)
CPPFLAGS += $(addprefix -I, $(INC_DIR))

TAR_DIR := .
EXECUTABLE := $(TAR_DIR)/test

$(EXECUTABLE) : $(OBJS)
	$(CXX) -o $(EXECUTABLE) $(OBJS) $(addprefix -L, $(LIB_DIR)) $(addprefix -l, $(LIBS))