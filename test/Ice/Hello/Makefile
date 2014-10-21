FILES := client.cpp HelloI.cpp Hello.cpp
ICE_HOME := /home/liqing/software/Ice-3.5.0/cpp
INC_DIR := $(ICE_HOME)/include
LIB_DIR := $(ICE_HOME)/lib
LIBS := -lIceUtil -lIce

SRVOBJ := client
CLIOBJ := client

$(SRVOBJ):
	g++ $(FILES) -I. -I$(INC_DIR) -L$(LIB_DIR) $(LIBS) -o $(SRVOBJ)
	
clean:
	rm $(SRVOBJ)