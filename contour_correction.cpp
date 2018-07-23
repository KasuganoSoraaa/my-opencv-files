#include<opencv2/opencv.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<iostream>
using namespace cv;
using namespace std;

Mat ROI_process(Mat img)
{
	Mat grayimg, bnrimg, dstimg;
	cvtColor(img, grayimg, COLOR_BGR2GRAY);
	threshold(grayimg, bnrimg, 80, 200, CV_THRESH_BINARY);
	vector<vector<Point>> contours;
	findContours(bnrimg, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	for (int j = 0; j < contours.size(); j++)
	{
		Rect rect = boundingRect(Mat(contours[j]));
		if (rect.area() < 600)
		{
			continue;
		}
		dstimg = img(rect);
		imshow("dst", dstimg);
	}
	return dstimg;
}
Mat contour_correction(string img_name)
{
	Mat srcimg = imread(img_name);
	Mat grayimg, bnrimg, dstimg;
	cvtColor(srcimg, grayimg, COLOR_BGR2GRAY);
	threshold(grayimg, bnrimg, 100, 200, CV_THRESH_BINARY);
	//imshow("1", srcimg);
	//imshow("2", grayimg);
	//imshow("3", bnrimg);

	vector<vector<Point>> contours;
	findContours(bnrimg, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	cout << contours.size() << endl;
	for (int i = 0; i < contours.size(); i++)
	{
		CvPoint2D32f rectpoint[4];
		CvBox2D rect = minAreaRect(Mat(contours[i]));

		cvBoxPoints(rect, rectpoint);
		float angle = rect.angle;
		cout << angle << endl;

		int line1 = sqrt((rectpoint[1].y - rectpoint[0].y)*(rectpoint[1].y - rectpoint[0].y) + (rectpoint[1].x - rectpoint[0].x)*(rectpoint[1].x - rectpoint[0].x));
		int line2 = sqrt((rectpoint[3].y - rectpoint[0].y)*(rectpoint[3].y - rectpoint[0].y) + (rectpoint[3].x - rectpoint[0].x)*(rectpoint[3].x - rectpoint[0].x));

		if (line1 * line2 < 600)
		{
			continue;
		}

		if (line1 > line2)
		{
			angle += 90;
		}
		Mat ROIsrcimg(srcimg.rows, srcimg.cols, CV_8UC3);
		ROIsrcimg.setTo(0);
		drawContours(bnrimg, contours, -1, Scalar(255), CV_FILLED);
		srcimg.copyTo(ROIsrcimg, bnrimg);
		/*namedWindow("ROIsrcimg", 1);
		imshow("RoiSrcImg", ROIsrcimg);*/
		Mat Ratationedimg(ROIsrcimg.rows, ROIsrcimg.cols, CV_8UC1);
		Ratationedimg.setTo(0);
		Point2f center = rect.center;
		Mat M2 = getRotationMatrix2D(center, angle, 1);
		warpAffine(ROIsrcimg, Ratationedimg, M2, ROIsrcimg.size(), 1, 0, Scalar(0));
		/*imshow("旋转之后", RatationedImg);*/
		dstimg = Ratationedimg;
	}
	ROI_process(dstimg);
	return dstimg;
}
int main()
{

	Mat asd = contour_correction("rmb.png"), asf;
	asf = ROI_process(asd);
	return 0;
}
