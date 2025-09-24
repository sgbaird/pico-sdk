# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "/home/runner/work/pico-sdk/pico-sdk/test/pico_wpa2_enterprise_example/build_interactive/_deps/picotool-src/enc_bootloader")
  file(MAKE_DIRECTORY "/home/runner/work/pico-sdk/pico-sdk/test/pico_wpa2_enterprise_example/build_interactive/_deps/picotool-src/enc_bootloader")
endif()
file(MAKE_DIRECTORY
  "/home/runner/work/pico-sdk/pico-sdk/test/pico_wpa2_enterprise_example/build_interactive/_deps/picotool-build/enc_bootloader"
  "/home/runner/work/pico-sdk/pico-sdk/test/pico_wpa2_enterprise_example/build_interactive/_deps/picotool-build/enc_bootloader"
  "/home/runner/work/pico-sdk/pico-sdk/test/pico_wpa2_enterprise_example/build_interactive/_deps/picotool-build/enc_bootloader/tmp"
  "/home/runner/work/pico-sdk/pico-sdk/test/pico_wpa2_enterprise_example/build_interactive/_deps/picotool-build/enc_bootloader/src/enc_bootloader-stamp"
  "/home/runner/work/pico-sdk/pico-sdk/test/pico_wpa2_enterprise_example/build_interactive/_deps/picotool-build/enc_bootloader/src"
  "/home/runner/work/pico-sdk/pico-sdk/test/pico_wpa2_enterprise_example/build_interactive/_deps/picotool-build/enc_bootloader/src/enc_bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/runner/work/pico-sdk/pico-sdk/test/pico_wpa2_enterprise_example/build_interactive/_deps/picotool-build/enc_bootloader/src/enc_bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/runner/work/pico-sdk/pico-sdk/test/pico_wpa2_enterprise_example/build_interactive/_deps/picotool-build/enc_bootloader/src/enc_bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
