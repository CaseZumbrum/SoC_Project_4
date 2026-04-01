// based on https://opencv.org/edge-detection-using-opencv/
#include <opencv2/opencv.hpp>

using namespace cv;

int main() {
    // read in image as greyscale
    Mat img = imread("images/coins.png", IMREAD_GRAYSCALE);
    if (img.empty()) return -1;
    Mat sobelx, sobely, gradient;
 
    // Use sobel vector in x direction, detects horizontal edges
    Sobel(img, sobelx, CV_64F, 1, 0, 3);
    // use sobel vector in y direction, detects vertical edges
    Sobel(img, sobely, CV_64F, 0, 1, 3);
 
    // Combine x and y sobel vectors to get output (detects all edges)
    magnitude(sobelx, sobely, gradient);
 
    // scale gradiant to be 0->255
    Mat gradient_abs;
    convertScaleAbs(gradient, gradient_abs);
 
    // write image
    imwrite("outImage_Zumbrum.jpg", gradient_abs);
    
    return 0;
}