# RedSquare

# Quick Start Debian

## Install correct GF version

```console
$ git clone https://github.com/GamedevFramework/gf.git
$ cd gf/
$ git submodule update --init --recursive
$ git checkout 6bef0afd729f558ca0c7eeeb703c072ef43684cb
$ git reset --hard
$ mkdir build && cd build/
$ cmake ../
$ make -j5
$ sudo make install
```

## Install RedSquare

```console
$ git clone https://github.com/NiiRoZz/RedSquare.git
$ cd RedSquare/
$ git submodule update --init --recursive
$ mkdir build && cd build/
$ cmake ../
$ make -j5
$ sudo make install
```
