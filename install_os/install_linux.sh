mkdir -p ../build
cd ../build
cmake -DCMAKE_INSTALL_PREFIX=/opt/nxudp ..
make
sudo make install