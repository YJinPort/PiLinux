#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
	Mat m1(4, 3, CV_32FC3);

	cout << "dimsCnt = " << m1.dims << endl;
	cout << "rowsCnt = " << m1.rows << endl;
	cout << "colsCnt = " << m1.cols << endl;
	cout << "matSize = " << m1.size() << endl << endl;

	cout << "totElemCnt = " << m1.total() << endl;
	cout << "elemSize = " << m1.elemSize() << endl;
	cout << "elemSizeOfChannel = " << m1.elemSize1() << endl << endl;

	cout << "type = " << m1.type() << endl;
	cout << "type(channelCnt|depth) = " << ((m1.channels() - 1) << 3) + m1.depth() << endl;
	cout << "depth = " << m1.depth() << endl;
	cout << "channel = " << m1.channels() << endl << endl;

	cout << "step = " << m1.step << endl;
	cout << "step1() = " << m1.step1() << endl;

	return 0;
}
