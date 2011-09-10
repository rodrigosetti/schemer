#! /bin/make

tokenizer.o: tokenizer.c tokenizer.h
	$(CC) -c tokenizer.c

