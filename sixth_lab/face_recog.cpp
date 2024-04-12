
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


int main() {
	setlocale(0, "");
	
	VideoCapture cap("ZUA.mp4");
	Mat gray_img;
	Mat image_blurred;
	int frame_width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
	int frame_height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
	VideoWriter video("outcpp.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 10, Size(frame_width, frame_height));

	if (!cap.isOpened()) {
		cout << "Error opening video stream or file" << endl;
		return -1;
	} 


	else{
		while (1) {

			Mat frame;
			// Capture frame-by-frame
			cap >> frame;

			// If the frame is empty, break immediately
			if (frame.empty())
				break;
			cvtColor(frame, gray_img, COLOR_BGR2GRAY);
			GaussianBlur(gray_img, image_blurred, Size(3, 3), 0);

			CascadeClassifier face_cascade;
			if (!face_cascade.load(cv::samples::findFile("haarcascade_eye.xml"))) {
				cout << "File not found Error!\n";
				return -1;
			}

			vector<Rect> eyes;
			face_cascade.detectMultiScale(image_blurred, eyes, 2.4, 15);

			for (const auto& eye : eyes) {
				rectangle(frame, eye, Scalar(255, 0, 15), 2);
			}


			if (!face_cascade.load(cv::samples::findFile("haarcascade_frontalface_default.xml"))) {
				cout << "File not found Error!\n";
				return -1;
			}

			vector<Rect> faces;
			face_cascade.detectMultiScale(image_blurred, faces, 1.1, 5);

			for (const auto& face : faces) {
				rectangle(frame, face, Scalar(255, 255, 0), 2);
			}


			if (!face_cascade.load(cv::samples::findFile("haarcascade_smile.xml"))) {
				cout << "File not found Error!\n";
				return -1;
			}

			vector<Rect> smiles;
			face_cascade.detectMultiScale(image_blurred, smiles, 1.9, 25);

			for (const auto& smile : smiles) {
				rectangle(frame, smile, Scalar(0, 255, 0), 2);
			}
			video.write(frame);
			// Display the resulting frame
			imshow("Frame", frame);

			// Press  ESC on keyboard to exit
			char c = (char)waitKey(25);
			if (c == 27)
				break;
		}
		
	}
	
	cap.release();


	
	return 0;
}