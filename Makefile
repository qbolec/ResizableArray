HELPERS = RegularVector.o\
	  RegularVectorFactory.o\
	  WaterlooVector.o\
	  WaterlooVectorFactory.o

COMMON = Common.h\
	 AVector.h\
	 AVectorFactory.h\
	 SuccinctVectorFactory.h\
	 SuccinctVector.h\
	 RegularVectorFactory.h\
	 WaterlooVectorFactory.h

all : SpeedTest SpeedTable MemoryTest MemoryTable SpeedTest

CC=g++

SpeedTest.o : SpeedTest.cpp $(COMMON)

SpeedTable.o : SpeedTable.cpp $(COMMON)

MemoryTest.o : MemoryTest.cpp $(COMMON)

MemoryTable.o : MemoryTable.cpp $(COMMON)


SpeedTest: SpeedTest.o $(HELPERS)

SpeedTable: SpeedTable.o $(HELPERS)

MemoryTest: MemoryTest.o $(HELPERS)

MemoryTable: MemoryTable.o $(HELPERS)

