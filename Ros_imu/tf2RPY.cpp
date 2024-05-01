#include <ros/ros.h>
#include <tf2_ros/transform_listener.h>
#include <geometry_msgs/TransformStamped.h>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>

void tfCallback(const geometry_msgs::TransformStamped::ConstPtr& tf_msg) {
    // Convert quaternion to Euler angles
    tf2::Quaternion tf_quaternion;
    tf2::fromMsg(tf_msg->transform.rotation, tf_quaternion);
    double roll, pitch, yaw;
    tf2::Matrix3x3(tf_quaternion).getRPY(roll, pitch, yaw);

    // Log transformed orientation
    ROS_INFO("Transformed orientation (Roll, Pitch, Yaw): [%f, %f, %f]", 
             roll, pitch, yaw);
}

int main(int argc, char** argv) {
    ros::init(argc, argv, "tf_orientation_listener");
    ros::NodeHandle nh;

    // Initialize TF listener
    tf2_ros::Buffer tfBuffer;
    tf2_ros::TransformListener tfListener(tfBuffer);

    // Subscribe to TF topic
    ros::Subscriber tf_sub = nh.subscribe<geometry_msgs::TransformStamped>("tf_topic", 10, tfCallback);

    ros::spin();

    return 0;
}
