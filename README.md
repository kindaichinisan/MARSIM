# MARSIM
MARSIM: A light-weight point-realistic simulator for LiDAR-based UAVs

Paper is available on Arxiv: https://arxiv.org/abs/2211.10716

The video is available on youtube: https://youtu.be/hiRtcq-5lN0

## Update

Ubuntu 20.04 is also supported in ubuntu20 branch.

**Ten realistic maps (low and high resolution) have been realeased in the realease packages.**

**A new branch that merge with FUEL has been released in the fuel_ubuntu20 branch.**

## Prerequisited

### Ubuntu and ROS

Ubuntu 16.04~20.04.  [ROS Installation](http://wiki.ros.org/ROS/Installation).

### PCL && Eigen && glfw3

PCL>=1.6, Follow [PCL Installation](https://pointclouds.org/). 

Eigen>=3.3.4, Follow [Eigen Installation](https://eigen.tuxfamily.org/index.php?title=Main_Page).

glfw3:
```
sudo apt-get install libglfw3-dev libglew-dev
```

### make
```
mkdir -p marsim_ws/src
cd marsim_ws/src
git clone git@github.com:hku-mars/MARSIM.git
cd ..
catkin_make
```

## run the simulation

```
source devel/setup.bash
roslaunch test_interface single_drone_avia.launch
```
Click on 3Dgoal tool on the Rviz, you can give the UAV a position command to control its flight.

For now, we provide several launch files for users, which can be found in test_interface/launch folder.

You can change the parameter in launch files to change the map and LiDAR to be simulated.

** If you want to use the GPU version of MARSIM, please set the parameter "use_gpu" to true. **

## run the simulation with FUEL algorithm

You should first change the branch to fuel_ubuntu20 branch. If you are using ubuntu 20.04, you should first download Nlopt and make install it in your environment. Then you can run the simulation by the command below:
```
source devel/setup.bash
roslaunch exploration_manager exploration.launch
```
Then click on 2Dgoal tool on the Rviz, randomly click on the map, and FUEL would automously run.

## Acknowledgments
Thanks for [FUEL](https://github.com/HKUST-Aerial-Robotics/FUEL.git)

## Future
More realistic maps and functions are going to be released soon.

## Map (pcd)
/home/lexus/HWJ/use_cloud_compare/32-mercedes-benz-gls-580-2020/uploads_files_2787791_Mercedes+Benz+GLS+580_forest.pcd: forest with car
/home/lexus/HWJ/3D_model/palm_tree/palm_tree_plantation_less_less_plane.pcd: plantation
/home/lexus/HWJ/3D_model/palm_tree/palm_tree_plantation_less_less_plane_with_car2.pcd: plantation with car
/home/lexus/HWJ/pcd_from_Kelvin/20250610_MH_outdoor.pcd: from Kelvin
/home/lexus/HWJ/pcd_from_Kelvin/corrected_map_20250610.pcd: from Kelvin (does not work)
/home/lexus/HWJ/3D_model_from_LeongFei/pcd_conversion/plantation_with_car_LF_palm.pcd: replace /home/lexus/HWJ/3D_model/palm_tree/palm_tree_plantation_less_less_plane_with_car2.pcd with LF palm tree
/home/lexus/HWJ/pcd_from_Kelvin/livox/20250612_Bishan_Run1.pcd: from Kelvin in Bishan.
/home/lexus/HWJ/pcd_from_Kelvin/livox/20250612_Bishan_Run2.pcd: from Kelvin in Bishan.

## add livox ros driver 2 to this project
Add find_package(livox_ros_driver2) and catkin_package(livox_ros_driver2) to CMakeLists.txt
Add <build_depend>livox_ros_driver2</build_depend> and <run_depend>livox_ros_driver2</run_depend> to package.xml

after building livox ros driver 2, copy /home/lexus/INP2025/ws_livox/devel/include/livox_ros_driver2 to /home/lexus/INP2025/marsim_ws/devel/include/livox_ros_driver2.

May not be standard practice but works.

Add #include "livox_ros_driver2/CustomMsg.h" to file that uses it.

After adding the code in pointcloud_render_node.cpp, use rostopic list to check if topic exists: /quad0_pcl_render_node/pointcloud_ard_drone_topic

rostopic echo /quad0_pcl_render_node/pointcloud_ard_drone_topic.

Before this, need to source /home/lexus/INP2025/ws_livox/devel/setup.bash else it will complain ERROR: Cannot load message class for [livox_ros_driver2/CustomMsg]. Are your messages built?

## Useful ros command
rostopic echo /quad0_pcl_render_node/pointcloud_ard_drone_topic
rostopic hz /quad0_pcl_render_node/pointcloud_ard_drone_topic quad_0/imu
source /home/lexus/INP2025/ws_livox/devel/setup.bash
rostopic list
roslaunch test_interface single_drone_avia.launch: Launch multiple nodes or a node with parameters/configuration.
rosrun turtlesim turtlesim_node: Run a single node from a package.
rosrun rqt_graph rqt_graph