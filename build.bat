@ECHO OFF

IF EXIST build (
    rem file exists
) ELSE (
    mkdir build
)

cd build
cmake -G"MinGW Makefiles" -DCMAKE_CXX_COMPILER=clang++ ..
mingw32-make
cd ../