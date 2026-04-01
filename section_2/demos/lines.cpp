// based on https://docs.opencv.org/3.4/d9/db0/tutorial_hough_lines.html

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;
int main(int argc, char** argv)
{
    Mat edge_detected, line_detected;

    // Load image
    Mat input = imread("peppers.png", IMREAD_GRAYSCALE );
    if(input.empty()){
        return -1;
    }

    // Edge detection
    Canny(input, edge_detected, 50, 200, 3);
    // Copy edges to the images that will display the results in BGR
    cvtColor(edge_detected, line_detected, COLOR_GRAY2BGR);

    // Probabilistic Line Transform
    vector<Vec4i> linesP;
    HoughLinesP(edge_detected, linesP, 1, CV_PI/180, 50, 50, 10 ); // runs the actual detection
    for( size_t i = 0; i < linesP.size(); i++ )
    {
        Vec4i l = linesP[i];
        line( line_detected, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, LINE_AA);
    }

    imwrite("out.png", line_detected);
    return 0;
}