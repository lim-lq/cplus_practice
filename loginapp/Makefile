FILES := loginapp.cc main.cc socket_wrapper.cc
OBJ := loginapp
LIBS := -lmysqlclient
INCS := -I$(BOOST_ROOT)

$(OBJ):
	g++ $(FILES) $(INCS) $(LIBS) -o $(OBJ)
	
clean:
	rm $(OBJ)