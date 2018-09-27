#!/bin/bash
gcc -shared -fPIC -ldl -o inject1.so inject1.c
#linux
#LD_PRELOAD=$PWD/inject.so ./main
#mac
DYLD_INSERT_LIBRARIES=/Users/didi/c/echoServer/ld_preload/inject1.so DYLD_FORCE_FLAT_NAMESPACE=1 ./main
