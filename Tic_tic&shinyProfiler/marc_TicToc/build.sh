#!/bin/bash

#auther:fengrusong 


#获取执行脚本所在的目录
cur_dir=$(cd `dirname $0`; pwd)
echo cur_dir=$cur_dir


#=========================================pc build =============================================================
echo "#=========================================pc build ============================================================="
if true;then
cd $cur_dir
mkdir -p pc_build
mkdir -p pc_install

cd pc_build

# cmake \
#     -DCMAKE_INSTALL_PREFIX=../pc_install \
#     ..

cmake -DCMAKE_INSTALL_PREFIX=../pc_install ..

make #VERBOSE=0

#make install

#run
# 添加后直接运行./Marc-TicToc，未添加就去pc_build里面运行./Marc-TicToc
./Marc-TicToc

fi 


#=========================================android build =============================================================
if false;then
echo "#=========================================android build ============================================================="
cd $cur_dir
NDK_ROOT_DIR=/mnt/d/ligang/ndk/android-ndk-r22-linux-x86_64/
NDK_VER=android-ndk-r22
NDK_API_LEVEL=28
NDK_ANDROID_PLATFORM=android-28


mkdir -p android_build
mkdir -p android_install
cd android_build


cmake \
-DCMAKE_TOOLCHAIN_FILE=$NDK_ROOT_DIR/$NDK_VER/build/cmake/android.toolchain.cmake \
-DANDROID_NDK=$NDK_ROOT_DIR/$NDK_VER \
-DANDROID_ABI=arm64-v8a \
-DANDROID_PLATFORM=$NDK_ANDROID_PLATFORM \
-DANDROID_NATIVE_API_LEVEL=$NDK_API_LEVEL \
-DCMAKE_SYSROOT=${NDK_BUILD_DIR}/platforms/$NDK_ANDROID_PLATFORM/arch-arm/ \
-DCMAKE_INSTALL_PREFIX=../android_install/ \
-DCMAKE_BUILD_TYPE=Release \
-DCMAKE_C_FLAGS="-O3 -g" \
-DCMAKE_CXX_FLAGS="-O3  -g" \
../

#-std=c++14

make -j4
make install
fi 