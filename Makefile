# \file Makefile
# Makefile for the FemtoFitting project

CXX ?= g++
AR ?= ar

LIBS   = $(shell root-config --libs) -lMinuit
CFLAGS = $(shell root-config --cflags) -Wall

CLASSES = LednickyEqn \
					LednickyInfo \
					PairSystem \
					ParameterConstraint \
					Fitter \

OBJ_FILES = $(addsuffix .o, ${CLASSES})

#IFLAGS = -I$(ROOTSYS)/include
.PHONY: clean

all: runMe

runMe: Faddeeva.o FemtoFitting.a Main.cxx
	g++ Main.cxx Faddeeva.o FemtoFitting.a -o runMe $(LIBS) $(CFLAGS)

FemtoFitting.a: ${OBJ_FILES}
	${AR} -r $@ $^

Faddeeva.o: Faddeeva.cc
	g++ -c Faddeeva.cc

%.o: %.cxx %.h
	${CXX} ${CFLAGS} -c $< -o $@

clean:
	rm -f runMe *.o
