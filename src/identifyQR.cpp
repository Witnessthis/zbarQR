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
   scanner.scan(image);
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

           cout << "pts[i].x: " << vp[i].x << endl;
           cout << "pts[i].y: " << vp[i].y << endl;

           line(imgout,vp[i],vp[(i+1)%4],Scalar(255,0,0),3);
       }
   }  
      imshow("imgout.jpg",imgout);  
   // clean up  
   image.set_data(NULL, 0);  
       waitKey();  
 }  
