#include "zbar.h"  
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
        RotatedRect r = minAreaRect(vp);
        Point2f pts[4];
        r.points(pts);
        for(int i=0;i<4;i++){

            cout << "pts[i]: " << pts[i] << endl;
            cout << "pts[(i+1)%4]: " << pts[(i+1)%4] << endl;

            line(cv_ptr->image,pts[i],pts[(i+1)%4],Scalar(255,0,0),3);

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
