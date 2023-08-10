from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
import os

def generate_launch_description():
   return LaunchDescription([
        Node(
            package='neil_localization_hack',
            namespace='geo_transform',
            executable='geo_transform',
            name='geo_transform'
        ),
        Node(
            package='neil_localization_hack',
            namespace='path_publisher',
            executable='path_publisher',
            name='path_publisher'
        ),
        Node(
         package="tf2_ros",
         executable="static_transform_publisher",
         name="swri_transform",
         arguments=["0", "0", "0", "0", "0", "0", "odom", "base_link"]
        ),
        Node(
            package='rviz2',
            namespace='',
            executable='rviz2',
            name='rviz2',
            arguments=['-d', [os.path.join(get_package_share_directory("neil_localization_hack"), 'params', 'localization_with_zed.rviz')]]
        )
   ])