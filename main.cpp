#include <iostream>
#include <cstdlib>
#include <opencv2/core/base.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/text.hpp>
#include <opencv2/imgcodecs.hpp>
#include <cv.hpp>

int main(int argc, char **argv) {
    cv::Mat img;
    std::string voca = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ'.:(),@-";
    
    img = cv::imread(argv[1]);

    auto ocr = cv::text::OCRTesseract::create(nullptr, "eng", voca.c_str());

    std::vector<cv::Rect> rectangles;
    std::string out;
    ocr->run(img, out, &rectangles);

    for (auto r : rectangles)
        cv::rectangle(img, r.tl(), r.br(), cv::Scalar(255, 0, 0));

    std::cout << out << std::endl;

    cv::imshow("res", img);
    cv::waitKey();

    return 0;
}
