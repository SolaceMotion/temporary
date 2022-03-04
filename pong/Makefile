# define cpp compiler
CXX		  := g++

# define compile time flags
CXX_FLAGS := -Wall -Wextra -std=c++17 -ggdb

# define bin directory
BIN		:= bin

# define source directory
SRC		:= src

# define include directory
INCLUDE	:= include

# define lib directory
LIB		:= lib

# define library paths in addition to /usr/lib
#   if I wanted to include libraries not in /usr/lib I'd specify
#   their path using -Lpath, something like:
LIBRARIES	:=

# define main exectutable
EXECUTABLE	:= main


all: compile link 

compile:
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -c main.cpp

link:
	$(CXX) main.o -o main -L$(LIB) -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system
