#include <ros/ros.h>
#include <sensor_msgs/Imu.h>
#include <tf2_ros/transform_broadcaster.h>
#include <geometry_msgs/TransformStamped.h>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2/LinearMath/Matrix3x3.h>
#include <imu_tools/filter_base.h>
#include <imu_tools/FilterMadgwick.h>

// Global variables
tf2_ros::TransformBroadcaster *tf_broadcaster;
imu_tools::FilterMadgwick *madgwick_filter;

void imuCallback(const sensor_msgs::Imu::ConstPtr& imu_msg) {
    // Convert IMU message to Eigen matrix
    Eigen::Vector3d linear_acceleration(imu_msg->linear_acceleration.x,
                                         imu_msg->linear_acceleration.y,
                                         imu_msg->linear_acceleration.z);
    Eigen::Vector3d angular_velocity(imu_msg->angular_velocity.x,
                                      imu_msg->angular_velocity.y,
                                      imu_msg->angular_velocity.z);

    // Update filter with IMU data
    madgwick_filter->update(angular_velocity, linear_acceleration, imu_msg->header.stamp);

    // Get quaternion from filter
    Eigen::Quaterniond quaternion = madgwick_filter->getOrientation();

    // Convert quaternion to Euler angles
    tf2::Quaternion tf_quaternion(quaternion.x(), quaternion.y(), quaternion.z(), quaternion.w());
    tf2::Matrix3x3(tf_quaternion).getRPY(roll, pitch, yaw);

    // Log transformed IMU data
    ROS_INFO("Transformed IMU data (Roll, Pitch, Yaw): [%f, %f, %f]", 
             roll, pitch, yaw);
}

int main(int argc, char** argv) {
    ros::init(argc, argv, "imu_transformer");
    ros::NodeHandle nh;

    // Initialize Madgwick filter
    madgwick_filter = new imu_tools::FilterMadgwick(0.0025); // Sample period: 0.0025s (400Hz)

    // Initialize TF broadcaster
    tf_broadcaster = new tf2_ros::TransformBroadcaster();

    // Subscribe to IMU data
    ros::Subscriber imu_sub = nh.subscribe<sensor_msgs::Imu>("imu_data", 10, imuCallback);

    ros::spin();

    delete madgwick_filter;
    delete tf_broadcaster;

    return 0;
}
