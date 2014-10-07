FILE := main.cc server.cc
OBJ := chat_server
LIBS := -lpthread
CC := g++

$(OBJ):
	$(CC) $(FILE) $(LIBS) -o $(OBJ)
	
clean:
	rm $(OBJ)