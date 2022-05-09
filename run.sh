#!/bin/fish

./bfc test.bf > tmp.asm
nasm -felf64 tmp.asm
ld -o tmp tmp.o
./tmp
