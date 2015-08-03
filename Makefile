# \file Makefile
# Makefile for the FemtoFitting project

LIBS   = $(shell root-config --libs) -lMinuit
CFLAGS = $(shell root-config --cflags) -Wall

CXX ?= g++
AR ?= ar


CLASSES = LednickyEqn \
					LednickyInfo \
					PairSystem \
					ParameterConstraint \
					Fitter \

OBJ_FILES = $(addsuffix .o, ${CLASSES})

.PHONY: all clean

all: runMe

runMe: Main.cxx Faddeeva.o FemtoFitting.a
	g++  ${CFLAGS} $< Faddeeva.o FemtoFitting.a -o $@ ${LIBS}

FemtoFitting.a: ${OBJ_FILES}
	${AR} -r $@ $^

Faddeeva.o: Faddeeva.cc
	g++ -c Faddeeva.cc

%.o: %.cxx %.h
	${CXX} ${CFLAGS} -c $< -o $@

clean:
	rm -f runMe *.o
