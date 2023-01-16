#include <opencv2/core.hpp>

namespace cv {
void ProcImg(int, void*);

class Aim {
  friend void cv::ProcImg(int, void*);
  typedef std::vector<std::vector<cv::Point>> VecOfLines;
  typedef std::vector<cv::Point> VecOfPoints;

 public:
  Aim(int treshold = 0);
  void SetFrame(cv::Mat frame);
  void SetupTreshold();
  VecOfLines GetContours();
  cv::Mat FrameWithContours(cv::Mat image = cv::Mat());
  VecOfPoints GetMassCenters();
  cv::Mat FrameWithMassCenters(cv::Mat image = cv::Mat());
   
 private:
  int mTreshold = 0;
  cv::Mat mCurrentFrame;
  cv::Mat mCurrentFrameGray;
  cv::Mat mCurrentFrameBin;
};
}  // namespace cv
