#include <jni.h>
#include <opencv2/opencv.hpp>
#include <vector>
#include <com_example_opencv4androidexample_ImageProc.h>

using namespace std;
using namespace cv;

extern "C" {

void cartoonifyImage(Mat &srcFrame, Mat &resFrame);

JNIEXPORT void JNICALL Java_com_example_opencv4androidexample_ImageProc_CartoonProc(
		JNIEnv *env, jclass thiz, jintArray buf, jintArray res, jint w, jint h) {
	jint *cbuf, *bgra;
	cbuf = env->GetIntArrayElements(buf, 0);
	bgra = env->GetIntArrayElements(res, 0);

	Mat src, dst, gray, mbgra, imgData;
	Size size;
	size.width = w;
	size.height = h;

	src = Mat(size, CV_8UC3);
	dst = Mat(size, CV_8UC3);
	imgData = Mat(size, CV_8UC4, (unsigned char*) cbuf);
	mbgra = Mat(size, CV_8UC4, (unsigned char*) bgra);
	cvtColor(imgData, src, CV_BGRA2BGR);

	cartoonifyImage(src, dst);
	cvtColor(dst, mbgra, CV_BGR2BGRA);

	env->ReleaseIntArrayElements(buf, cbuf, 0);
	env->ReleaseIntArrayElements(res, bgra, 0);
}

void cartoonifyImage(Mat &srcFrame, Mat &resFrame) {
	const int MEDIAN_BLUR_FILTER_SIZE = 7;
	const int LAPLACIAN_FILTER_SIZE = 5;
	const int EDGES_THRESHOLD = 80;
	const int REPETITION = 7;

    Mat grayFrame;
    cvtColor(srcFrame, grayFrame, CV_BGR2GRAY);
    medianBlur(grayFrame, grayFrame, MEDIAN_BLUR_FILTER_SIZE);

    Mat edges;
    Laplacian(grayFrame, edges, CV_8U, LAPLACIAN_FILTER_SIZE);

    Mat mask;
    threshold(edges, mask, EDGES_THRESHOLD, 255, THRESH_BINARY_INV);

    Size size = srcFrame.size();
    Size halfSize;
    halfSize.height = size.height / 2;
    halfSize.width = size.width / 2;
    Mat smallFrame = Mat(halfSize, CV_8UC3);
    resize(srcFrame, smallFrame, halfSize, 0, 0, INTER_LINEAR);

    Mat tmp = Mat(halfSize, CV_8UC3);
    for (int i = 0; i < REPETITION; i++) {
        int ksize = 9;
        double sigmaColor = 9;
        double sigmaSpace = 7;
        bilateralFilter(smallFrame, tmp, ksize, sigmaColor, sigmaSpace);
        bilateralFilter(tmp, smallFrame, ksize, sigmaColor, sigmaSpace);
    }

    Mat bigFrame = Mat(size, CV_8UC3);
    resize(smallFrame, bigFrame, size, 0, 0, INTER_LINEAR);
    resFrame.setTo(0);
    bigFrame.copyTo(resFrame, mask);
}

JNIEXPORT void JNICALL Java_com_example_opencv4androidexample_ImageProc_CartoonProcNew(
		JNIEnv*, jclass, jlong addrSrc, jlong addrDst)
{
	const int MEDIAN_BLUR_FILTER_SIZE = 7;
	const int LAPLACIAN_FILTER_SIZE = 5;
	const int EDGES_THRESHOLD = 80;
	const int REPETITION = 7;

	Mat srcFrame = *(Mat*)addrSrc;
	Mat dstFrame = *(Mat*)addrDst;

	Mat src, dst;
	cvtColor(srcFrame, src, CV_BGRA2BGR);

	cartoonifyImage(src, dst);

    cvtColor(dst, dstFrame, CV_BGR2BGRA);
}

}

