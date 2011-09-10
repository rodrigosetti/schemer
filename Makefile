#! /bin/make

tokenizer.o: tokenizer.cpp tokenizer.h
	$(CXX) -c tokenizer.cpp

environment.o: environment.cpp environment.h
	$(CXX) -c environment.cpp

parser.o: parser.cpp parser.h environment.h tokenizer.h
	$(CXX) -c parser.cpp
