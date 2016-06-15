#include "zbar.h"  
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <cmath>
#include <cstdio>

 #include "opencv2/imgproc.hpp"
 #include "opencv2/highgui.hpp"
 #include <iostream>
#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>

 using namespace std;
 using namespace zbar;  
 using namespace cv;
namespace enc = sensor_msgs::image_encodings;
void imageCallback(const sensor_msgs::ImageConstPtr& msg);
ImageScanner scanner;

int main(int argc, char **argv){
     //Init QR scanner
    scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);

    // ROS
     ros::init(argc, argv, "image_listener");
     ros::NodeHandle nh;
     cv::namedWindow("Image");
     //cv::namedWindow("Traces");
     //cv::namedWindow("QR code");
     cv::startWindowThread();

    image_transport::ImageTransport it(nh);

    //Front camera
    //ros::Subscriber frontImageRaw_sub = nh.subscribe("ardrone/front/image_raw", 10, imageCallback);
    //PC camera
    ros::Subscriber frontImageRaw_sub = nh.subscribe("camera/image_raw", 5, imageCallback);

    /* Toggle to downward camera, show downward.
	//ros::ServiceClient toggleCam_srv;
	toggleCam_srv        = nh.serviceClient<std_srvs::Empty>(nh.resolveName("ardrone/togglecam"),1);

	std_srvs::Empty toggleCam_srv_srvs;
	toggleCam_srv.call(toggleCam_srv_srvs);

	ros::Subscriber frontImageRaw_sub = nh.subscribe("ardrone/bottom/image_raw", 10, imageCallback);
	 */

    ros::spin();
    cv::destroyWindow("Image");
    cv::destroyWindow("Traces");
    cv::destroyWindow("QR code");



 }


void imageCallback(const sensor_msgs::ImageConstPtr& msg) {
    cv_bridge::CvImagePtr cv_ptr;

    cv_ptr = cv_bridge::toCvCopy(msg, enc::BGR8);

    // obtain image data
    /*char file[256];
    cin>>file;
    Mat img = imread(file,0);
    */

    Mat imgout;
    cvtColor(cv_ptr->image,imgout,CV_BGR2GRAY);
    int width = cv_ptr->image.cols;
    int height = cv_ptr->image.rows;

    int32_t topLength;
    int32_t botLength;
    int32_t leftLength;
    int32_t rightLength;

    uchar *raw = (uchar *)imgout.data;
    // wrap image data
    Image qrImage(width, height, "Y800", raw, width * height);
    // scan the image for barcodes
    int n = scanner.scan(qrImage);
    // extract results
    string decodedQr;
    for(Image::SymbolIterator symbol = qrImage.symbol_begin();
        symbol != qrImage.symbol_end();
        ++symbol) {
        vector<Point> vp;
        // do something useful with results
        cout << "decoded " << symbol->get_type_name()
        << " symbol \"" << symbol->get_data() << '"' <<" "<< endl;
        int n = symbol->get_location_size();
        for(int i=0;i<n;i++){
            vp.push_back(Point(symbol->get_location_x(i),symbol->get_location_y(i)));
        }

        for(int i=0;i<vp.size();i++){

            if(i == 0){
                botLength = sqrt(pow(vp[i].x-vp[(i+1)%4].x, 2) + pow(vp[i].y-vp[(i+1)%4].y, 2));
                cout << "botLength: " << botLength << endl;
            }
            else if (i == 1){
                leftLength = sqrt(pow(vp[i].x-vp[(i+1)%4].x, 2) + pow(vp[i].y-vp[(i+1)%4].y, 2));
                cout << "leftLength: " << leftLength << endl;
            }
            else if (i == 2){
                topLength = sqrt(pow(vp[i].x-vp[(i+1)%4].x, 2) + pow(vp[i].y-vp[(i+1)%4].y, 2));
                cout << "topLength: " << topLength << endl;
            }
            else if (i == 3){
                rightLength = sqrt(pow(vp[i].x-vp[(i+1)%4].x, 2) + pow(vp[i].y-vp[(i+1)%4].y, 2));
                cout << "rightLength: " << rightLength << endl;
            }


        decodedQr = symbol->get_data();
        cout<<"Angle: "<<r.angle<<endl;
        putText(cv_ptr->image, decodedQr, Point(250, 435), FONT_HERSHEY_PLAIN, 1, Scalar(0, 255, 0), 1, 8);
    }
    imshow("Image",cv_ptr->image);
    // clean up
    qrImage.set_data(NULL, 0);
    waitKey(3);
}
