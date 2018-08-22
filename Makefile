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
					ArgParse \

OBJ_FILES = $(addsuffix .o, ${CLASSES})

.PHONY: all clean test

all: runMe runLednicky

runMe: Main.cxx Faddeeva.o FemtoFitting.a
	${CXX} ${CFLAGS} $< Faddeeva.o FemtoFitting.a -o $@ ${LIBS}

runLednicky: RunLednicky.cxx Faddeeva.o FemtoFitting.a
	${CXX} ${CFLAGS} $< Faddeeva.o FemtoFitting.a -o $@ ${LIBS}

FemtoFitting.a: ${OBJ_FILES}
	${AR} -r $@ $^

Faddeeva.o: Faddeeva.cc Faddeeva.hh
	${CXX} -c Faddeeva.cc

%.o: %.cxx %.h
	${CXX} ${CFLAGS} -c $< -o $@

test.exe: FemtoFitting.a tests/*
	${CXX} ${CFLAGS} -Itests/bandit -I. tests/main.cxx -o $@ FemtoFitting.a ${LIBS}

test: test.exe
	./test.exe

clean:
	rm -f runMe *.o
