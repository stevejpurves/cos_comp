#Standalone Cosine Compression

This repo transpiles a standalone version of the SU tools for cosine compression into JavaScript. There is a C version of the tools under the folder "c".

To compile with Emscripten (https://github.com/kripken/emscripten) use:

<strong>emcc -o dctcomp.js mains/dctcomp.c libs/*</strong>
