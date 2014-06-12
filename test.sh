#! /bin/sh

echo -e '\nBuild with g++'
command -v g++ && make gcc;

echo -e '\nBuild with clang++'
command -v clang++ && make clang;

echo -e '\nBuild with arm-none-eabi-g++'
command -v arm-none-eabi-g++ && make arm;
