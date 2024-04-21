#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>

using namespace std;
using namespace cv;


void detectCard(string& cardName, Mat& card, vector<Mat>& cardsImages, vector<string>& cardsNames, vector<Mat>& cardsDescriptors, vector<vector<KeyPoint>>& cardsKeypoints) {

	Mat cardDescriptors;
	vector<KeyPoint> cardKeypoints;
	Ptr<ORB> detector = ORB::create();
	Ptr<BFMatcher> matcher = BFMatcher::create();
	detector->detectAndCompute(card, noArray(), cardKeypoints, cardDescriptors);

	if (cardDescriptors.empty()) {
		cardName = "";
		return;
	}

	int maxI = -1;
	int maxCount = 0;

	for (int i = 0; i < cardsImages.size(); i++) {

		if (cardsDescriptors[i].empty()) {
			continue;
		}

		vector<vector<DMatch>> knn_matches;

		matcher->knnMatch(cardsDescriptors[i], cardDescriptors, knn_matches, 3);

		vector<DMatch> correct;

		for (size_t i = 0; i < knn_matches.size(); i++) {
			if (knn_matches[i][0].distance < 0.75 * knn_matches[i][1].distance) {
				correct.push_back(knn_matches[i][0]);
			}
		}

		if (maxCount < correct.size()) {
			maxCount = static_cast<int>(correct.size());
			maxI = i;
		}
	}

	if (maxI == -1) {
		cardName = "";
	}
	else {
		cardName = cardsNames[maxI];
	}
}


int main() {

	vector<Mat> card_img;
	vector<string> card_nm;
	vector<Mat> card_descr;
	vector<vector<KeyPoint>> card_key;

	Mat card;
	card = imread("cards/TusB.png");
	GaussianBlur(card, card, Size(5, 5), 2);
	card_img.push_back(card);
	card_nm.push_back("Tus_buby");

	card = imread("cards/DamaCh.png");
	GaussianBlur(card, card, Size(5, 5), 3);
	card_img.push_back(card);
	card_nm.push_back("Dama_cherv");

	card = imread("cards/ValetK.png");
	GaussianBlur(card, card, Size(5, 5),3);
	card_img.push_back(card);
	card_nm.push_back("Valet_krest");

	card = imread("cards/KingV.png");
	GaussianBlur(card, card, Size(5, 5), 2);
	card_img.push_back(card);
	card_nm.push_back("King_viny");


	Ptr<ORB> detector = ORB::create();

	for (int i = 0; i < card_img.size(); i++) {
		Mat dis;
		vector<KeyPoint> keys;
		detector->detectAndCompute(card_img[i], noArray(), keys, dis);
		card_key.push_back(keys);
		card_descr.push_back(dis);
	}

	Mat img = imread("cards_orig.jpg");

	vector<vector<Point>> contours;

	Mat blur, thresh, canny, gray;
	cvtColor(img, gray, COLOR_BGR2GRAY);
	GaussianBlur(gray, blur, Size(3, 3),2);

	Canny(blur, canny, 100, 200);
	findContours(canny, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);

	for (const auto& contour : contours) {
		vector<Point> contoursPoly;

		approxPolyDP(contour, contoursPoly, 1, true);

		RotatedRect cardRect = minAreaRect(contoursPoly);

		if (cardRect.size.width < 100 || cardRect.size.height < 100) {
			continue;
		}

		Mat card;
		string cardName;

		Mat rotatedMatrix, rotatedImage;
		rotatedMatrix = getRotationMatrix2D(cardRect.center, cardRect.angle, 1.0);
		warpAffine(img, rotatedImage, rotatedMatrix, img.size(), INTER_CUBIC);
		getRectSubPix(rotatedImage, cardRect.size, cardRect.center, card);

		rotate(card, card, ROTATE_180);

		if (card.size[0] < card.size[1]) {
			rotate(card, card, ROTATE_90_CLOCKWISE);
		}

		detectCard(cardName, card, card_img, card_nm, card_descr, card_key);

		if (cardName != "") {
			Point2f boxPoints[4];
			cardRect.points(boxPoints);

			for (int j = 0; j < 4; j++) {
				line(img, boxPoints[j], boxPoints[(j + 1) % 4], Scalar(0, 255, 0), 4, LINE_AA);
			}
			putText(img, cardName, cardRect.center, 1, 2, Scalar(0, 0, 0), 3);
		}
	}
	imwrite("cards_result.jpg", img);
	waitKey(0);
}