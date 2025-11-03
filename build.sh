#!/bin/bash
# 检查build目录是否存在，不存在则创建
if [ ! -d "build" ]; then
    mkdir build
    echo "Created build directory"
fi

python3 CMakeLists.py
cd build
# cmake -G Xcode ..
cmake ..
make
# python3 ../campile_shader.py
./app
