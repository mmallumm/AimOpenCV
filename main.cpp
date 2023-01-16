#include <opencv2/opencv.hpp>

#include "Aim/Aim.hpp"
#include "task/cvDirectory.h"

#define TASK 1

#if TASK == 1
int main() {
  std::string InputPath = "../task/img_zadan/allababah/";
  std::string OutputPath = "../task/output/allababah/";
  auto NameList =
      cv::Directory::GetListFiles("../task/img_zadan/allababah/", ".jpg");

  cv::Aim aim;
  aim.SetFrame(cv::imread(InputPath + NameList[0]));    
  aim.SetupTreshold();
  for(auto& Name : NameList) {
    cv::Mat InputImg = cv::imread(InputPath + Name);
    aim.SetFrame(InputImg);
    cv::Mat ToShow = aim.FrameWithMassCenters(aim.FrameWithContours());    
    cv::imshow("result", ToShow);
    cv::waitKey(-1);
  }

  return 0;
}
#elif TASK == 2
int main() {
  std::string InputPath = "../task/img_zadan/allababah/";
  std::string OutputPath = "../task/output/allababah/";
  auto NameList =
      cv::Directory::GetListFiles("../task/img_zadan/allababah/", ".jpg");

  cv::Aim aim;
  aim.SetFrame(cv::imread(InputPath + NameList[0]));    
  aim.SetupTreshold();
  for(auto& Name : NameList) {
    cv::Mat InputImg = cv::imread(InputPath + Name);
    aim.SetFrame(InputImg);
    cv::Mat ToShow = aim.FrameWithMassCenters(aim.FrameWithContours());    
    cv::imshow("result", ToShow);
    cv::waitKey(-1);
  }

  return 0;
}
#endif