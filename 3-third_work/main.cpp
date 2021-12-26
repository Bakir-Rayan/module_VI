#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

double PSNR(Mat I1, Mat I2)
{

    double psnr, eqm = 0;
    double a, b;
    for (int i = 0; i < I1.rows; i++)
    {
        for (int j = 0; j < I1.cols; j++)
        {
            a = I1.at<uchar>(i, j);
            b = I2.at<uchar>(i, j);
            eqm = eqm + pow(a - b, 2.);
        }
    }
    eqm = eqm / (I1.rows * I1.cols);
    psnr = 10 * log10(255 * 255 / eqm);
    return psnr;
}

int main( int argc, char** argv )
{
    // on lit l'image originale
    Mat image = imread("image8.jpg",IMREAD_COLOR);
    if(! image.data)
    {
        cout<<"Could not open file" << endl;
        return -1;
    }

    Mat img_gn1 = imread("image8_gn1.bmp",IMREAD_COLOR);
    if(! img_gn1.data)
    {
        cout<<"Could not open file image8_gn1.bmp" << endl;
        return -1;
    }
    
    Mat img_gn2 = imread("image8_gn2.bmp",IMREAD_COLOR);
    if(! img_gn2.data)
    {
        cout<<"Could not open file image8_gn2.bmp" << endl;
        return -1;
    }
    
    Mat img_sp = imread("image8_sp.bmp",IMREAD_COLOR);
    if(! img_sp.data)
    {
        cout<<"Could not open file image8_sp.bmp" << endl;
        return -1;
    }
    // on affiche l'image originale
    namedWindow("RGB image", WINDOW_AUTOSIZE );
    imshow("RGB image", image);
    namedWindow("G1", WINDOW_AUTOSIZE );
    imshow("G1", img_gn1);
    namedWindow("G2", WINDOW_AUTOSIZE );
    imshow("G2", img_gn2);
    namedWindow("SP", WINDOW_AUTOSIZE );
    imshow("SP", img_sp);

    // on reduit le bruit dans les images

    Mat img_gn1_reduit1, img_gn1_reduit2, img_gn1_reduit3, img_gn1_reduit4;

    medianBlur(img_gn1, img_gn1_reduit1, 3);
    bilateralFilter(img_gn1, img_gn1_reduit2, 15, 3, 3); 
    GaussianBlur(img_gn1, img_gn1_reduit3, Size(3, 3), 3, 3);
    blur(img_gn1, img_gn1_reduit4, Size(3, 3));

    imshow("GaussianBlur gn1", img_gn1_reduit3); 
    imshow("median_blur gn1", img_gn1_reduit1); 
    imshow("bilateral_filter gn1", img_gn1_reduit2); 
    imshow("reg_blur gn1", img_gn1_reduit4);
    
    Mat img_gn2_reduit1, img_gn2_reduit2, img_gn2_reduit3, img_gn2_reduit4;

    medianBlur(img_gn2, img_gn2_reduit1, 5);
    bilateralFilter(img_gn2, img_gn2_reduit2, 19, 7, 7); 
    GaussianBlur(img_gn2, img_gn2_reduit3, Size(3, 3), 1, 1);
    blur(img_gn2, img_gn2_reduit4, Size(3, 3));

    imshow("GaussianBlur gn2", img_gn2_reduit3); 
    imshow("median_blur gn2", img_gn2_reduit1); 
    imshow("bilateral_filter gn2", img_gn2_reduit2); 
    imshow("reg_blur gn2", img_gn2_reduit4);

    Mat img_sp_reduit1, img_sp_reduit2, img_sp_reduit3, img_sp_reduit4;

    medianBlur(img_sp, img_sp_reduit1, 3);
    bilateralFilter(img_sp, img_sp_reduit2, 11, 1, 1); 
    GaussianBlur(img_sp, img_sp_reduit3, Size(5, 5), 1, 1);
    blur(img_sp, img_sp_reduit4, Size(3, 3));

    imshow("GaussianBlur sp", img_sp_reduit3); 
    imshow("median_blur sp", img_sp_reduit1); 
    imshow("bilateral_filter sp", img_sp_reduit2); 
    imshow("reg_blur sp", img_sp_reduit4);

    // on calcule le PSNR de l'image originale et de l'image G1
    double psnr_img_gn1 = PSNR(image, img_gn1_reduit1);
    double psnr_img_gn12 = PSNR(image, img_gn1_reduit2);
    double psnr_img_gn13 = PSNR(image, img_gn1_reduit3);
    double psnr_img_gn14 = PSNR(image, img_gn1_reduit4);

    double psnr_img_gn2 = PSNR(image, img_gn2_reduit1);
    double psnr_img_gn21 = PSNR(image, img_gn2_reduit2);
    double psnr_img_gn22 = PSNR(image, img_gn2_reduit3);
    double psnr_img_gn23 = PSNR(image, img_gn2_reduit4);

    double psnr_img_sp = PSNR(image, img_sp_reduit1);
    double psnr_img_sp2 = PSNR(image, img_sp_reduit2);
    double psnr_img_sp3 = PSNR(image, img_sp_reduit3);
    double psnr_img_sp4 = PSNR(image, img_sp_reduit4);

    cout << "PSNR de l'image originale et de l'image G1 : " << psnr_img_gn1 << endl;
    cout << "PSNR de l'image originale et de l'image G1 : " << psnr_img_gn12 << endl;
    cout << "PSNR de l'image originale et de l'image G1 : " << psnr_img_gn13 << endl;
    cout << "PSNR de l'image originale et de l'image G1 : " << psnr_img_gn14 << endl;

    cout << "PSNR de l'image originale et de l'image G2 : " << psnr_img_gn2 << endl;
    cout << "PSNR de l'image originale et de l'image G2 : " << psnr_img_gn21 << endl;
    cout << "PSNR de l'image originale et de l'image G2 : " << psnr_img_gn22 << endl;
    cout << "PSNR de l'image originale et de l'image G2 : " << psnr_img_gn23 << endl;

    cout << "PSNR de l'image originale et de l'image SP : " << psnr_img_sp << endl;
    cout << "PSNR de l'image originale et de l'image SP : " << psnr_img_sp2 << endl;
    cout << "PSNR de l'image originale et de l'image SP : " << psnr_img_sp3 << endl;
    cout << "PSNR de l'image originale et de l'image SP : " << psnr_img_sp4 << endl;

    waitKey(0);
    destroyAllWindows();
    return 0;
}