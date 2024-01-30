#/bin/bash

# cleanup
rm -f out

# compile + link
clang++ \
  -std=c++20 \
  -Wno-deprecated-anon-enum-enum-conversion \
  -o out minislam2/main.cc minislam2/feature_manager.cc minislam2/visual_odometry.cc \
  `pkg-config --cflags --libs opencv4` \
  `pkg-config --cflags --libs sdl2` \

# run
./out
