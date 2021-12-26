#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
    namedWindow("original image", WINDOW_AUTOSIZE);
    Mat image;
    image = imread("image8_gn1.bmp",IMREAD_COLOR);
    imshow("original image", image);
    if(! image.data)
        {
            cout<<"Could not open file" << endl;
            return -1;
        }
    namedWindow("test image", WINDOW_AUTOSIZE);

    Mat output;
    fastNlMeansDenoisingColored(image, output, 10, 10, 27);
    imshow("test image", output);
    waitKey(0);
    return 0;
}