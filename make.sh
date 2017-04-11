#!/bin/sh

g++ -O0 -g -Wall -std=c++14 main.cpp fs.cpp input.cpp scheduler.cpp process.cpp `pkg-config fuse --cflags --libs` -o fs

