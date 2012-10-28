CC=g++
CXXFLAGS=--std=c++11

prefix=/opt/local

.PHONEY: all clean

all: goban-test goban-bench

goban-test: goban.o goban-test.o ${prefix}/lib/libboost_unit_test_framework-mt.a
goban-bench: goban.o goban-bench.o

goban-bench.o: goban-bench.cpp goban.hpp hikago002b.h
goban-test.o: goban-test.cpp goban.hpp
goban.o: goban.cpp goban.hpp

clean:
	-rm -f *.o goban-test goban-bench *~
