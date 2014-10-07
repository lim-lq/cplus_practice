FILES := main.cpp dbmgr.cpp mysql.cpp
LIBS := -lmysqlclient
INCS := -I$(BOOST_ROOT)
OBJ := dbmgr

$(OBJ):
	g++ $(FILES) $(LIBS) $(INCS) -o $(OBJ)
	
clean:
	rm $(OBJ)