#!/bin/bash

gcc -c ./asm.s
objdump -d ./asm.o > ./asm.d
