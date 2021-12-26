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
    //namedWindow("RGB image", WINDOW_AUTOSIZE );
    //imshow("RGB image", image);

    // on affiche l'image en niveau de gris
    Mat grayScaleImg;
    cvtColor(image, grayScaleImg, COLOR_BGR2GRAY);
    namedWindow("gray image", WINDOW_AUTOSIZE );
    imshow("gray image", grayScaleImg);

    Mat BinGray;
    threshold(grayScaleImg,BinGray, 0,255,THRESH_BINARY + THRESH_OTSU);

    imshow("image seuillee", BinGray);

    Mat D, D2;
    int dilation_type = 0; 
    int dilation_size = 3; 
    int erosion_size = 7; 
    Mat dst, dst2;
    Mat element1 = getStructuringElement(dilation_type, Size(2 * dilation_size + 1, 2 * dilation_size + 1), Point(dilation_size, dilation_size));
    Mat element2 = getStructuringElement(dilation_type, Size(2 * erosion_size + 1, 2 * erosion_size + 1), Point(erosion_size, erosion_size));
    erode(BinGray, D, element2); 
    imshow("image erodee", D);
    dilate(D, dst, element1); 
    imshow("image érodée puis dilatee", dst);

    dilate(BinGray, D2, element1); 
    imshow("image dilatee 2", D2);
    erode(D2, dst2, element2); 
    imshow("image dilatee puis érodée", dst2);

    waitKey(0);
    destroyAllWindows();
    return 0;

}