#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "geometry_msgs/msg/transform_stamped.hpp"
#include "geometry_msgs/msg/transform_stamped.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "geometry_msgs/msg/point.hpp"
#include "geometry_msgs/msg/pose_stamped.hpp"
#include "geometry_msgs/msg/quaternion.hpp"
#include "rclcpp/rclcpp.hpp"
#include "tf2/exceptions.h"
#include "tf2_ros/transform_listener.h"
#include "tf2_ros/buffer.h"
#include "nav_msgs/msg/path.hpp"
#include "tf2/LinearMath/Quaternion.h"

using namespace std::chrono_literals;

/**
 * This code will wait for a new transform to show up in the tf buffer, then add a pose to the 
 * std::vector that holds poses, then publish that path message ideally
*/
class PathPublisher : public rclcpp::Node
{
  public:
    PathPublisher()
    : Node("path_publisher"), count_(0)
    {
      publisher_ = this->create_publisher<nav_msgs::msg::Path>("gokart/path", 10);
      pub_timer_ = this->create_wall_timer(500ms, std::bind(&PathPublisher::timer_callback, this));
      target_frame_ = this->declare_parameter<std::string>("target_frame", "odom");
      tf_buffer_ = std::make_unique<tf2_ros::Buffer>(this->get_clock());
      tf_listener_ =std::make_shared<tf2_ros::TransformListener>(*tf_buffer_);
      tf_timer_ = this->create_wall_timer(1s, std::bind(&PathPublisher::construct_path, this));
    }

      std::vector<geometry_msgs::msg::PoseStamped> current_path_; 

  private:
    void timer_callback()
    {
      /**
       * create a path message from the vector current path and 
       * Publish the nav_msg_path object
       * 
      */
        auto path_msg = std::make_unique<nav_msgs::msg::Path>();
        path_msg->header.frame_id = "map";
        path_msg->header.stamp = this->get_clock()->now();

      if (!(this->current_path_.empty())){
        //how to allocate memory for this?
        //geometry_msgs::msg::PoseStamped pose_arr[path_size];
        /**
                for (const auto& pose: this->current_path_){
            path_msg->poses.push_back(pose);
        }
        */
       path_msg->poses = this->current_path_;

        //std::copy(this->current_path_.begin(),this->current_path_.end(),&path_msg.poses[0]);
        this->publisher_->publish(*path_msg);
      }
    }

    void construct_path()
    {
        // Store frame names in variables that will be used to
        // compute transformations
        std::string fromFrameRel = target_frame_.c_str();
        std::string toFrameRel = "map";

        geometry_msgs::msg::TransformStamped t;

        // Look up for the transformation between target_frame and turtle2 frames
        // and send velocity commands for turtle2 to reach target_frame
        try {
            t = tf_buffer_->lookupTransform(
            toFrameRel, fromFrameRel,
            tf2::TimePointZero);
        } catch (const tf2::TransformException & ex) {
             RCLCPP_INFO(
            this->get_logger(), "Could not transform %s to %s: %s",
            toFrameRel.c_str(), fromFrameRel.c_str(), ex.what());
            return;
        }

        this->generate_pose(t);
        
    }

    void generate_pose(const geometry_msgs::msg::TransformStamped& t){

      geometry_msgs::msg::Point p;
      geometry_msgs::msg::Quaternion q;
      geometry_msgs::msg::PoseStamped s;
    
      //from ros2 tutorials
      // Read message content and assign it to
      // corresponding tf variables
      //s.header.frame_id = "map";
      //t.child_frame_id = "odom";

      p.x = t.transform.translation.x;
      p.y = t.transform.translation.y;
      p.z = t.transform.translation.z;

      q.x = 0.0;
      q.y = 0.0;
      q.z = 0.0;
      q.w = 1.0;

      s.header.stamp = this->get_clock()->now();
      /**
       * is the header frame id here odom or map
      */
      s.header.frame_id = "odom";
      s.pose.position = p;
      s.pose.orientation = q;
      //update the current path message 
      this->current_path_.push_back(s);


    }
    // Boolean values to store the information
    // if the service for spawning turtle is available
    // if the turtle was successfully spawned
    rclcpp::TimerBase::SharedPtr tf_timer_{nullptr};
    std::shared_ptr<tf2_ros::TransformListener> tf_listener_{nullptr};
    std::unique_ptr<tf2_ros::Buffer> tf_buffer_;
    std::string target_frame_;
    rclcpp::TimerBase::SharedPtr pub_timer_;
    rclcpp::Publisher<nav_msgs::msg::Path>::SharedPtr publisher_;
    size_t count_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<PathPublisher>());
  rclcpp::shutdown();
  return 0;
}


