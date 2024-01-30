# minislam2

Simple implementation of monocular visual odometry.

Note: This is a rewrite of https://github.com/markoelez/minislam.

<p align="center">
    <img src="https://github.com/markoelez/minislam2/blob/master/img/img.png?raw=true" height="200" alt="img">
</p>
<p align="center">
<img src="https://github.com/markoelez/minislam2/blob/master/img/map.png?raw=true" height="400" alt="img">
</p>


### Usage

build:
```sh
mkdir build && cd build
cmake ..
cmake --build .

# or
./scripts/build.sh
```

run example:
```sh
# build, then
./build/minislam

# or
./scripts/run.sh
```