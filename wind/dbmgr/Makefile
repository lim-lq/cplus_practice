ROOT := ../../wind
COMMON := ../common
LOG4CPLUS := ../log4cplus
BOOST := /home/sointux/software/boost_1_56_0
LIB_DIR := /home/sointux/software/log4cplus-REL_1_1_2/src/.libs
INC_DIR := $(COMMON) $(ROOT)/network $(ROOT)/mysql $(ROOT)/thread . $(LOG4CPLUS)/include $(ROOT)/encryptor $(BOOST)

LIBS := pthread log4cplus mysqlclient crypto

SRC := $(wildcard *.cpp) $(wildcard $(ROOT)/network/*.cpp) $(wildcard $(ROOT)/mysql/*.cpp) $(wildcard $(ROOT)/thread/*.cpp) $(wildcard $(ROOT)/encryptor/*.cpp)
OBJS := $(patsubst %.cpp, %.o, $(SRC))

CFLAGS := -g -Wall
CXXFLAGS := $(CFLAGS)
CPPFLAGS := $(addprefix -I, $(INC_DIR))

TAR_DIR := ./
EXECUTABLE := $(TAR_DIR)/dbmgr

$(EXECUTABLE) : $(OBJS)
	rm -f $@
	$(CXX) -o $(EXECUTABLE) $(OBJS) $(addprefix -l, $(LIBS)) $(addprefix -L, $(LIB_DIR)) $(addprefix -I, $(INC_DIR))

clean:
	rm -f $(OBJS)	