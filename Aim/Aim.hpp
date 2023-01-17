#include <opencv2/core.hpp>

namespace cv {
void ProcImg(int, void*);

class Aim {
 public:
  friend void cv::ProcImg(int, void*);

  typedef std::vector<std::vector<cv::Point>> VecOfLines;
  typedef std::vector<cv::Point> VecOfPoints;

 public:
  Aim(int treshold = 0);
  void SetFrame(cv::Mat frame);
  void SetFrameInRange(cv::Mat frame, cv::Scalar lowPoint,
                       cv::Scalar highPoint);
  void AddFrameInRange(cv::Mat frame, cv::Scalar lowPoint,
                       cv::Scalar highPoint);
  void SetupTreshold();
  void DialateBinFrame(cv::Mat mask = cv::Mat());
  void ErodeBinFrame(cv::Mat mask = cv::Mat());
  VecOfLines GetContours();
  VecOfPoints GetMassCenters();
  cv::Mat FrameWithContours(cv::Scalar color = {255, 0, 0}, int thickness = 1,
                            cv::Mat image = cv::Mat());
  cv::Mat FrameWithMassCenters(int radius, cv::Scalar color = {0, 0, 255},
                               int thickness = 1, cv::Mat image = cv::Mat());

 private:
  int mTreshold = 0;
  cv::Mat mCurrentFrame;
  cv::Mat mCurrentFrameGray;
  cv::Mat mCurrentFrameBin;
};
}  // namespace cv
