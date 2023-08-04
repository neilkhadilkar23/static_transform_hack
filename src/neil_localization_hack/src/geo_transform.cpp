// Copyright 2016 Open Source Robotics Foundation, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

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
 
using std::placeholders::_1;

class MapFrameInit : public rclcpp::Node
{
public:
  MapFrameInit() : Node("map_frame_init") {
    subscription_ = this->create_subscription<sensor_msgs::msg::NavSatFix>(
    "/gps/fix", 10, std::bind(&MapFrameInit::topic_callback, this, _1));
    this->origin_set_ = false;
    this->tf_broadcaster_ = std::make_unique<tf2_ros::TransformBroadcaster>(*this);
  }
  bool origin_set_;
  double m_lat, m_long, m_alt;
  const GeographicLib::Geocentric& earth = GeographicLib::Geocentric::WGS84();

private:
  void topic_callback(const sensor_msgs::msg::NavSatFix::SharedPtr msg);
  //consts here prevent changing msg and C* this
  void generate_transform(const std::vector<double>& msg);
  void geo_to_cartesian(double lat, double lon, double alt, std::vector<double>& local_cart);

  bool covariance_check(const sensor_msgs::msg::NavSatFix::SharedPtr msg, double upper_bound_);
  rclcpp::Subscription<sensor_msgs::msg::NavSatFix>::SharedPtr subscription_;
  std::unique_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;
};

void MapFrameInit::topic_callback(const sensor_msgs::msg::NavSatFix::SharedPtr msg){
    if (!(this->origin_set_)){
      if (this->covariance_check(msg, 30.0)) {
        this->m_lat = msg->latitude;
        this->m_long = msg->longitude;
        this->m_alt = msg->altitude;
        this->origin_set_ = true;

        RCLCPP_INFO(this->get_logger(), "me setting the latitude" + std::to_string(this->m_lat));
        RCLCPP_INFO(this->get_logger(), "me setting the longitude" + std::to_string(this->m_long));
        RCLCPP_INFO(this->get_logger(), "me setting the altitude" + std::to_string(this->m_alt));
      }
    } else {
      std::vector<double> transform;
      this->geo_to_cartesian(msg->latitude, msg->longitude, msg->altitude, transform);
      this->generate_transform(transform);
    }
    
  }
  //consts here prevent changing msg and C* this
void MapFrameInit::generate_transform(const std::vector<double>& msg){
    geometry_msgs::msg::TransformStamped t;
    //from ros2 tutorials
    // Read message content and assign it to
    // corresponding tf variables
    t.header.stamp = this->get_clock()->now();
    t.header.frame_id = "map";
    t.child_frame_id = "odom";

    t.transform.translation.x = msg.at(0);
    t.transform.translation.y = msg.at(1);
    t.transform.translation.z = msg.at(2);

    tf2::Quaternion q;
    q.setRPY(0, 0, 0);
    t.transform.rotation.x = q.x();
    t.transform.rotation.y = q.y();
    t.transform.rotation.z = q.z();
    t.transform.rotation.w = q.w();

    // Send the transformation
    tf_broadcaster_->sendTransform(t);



  }


void MapFrameInit::geo_to_cartesian(double lat, double lon, double alt, std::vector<double>& local_cart){
    GeographicLib::LocalCartesian proj(this->m_lat,this->m_long,this->m_alt, this->earth);
    double x, y, z;
    proj.Forward(lat, lon, alt, x, y, z);
    local_cart = {x, y, z};
  }

bool MapFrameInit::covariance_check(const sensor_msgs::msg::NavSatFix::SharedPtr msg, double upper_bound_){
  
  auto lambda = [&](double a, double b){return abs(a) + abs(b);};
  bool good_cov = (std::accumulate(std::begin(msg->position_covariance), std::end(msg->position_covariance), 0.0, lambda) < upper_bound_);
  return good_cov;

  }

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MapFrameInit>());
  rclcpp::shutdown();
  return 0;
}