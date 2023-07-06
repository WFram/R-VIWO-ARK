MYPWD=$(pwd)
export ROS_PACKAGE_PATH=${ROS_PACKAGE_PATH}:${MYPWD}/Examples_old/ROS

echo "Building ROS nodes"

cd Examples_old/ROS/ORB_SLAM3
mkdir build
cd build
cmake .. -DROS_BUILD_TYPE=Release \
  -DPYTHON_EXECUTABLE:FILEPATH=/usr/bin/python3
make -j4
