LIBS=-lboost_filesystem -lboost_system -lpthread
CC=clang++
CFLAGS=-stdlib=libc++ -std=c++14

all: generator classificator

generator:
	$(CC) $(CFLAGS) mainGenerator.cpp -o generator $(LIBS)

classificator:
	$(CC) $(CFLAGS) mainClassificator.cpp KNNClassifier.cpp -o classificator
