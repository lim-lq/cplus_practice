LIB_DIR := .
INC_DIR := .
LIBS := mysqlclient

TAR_DIR := .

EXECUTABLE := mysqlclient

SRC := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp, %.o, $(SRC))

CFLAGS := -g -Wall
CXXFLAGS := $(CFLAGS)
CPPFLAGS += $(addprefix -I, $(INC_DIR))

$(EXECUTABLE) : $(OBJS)
	rm -f $@
	$(CXX) -o $(EXECUTABLE) $(OBJS) $(addprefix -L, $(LIB_DIR)) $(addprefix -l, $(LIBS))