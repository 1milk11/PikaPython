ROOT=$PWD
reset
cp config/pika_config_default.h config/pika_config.h

# git submodule init
# git submodule update
git config --unset-all core.filemode
git config --add core.filemode false
chmod +x *.sh 
sh update-compiler.sh
chmod +x package/pikascript/rust-msc-latest-linux
cp package/pikascript/rust-msc-latest-linux /bin 
cd package/pikascript/pikascript-core
# git checkout master
cd $ROOT
rm build -rf
mkdir build
./package/pikascript/rust-msc-latest-linux -c package/pikascript/cjson_test.py
sh api-make.sh
cd build
cmake -G Ninja -DCMAKE_BUILD_TYPE=Debug -DPIKA_CONFIG_ENABLE=ON ..

cd $ROOT
rm Release -rf
mkdir Release
cd Release 
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release -DPIKA_CONFIG_ENABLE=ON ..

cd $ROOT
