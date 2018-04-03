#include <iostream>
#include <string>
#include <algorithm>
#include "opencv2\core\core.hpp"
#include "opencv2/face.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\objdetect\objdetect.hpp"
#include "opencv2\opencv.hpp"
#include <string.h>
#include <math.h>

#define CLEAR_FACTOR 2
#define TOLERANCE 0.07

using namespace std;
using namespace cv;

int main(int argv, char *ch[])
{
	cout << "Tolerance: 3-4%\n";
	double requ = 0.12;
	string file = "e:/testd.jpg";
	//"e:/test12.jpg"
	
	//double size = atof(ch[2]);
	//size = .8;
	Mat image = imread(file, CV_LOAD_IMAGE_COLOR);
	if (image.empty())	return -1;
	Mat img;
	imshow("ORIGINAL IMAGE", image);
	cvtColor(image, img, CV_BGR2GRAY);
	equalizeHist(img, img);
	//threshold(img, img, 25, 255, THRESH_OTSU | THRESH_BINARY_INV);
	//medianBlur(img, img, 7);;
	GaussianBlur(img,img,Size(13,13),0.2,0.2);
	adaptiveThreshold(img, img,130,CV_ADAPTIVE_THRESH_MEAN_C,THRESH_BINARY_INV,25,0);
	threshold(img, img, 128, 255, CV_THRESH_OTSU | CV_THRESH_BINARY);
	//imshow("TEST", img);
	Mat kernel = getStructuringElement(MORPH_CROSS, Size(3, 3));
	morphologyEx(img, img, 3, kernel);
	Mat dilated;
	erode(img, dilated, kernel, Point(-1, -1), CLEAR_FACTOR);
	dilate(dilated, dilated, kernel, cv::Point(-1, -1), CLEAR_FACTOR);
	imshow("ADF", dilated);
	vector<vector<Point> > contours, fina;
	float lim = float(img.rows*img.cols);
	findContours(dilated, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	for (int i = 0;i < contours.size();i++) {
		if (contourArea(contours[i]) >= 0.08*lim)
//			if (contourArea(contours[i]) <= requ*lim && contourArea(contours[i]) >= 0.08*lim)

			fina.push_back(contours[i]);
	}
	vector<vector<Point> > contours_poly(fina.size());
	vector<Rect> boundRect(fina.size());
	for (int i = 0; i < fina.size(); i++)
	{
		approxPolyDP(Mat(fina[i]), contours_poly[i], 3, true);
		boundRect[i] = boundingRect(Mat(contours_poly[i]));
	}

	//drawContours(image, fina, -1, Scalar(0, 255, 0), 2);
	for (int i = 0; i < fina.size(); i++) {
		Rect r = boundRect[i];
		double t_area = (r.width * r.height);
		//cout << t_area << endl;
		double ratio = t_area / (image.cols*image.rows);
//		if ((ratio + TOLERANCE) >= size && size <= (ratio - TOLERANCE)) {
		if (t_area <= requ*lim && t_area >= 0.08*lim) {		//comment this line

			rectangle(image, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 0, 255), 2, 8, 0);

			//}
			//cout << (r.width * r.height) << endl;
			//cout<<
			cout << "Percentage: " << ceil(ratio * 100) <<"%" <<endl;
		}			//comment this 
			}
	
	imshow("e:/result_test12.jpg", image);
	//imshow("Result", image);
	waitKey(0);
	return 0;
}