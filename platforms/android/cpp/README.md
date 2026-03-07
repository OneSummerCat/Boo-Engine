# Android平台
## 配置
 - 1. NDK和Cmake配置
 ```
   // android/app/build.gradle 配置
   android {
      // ... 现有配置 ...

      defaultConfig {
          // ... 现有配置 ...

          externalNativeBuild {
              cmake {
                  cppFlags "-std=c++17 -frtti -fexceptions"
                  abiFilters 'arm64-v8a', 'armeabi-v7a'
              }
          }
      }
      externalNativeBuild {
          cmake {
              path file('src/main/cpp/CMakeLists.txt')
              version '3.22.1'
          }
      }
  }

 ```
## 三方库
 1. freetype(已经包含在引擎中)
 ```
 // 下载地址：https://download.savannah.gnu.org/releases/freetype/freetype-2.13.2.tar.gz  解压
 // 创建编译脚本 build_freetype.sh：  使用NDK编译freetype,如果ndk版本变化了,需要更新ndk路径重新编译
#!/bin/bash
  # 配置
  NDK_PATH="/Users/yangzongyuan/software/Android/SDK/ndk/23.2.8568313"
  FREETYPE_SRC="freetype-2.13.2"
  INSTALL_DIR="$(pwd)/freetype-android"
  ANDROID_API=28

  echo "开始编译 FreeType for Android..."
  echo "NDK 路径: $NDK_PATH"
  echo "源码目录: $FREETYPE_SRC"
  echo "安装目录: $INSTALL_DIR"

  cd $FREETYPE_SRC

  # 编译 arm64-v8a 架构
  echo ""
  echo "=========================================="
  echo "编译 arm64-v8a..."
  echo "=========================================="
  mkdir -p build-arm64
  cd build-arm64

  cmake .. \
      -DCMAKE_POLICY_VERSION_MINIMUM=3.5 \
      -DCMAKE_TOOLCHAIN_FILE="$NDK_PATH/build/cmake/android.toolchain.cmake" \
      -DANDROID_ABI=arm64-v8a \
      -DANDROID_PLATFORM=android-$ANDROID_API \
      -DCMAKE_BUILD_TYPE=Release \
      -DBUILD_SHARED_LIBS=OFF \
      -DFT_DISABLE_HARFBUZZ=ON \
      -DFT_DISABLE_PNG=ON \
      -DFT_DISABLE_ZLIB=ON \
      -DFT_DISABLE_BZIP2=ON \
      -DFT_DISABLE_BROTLI=ON \
      -DCMAKE_INSTALL_PREFIX="$INSTALL_DIR/arm64-v8a"

  make -j8
  make install

  cd ..

  # 编译 armeabi-v7a 架构
  echo ""
  echo "=========================================="
  echo "编译 armeabi-v7a..."
  echo "=========================================="
  mkdir -p build-armv7
  cd build-armv7

  cmake .. \
      -DCMAKE_POLICY_VERSION_MINIMUM=3.5 \
      -DCMAKE_TOOLCHAIN_FILE="$NDK_PATH/build/cmake/android.toolchain.cmake" \
      -DANDROID_ABI=armeabi-v7a \
      -DANDROID_PLATFORM=android-$ANDROID_API \
      -DCMAKE_BUILD_TYPE=Release \
      -DBUILD_SHARED_LIBS=OFF \
      -DFT_DISABLE_HARFBUZZ=ON \
      -DFT_DISABLE_PNG=ON \
      -DFT_DISABLE_ZLIB=ON \
      -DFT_DISABLE_BZIP2=ON \
      -DFT_DISABLE_BROTLI=ON \
      -DCMAKE_INSTALL_PREFIX="$INSTALL_DIR/armeabi-v7a"

  make -j8
  make install

  cd ../..

  echo ""
  echo "=========================================="
  echo "编译完成！"
  echo "=========================================="
  echo "编译产物位置："
  echo "  $INSTALL_DIR/arm64-v8a/lib/libfreetype.a"
  echo "  $INSTALL_DIR/armeabi-v7a/lib/libfreetype.a"
  echo "  $INSTALL_DIR/arm64-v8a/include/"

  //  执行编译：
  //  chmod +x build_freetype.sh
  //  ./build_freetype.sh
 ```
 2. vulkan-shaderc
 ```
 https://developer.android.google.cn/ndk/guides/graphics/shader-compilers.html?hl=zh-cn

使用NDK编译vulkan-shaderc,如果ndk版本变化了,需要更新ndk路径重新编译
 /Users/yangzongyuan/software/Android/SDK/ndk/23.2.8568313/ndk-build NDK_PROJECT_PATH=. APP_BUILD_SCRIPT=Android.mk \
    APP_STL:=c++_static APP_ABI=all libshaderc_combined -j16
/Users/yangzongyuan/software/Android/SDK/ndk/23.2.8568313/ndk-build NDK_PROJECT_PATH=. APP_BUILD_SCRIPT=Android.mk \
    APP_STL:=c++_static APP_ABI=arm64-v8a libshaderc_combined -j16
 ```

常见问题
