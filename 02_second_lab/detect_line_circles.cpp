#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

using namespace std;
using namespace cv;

bool is_open(Mat image) {
    if (image.empty()) {
        cout << "Failed to open image!" << endl;
        return false;
    }
    return true;
}

int  change_color(Mat image, int number, string win_name) {
    Mat color_space;
    cvtColor(image, color_space, number);
    
    imshow(win_name, color_space);
    waitKey(1000);
    destroyWindow(win_name);
    return 0;
}


int main() {
    setlocale(0, "");
    
    Mat image = imread("playstation.jpg");
   
    if (is_open(image)) {
        Mat image_for_lines, image_for_circels;
        image.copyTo(image_for_lines);
        image.copyTo(image_for_circels);

        imshow("BGR", image);
        waitKey(2000);
        destroyWindow("BGR");

        change_color(image, COLOR_BGR2RGB, "RGB");
        change_color(image, COLOR_BGR2HSV, "HSV");
        change_color(image, COLOR_BGR2Lab, "LAB");
        change_color(image, COLOR_BGR2XYZ, "XYZ");

        Mat gray_img;
        cvtColor(image, gray_img, COLOR_BGR2GRAY);

        imshow("GRAY", gray_img);
        waitKey(2000);
        destroyWindow("GRAY");
        
        Mat image_blur;
        GaussianBlur(gray_img, image_blur, Size(5, 5), 0);
        imshow("Blured image", image_blur);
        waitKey(1000);
        destroyWindow("Blured image");

        vector<Vec4i> lines;
        Mat angels_search;
        Canny(image_blur, angels_search, 150, 300);
        HoughLinesP(angels_search, lines, 1, CV_PI / 180, 50, 50, 20);
        for (size_t i = 0; i < lines.size(); i++) {
            Vec4i line_i = lines[i];
            line(image_for_lines, Point(line_i[0], line_i[1]), Point(line_i[2], line_i[3]), Scalar(0, 0, 255), 4);
        }

        imshow("Detected lines", image_for_lines);
        imwrite("lines.jpg", image_for_lines);
        waitKey(2000);
        destroyWindow("Detected lines");


        
        vector<Vec3f> circles;
        HoughCircles(image_blur, circles, HOUGH_GRADIENT_ALT, 1.5, 200,300,0.9);

        for (size_t i = 0; i < circles.size(); i++) {
            Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
            int rad = cvRound(circles[i][2]);
            circle(image_for_circels, center, rad, Scalar(0, 0, 255), 4, 8, 0);
        }
        imshow("Circels", image_for_circels);
        imwrite("cirels.jpg", image_for_circels);
        waitKey(2000);
        destroyAllWindows();
        return 0;
    }
    return -1;
    
}