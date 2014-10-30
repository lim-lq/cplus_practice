ROOT := ../../../wind
COMMON_DIR := $(ROOT)/common
LOG4CPLUS_DIR := $(ROOT)/log4cplus
INC_DIR :=  $(LOG4CPLUS_DIR)/include $(COMMON_DIR)
LIB_DIR := /home/sointux/software/log4cplus-REL_1_1_2/src/.libs

libs := log4cplus

tag_dir := .
EXECUTABLE := $(tag_dir)/test

SRC := $(wildcard *.cpp) $(wildcard ../*.cpp)
OBJS := $(patsubst %.cpp, %.o, $(SRC))
CLI_OBJS := $(patsubst %.cpp, %.o, $(wildcard *.cpp))

CFLAGS := -g -Wall
CXXFLAGS := $(CFLAGS)
CXXFLAGS += $(addprefix -I, $(INC_DIR))

$(EXECUTABLE) : $(OBJS)
	rm -f $@
	$(CXX) -o $(EXECUTABLE) $(CLI_OBJS) $(addprefix -l, $(libs)) $(addprefix -L, $(LIB_DIR))