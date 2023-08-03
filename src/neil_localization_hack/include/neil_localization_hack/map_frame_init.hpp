#ifndef MAP_FRAME_INIT_HPP_
#define MAP_FRAME_INIT_HPP_

#include <memory>
#include <iostream>
#include <cmath>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "sensor_msgs/msg/nav_sat_fix.hpp"
#include <GeographicLib/LambertConformalConic.hpp> 
#include <GeographicLib/Geocentric.hpp>
#include <GeographicLib/LocalCartesian.hpp>
#include <GeographicLib/Constants.hpp>
#include <functional>
#include <sstream>
#include <string>
#include "geometry_msgs/msg/transform_stamped.hpp"
#include "tf2/LinearMath/Quaternion.h"
#include "tf2_ros/transform_broadcaster.h"

class MapFrameInit : public rclcpp::Node
{
public:
  MapFrameInit();
  bool origin_set_ = false;
  double m_lat, m_long, m_alt;
  const GeographicLib::Geocentric& earth = GeographicLib::Geocentric::WGS84();

private:
  void topic_callback(const sensor_msgs::msg::NavSatFix::SharedPtr msg);
  void generate_transform(const std::vector<double>& msg);
  void geo_to_cartesian(double lat, double lon, double alt, std::vector<double>& local_cart);
  bool covariance_check(const sensor_msgs::msg::NavSatFix::SharedPtr msg, double upper_bound_);


  rclcpp::Subscription<sensor_msgs::msg::NavSatFix>::SharedPtr subscription_;
  std::unique_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;
};

#endif 
