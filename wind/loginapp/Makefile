COMMON := ../common
LOG4CPLUS := ../log4cplus

LIB_DIR := /home/sointux/software/log4cplus-REL_1_1_2/src/.libs
INC_DIR := $(LOG4CPLUS)/include . $(COMMON)
LIBS := pthread log4cplus

TAR_DIR := .

EXECUTABLE := $(TAR_DIR)/loginapp

SRC := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp, %.o, $(SRC))
DEPS := $(patsubst %.o, %.d, $(OBJS))

CFALGS := -g -Wall
CXXFLAGS := $(CFALGS)
CPPFLAGS += $(addprefix -I, $(INC_DIR))

CPPFLGAS += -MD

$(EXECUTABLE) : $(OBJS)
	$(CXX) -o $(EXECUTABLE) $(OBJS) $(addprefix -L, $(LIB_DIR)) $(addprefix -l, $(LIBS))