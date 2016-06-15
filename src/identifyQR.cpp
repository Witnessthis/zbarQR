#include "zbar.h"  
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <cmath>
#include <cstdio>

 using namespace std;  
 using namespace zbar;  
 using namespace cv;  
 int main(void){  
      ImageScanner scanner;  
      scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);  
       // obtain image data  
      char file[256];  
      cin>>file;  
      Mat img = imread(file,0);  
      Mat imgout;  
      cvtColor(img,imgout,CV_GRAY2RGB);  
      int width = img.cols;  
      int height = img.rows;
      int32_t topLength;
     int32_t botLength;
     int32_t leftLength;
     int32_t rightLength;
   uchar *raw = (uchar *)img.data;  
   // wrap image data  
   Image image(width, height, "Y800", raw, width * height);  
   // scan the image for barcodes  
   int n = scanner.scan(image);  
   // extract results  
   for(Image::SymbolIterator symbol = image.symbol_begin();  
     symbol != image.symbol_end();  
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

               if(i == 0){
                    botLength = sqrt(pow(pts[i].x-pts[(i+1)%4].x, 2) + pow(pts[i].y-pts[(i+1)%4].y, 2));
                    cout << "botLength: " << botLength << endl;
               }
               else if (i == 1){
                    leftLength = sqrt(pow(pts[i].x-pts[(i+1)%4].x, 2) + pow(pts[i].y-pts[(i+1)%4].y, 2));
                    cout << "leftLength: " << leftLength << endl;
               }
               else if (i == 2){
                    topLength = sqrt(pow(pts[i].x-pts[(i+1)%4].x, 2) + pow(pts[i].y-pts[(i+1)%4].y, 2));
                    cout << "topLength: " << topLength << endl;
               }
               else if (i == 3){
                    rightLength = sqrt(pow(pts[i].x-pts[(i+1)%4].x, 2) + pow(pts[i].y-pts[(i+1)%4].y, 2));
                    cout << "rightLength: " << rightLength << endl;
               }

//               cout << "pts[i]: " << pts[i] << endl;
//               cout << "pts[(i+1)%4]: " << pts[(i+1)%4] << endl;

               line(imgout,pts[i],pts[(i+1)%4],Scalar(255,0,0),3);
           }  
           cout<<"Angle: "<<r.angle<<endl;  
   }  
      imshow("imgout.jpg",imgout);  
   // clean up  
   image.set_data(NULL, 0);  
       waitKey();  
 }  
