srvFILES := server.cpp file_wrapper.cpp fileInterface.cpp consumer.cpp
cliFILES := client.cpp fileInterface.cpp
LIBS := -lIce -lIceUtil
INCDIR := -I. -I/home/liqing/software/Ice-3.5.0/cpp/include
LIBDIR := -L/home/liqing/software/Ice-3.5.0/cpp/lib
srvOBJ := server
cliOBJ := client

$(srvObj), $(cliOBJ):
	g++ $(srvFILES) $(LIBDIR) $(INCDIR) $(LIBS) -o $(srvOBJ) 
	g++ $(cliFILES) $(LIBDIR) $(INCDIR) $(LIBS) -o $(cliOBJ)

$(srvOBJ):
	g++ $(srvFILES) $(LIBDIR) $(INCDIR) $(LIBS) -o $(srvOBJ)
	
$(cliOBJ):
	g++ $(cliFILES) $(LIBDIR) $(INCDIR) $(LIBS) -o $(cliOBJ)
clean:
	rm $(srvOBJ)
	rm $(cliOBJ)