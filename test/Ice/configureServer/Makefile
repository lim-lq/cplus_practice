FILES := server.cpp configure_wrapper.cpp configureServerInterface.cpp
ICE_HOME := /opt/Ice-3.5.1
LIBDIR := $(ICE_HOME)/lib
INCDIR := -I. -I$(ICE_HOME)/include
LIBS := -lIce -lIceUtil
OBJ := server

$(OBJ):
	g++ $(FILES) -L$(LIBDIR) $(INCDIR) $(LIBS) -o $(OBJ)

clean:
	rm $(OBJ)