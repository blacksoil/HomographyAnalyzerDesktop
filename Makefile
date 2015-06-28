CFLAGS=`pkg-config --cflags opencv`
LIBS=`pkg-config --libs opencv`

builk:
	g++ $(CFLAGS) *.cc *.cpp $(LIBS) -o main

clean:
	rm -rf *.o main

default: builk
