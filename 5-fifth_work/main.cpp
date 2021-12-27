#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

RNG rng(12345);
#define PI 3.14159265

float min_dif_zero(float a, float b)
{
    if (a == 0) return b;
    else if (b == 0) return a; 
    else if (a < b) return a; 
    else return b;
}

float min_dif_zero1(float X[5])
{
    float mi;
    int i = 0;
    while (X[i] == 0) i++; 
    mi = X[i];
    for (int j = i+1; j < 5; j++)
    {
        if ((X[j] != 0) && ( X[j]<mi)) mi = X[j];
    }

    return mi;
}

Mat K_Means(Mat Input, int K)
{
    Mat samples(Input.rows * Input.cols, Input.channels(), CV_32F);
    for (int y = 0; y < Input.rows; y++){
        for (int x = 0; x < Input.cols; x++){
            for (int z = 0; z < Input.channels(); z++){
                if (Input.channels() == 3)
                {
                    samples.at<float>(y + x * Input.rows, z) = Input.at<Vec3b>(y, x)[z];
                }
                else
                {
                    samples.at<float>(y + x * Input.rows, z) = Input.at<uchar>(y, x);
                }
            }
        }
    }
    Mat labels;
    int attempts = 5;
    Mat centers;
    kmeans(samples, K, labels, TermCriteria(TermCriteria::MAX_ITER | TermCriteria::EPS, 10000, 0.0001), attempts, KMEANS_PP_CENTERS, centers);
    Mat new_image(Input.size(), Input.type());
    for (int y = 0; y < Input.rows; y++) {
        for (int x = 0; x < Input.cols; x++)
        {
            int cluster_idx = labels.at<int>(y + x * Input.rows, 0);
            if (Input.channels() == 3)
            {
                for (int i = 0; i < Input.channels(); i++)
                {
                    new_image.at<Vec3b>(y, x)[i] = centers.at<float>(cluster_idx, i);
                }
            }
            else 
            {
                new_image.at<uchar>(y, x) = centers.at<float>(cluster_idx, 0);
            }
        }
    }
    return new_image;
}


int main( int argc, char** argv )
{
     // on lit l'image originale
    Mat image = imread("image8_c.jpg",IMREAD_COLOR);
    if(! image.data)
    {
        cout<<"Could not open file" << endl;
        return -1;
    }
    imshow("originalimage", image);

    Mat img, ImgGray;
    
    cvtColor(image, ImgGray, COLOR_BGR2GRAY);
    threshold(ImgGray, img, 1, 255,THRESH_BINARY+THRESH_OTSU);
    imshow("image seuillee", img);

    // Etiquettage

    int C;
    cout << "Donner le nombre de régions :";
    cin >> C;
    Mat clas = K_Means(ImgGray, C); 
    Mat clas_col; 
    imshow("Segmentation", clas);

    int nl = image.rows;
    int nc = image.cols; 
    int i, j; 
    vector<uchar>index;
    index.push_back(clas.at<uchar>(0, 0));
    int u;
    int k = 1;
    for (i = 0; i < nl; i++)
    {
        for (j = 0; j < nc; j++)
        {
            u=0;
            while ((u < k) && (clas.at<uchar>(i, j)!=index[u]))
            {
            u++;
            }
            if (u >= k)
            {
            cout <<" u="<< u<< endl; 
            index.push_back(clas.at<uchar>(i, j)); 
            k++;
            }
        }
    }
    cout << " k= " << k << endl;
    for (i = 0; i < k; i++)
        cout << (int) index[i] << endl;
    int dilation_type = 0;
    int dilation_size = 1;
    int erosion_size = 1;
    Mat element1 = getStructuringElement(dilation_type, Size(2 * dilation_size + 1, 2 * dilation_size + 1), Point(dilation_size, dilation_size));
    Mat D;
    int ih, jh, ib, jb, ig,jg,id,jd;
    Point pt1, pt2;
    Mat clas1= Mat(image.rows, image.cols, CV_8UC1,Scalar(0));
    for (u = 0; u < C; u++)
    {
        for (i = 0; i < nl; i++)
        {
            for (j = 0; j < nc; j++)
            {
                if (clas.at<uchar>(i, j) == index[u])
                    clas1.at<uchar>(i, j) = (uchar) 255;
                else
                    clas1.at<uchar>(i, j) = (uchar)0;
            }
        }
         
        dilate(clas1, D, element1);
        erode (D,clas1, element1);
        imshow("Segmentation", clas1);


        i = 0;
        int flag = 0;
        while ((i < nl) && (flag == 0))
        {
            j = 0;
            while ((j < nc) && (clas1.at<uchar>(i, j) == (uchar)0)){
                j++;
            }
            if (j < nc)
            {
                flag = 1; 
                ih = i; 
                jh = j; 
                break;
            }
            i++;
        }
        i = nl-1;
        flag = 0;
        while ((i>=0 ) && (flag == 0))
        {
            j = nc-1;
            while ((j >=0) && (clas1.at<uchar>(i, j) == (uchar)0)){
                j--;
            }
            if (j >=0)
            {
                flag = 1; 
                ib = i; 
                jb= j; 
                break;
            }
            i--;
        }
        j = 0;
        flag = 0;
        while ((j < nc) && (flag == 0))
        {
            i = 0;
            while ((i < nl) && (clas1.at<uchar>(i, j) == (uchar)0)){
                i++;
            }
            if (i < nl)
            {
                flag = 1; 
                ig = i; 
                jg = j; 
                break;
            }
            j++;
        }

        j = nc-1;
        flag = 0;
        while ((j >= 0) && (flag == 0))
        {
            i = nl-1;
            while ((i >= 0) && (clas1.at<uchar>(i, j) == (uchar)0)){
                i--;
            }
            if (i >= 0)
            {
                flag = 1; 
                id = i; 
                jd = j; 
                break;
            }
            j--;
        }
        cout << ih << " " << jg << " " << ib << " " << jd << endl;
        pt1 = Point(jg, ih); 
        pt2 = Point(jd, ib);
        rectangle(clas1, pt1, pt2, Scalar(50, 255, 0), 2, 8);
        imshow("Segmentation 2", clas1);
        rectangle(image, pt1, pt2, Scalar(255, 50, 0), 2, 8);
        imshow("Segmentation 3", image);
        waitKey(0);
    }

    int xg, yg;
    cout << "Nombre de colonnes : " << nc << endl; 
    cout << "Nombre de lignes: " << nl << endl; 
    int compt = 0;
    int x; 
    xg = 0;
    yg = 0;
    for (i = 0; i < nl; i++)
    {
        for (j = 0; j < nc; j++)
        {
            if (clas1.at<uchar>(i, j) != 0)
            {
                compt++;
                xg = xg + j;
                yg = yg + i;
            }
        }
    }
    xg = xg / compt;
    yg = yg / compt;
    cout << "xg=" << xg << "  yg=" << yg << endl;

    Point2f ptc = Point(xg, yg);

    circle(image, ptc, 10, Scalar(0, 255, 255), FILLED, LINE_8);
    imshow("centre de gravité", image);
    waitKey(0); 
    imshow("Segmentation 4", clas1);
    waitKey(0);


    Rect ROI = Rect(pt1, pt2);
    Mat image_roi = image(ROI);
    imshow("ROI", image_roi);
    waitKey(0);

    int lowThreshold = 200;
    const int max_lowThreshold = 100;
    const int ratio = 2;
    const int kernel_size = 3;
    blur(image_roi, image_roi, Size(3, 3));
    Canny(image_roi, D, lowThreshold, max_lowThreshold*ratio, kernel_size);
    imshow("image contour", D);
    waitKey(0);

    vector<vector<Point> > contours;
    findContours(D, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);
    vector<vector<Point> > contours_poly(contours.size());
    vector<Rect> boundRect(contours.size());
    vector<Point2f>centers(contours.size());
    vector<float>radius(contours.size());
    for (size_t i = 0; i < contours.size(); i++)
    {
        approxPolyDP(contours[i], contours_poly[i], 3, true);
        boundRect[i] = boundingRect(contours_poly[i]);
        minEnclosingCircle(contours_poly[i], centers[i], radius[i]);
    }
    Mat drawing = Mat::zeros(D.size(), CV_8UC3);
    for (size_t t = 0; t < contours.size(); t++)
    {
        Scalar color = Scalar(rng.uniform(0, 256), rng.uniform(0, 256),
        rng.uniform(0, 256));
        drawContours(drawing, contours_poly, (int)t, color);
        rectangle(drawing, boundRect[t].tl(), boundRect[t].br(), color, 2);
        circle(drawing, centers[t], (int)radius[t], color, 2);
    }
    imshow("Contours", drawing);
    waitKey(0);

    int f=0;
    int area = contourArea(contours[0]);
    int per = arcLength(contours[0], "True");
    cout << "Surface= " << area << " perimetre= " << per << endl;
    float circ = (4. * PI * area) / (per * per);
    cout << "circularite= " << circ << endl;
    vector<float>dist;
    float z, maxdist = 0, mindist = 1000000, moydist = 0;
    for (i = 0; i < contours[0].size(); i++)
    {
        z = (contours[0][i].x - xg) * (contours[0][i].x - xg) + (contours[0][i].y - yg) * (contours[0][i].y - yg);
        z = sqrt(z);
        if (z < mindist) mindist = z;
        if (z > maxdist) maxdist = z;
        moydist = moydist + z;
        dist.push_back(z);
    }
    moydist = moydist / contours[0].size();
    cout << "maxsignature= " << maxdist << " minsignature= " << mindist << " moyenneSignature =" << moydist << endl;
    int histSize = 256; // taille binaire
    float range[] = { 0, 255 };
    const float* ranges[] = { range };
    MatND hist;

    // plot de l'histogramme
    int hist_w = 512; int hist_h = 400;
    int bin_w = cvRound((double)hist_w / histSize);
    Mat histImage(hist_h, hist_w, CV_8UC1, Scalar(0, 0, 0));
    normalize(hist, hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
    // plot signature
    histSize = contours[0].size();
    hist_w = 512; hist_h = 400;
    bin_w = cvRound((double)hist_w / histSize);
    Mat distImage(hist_h, hist_w, CV_8UC1, Scalar(0, 0, 0));
    normalize(dist, dist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
    for (i = 1; i < histSize; i++)
    {
        line(distImage, Point(bin_w * (i - 1), hist_h - cvRound(dist[i - 1])),
        Point(bin_w * (i), hist_h - cvRound(dist[i])),
        Scalar(255, 0, 0), 2, 8, 0);
    }
    namedWindow("Signature", 1);
    imshow("Signature", distImage);
    waitKey(0);

    //LBP de l'image ROI
    const char* output_tt = "LBP Result";
    int width = image_roi.cols;
    int height = image_roi.rows;
    Mat lbpImage = Mat::zeros(image_roi.rows - 2, image_roi.cols - 2, CV_8UC1);//3 * 3 window, there isone pixel on each side of the border
    for (int row = 1; row < height - 1; row++) {
        for (int col = 1; col < width - 1; col++) {
            uchar c = image_roi.at<uchar>(row, col);//Get the center pixel value
            uchar code = 0;//Signature
            code |= (image_roi.at<uchar>(row - 1, col - 1) > c) << 7;
            code |= (image_roi.at<uchar>(row - 1, col) > c) << 6;
            code |= (image_roi.at<uchar>(row - 1, col + 1) > c) << 5;
            code |= (image_roi.at<uchar>(row, col + 1) > c) << 4;
            code |= (image_roi.at<uchar>(row + 1, col + 1) > c) << 3;
            code |= (image_roi.at<uchar>(row + 1, col) > c) << 2;
            code |= (image_roi.at<uchar>(row + 1, col - 1) > c) << 1;
            code |= (image_roi.at<uchar>(row, col - 1) > c) << 0;
            lbpImage.at<uchar>(row - 1, col - 1) = code; // Original picture 1, 1 = Effect picture 0, 0
        }
    }
    imshow("image_LBP", lbpImage);
    waitKey(0);

    destroyAllWindows();
    return 0;
}