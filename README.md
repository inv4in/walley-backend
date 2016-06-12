# walley-backend
Library for cryptographically secure password managers

## Requirements
* [CMake (>=2.8)](http://cmake.org)
* [Boost (>=1.54)](http://boost.org)
* [Crypto++](https://www.cryptopp.com)
* [Doxygen](http://www.doxygen.org) (only required for documentation)

## Installation
There are no special options or configuration required to install. Proceed as you would with any
CMake project on your platform, e.g. on Unix

    mkdir build
    cd build
    cmake ..
    make
    make install

## Documentation
The library is documented with [Doxygen](http://www.doxygen.org). Simply run the `docs` target to
build HTML documentation for the library.
