#/bin/bash

# cleanup
rm -f out

# compile + link
clang++ \
  -std=c++20 \
  -Wno-deprecated-anon-enum-enum-conversion \
  -o out main.cc feature_manager.cc visual_odometry.cc \
  `pkg-config --cflags --libs opencv4` \
  `pkg-config --cflags --libs sdl2` \

# run
./out
