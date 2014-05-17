# building GLFW3
cd glfw
cmake .
make -j2
cp src/libglfw3.a ..
cd 