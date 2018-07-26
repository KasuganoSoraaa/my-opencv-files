#include "opencv2/opencv.hpp"
#include <opencv2/nonfree/nonfree.hpp>//SIFT
#include <opencv2/legacy/legacy.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include <vector>
#include<iostream>
using namespace std;
using namespace cv;

int main()
{
	Mat srcImg1 = imread("psb.jpg");
	Mat srcImg2 = srcImg1(Rect(228, 235, 142, 217)).clone();
	Point center = Point(srcImg2.rows, srcImg2.cols);
	double angle = 30;
	Mat rotMat = getRotationMatrix2D(center, angle, 1);
	warpAffine(srcImg2, srcImg2, rotMat, srcImg2.size(), INTER_LINEAR, BORDER_CONSTANT, Scalar(255, 255, 255));
	//定义SIFT特征检测类对象
	SiftFeatureDetector siftDetector;//SIFT=SiftFeatureDetector=SiftDescriptorExtractor    别名//定义KeyPoint变量
	vector<KeyPoint>keyPoints1(10000);
	vector<KeyPoint>keyPoints2(10000);
	//特征点检测
	siftDetector.detect(srcImg1, keyPoints1);
	siftDetector.detect(srcImg2, keyPoints2);
	//绘制特征点(关键点)
	Mat feature_pic1, feature_pic2;
	//drawKeypoints(srcImg1, keyPoints1, feature_pic1, Scalar(0, 0, 255));
	drawKeypoints(srcImg2, keyPoints2, feature_pic2, Scalar(0, 0, 255));
	drawKeypoints(srcImg1, keyPoints1, feature_pic1, Scalar(0, 0, 255));
	//颜色随机，带有方向
	//drawKeypoints(srcImg2, keyPoints2, feature_pic2, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	//显示原图
	//imshow("src1", srcImg1);
	//imshow("src2", srcImg2);
	//显示结果
	imshow("feature1", feature_pic1);
	imshow("feature2", feature_pic2);
	//cout << keyPoints1.size() << endl;
	//cout << keyPoints2.size() << endl;

	//计算特征点描述符 / 特征向量提取
	SiftDescriptorExtractor descriptor;
	Mat description1;
	descriptor.compute(srcImg1, keyPoints1, description1);
	Mat description2;
	descriptor.compute(srcImg2, keyPoints2, description2);
	cout << description1.cols << endl;
	cout << description1.rows << endl;

	//进行BFMatch暴力匹配
	BruteForceMatcher<L2<float>>matcher;    //实例化暴力匹配器
	vector<DMatch>matches(100000);   //定义匹配结果变量
	matcher.match(description1, description2, matches);  //实现描述符之间的匹配

														// 匹配结果筛选
	nth_element(matches.begin(), matches.begin() + 12, matches.end());   //提取出前30个最佳匹配结果     
	matches.erase(matches.begin() + 12, matches.end());    //剔除掉其余的匹配结果

	Mat result;
	drawMatches(srcImg1, keyPoints1, srcImg2, keyPoints2, matches, result, Scalar(255, 255, 255), Scalar::all(-1));//匹配特征点绿色，单一特征点颜色随机
	imshow("Match_Result", result);

	waitKey(0);
}
