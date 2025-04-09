#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <sensor_msgs/Image.h>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <filesystem> // requires C++17

namespace fs = std::filesystem;

std::ofstream log_file;
std::string output_dir;

void odomCallback(const nav_msgs::Odometry::ConstPtr& msg) {

    ros::Time timestamp = msg->header.stamp;
    // Convert the timestamp to seconds
    double seconds = timestamp.toSec();
    log_file << std::fixed << std::setprecision(4) << seconds << ", "
             << msg->pose.pose.position.x << ", "
             << msg->pose.pose.position.y << ", "
             << msg->pose.pose.position.z << ", "
             << msg->pose.pose.orientation.w << ", "
             << msg->pose.pose.orientation.x << ", "
             << msg->pose.pose.orientation.y << ", "
             << msg->pose.pose.orientation.z
             << std::endl;
}

void depthImageCallback(const sensor_msgs::ImageConstPtr& msg) {
    cv_bridge::CvImagePtr cv_ptr;
    try {
        cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::TYPE_32FC1);
    } catch (cv_bridge::Exception& e) {
        ROS_ERROR("cv_bridge exception: %s", e.what());
        return;
    }

    cv::Mat depth_image = cv_ptr->image;
    // log_file << "Depth Image at Time: " << msg->header.stamp << std::endl;
    // for (int i = 0; i < depth_image.rows; ++i) {
    //     for (int j = 0; j < depth_image.cols; ++j) {
    //         log_file << depth_image.at<float>(i, j) << " ";
    //     }
    //     log_file << std::endl;
    // }
    // Normalize the image to the range [0, 255]
    cv::Mat normalized_image;
    cv::normalize(depth_image, normalized_image, 0, 255, cv::NORM_MINMAX);

    // Convert the image to 8-bit
    cv::Mat depth_image_8bit;
    normalized_image.convertTo(depth_image_8bit, CV_8UC1);

    // Accessing the timestamp of the message
    ros::Time timestamp = msg->header.stamp;
    // Convert the timestamp to seconds
    double seconds = timestamp.toSec();
    std::string timestamp_str = std::to_string(seconds);
    std::replace(timestamp_str.begin(), timestamp_str.end(), '.', '_');

    std::string filepath = output_dir + "/" + timestamp_str + ".png";
    cv::imwrite(filepath, normalized_image);
    ROS_INFO("Saved depth image to %s", filepath.c_str());
}

int main(int argc, char** argv) {
    ros::init(argc, argv, "drone_logger");
    ros::NodeHandle nh("~");
    ros::NodeHandle nh_global;

    std::string filename;
    nh.param<std::string>("output_dir", output_dir, "/tmp/ros_output");
    nh.param<std::string>("filename", filename, "drone_log.txt");

    // Create directory if it doesn't exist
    std:: cout << "output_dir :" << output_dir << std::endl;
    if (!fs::exists(output_dir)) {
        fs::create_directories(output_dir);
    }

    // Combine output_dir and filename into a full path
    fs::path full_path = fs::path(output_dir) / filename;
    log_file.open(full_path);

    log_file << "Time_ns, pos_x, pos_y, pos_z, orient_w, orient_x, orient_y, orient_z" << std::endl;

    ros::Subscriber odom_sub = nh_global.subscribe("odom", 1000, odomCallback);
    ros::Subscriber depth_image_sub = nh_global.subscribe("depth_img", 1000, depthImageCallback);

    ros::spin();

    log_file.close();
    return 0;
}