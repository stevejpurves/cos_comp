#!/bin/bash
emcc -o out/dctuncomp.js -s EXPORTED_FUNCTIONS="['_decompress']" -s ALLOW_MEMORY_GROWTH=1 mains/dctuncomp_fn.c libs/*