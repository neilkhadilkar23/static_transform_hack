from launch import LaunchDescription
from launch_ros.actions import Node

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
        )
   ])