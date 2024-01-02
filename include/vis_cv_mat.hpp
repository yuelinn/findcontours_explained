//TODO: remember to put documentation in the header files too!
// for visualisation

#ifndef VIS_CV_MAT
#define VIS_CV_MAT

#include <opencv2/opencv.hpp>

class Vis_cv_mat 
{
  std::string output_dir;
  static int step_count;

  cv::Scalar bg_clr = cv::Scalar(255,255,255);
  cv::Scalar grid_clr = cv::Scalar(200,200,200);
  cv::Scalar text_clr = cv::Scalar(0,0,0);
  cv::Scalar start_clr = cv::Scalar(0,0,255);
  cv::Scalar highlight_clr = cv::Scalar(214, 133, 159);  // light purple 159, 133, 214
  cv::Scalar start_pivot_clr = cv::Scalar(255, 0, 0);  // blue 
  // cv::Scalar start_pivot_clr = cv::Scalar(217, 176, 115);  // light blue 115, 176, 217
  cv::Scalar q_clr = cv::Scalar(151, 233, 247);  // light yello 247, 233, 151
  cv::Scalar r_clr = cv::Scalar(209, 208, 151);  // light cyan 151, 208, 209


  cv::Mat vis_mat;
  int in_rows, in_cols;
  int output_height;
  double inout_ratio;

  void init_mat(int cv_dtype);
  bool save_img();


  cv::Point start_px = cv::Point(-1,-1);
  cv::Point start_pivot_px = cv::Point(-1,-1);
  cv::Point offset = cv::Point(2,2);


  public:
  Vis_cv_mat(std::string output_dir_arg, int output_height_arg);
  bool draw_mat(
      cv::Mat bin_img, 
      cv::Point curr_px = cv::Point(-1,-1),
      cv::Point start_px_arg = cv::Point(-1,-1),
      cv::Point start_pivot_px_arg = cv::Point(-1,-1),
      cv::Point q_px = cv::Point(-1,-1),
      cv::Point r_px = cv::Point(-1,-1)
      );

  void reset_border();




};

#endif
