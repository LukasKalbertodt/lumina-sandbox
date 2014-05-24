clear
clear
cd lumina
make -j8 CPLUS_INCLUDE_PATH=../libs/glfw/include
cd ..
make clean
make -j8 test.bin
echo "----------------------------------------"
echo ""
./test.bin