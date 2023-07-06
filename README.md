# R-VIWO-ARK: Robust Visual Inertial Wheel Odometry with Adaptive Robust Kernel

R-VIWO-ARK is a Visual Inertial Wheel odometry pipeline for
robust and accurate state estimation. It is based on
ORB-SLAM3 which is a real-time SLAM library able to perform
Visual, Visual-Inertial and Multi-Map SLAM with monocular,
stereo and RGB-D cameras, using pinhole and fisheye camera
models

# 1. Prerequisites
The pipeline has been tested on **Ubuntu 20.04**.

## C++14 Compiler
The new thread and chrono functionalities of C++14 are used.

## Pangolin
For visualization purpose
[Pangolin](https://github.com/stevenlovegrove/Pangolin) is
used. Dowload and install instructions can be found at
https://github.com/stevenlovegrove/Pangolin.

## OpenCV >= 4
We use [OpenCV](http://opencv.org) to perform image
processing. Download and install instructions can be found
at http://opencv.org.

## Eigen3
Required by **g2o** library. Download and install
instructions can be found at
http://eigen.tuxfamily.org. **Required at least 3.1.0**.

## DBoW2 and g2o (Included in Thirdparty folder)
For place recognition tasks the modified version of the
[DBoW2](https://github.com/dorian3d/DBoW2) library is used.
Graph-based optimization tools are provided by
[g2o](https://github.com/RainerKuemmerle/g2o) library
Both modified libraries (which are BSD) are included in the
*Thirdparty* folder.

## Python
Required to calculate the alignment of the trajectory with
the ground truth. **Required Numpy module**.

# 2. Building R-VIWO-ARK and examples

Clone the repository:
```
git clone https://github.com/WFram/R-VIWO-ARK.git
```

To build the *Thirdparty* libraries and *R-VIWO-ARK* the
script `build.sh` is provided. Execute:
```
cd R-VIWO-ARK
chmod +x build.sh
./build.sh
```

This will create **libR-VIWO-ARK.so** at *lib* folder and
the executables in *Examples* folder.