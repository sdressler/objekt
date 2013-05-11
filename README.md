objekt
======

objekt will be a set of LLVM passes to manage memory objects. Currently, only a
single module exists to determine the data volume of a kernel function (kernel
data volume, KDV). The KDV is the sum of memory footprints for a given function.
Additionally, the data-direction, e.g. in or out, is determined.

Prerequisites
-------------

- LLVM 3.2 (greater will also do)
- C++ compiler
- Optional: dot to generate graphs

Installation
------------

1. Adapt `Makefile.in`, especially set the path to your LLVM installation
   (should be sufficient)
2. Build objekt by typing `make`

Disclaimer
==========

This software is at an very early stadium. It does not support every imaginable
data type but is quite flexible to adapt.

Usage
=====

KDV Module
----------

The KDV module consists of a two-step processing. As input, it requires an
annotated bit-code file. The annotation is done by marking the desired kernel
(of which its KDV shall be determined). Marking is done with the annotate
attribute, e.g. you have to put

    __attribute__((annotate("kernel")))

directly prior the method to be evaluated (look in the examples folder for ...
examples).

Once you compiled the source with LLVM and obtained a bitcode file, you can run
`opt` on it. Load `object.so` and enable the `kdv` switch to run the KDV pass:

    opt -load ./object.so -kdv input.bc > output.bc

When the pass is done, compile `output.bc` and run the executable. Make sure,
that you also compile the `helper.cpp` file from within the `aux` directory.
This file provides the handler which is called and currently outputs the
estimated KDV. Currently, only this helper function exists, but you can adapt it
to your needs, as long the function interface remains (sorry for that!).

The KDV module currently is somewhat limited, e.g. the following data types
**are** supported:

- Arrays 1D, 2D as long as you use malloc / new
- STL containers string, vector, map
- Classes with (multi-)inheritance and templates using only supported data types
- Classes with recursive structures (e.g. ptr to ptr)

Please report Bugs
==================

I'm happy about bug reports and improvement suggestions. Please file them at
GitHub.
