#include <opencv2/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
    Mat image;
    image = imread("lena.png",IMREAD_COLOR);
    if(! image.data)
        {
            cout<<"Could not open file" << endl;
            return -1;
        }
    namedWindow("lena image", WINDOW_AUTOSIZE);
    imshow("lena image", image);
    waitKey(0);
    return 0;
}