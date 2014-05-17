clear
cd lumina
make -j8 ASAN=1 CXX=g++ INCLUDE_PATH=-I../libs/glfw/include
cd ..
make clean
make -j8 ASAN=1 CXX=g++ test.bin
echo "----------------------------------------"
echo ""
optirun ./test.bin