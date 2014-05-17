# building GLEW
wget -O glew-1.10.0.zip http://sourceforge.net/projects/glew/files/glew/1.10.0/glew-1.10.0.zip/download
unzip glew-1.10.0.zip
rm glew-1.10.0.zip
cd glew-1.10.0
make -j2
echo "##### root privileges needed to install glew #####"
sudo make install
cd ..