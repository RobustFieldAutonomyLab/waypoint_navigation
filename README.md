# Waypoint Navigation using a Prior Map

## Overview

This package creates a rapid reliable waypoint navigation architecture using [ICP localization](https://github.com/leggedrobotics/icp_localization) to a prior map. It subscribes to lidar scans and odometry data to register location in a given map. Using the [move_base](http://wiki.ros.org/move_base) navigation stack, we built a map server to run with robust 3D localization.

The user should build a reference map (point cloud) as a `.pcd` file and define an initial pose in the reference map. We recommend using [hdl_graph_slam](https://github.com/koide3/hdl_graph_slam). 

Released under [BSD 3-Clause license](LICENSE).

**Author:** Erik Pearson

**Maintainer:** Erik Pearson, [epearson@stevens.edu](epearson@stevens.edu)

| waypoint navigation in a lab space |
|-----------------|
|[![lab](doc/lab_example.gif)](doc/lab_example.gif)|

## Installation

Install [ICP localization](https://github.com/leggedrobotics/icp_localization) in your workspace and check to confirm it builds. Recommended to build in release mode for performance (`catkin config -DCMAKE_BUILD_TYPE=Release`). Then install this package for waypoint navigation:

```bash
# in your source folder 'src'
git clone https://github.com/RobustFieldAutonomyLab/waypoint_navigation.git
```

We also recommend installing [hdl_graph_slam](https://github.com/koide3/hdl_graph_slam) to quickly build reference maps.

## Usage

You can launch the program in simulation with: `roslaunch waypoint_navigation waypoint_navigation_simulation.launch`. 

Goal positions are in the params sub folder in the format \[pose x, pose y, orientation z, orientation w\]

For use on a robot, we recommend using `waypoint_navigation.launch`, which uses `icp_node.launch` for localization parameters and `goals.yaml` for waypoint positions.

The `pcd_filepath` parameter in the [icp_node.launch](https://github.com/RobustFieldAutonomyLab/waypoint_navigation/launch/icp_node.launch) file should point to the location where you stored your refrence map (pointcloud) in the `.pcd` format.

## Configuration

The configuration for iterative closest point is split into three `.yaml` files.  

The `icp.yaml` file configures the ICP settings such as error metric and outlier filters. Any filter that is applied to the map can also be defined here.

The `input_filters.yaml` file configures operations that are applied to each scan of the range sensors. Subsampling, cropping and normal computation are configured in this file. Two examples have been provided (one for the velodyne puck range sensor and the other one for the ouste OS1 sensor).   

The filtering and the ICP can be configured by adding your own custom configuration `.yaml` files. Documentation on how to do that can be found [here](https://libpointmatcher.readthedocs.io/en/latest/Configuration/#creating-custom-configurations-with-yaml).  

