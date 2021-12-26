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

    /// calcule de l'histogramme pour l'image au niveau de gris
    int histSize = 256;
    float range[] = { 0, 255 };
    const float* ranges[] = { range };

    MatND hist;
    calcHist(&grayScaleImg, 1, 0, Mat(), hist, 1, &histSize, ranges,true, false);

    int hist_w = 512; int hist_h = 400;
    int bin_w = cvRound((double)hist_w / histSize);

    Mat histImage(hist_h, hist_w, CV_8UC1, Scalar(255, 255, 255)); 
    normalize(hist, hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
    for (int i = 1; i < histSize; i++)
    {
        line(histImage, Point(bin_w * (i - 1),hist_h - cvRound(hist.at<float>(i - 1))),
        Point(bin_w * (i), hist_h -cvRound(hist.at<float>(i))),
        Scalar(0, 0, 0), 2, 8, 0);
    }
    
    //namedWindow("hist", WINDOW_AUTOSIZE); 
    //imshow("hist", histImage);
    
    // on egalise l'image en niveau de gris
    Mat eq_img;
    equalizeHist(grayScaleImg, eq_img);
    //imshow("Egalisation d’histogramme", eq_img);

    MatND hist2;
    calcHist(&eq_img, 1, 0, Mat(), hist2, 1, &histSize, ranges,true, false);

    Mat histImage2(hist_h, hist_w, CV_8UC1, Scalar(255, 255, 255)); 
    normalize(hist2, hist2, 0, histImage2.rows, NORM_MINMAX, -1, Mat());
    for (int i = 1; i < histSize; i++)
    {
        line(histImage2, Point(bin_w * (i - 1),hist_h - cvRound(hist2.at<float>(i - 1))),
        Point(bin_w * (i), hist_h -cvRound(hist2.at<float>(i))),
        Scalar(0, 0, 0), 2, 8, 0);
    }
    
    //namedWindow("histogramme de l'image egaliser", WINDOW_AUTOSIZE); 
    //imshow("histogramme de l'image egaliser", histImage2);

    // Binarisation 
    
    // on transforme l'image RGB en HSV et on extrait les channels qui compose l'image HSV
    Mat hsvImg;
    cvtColor(image, hsvImg, COLOR_BGR2HSV);
    vector<Mat>channels;
    split(hsvImg, channels);
    namedWindow("H", WINDOW_AUTOSIZE); 
    imshow("H", channels[0]);
    namedWindow("S", WINDOW_AUTOSIZE); 
    imshow("S", channels[1]);

    Mat BinGray;
    threshold(grayScaleImg,BinGray, 0,255,THRESH_BINARY + THRESH_OTSU);
    namedWindow("binarisation grascale", WINDOW_AUTOSIZE); 
    imshow("binarisation grascale", BinGray);

    Mat BinH;
    threshold(channels[0],BinH, 0,255,THRESH_BINARY + THRESH_OTSU);
    namedWindow("binarisation H", WINDOW_AUTOSIZE); 
    imshow("binarisation H", BinH);

    Mat BinS;
    threshold(channels[1],BinS, 0,255,THRESH_BINARY + THRESH_OTSU);
    namedWindow("binarisation S", WINDOW_AUTOSIZE); 
    imshow("binarisation S", BinS);

    // Produit de l'image grayscale et l'image binarise
    Mat res = grayScaleImg;
    for (int i = 0; i < image.rows; i++)
        for (int j = 0; j < image.cols; j++)
            res.at<uchar>(i, j)=(BinGray.at<uchar>(i, j)/255)*grayScaleImg.at<uchar>(i, j);
    imshow("Produit", res);

    waitKey(0);
    destroyAllWindows();
    return 0;
}