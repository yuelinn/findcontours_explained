//TODO: remember to put documentation in the header files too!
// Satoshi Suzuki and Keiichi Abe. Topological structural analysis of digitized binary images by border following. Computer Vision, Graphics, and Image Processing, 30(1):32–46, 1985.
// Algorithm 1 as described in Appendix I

#include <opencv2/opencv.hpp>
#include "vis_cv_mat.hpp"

using namespace std;

// TODO this is only for D8 maybe do one which works for D4 too?
std::tuple<int, int> lookCCW(
    cv::Mat img, 
    int pivot_r, 
    int pivot_c, 
    int start_r, 
    int start_c,
    cv::Mat visited_mat,
    Vis_cv_mat visualiser,
    bool is_vis=true
    ) 
{
  int found_r =-1, found_c = -1;
  bool is_found = false;

  cout << "r: " << start_r << "; c: " << start_c << endl; 
  cv::Mat pivot_D8 = img(cv::Rect(pivot_c-1, pivot_r-1, 3, 3));  // TODO safe indexing

  int CCW_i_int[3][3] = {
    { 1, 0,  0}, 
    { 1, 0, -1}, 
    { 0, 0, -1}};
  int CCW_j_int[3][3] = {
    { 0, -1, -1}, 
    { 0,  0,  0}, 
    { 1,  1,  0}};

  int d_i, d_j;

  cv::Point q_pt(start_c, start_r);  // x,y
  for(int tick = 0; tick < 8; ++tick)
  {
    // position relative to (pivot_r-1, pivot_c -1)
    d_i = start_r - pivot_r + 1; 
    d_j = start_c - pivot_c + 1;

    // move according to matrix
    start_r = CCW_i_int[d_i][d_j] + start_r;
    start_c = CCW_j_int[d_i][d_j] + start_c;
    if(is_vis)
    {
      cv::Point curr_pt(pivot_c,pivot_r);  // x,y
      cv::Point r_pt(start_c, start_r);  // x,y
      visualiser.draw_mat(img, curr_pt, cv::Point(-2,-2), q_pt, cv::Point(-2,-2), r_pt);
    }
    if(img.at<int>(start_r, start_c) != 0)
    {
      found_r = start_r;
      found_c = start_c;
      is_found = true;
      break;
    }

    // mark cell as visited if not found
    visited_mat.at<int>(start_r, start_c) = 1;

    // cout << "r: " << start_r << "; c: " << start_c << endl; 
  }

  if(!is_found)
  {
    found_r = -1;
    found_c = -1;
  }

  return make_tuple(found_r, found_c);
}



// TODO this is only for D8 maybe do one which works for D4 too?
std::tuple<int, int> lookCW(
    cv::Mat img, 
    int pivot_r, 
    int pivot_c, 
    int start_r, 
    int start_c,
    Vis_cv_mat visualiser,
    bool is_vis=true
    ) 
{
  int found_r =-1, found_c = -1;
  bool is_found = false;

  // cout << "r: " << start_r << "; c: " << start_c << endl; 
  cv::Mat pivot_D8 = img(cv::Rect(pivot_c-1, pivot_r-1, 3, 3));  // TODO safe indexing
  // cout << pivot_D8 << endl;

  // TODO double is really not necessary for this
  double CW_i_double[3][3] = {
    { 0, 0, 1}, 
    {-1, 0, 1}, 
    {-1, 0, 0}};
  double CW_j_double[3][3] = {
    {1,  1,  0}, 
    {0,  0,  0}, 
    {0, -1, -1}};

  int d_i, d_j;
  for(int tick = 0; tick < 8; ++tick)
  {
    d_i = start_r - pivot_r + 1; 
    d_j = start_c - pivot_c + 1;

    start_r = CW_i_double[d_i][d_j] + start_r;
    start_c = CW_j_double[d_i][d_j] + start_c;


    if(is_vis)
    {
      // visualise
      cv::Point curr_pt(pivot_c,pivot_r);  // x,y
      // draw (i2,j2)
      cv::Point q_pt(start_c, start_r);  // x,y
      visualiser.draw_mat(img, curr_pt, cv::Point(-2,-2), q_pt);
    }

    if(img.at<int>(start_r, start_c) != 0)
    {
      found_r = start_r;
      found_c = start_c;
      is_found = true;

      break;
    }

    // cout << "r: " << start_r << "; c: " << start_c << endl; 
  }

  if(!is_found)
  {
    found_r = -1;
    found_c = -1;
  }
  return make_tuple(found_r, found_c);
}


cv::Mat findcontours(cv::Mat bin_input, string img_vis_dir="", int vis_size=1024)
{
  bin_input.convertTo(bin_input, CV_32S);  // needs to be able to hold negative integers
  // TODO check the max number of components in the image, choose datatype based on range required.

  int num_rows = bin_input.rows;
  int num_cols = bin_input.cols;
  // cout << "bin img rows: " << num_rows << "; cols: " << num_cols << endl;  

  // raster scan
  int f_ij, i1, j1, i2, j2, i3, j3, i4, j4;
  int NBD, LNBD;
  bool is_detect_border = false; // Whether or not to do border following (step 3)
  NBD = 1;
  bool  is_vis = false;

  Vis_cv_mat visualiser (img_vis_dir, vis_size);  // initialise visualiser 
  if(img_vis_dir.length() > 0)
  {
    is_vis = true;
  }

  if(is_vis)
  {
    visualiser.draw_mat(bin_input);
  }

  for (int i = 0; i < num_rows; i++)
  {
    LNBD = 1;
    for (int j = 0; j < num_cols; j++)
    {
      // cout << "working on pixel coord "<< i << ", " << j << endl; 

      if(is_vis)
      {
        cv::Point curr_pt(j,i);  // x,y
        visualiser.draw_mat(bin_input, curr_pt);
      }

      f_ij = bin_input.at<int>(i,j);
      if(f_ij != 0)
      {
        cout << "working on pixel coord "<< i << ", " << j << endl; 

        // TODO make safe checks in case borders are touching the image frame 
        if( (f_ij == 1) && (bin_input.at<int>(i,j-1) == 0))  // 1(a) outer border
        {
          NBD++;
          i2 = i;
          j2 = j-1;
          is_detect_border = true;
        }
        else if( (f_ij >= 1) && (bin_input.at<int>(i,j+1) == 0 ))  // 1(b) hole border
        {
          NBD++;
          i2 = i;
          j2 = j+1;

          if(f_ij > 1)
          {
            LNBD = f_ij;
          }
          is_detect_border = true;
        }

        // 2 
        // TODO make tree for heirarchy


        if(is_detect_border)
        {
          is_detect_border = false;

          cv::Point start_pt(j,i);  // x,y
          if(is_vis)
          {
            cv::Point curr_pt(j,i);  // x,y
            visualiser.draw_mat(bin_input, curr_pt, start_pt);

            // draw (i2,j2)
            cv::Point q_pt(j2,i2);  // x,y
            visualiser.draw_mat(bin_input, curr_pt, start_pt, q_pt);
          }

          // 3 
          // 3.1
          // look around clockwise
          tie(i1, j1) = lookCW(bin_input, i, j, i2, j2, visualiser);

          if(is_vis)
          {
            cv::Point curr_pt(j,i);  // x,y
            cv::Point i1_pt(j1, i1);  // x,y
            visualiser.draw_mat(bin_input, curr_pt, start_pt, i1_pt, i1_pt);
          }

          if(( (i1 < 0) || (j1 < 0)))  // non-zero element not found
          {
            bin_input.at<int>(i, j) = -1*NBD;
          }
          else
          {
            // 3.2 
            i2 = i1;
            j2 = j1;
            i3 = i;
            j3 = j;

            // 3.3
            while(true)
            {
              if(is_vis)
              {
                cv::Point curr_pt(j3,i3);  // x,y
                cv::Point q_pt(j2, i2);  // x,y
                visualiser.draw_mat(bin_input, curr_pt, start_pt, q_pt);
              }

              cv::Mat visited_mat;
              visited_mat = cv::Mat::zeros(bin_input.rows, bin_input.cols, bin_input.type());
              tie(i4, j4) = lookCCW(bin_input, i3, j3, i2, j2, visited_mat, visualiser);
              cout << "visited squares mat= " << endl << visited_mat << endl << endl;

              // 3.4 
              // TODO check safe indexing
              if(visited_mat.at<int>(i3, j3+1) == 1)  // 3.4(a)
              {
                bin_input.at<int>(i3, j3) = -1 * NBD;
              }
              else if(bin_input.at<int>(i3, j3) == 1)
              {
                bin_input.at<int>(i3, j3) = NBD;
              }

              // 3.5 check if at starting point or not
              if((i4 == i) && (j4 ==j) && (i3 == i1) && (j3 == j1))
              {
                break;
              }
              else
              {
                i2 = i3;
                j2 = j3;
                i3 = i4;
                j3 = j4;
              }
            }
          }
        }

        // 4
        if(f_ij != 1)
        {
          LNBD = f_ij;
        }
        visualiser.reset_border();

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
  string img_vis_dir = "../vis/";

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
  output_mat = findcontours(bin_input, img_vis_dir);

  return 0;
}
