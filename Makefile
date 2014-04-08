Source=main.cpp Board.cpp
TARGET=2048
default:
	g++ -O3 $(Source) $(INCLUDES) -o $(TARGET) $(LIB) -std=c++0x
clean:
	rm 2048
