#!/bin/sh

g++ -O0 -g -Wall -std=c++14 main.cpp manager.cpp setupper.cpp fs.cpp input.cpp scheduler.cpp process.cpp `pkg-config fuse libuv --cflags --libs` -o hog
