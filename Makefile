CC = g++
MEX_CC = mex
LIBS = -L/usr/local/lib -lxerces-c
FLAGS = -Wall -ggdb -fPIC -g -DDEBUG
MAT_FLAGS = -cxx -largeArrayDims
INCLUDES = -I/usr/local/include -I/usr/local/matlab/extern/include

SAXParserMAT: SAXParser.o MovieSAXHandler.o Tokenizer.o
	$(MEX_CC) $(MAT_FLAGS) $(LIBS) $(INCLUDES) $^

SAXParser.o: SAXParser.cpp MovieSAXHandler.hpp InvertedIndex.hpp
	$(CC) -DMAT $(FLAGS) $(LIBS) $(INCLUDES) -c $<

MovieSAXHandler.o: MovieSAXHandler.cpp MovieSAXHandler.hpp EntityTree.hpp Node.hpp RepresentationStrategy.hpp xerces_strings.hpp InvertedIndex.hpp Exceptions.hpp NodeCollection.hpp
	$(CC) $(FLAGS) $(LIBS) $(INCLUDES) -c $<

Tokenizer.o: Tokenizer.cpp Tokenizer.hpp
	$(CC) $(FLAGS) $(LIBS) $(INCLUDES) -c $<

.PHONY: clean
clean: 
	rm SAXParser MovieSAXHandler.o SAXParser.o Tokenizer.o
