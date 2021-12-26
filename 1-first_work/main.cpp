#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
    // on lit l'image originale
    Mat image = imread("image8.jpg",IMREAD_COLOR);
    if(! image.data)
    {
        cout<<"Could not open file" << endl;
        return -1;
    }

    // on affiche l'image originale
    namedWindow("RGB image", WINDOW_AUTOSIZE );
    imshow("RGB image", image);

    // on applique la transformation HSV à l'image
    Mat hsvImg;
    cvtColor(image, hsvImg, COLOR_BGR2HSV);

    // on separe l'image HSV en plusieurs images - channels
    vector<Mat>channels;
    split(hsvImg, channels);

    // on affiche chaque channels separement 
    namedWindow("HSV image", WINDOW_AUTOSIZE );
    imshow("HSV image", hsvImg);
    imshow("H", channels[0]);
    imshow("S", channels[1]);
    imshow("V", channels[2]); 

    // on affiche l'image en niveau de gris
    Mat grayImg;
    cvtColor(image, grayImg, COLOR_BGR2GRAY);
    namedWindow("gray image", WINDOW_AUTOSIZE );
    imshow("gray image", grayImg);

    waitKey(0);
    destroyAllWindows();
    return 0;
}