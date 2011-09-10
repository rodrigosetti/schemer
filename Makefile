#! /bin/make

tokenizer.o: tokenizer.c tokenizer.h
	$(CC) -c tokenizer.c

parser.o: parser.c parser.h
	$(CC) -c parser.c
