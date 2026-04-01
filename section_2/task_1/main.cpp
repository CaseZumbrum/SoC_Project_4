#include <stdio.h>
#include <opencv2/opencv.hpp>
 
using namespace cv;
 
int main(int argc, char** argv )
{
 
    // read in image
    Mat image;
    image = imread( argv[1], IMREAD_COLOR );
 
    if ( !image.data )
    {
        return -1;
    }

    // set text properties
    String text = "Hello OpenCV";
    Point org(50, 100);
    int fontFace = FONT_HERSHEY_PLAIN;
    double fontScale = 2;
    Scalar color(255, 255, 255); 
    putText(image, text, org, fontFace, fontScale, color);


    // namedWindow("Display Image", WINDOW_AUTOSIZE );
    // imshow("Display Image", image);
    // waitKey(0);

    // write image
    imwrite("hello_Zumbrum.jpg", image);
 
    
 
    return 0;
}