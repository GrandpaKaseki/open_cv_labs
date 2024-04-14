#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
using namespace cv;
using namespace std;


int main() {
    VideoCapture cap("video.mp4");
    int frame_width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    int frame_height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    VideoWriter video("white_square.mp4", cap.get(cv::CAP_PROP_FOURCC), cap.get(cv::CAP_PROP_FPS), Size(frame_width, frame_height));


    if (!cap.isOpened()) {
        cout << "Error opening video stream or file" << endl;
        return -1;
    }
    Moments prev_center;
    bool first = true;
    while (1) {

        Mat img;
        cap >> img;

        if (img.empty())
            break;

        Mat img_HSV;
        cvtColor(img, img_HSV, COLOR_BGR2HSV);

        Mat white;
        inRange(img_HSV, Scalar(95, 12, 150), Scalar(255, 50, 255), white);
        
        Mat edges;
        Canny(white, edges, 100, 300);
        
        Mat img_blur;
        GaussianBlur(edges, img_blur, Size(3, 3), 0);

        vector<vector<Point>> contour;
        vector<Vec4i> hierarchy;
        findContours(img_blur, contour, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
        if (first) {
            prev_center = moments(contour[0]);
            first = false;
        }
        Mat draw = Mat::zeros(edges.size(), CV_8UC3);
        size_t i = 0;
        Moments prev_center = moments(contour[0]);
        for (size_t i = 0; i < contour.size(); i++) {
            vector<Point> approx;
            approxPolyDP(contour[i], approx, arcLength(contour[i], true) * 0.01, true);

            Moments center = moments(contour[i]);
            
            int ax = center.m10 / center.m00;
            int ay = center.m01 / center.m00;
            int bx = prev_center.m10 / prev_center.m00;
            int by = prev_center.m01 / prev_center.m00;
            if ((abs(ax - bx) <= 1) && (abs(ay - by) <= 1)) {
                drawContours(img, contour, i, Scalar(0, 0, 255), 1, LINE_8, hierarchy, 1);
                Point text_coord(ax, ay);
                putText(img, "Square", text_coord, FONT_HERSHEY_SIMPLEX, 0.8, Scalar(0, 0, 255), 2);
                Moments prev_center = center;
            }
        }
        imshow("White square", img);
        video.write(img);
        char c = (char)waitKey(25);
        if (c == 27)
            break;
    }

    cap.release();
    destroyAllWindows();

    return 0;
}