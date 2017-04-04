#include <opencv2/core/core.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "opencv2/objdetect/objdetect.hpp"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

using namespace cv;
using namespace std;

Mat src;

int main( int argc, char** argv )
{
    /// Load source image
    src = imread( argv[1], 1 );

    /// Create Window with source image
    char* source_window = "Source";
    namedWindow( source_window, CV_WINDOW_AUTOSIZE );
    imshow( source_window, src );

    waitKey(0);
    return(0);
}
