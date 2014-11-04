ROOT := ../../wind
COMMON := ../common
LOG4CPLUS := ../../log4cplus

LIB_DIR := $(LOG4CPLUS)/lib
INC_DIR := $(COMMON) $(ROOT)/network $(ROOT)/mysql $(ROOT)/thread . $(LOG4CPLUS)/include

LIBS := pthread log4cplus mysqlclient

SRC := $(wildcard *.cpp) $(wildcard $(ROOT)/network/*.cpp) $(wildcard $(ROOT)/mysql/*.cpp) $(wildcard $(ROOT)/thread/*.cpp)
OBJS := $(patsubst %.cpp, %.o, $(SRC))

CFLAGS := -g -Wall
CXXFLAGS := $(CFLAGS)
CPPFLAGS := $(addprefix -I, $(INC_DIR))

TAR_DIR := ./
EXECUTABLE := $(TAR_DIR)/dbmgr

$(EXECUTABLE) : $(OBJS)
	rm -f $@
	$(CXX) -o $(EXECUTABLE) $(OBJS) $(addprefix -l, $(LIBS)) $(addprefix -L, $(LIB_DIR)) $(addprefix -I, $(INC_DIR))