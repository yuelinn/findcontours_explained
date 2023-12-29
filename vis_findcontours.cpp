//TODO: remember to put documentation in the header files too!
// Satoshi Suzuki and Keiichi Abe. Topological structural analysis of digitized binary images by border following. Computer Vision, Graphics, and Image Processing, 30(1):32–46, 1985.
// Algorithm 1 as described in Appendix I

#include <opencv2/opencv.hpp>

using namespace std;


cv::Mat findcontours(cv::Mat bin_input)
{
  int num_rows = bin_input.rows;
  int num_cols = bin_input.cols;
  // cout << "bin img rows: " << num_rows << "; cols: " << num_cols << endl;  

  // raster scan
  int f_ij, i1, j1, i2, j2, i3, j3, i4, j4;
  int NBD, LNBD;
  bool is_detect_border = False; // Whether or not to do border following (step 3)
  NBD = 1;

  for (int i = 0; i < num_rows; i++)
  {
    LNBD = 1;
    for (int j = 0; j < num_cols; j++)
    {
      // cout << "working on pixel coord "<< i << ", " << j << endl; 
      f_ij = bin_input.at<uchar>(i,j);
      if(f_ij != 0)
      {
        cout << "working on pixel coord "<< i << ", " << j << endl; 

        // TODO make safe checks in case borders are touching the image frame 
        if( (f_ij == 1) && (bin_input.at<uchar>(i,j-1) == 0))  // 1(a) outer border
        {
          NBD++;
          i2 = i;
          j2 = j-1;
          is_detect_border = True;
        }
        else if( (f_ij >= 1) && (bin_input.at<uchar>(i,j+1) == 0 ))  // 1(b) hole border
        {
          NBD++;
          i2 = i;
          j2 = j+1;

          if(f_ij > 1)
          {
            LNBD = f_ij;
          }
          is_detect_border = True;
        }

        // 2 
        // TODO make tree for heirarchy


        if(is_detect_border)
        {
          is_detect_border = False;

        }

        // 4
        if(f_ij != 1)
        {
          LNBD = f_ij;
        }

        cout << "updated mat= " << endl << bin_input << endl << endl;
        cout << "NBD: " << NBD << "; LNBD: " << LNBD << endl;
      }
    }
  }

  return bin_input;
}


int main (void)
{
  string img_fp = "../img.png";

  cv::Mat image;
  image = cv::imread(img_fp, cv::IMREAD_GRAYSCALE);
  if ( !image.data )  // checks if the imread is successful or not
  {
    printf("No image data \n");
    return -1;
  }

  /**
  // just display for sanity check 
  cv::namedWindow("Display Image", cv::WINDOW_FREERATIO );
  cv::imshow("Display Image", image);
  cv::waitKey(0);
   **/

  // convert image into binary
  cv::Mat bin_input;
  cv::threshold(image, bin_input, 125, 1, cv::THRESH_BINARY);

  cout << "binary input = " << endl << bin_input << endl << endl;
  // cout << "bin img type " << bin_input.type() << endl;  // CV_8U

  cv::Mat output_mat;
  output_mat = findcontours(bin_input);

  return 0;
}
