#!/bin/bash

emcc -o out/dctuncomp.js -s EXPORTED_FUNCTIONS="['_decompress']" -v mains/dctuncomp_fn.c libs/*