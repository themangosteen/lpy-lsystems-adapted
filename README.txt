This is a modified version of the L-Py framework by Frédéric Boudon et al.
For original documentation refer to:
http://openalea.gforge.inria.fr/dokuwiki/doku.php?id=packages:vplants:lpy:main
as well as the paper:
F. Boudon, T. Cokelaer, C. Pradal and C. Godin, L-Py, an open L-systems framework in Python, FSPM 2010

L-Py is an L-System simulation framework for Python implemented in C++ and exposed to Python via Boost::Python.

This is a custom version intended to be used for lstring derivation only, leaving out the aspects of turtle interpretation and visualization originally provided by the PlantGL framework. Dependencies on an external PlantGL lib have been removed.
The intention is to be able to provide custom implementation of the interpretation aspects (e.g. via the Blender Python interface) and to be able to use the powerful lstring derivation capabilities of the L-Py framework as a more modular library.

Nikolaus Leopold, Summer 2016


=== DEPENDENCIES ===

the following packages need to be installed:
CMake 2.8.12
Python 3.5
Boost::Python3 1.49
Qt5Core

optionally use cmake -DBUILDPYTHON2=ON to build with minimum deps
Python 2.7
Boost::Python 1.49


=== INSTALLATION ===

NOTE: build files currently only written for Linux systems!

To build lpy as shared library:
1. enter the /build directory
2. run "cmake .." to generate make build files
3. run "make" in /build dir to build

on Linux you may have to rename the resulting liblpy.so to lpy.so to import in Python.
