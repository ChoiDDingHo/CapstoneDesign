#include <ros/ros.h>
#include <sensor_msgs/Imu.h>
#include <sensor_msgs/FluidPressure.h>
#include <std_msgs/Header.h>
#include <librealsense2/rs.hpp>

// Function to initialize the camera pipeline
rs2::pipeline initialize_camera() {
    rs2::pipeline p;
    rs2::config conf;
    conf.enable_stream(RS2_STREAM_ACCEL);
    conf.enable_stream(RS2_STREAM_GYRO);
    p.start(conf);
    return p;
}

// Function to convert gyro data to Eigen Vector3d
Eigen::Vector3d gyro_data(const rs2_vector& gyro) {
    return Eigen::Vector3d(gyro.x, gyro.y, gyro.z);
}

// Function to convert accel data to Eigen Vector3d
Eigen::Vector3d accel_data(const rs2_vector& accel) {
    return Eigen::Vector3d(accel.x, accel.y, accel.z);
}

// Function to publish IMU data
void publish_imu_data(rs2::pipeline& camera_pipe) {
    ros::NodeHandle nh;
    ros::Publisher pub = nh.advertise<sensor_msgs::Imu>("imu_data", 10);
    ros::Rate rate(400);  // 400Hz

    while (ros::ok()) {
        rs2::frameset f = camera_pipe.wait_for_frames();
        auto accel = accel_data(f.first(RS2_STREAM_ACCEL).as<rs2::motion_frame>().get_motion_data());
        auto gyro = gyro_data(f.first(RS2_STREAM_GYRO).as<rs2::motion_frame>().get_motion_data());

        sensor_msgs::Imu imu_msg;
        imu_msg.header.stamp = ros::Time::now();
        imu_msg.header.frame_id = "imu_link";

        imu_msg.linear_acceleration.x = accel.x();
        imu_msg.linear_acceleration.y = accel.y();
        imu_msg.linear_acceleration.z = accel.z();

        imu_msg.angular_velocity.x = gyro.x();
        imu_msg.angular_velocity.y = gyro.y();
        imu_msg.angular_velocity.z = gyro.z();

        pub.publish(imu_msg);
        rate.sleep();
    }
}

int main(int argc, char** argv) {
    ros::init(argc, argv, "d455_imu_publisher");
    rs2::pipeline camera_pipe = initialize_camera();
    publish_imu_data(camera_pipe);
    return 0;
}
