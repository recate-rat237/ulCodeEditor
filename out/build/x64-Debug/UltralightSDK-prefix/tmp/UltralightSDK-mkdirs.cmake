# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/recate/source/repos/ulCodeEditor/out/build/x64-Debug/SDK"
  "C:/Users/recate/source/repos/ulCodeEditor/out/build/x64-Debug/UltralightSDK-prefix/src/UltralightSDK-build"
  "C:/Users/recate/source/repos/ulCodeEditor/out/build/x64-Debug/UltralightSDK-prefix"
  "C:/Users/recate/source/repos/ulCodeEditor/out/build/x64-Debug/UltralightSDK-prefix/tmp"
  "C:/Users/recate/source/repos/ulCodeEditor/out/build/x64-Debug/UltralightSDK-prefix/src/UltralightSDK-stamp"
  "C:/Users/recate/source/repos/ulCodeEditor/out/build/x64-Debug/UltralightSDK-prefix/src"
  "C:/Users/recate/source/repos/ulCodeEditor/out/build/x64-Debug/UltralightSDK-prefix/src/UltralightSDK-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/recate/source/repos/ulCodeEditor/out/build/x64-Debug/UltralightSDK-prefix/src/UltralightSDK-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/recate/source/repos/ulCodeEditor/out/build/x64-Debug/UltralightSDK-prefix/src/UltralightSDK-stamp${cfgdir}") # cfgdir has leading slash
endif()
