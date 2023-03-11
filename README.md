# janet-cmake-template

Smol template / example program which embeds [Janet](https://janet-lang.org/) into C++.

See CMakeLists.txt, there is nothing really more than there. Yes, I know, that `file(GLOB)` is pretty much antipattern.

Mostly for my own needs for another project which I use Cmake with and don't really want to put amalgamated janet.c to its repository.

### Extra features

`JANET_STATIC` Cmake flag enables static linking instead of providing .so/.dll/.dylib

### License

[MIT](/LICENSE)
