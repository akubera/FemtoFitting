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
					ArgParse \

OBJ_FILES = $(addsuffix .o, ${CLASSES})
.PHONY: clean

all: runMe

runMe: Faddeeva.o FemtoFitting.a Main.cxx
	${CXX} -o $@ Main.cxx Faddeeva.o FemtoFitting.a $(LIBS) $(CFLAGS)

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
