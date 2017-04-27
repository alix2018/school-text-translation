#include <iostream>
#include <cstdlib>
#include <opencv2/core/base.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/text.hpp>
#include <opencv2/imgcodecs.hpp>
#include <cv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char **argv)
{
    /// Load source image
    Mat srcImg = imread( argv[1], 1 );
    Mat resImg(srcImg.clone());

    std::string voca = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ'.:(),@-";

    auto ocr = text::OCRTesseract::create(nullptr, "eng", voca.c_str());

    vector<Rect>   rectangles;
    vector<string> componentsRect;
    string out;
    ocr->run(srcImg, out, &rectangles, &componentsRect);

    Mat threshCrop, grayCrop, cropImg;

    // For each rectangle of text found by tesseract
    for (int i=0; i < rectangles.size(); i++)
    {
        if(componentsRect[i].size() > 1)
        {
            // Removing text

            // Cropped img
            cropImg = Mat(srcImg, rectangles[i]);
            // Convert to Gray
            cvtColor(cropImg, grayCrop, CV_RGB2GRAY);
            // Threshold
            threshold(grayCrop, threshCrop, 0, 255, THRESH_BINARY + THRESH_OTSU);

            // FindContours
            vector<vector<Point> > contours;
            vector<Vec4i> hierarchy;
            findContours(threshCrop, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);

            // if text color == white, inverse color
            //cout << (int) threshCrop.at<unsigned char>(contours[0][0].x,contours[0][0].y) << endl;
            if ( (int) threshCrop.at<unsigned char>(contours[0][0].x,contours[0][0].y) != 255 )
            {
                // create a matrix with all elements equal to 255 for subtraction
                Mat sub_mat = Mat::ones(threshCrop.size(), threshCrop.type())*255;

                //subtract the original matrix by sub_mat to give the negative output new_image
                subtract(sub_mat, threshCrop, threshCrop);
            }

            // extract background of cropped Mat
            auto m = mean(cropImg, threshCrop);

            // Draw mean reactangle
            rectangle(resImg, rectangles[i].tl(), rectangles[i].br(), cv::Scalar(m[0], m[1], m[2]), CV_FILLED);

            // Draw translate text
            putText(resImg, componentsRect[i],
                      Point(rectangles[i].tl().x, rectangles[i].br().y), FONT_HERSHEY_COMPLEX, 1, Scalar(0, 0, 0));


            //cv::imshow("mask", threshCrop);
            //cv::waitKey();
        }
    }

    std::cout << out << std::endl;

    cv::imshow("result", resImg);
    cv::waitKey();

    /// Create Window with source image
    char* source_window = "Source";
    namedWindow( source_window, CV_WINDOW_AUTOSIZE );

    cv::imshow(source_window, srcImg);
    cv::waitKey();

    return 0;
}

