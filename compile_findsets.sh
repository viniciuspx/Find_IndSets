#!/bin/bash 

echo "Compiling."

gcc -c findSets.c

gcc findSets.o -o findSets

echo "Compilation ready, cleaning all .o files and other files."

rm findSets.o

echo "Done."                      
