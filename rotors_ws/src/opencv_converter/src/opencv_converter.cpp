#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <chrono>
#include <iostream>
#include <thread>

using namespace std;
using namespace std::chrono;

static const std::string OPENCV_WINDOW = "Image window";

class ImageConverter
{
  ros::NodeHandle nh_;
  image_transport::ImageTransport it_;
  image_transport::Subscriber image_sub_;
  image_transport::Publisher image_pub_;

public:
  ImageConverter()
    : it_(nh_)
  {
    // Subscrive to input video feed and publish output video feed
    image_sub_ = it_.subscribe("/firefly/camera_red_iris/image_raw", 1,
      &ImageConverter::imageCb, this);
    image_pub_ = it_.advertise("/image_converter/output_video", 1);

    cv::namedWindow(OPENCV_WINDOW);
  }

  ~ImageConverter()
  {
    cv::destroyWindow(OPENCV_WINDOW);
  }
  void imageCb(const sensor_msgs::ImageConstPtr& msg)
  {
    static int ctr=0;
    
    auto next = steady_clock::now();  
    cv_bridge::CvImagePtr cv_ptr;
    try
    {
      cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    }
    catch (cv_bridge::Exception& e)
    {
      ROS_ERROR("cv_bridge exception: %s", e.what());
      return;
    }
    string savingname="/home/arth/Drone_shots/MapGen" +to_string(ctr) +".jpg";
    cv::imwrite(savingname, cv_ptr->image);
    
    //string image_window = "SAVED IMAGE";
    // // Creating a window
    // cv::namedWindow(image_window);
  
    // // Showing the image in the defined window
    // cv::imshow(image_window, cv_ptr->image);
  
    // // waiting for any key to be pressed
    // cv::waitKey(0);
  
    // // destroying the creating window
    // cv::destroyWindow(image_window);

    // // Update GUI Window
    // cv::imshow(OPENCV_WINDOW, cv_ptr->image);
    // cv::waitKey(3);

    // Output modified video stream
    image_pub_.publish(cv_ptr->toImageMsg());
    ctr=ctr+1;
    next += 6s;
    std::this_thread::sleep_until(next);
  
  }
};

int main(int argc, char** argv)
{
  ros::init(argc, argv, "image_converter");
  ImageConverter ic;
  while(ros :: ok)
  {
    ros::spinOnce();
  }
  // writing the image to a defined location as JPEG
  
  
  // if the image is not saved
  // if (check == false) {
  //   cout << "Mission - Saving the image, FAILED" << endl;
  
  //   // wait for any key to be pressed
  //   cin.get();
  //   return -1;
  // }
  
  // cout << "Successfully saved the image. " << endl;
  
  // Naming the window
 
  return 0;
}