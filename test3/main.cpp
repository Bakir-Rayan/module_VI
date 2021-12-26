#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

// DRINKWATER

class WatershedSegmenter {

	private:
		Mat markers;
	public:
		void setMarkers(const Mat& markerImage) {
			markerImage.convertTo(markers, CV_32S);
		}

		Mat process(const Mat& image) 
		{
			watershed(image, markers);
			return markers;
		}
};

// K MEANS 

Mat K_Means(Mat Input, int K)
{
	Mat samples(Input.rows * Input.cols, Input.channels(), CV_32F);
	for (int y = 0; y < Input.rows; y++)
		for (int x = 0; x < Input.cols; x++)
			for (int z = 0; z < Input.channels(); z++)
				if (Input.channels() == 3)
				{
					samples.at<float>(y + x * Input.rows, z) = Input.at<Vec3b>(y, x)[z];
				}
				else
				{
					samples.at<float>(y + x * Input.rows, z) = Input.at<uchar>(y, x);
				}

	Mat labels;
	int attempts = 5;
	Mat centers;
	kmeans(samples, K, labels, TermCriteria(TermCriteria::MAX_ITER | TermCriteria::EPS, 10000, 0.0001), attempts, KMEANS_PP_CENTERS, centers);
	Mat new_image(Input.size(), Input.type());

	for (int y = 0; y < Input.rows; y++)
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
			else {
				new_image.at<uchar>(y, x) = centers.at<float>(cluster_idx, 0);
			}
		}
	imshow("clustered image", new_image);
	return new_image;
}



int main()
{
	Mat image, img_G_1, img_G_2, img_S_P, ImgGray;

	image = imread("image8.jpg");

	// DRINKWATER

	Mat binary;

	cvtColor(image, binary, COLOR_BGR2GRAY);
	threshold(binary, binary, 100, 255, THRESH_BINARY);
	imshow("originalimage", image);
	imshow("originalbinary", binary);

	// Eliminate noise and smaller objects

	Mat fg;
	erode(binary, fg, Mat(), Point(-1, -1), 2);
	imshow("fg", fg);

	// Identify image pixels without objects

	Mat bg;
	dilate(binary, bg, Mat(), Point(-1, -1), 3);
	threshold(bg, bg, 1, 128, cv::THRESH_BINARY_INV);
	imshow("bg", bg);

	// Create markers image

	Mat markers(binary.size(), CV_8U, cv::Scalar(0));
	markers = fg + bg;
	imshow("markers", markers);

	// Create watershed segmentation object

	WatershedSegmenter segmenter;
	segmenter.setMarkers(markers);
	Mat result = segmenter.process(image);
	result.convertTo(result, CV_8U);
	imshow("final_result", result);
	waitKey(0);

	// Filtrage mean Shift
	Mat res;
	pyrMeanShiftFiltering(image, res, 20, 45, 3);
	imwrite("meanshift.png", res);
	imshow("Meanshift", res);
	waitKey();

	// K MEANS 

	int C;
	cout << "Donner le nombre de régions :";
	cin >> C;
	cvtColor(image, ImgGray, COLOR_BGR2GRAY);
	Mat clas = K_Means(ImgGray, C);
	imshow("Segmentation", clas);
	waitKey(0);

}