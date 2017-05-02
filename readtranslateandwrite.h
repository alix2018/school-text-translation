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

Mat readTranslateAndWrite(string pathImg, vector<Rect> &rectangles, vector<string> &componentsRect)
{
    /// Load source image
    Mat srcImg = imread( pathImg );

    std::string voca = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ'.:(),@-";

    auto ocr = text::OCRTesseract::create(nullptr, "eng", voca.c_str());
    
    string out;
    ocr->run(srcImg, out, &rectangles, &componentsRect);

    return srcImg;
}

void rewriteImage(const string &pathImg, const Mat srcImg, Mat &resImg, const vector<Rect> &rectangles,
                    const vector<string> &componentsRect, int i)
{
    Mat threshCrop, grayCrop, cropImg;

    char* source_window = "Source";
    namedWindow( source_window, CV_WINDOW_AUTOSIZE );

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
        /*
        if (contours.size() == 0)
            return;
        */

        // if text color == white, inverse color
        if ( (int) threshCrop.at<unsigned char>(contours[0][0].x,contours[0][0].y) != 255 )
        {
            // create a matrix with all elements equal to 255 for subtraction
            Mat sub_mat = Mat::ones(threshCrop.size(), threshCrop.type()) * 255;

            //subtract the original matrix by sub_mat to give the negative output new_image
            subtract(sub_mat, threshCrop, threshCrop);
        }

        // extract background of cropped Mat
        auto m = mean(cropImg, threshCrop);

        // Draw mean reactangle on result Image
        rectangle(resImg, rectangles[i].tl(), rectangles[i].br(), Scalar(m[0], m[1], m[2]), CV_FILLED);

        /******* Adapt text size to rectangle *******/
        Size textRectangle = rectangles[i].size();


        String text = componentsRect[i];



        int fontFace = FONT_HERSHEY_SIMPLEX;
        double fontScale = 20;
        int thickness = textRectangle.height/16;
        if (thickness == 0)
            thickness = 1;
        int baseline=0;

        Size textSize = getTextSize(text, fontFace, fontScale, thickness, &baseline);
        while ( textRectangle.height < textSize.height
                || textRectangle.width < textSize.width )
        {
            fontScale -= 0.1;
            textSize = getTextSize(text, fontFace, fontScale, thickness, &baseline);
        }

        // center the text
        Point textOrg(rectangles[i].tl().x + (rectangles[i].width - textSize.width)/2,
                      rectangles[i].br().y - (rectangles[i].height - textSize.height)/2);

        // then put the text in the mean rectangle

        //QtFont font = fontQt("Times", fontScale);
        //addText(resImg, text, textOrg, font);
        putText(resImg, text, textOrg, fontFace, fontScale, Scalar_<int>::all(255), thickness, 8);
        //cout << srcImg.at<Vec3b>(rectangles[i].tl().x + contours[0][0].x, rectangles[i].tl().y + contours[0][0].y) << endl;

    }
}

string writeToFile(const string &pathImg, const Mat &resImg)
{
    // Create Window with source image
    //char* source_window = "Source";
    //namedWindow( source_window, CV_WINDOW_AUTOSIZE );

    imshow("Source", resImg);
    waitKey();

    size_t pos = pathImg.find_last_of('.');

    string pathResImg = pathImg;
    pathResImg.insert(pos, "_trad");

    imwrite(pathResImg, resImg);
    cout << "image : " << pathResImg << " created !";

    return pathResImg;
}

