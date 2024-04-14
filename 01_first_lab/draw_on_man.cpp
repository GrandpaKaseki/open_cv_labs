
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

using namespace std;
using namespace cv;

bool open_check(Mat img) {
	if (img.empty()) {
		cout << "Error! Can`t open image." << endl;
		return false;
	}
}

void draw_figures(Mat img) {

	std::string text = format("Width: %d, Height: %d", img.cols, img.rows);

	putText(img, text, Point(25, 25), FONT_HERSHEY_PLAIN, 2, Scalar(0, 0, 255), 2);

	// draw chain
	circle(img, Point(601, 426), 15, Scalar(0, 0, 0), 3);
	circle(img, Point(580, 420), 15, Scalar(0, 0, 0), 3);
	circle(img, Point(560, 420), 15, Scalar(0, 0, 0), 3);
	circle(img, Point(540, 420), 15, Scalar(0, 0, 0), 3);
	circle(img, Point(520, 420), 15, Scalar(0, 0, 0), 3);

	// draw nail and wall breaks  
	circle(img, Point(520, 420), 7, Scalar(0, 0, 0), -1);
	line(img, Point(520, 420), Point(479, 314), Scalar(0, 0, 0), 2);
	line(img, Point(520, 420), Point(507, 463), Scalar(0, 0, 0), 2);

	// draw funny russian t-shirt
	rectangle(img, Point(574, 98), Point(903, 119), Scalar(255, 0, 0), -1);
	rectangle(img, Point(609, 161), Point(849, 196), Scalar(255, 0, 0), -1);
	rectangle(img, Point(621, 215), Point(850, 230), Scalar(255, 0, 0), -1);
	rectangle(img, Point(621, 245), Point(850, 260), Scalar(255, 0, 0), -1);
	rectangle(img, Point(621, 275), Point(850, 290), Scalar(255, 0, 0), -1);
	rectangle(img, Point(621, 305), Point(850, 320), Scalar(255, 0, 0), -1);
	rectangle(img, Point(621, 335), Point(850, 350), Scalar(255, 0, 0), -1);
	rectangle(img, Point(621, 365), Point(850, 380), Scalar(255, 0, 0), -1);
	rectangle(img, Point(621, 395), Point(850, 410), Scalar(255, 0, 0), -1);
	
	imwrite("result.jpg", img);
	imshow("Image", img);
}

void quartering(Mat image) {
	int img_rows = image.rows;
	int img_cols = image.cols;

	Mat top_left_quart = image(Range(0, img_rows / 2), Range(0, img_cols / 2));
	Mat top_right_quart = image(Range(0, img_rows / 2), Range(img_cols / 2, img_cols));
	Mat bot_left_quart = image(Range(img_rows / 2, img_rows), Range(0, img_cols / 2));
	Mat bot_right_quart = image(Range(img_rows / 2, img_rows), Range(img_cols / 2, img_cols));

	imshow("Top_left_quart", top_left_quart);
	imshow("Top_right_quart", top_right_quart);
	imshow("Botttom_left_quart", bot_left_quart);
	imshow("Botttom_right_quart", bot_right_quart);
}

void mask(Mat img) {
	Mat mask(img.size(), CV_8UC1, Scalar(0));
	rectangle(mask, Point(480, 400), Point(560, 480), Scalar(255, 255, 255), -1);
	Mat res;
	img.copyTo(res, mask);
	
	imshow("Mask Result", res);
}

int main() {
	setlocale(0, "");

	Mat img = imread("C:/Users/KOPAT/source/repos/OpenCVProject/OpenCVProject/black_man.jpg");
	// imshow("Image", image);
	if (open_check(img)) {
		draw_figures(img);
		quartering(img);
		mask(img);
		waitKey(0);
	}



	return 0;
}