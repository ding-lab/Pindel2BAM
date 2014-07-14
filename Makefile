# This is the Makefile for the program that 
# converts Pindel output into a SAM file.
# 14 July 2014 -Adam D Scott

#CC is compiler to use
CC=g++

#CFLAGS is(are) compiler flags
CFLAGS=-c -Wall

p2s: pindel2sam.o
	$(CC) pindel2sam.o -o pin2sam

p2s.o: pindel2sam.cpp
	$(CC) $(CFLAGS) pindel2sam.cpp

clean:
	rm pindel2sam.o pin2sam
