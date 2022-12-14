#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

int main()
{
	Mat image = Mat::zeros(300, 400, CV_8UC3);
	Scalar color(255, 255, 0);
	vector<int> jpegParams, pngParams;
	jpegParams.push_back(IMWRITE_JPEG_QUALITY);
	jpegParams.push_back(80);
	pngParams.push_back(IMWRITE_PNG_COMPRESSION);
	pngParams.push_back(9);

	circle(image, Point(100, 100), 50, color, -1);

	imwrite("jpegImage.jpg", image, jpegParams);
	imwrite("pngImage.png", image, pngParams);

	return 0;
}
