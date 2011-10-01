#! /bin/make

schemer: tokenizer.o parser.o environment.o schemer.o main.o exceptions.o
	$(CXX) -o schemer tokenizer.o parser.o environment.o schemer.o main.o

main.o: main.cpp parser.h exceptions.h schemer.h environment.h
	$(CXX) -c main.cpp

exceptions.o: exceptions.cpp exceptions.h
	$(CXX) -c exceptions.cpp

schemer.o: schemer.cpp parser.h exceptions.h schemer.h environment.h
	$(CXX) -c schemer.cpp

tokenizer.o: tokenizer.cpp tokenizer.h exceptions.h
	$(CXX) -c tokenizer.cpp

environment.o: environment.cpp environment.h exceptions.h
	$(CXX) -c environment.cpp

parser.o: parser.cpp parser.h environment.h tokenizer.h exceptions.h
	$(CXX) -c parser.cpp

.PHONY: clean
clean:
	$(RM) tokenizer.o parser.o environment.o schemer.o main.o exceptions.o


