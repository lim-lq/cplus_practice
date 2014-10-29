ROOT := ../../../wind
COMMON_DIR := $(ROOT)/common
LOG4CPLUS_DIR := $(ROOT)/log4cplus
INC_DIR :=  $(LOG4CPLUS_DIR)/include $(COMMON_DIR)
LIB_DIR := $(LOG4CPLUS_DIR)/lib

libs := log4cplus

tag_dir := .
EXECUTABLE := $(tag_dir)/test

SRC := $(wildcard *.cpp) $(wildcard ../*.cpp)
OBJS := $(patsubst %.cpp, %.o, $(SRC))

CFLAGS := -g -Wall
CXXFLAGS := $(CFLAGS)
CXXFLAGS += $(addprefix -I, $(INC_DIR))

$(EXECUTABLE) : $(OBJS)
	rm -f $@
	$(CXX) -o $(EXECUTABLE) $(OBJS) $(addprefix -l, $(libs)) $(addprefix -L, $(LIB_DIR))