//TODO: remember to put documentation in the header files too!

#include <opencv2/opencv.hpp>

using namespace std;

int main (void) {

  string img_fp = "../img.png";

  cv::Mat image;
  image = cv::imread(img_fp, cv::IMREAD_COLOR );
  if ( !image.data )  // checks if the imread is successful or not
  {
    printf("No image data \n");
    return -1;
  }

  // just display for sanity check 
  cv::namedWindow("Display Image", cv::WINDOW_FREERATIO );
  cv::imshow("Display Image", image);
  cv::waitKey(0);





  return 0;
}
