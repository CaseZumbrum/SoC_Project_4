#include <opencv2/opencv.hpp>

using namespace cv;

int main() {
    // read in image as greyscale
    Mat img = imread("peppers.png");
    if (img.empty()) return -1;
 
    // 3x3 averaging blur kernel
    Mat blurKernel = (Mat_<float>(3, 3) << 1.f/9.f, 1.f/9.f, 1.f/9.f,
                                              1.f/9.f, 1.f/9.f, 1.f/9.f,
                                              1.f/9.f, 1.f/9.f, 1.f/9.f);

    Mat output_image;
    filter2D(img, output_image, CV_8U, blurKernel);
 
    imwrite("out.png", output_image);
    
    return 0;
}