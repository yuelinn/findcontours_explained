#include "vis_cv_mat.hpp"
#include <filesystem>

int Vis_cv_mat::step_count=0;


// constructor
Vis_cv_mat::Vis_cv_mat(std::string output_dir_arg, int output_height_arg)
{
  output_dir = output_dir_arg;
  output_height = output_height_arg;
  in_rows=-1;
  in_cols=-1;
}

// TODO maybe consider storing some variables in the class 
bool Vis_cv_mat::draw_mat(
    cv::Mat bin_img, 
    cv::Point curr_px,  // p, also i3,j3
    cv::Point start_px_arg,  // i, j 
    cv::Point q_px,  // i2, j2 
    cv::Point start_pivot_px_arg,  // i1, j1 
    cv::Point r_px // i4, j4
    )
{
  in_rows=bin_img.rows;
  in_cols=bin_img.cols;
  init_mat(CV_8UC3);

  if(start_px_arg.x >= 0 && start_px_arg.y >= 0)
  {
    start_px = start_px_arg;
  }

  if(start_pivot_px_arg.x >= 0 && start_pivot_px_arg.y >= 0)
  {
    start_pivot_px = start_pivot_px_arg;
  }


  // draw the grid
  int grid_size = int(inout_ratio);
  double min_x=0, min_y=0;
  for(int r=0; r < in_rows; ++r)
  {
    min_y = r * grid_size;
    for(int c=0; c < in_cols; ++c)
    {
      min_x = c * grid_size;

      // draw rectangle grid
      cv::Point top_left(min_x, min_y), bottom_right(min_x + grid_size, min_y + grid_size);
      cv::rectangle(vis_mat, top_left, bottom_right, Vis_cv_mat::grid_clr, 2);

      // color pixels
      if(bin_img.at<int>(r,c) != 0)
      {
        cv::rectangle(vis_mat, top_left, bottom_right, Vis_cv_mat::px_clr, -1);
      }

      // highlight current pixel
      if(curr_px.x == c && curr_px.y == r)
      {
        cv::Scalar clr;
        if(is_raster_scan)
        {
          clr = Vis_cv_mat::raster_scan_clr;
        } 
        else
        {
          clr = Vis_cv_mat::highlight_clr;
        }

        cv::rectangle(
            vis_mat, 
            top_left,
            bottom_right,
            clr,
            -1
            );
      }

      // highlight start of search point, q=(i2, j2)
      if(q_px.x == c && q_px.y == r)
      {
        cv::rectangle(vis_mat, top_left, bottom_right, Vis_cv_mat::q_clr, -1);
      }

      // highlight start of search point, q=(i2, j2)
      if(r_px.x == c && r_px.y == r)
      {
        cv::rectangle(vis_mat, top_left, bottom_right, Vis_cv_mat::r_clr, -1);
      }


      // highlight starting pixel for current border
      if(start_px.x == c && start_px.y == r)
      {
        cv::rectangle(
            vis_mat, 
            top_left + Vis_cv_mat::offset, 
            bottom_right - Vis_cv_mat::offset, 
            Vis_cv_mat::start_clr, 
            2);
      }

      // highlight start_pivot_px=(i1, j1) for current border
      if(start_pivot_px.x == c && start_pivot_px.y == r)
      {
        cv::rectangle(
            vis_mat, 
            top_left+Vis_cv_mat::offset, 
            bottom_right-Vis_cv_mat::offset, 
            Vis_cv_mat::start_pivot_clr, 
            2);
      }


      // draw pixel values
      cv::Point center (min_x + grid_size/2, min_y + grid_size/2);
      int font_face = cv::FONT_HERSHEY_SIMPLEX;
      double font_scale = 1;
      cv::putText(vis_mat, std::to_string(bin_img.at<int>(r,c)), center, font_face, font_scale, text_clr);

    }
  }

  return Vis_cv_mat::save_img();
}

void Vis_cv_mat::init_mat(int cv_dtype)
{
  int vis_rows, vis_cols;
  vis_rows = output_height;
  vis_cols = int(double(in_cols) / double(in_rows) * double(output_height));
  inout_ratio = double(output_height) / double(in_rows);
  vis_mat = cv::Mat::ones(vis_rows, vis_cols, cv_dtype);
  vis_mat = bg_clr;
  return;
}

bool Vis_cv_mat::save_img()
{
  std::string filepath;
  filepath = std::filesystem::path(output_dir)/ (std::to_string(step_count) + ".png");
  ++step_count;
  return cv::imwrite(filepath, vis_mat);
}

// to be called when the current border is done processing, before resuming raster scan
void Vis_cv_mat::reset_border()
{
  start_px = cv::Point(-1,-1);
  start_pivot_px = cv::Point(-1,-1);
  is_raster_scan = true;
  return;
}
