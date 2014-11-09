ROOT := ../../wind
COMMON := ../common
LOG4CPLUS := ../log4cplus
BOOST := /home/sointux/software/boost_1_56_0
LIB_DIR := /home/sointux/software/log4cplus-REL_1_1_2/src/.libs
INC_DIR := $(LOG4CPLUS)/include . $(COMMON) $(BOOST) $(ROOT)/thread $(ROOT)/encryptor $(ROOT)/network
LIBS := pthread log4cplus crypto

TAR_DIR := .

EXECUTABLE := $(TAR_DIR)/loginapp
CLIENT := test/client

CLIENT_SRC := $(wildcard test/*.cpp) $(wildcard $(ROOT)/encryptor/*.cpp) $(wildcard $(ROOT)/network/*.cpp)
CLIENT_OBJS := $(patsubst %.cpp, %.o, $(CLIENT_SRC))

SRC := $(wildcard *.cpp) $(wildcard $(ROOT)/thread/*.cpp) $(wildcard $(ROOT)/encryptor/*.cpp) $(wildcard $(ROOT)/network/*.cpp)
OBJS := $(patsubst %.cpp, %.o, $(SRC))
DEPS := $(patsubst %.o, %.d, $(OBJS))

CFALGS := -g -Wall
CXXFLAGS := $(CFALGS)
CPPFLAGS += $(addprefix -I, $(INC_DIR))

CPPFLGAS += -MD



$(EXECUTABLE) : $(OBJS)
	rm -f $@
	$(CXX) -o $(EXECUTABLE) $(OBJS) $(addprefix -L, $(LIB_DIR)) $(addprefix -l, $(LIBS))

$(CLIENT) : $(CLIENT_OBJS)
	rm -f $@
	$(CXX) -o $(CLIENT) $(CLIENT_OBJS) $(addprefix -L, $(LIB_DIR)) $(addprefix -l, $(LIBS)) $(addprefix -I, $(INC_DIR))

clean:
	rm -f $(OBJS)	