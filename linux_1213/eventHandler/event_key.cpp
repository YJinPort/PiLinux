#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
	Mat image(200, 300, CV_8U, Scalar(255));
	namedWindow("keyboard Event", WINDOW_AUTOSIZE);
	imshow("keyboard Event", image);

	while(1) {
		int key = waitKey(100);

		if(key == 27) break;
		
		switch(key)
		{
			case 'a': cout << "input 'a' " << endl; break;
			case 'b': cout << "input 'b' " << endl; break;
			case 0x41: cout << "input 'A' " << endl; break;
			case 66: cout << "input 'B' " << endl; break;

			case 0x51: cout << "input Left " << endl; break;
			case 0x52: cout << "input Up " << endl; break;
			case 0x53: cout << "input Right " << endl; break;
			case 0x54: cout << "input Down " << endl; break;
		}
	}

	return 0;
}
