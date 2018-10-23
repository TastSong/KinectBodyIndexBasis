#include <iostream>
#include <Kinect.h>
#include <opencv2\highgui\highgui.hpp>

using   namespace   std;
using   namespace   cv;

int main(void)
{
	IKinectSensor   * mySensor = nullptr;               //Sensor
	GetDefaultKinectSensor(&mySensor);
	mySensor->Open();

	IBodyIndexFrameSource   * mySource = nullptr;       //Source
	mySensor->get_BodyIndexFrameSource(&mySource);

	int height = 0, width = 0;
	IFrameDescription   * myDescription = nullptr;
	mySource->get_FrameDescription(&myDescription);
	myDescription->get_Height(&height);
	myDescription->get_Width(&width);

	IBodyIndexFrameReader   * myReader = nullptr;       //Reader
	mySource->OpenReader(&myReader);

	IBodyIndexFrame     * myFrame = nullptr;            //Frame
	Mat img(height, width, CV_8UC3);
	Vec3b   color[7] = { Vec3b(0, 0, 255), Vec3b(0, 255, 255), Vec3b(255, 255, 255), Vec3b(0, 255, 0), Vec3b(255, 0, 0), Vec3b(255, 0, 255), Vec3b(0, 0, 0) };

	while (1)
	{
		if (myReader->AcquireLatestFrame(&myFrame) == S_OK)
		{
			UINT    size = 0;
			BYTE    * buffer = nullptr;
			myFrame->AccessUnderlyingBuffer(&size, &buffer);
			for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++)
			{
				int index = buffer[i * width + j];      //0-5代表人体，其它值代表背景，用此将人体和背景渲染成不同颜色
				if (index <= 5)
					img.at<Vec3b>(i, j) = color[index];
				else
					img.at<Vec3b>(i, j) = color[6];

			}
			imshow("TEST", img);
			myFrame->Release();
		}
		if (waitKey(30) == VK_ESCAPE)
			break;
	}
	myReader->Release();
	myDescription->Release();
	mySource->Release();
	mySensor->Close();
	mySensor->Release();

	return  0;
}