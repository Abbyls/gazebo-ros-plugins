# gazebo-ros-plugins
This is a simple example for establishing a modelPlugin for gazebo, which uses ROS to subscribe /joint_states topic

## Installation
```
mkdir -p ~/ur5e_plugins/src 
cd ~/ur5e_plugins 
catkin_make

cd src
git clone https://github.com/Abbyls/gazebo-ros-plugins.git
cd ..
catkin_make
```
then, you will see the plugin has been installed under the directory: ~/ur5e_plugins/devel/lib


## Usage
Before you launch your gazebo world:\
1.
```
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:~/ur5e_plugins/devel/lib
```
This is a temporary setting, which is available in this terminal, if you want to use it frequently, add it into ~/.bashrc file\

2. add this into your gazebo model file:
```
<plugin name="eePose_plugin" filename="libeePose_plugin.so"/>
```
3. launch your gazebo file


