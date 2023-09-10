#!/bin/bash
gcc test.c FileSystem.c Directory.c -o test `pkg-config fuse --cflags --libs`
