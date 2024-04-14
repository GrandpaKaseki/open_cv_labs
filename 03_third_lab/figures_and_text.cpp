#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>
#include "pheonix.h"


using namespace std;
using namespace cv;

bool is_open(Mat image) {
    if (image.empty()) {
        cout << "Failed to open image!" << endl;
        return false;
    }
    return true;
}

int main() {
    setlocale(0, "");

    Mat img1 = imread("playstation.jpg");
    if (is_open(img1))
    {

        Mat image_gray, img_blur, angles;
        vector<vector<Point>> contours;

        cvtColor(img1, image_gray, COLOR_BGR2GRAY);
        GaussianBlur(image_gray, img_blur, Size(3, 3), 0);
        Canny(img_blur, angles, 100, 300);
        findContours(angles, contours, RETR_TREE, CHAIN_APPROX_TC89_L1);
        Mat contours_draw = Mat::zeros(angles.size(), CV_8UC3);
        vector<vector<Point>> already_drawn;
        for (int i = 0; i < contours.size(); i++) {
            vector<Point> approx;
            approxPolyDP(contours[i], approx, arcLength(contours[i], true) * 0.01, true);
            int edges_num = approx.size();

            string name_figure;
            Scalar color_figure;
            Moments poisk_center = moments(contours[i]);
            
            if (edges_num == 4) {
                name_figure = "Square";
                color_figure = Scalar(0, 0, 255);
            }
            else {
                if (edges_num == 3) {
                    name_figure = "Triangle";
                    color_figure = Scalar(0, 255, 0);
                }
                else {
                    if (edges_num > 15) {
                        name_figure = "Circle";
                        color_figure = Scalar(255, 0, 0);
                    }
                }
            }
            int shift = -150;
            for (int j = 0; j < already_drawn.size(); j++) {
                int ax = int(poisk_center.m10 / poisk_center.m00);
                int ay = int(poisk_center.m01 / poisk_center.m00);
                int bx = int(moments(already_drawn[j]).m10 / moments(already_drawn[j]).m00);
                int by = int(moments(already_drawn[j]).m01 / moments(already_drawn[j]).m00);
                if (abs(ax - bx) < 5 && abs(ay - by) < 5) {
                    shift = 150;
                }
            }
            already_drawn.push_back(contours[i]);
            drawContours(img1, contours, i, color_figure, 2);
            Point text_coord(poisk_center.m10 / poisk_center.m00 - shift, poisk_center.m01 / poisk_center.m00);
            putText(img1, name_figure, text_coord, FONT_HERSHEY_SIMPLEX, 0.8, color_figure, 2);
        }
        imwrite("figures_res.jpg", img1);
        waitKey(2000);
    }
    Mat img2 = imread("laught.jpg");
    if (is_open(img2)) {

        Mat img2_blur, img2_canny;
        GaussianBlur(img2, img2_blur, Size(3, 3), 0);
        Canny(img2_blur, img2_canny, 100, 200);
        imwrite("text_res.jpg", img2_canny);
        waitKey(0);
        return 0;
    }
    return -1;
    
}