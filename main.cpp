#include <opencv2/opencv.hpp>

#include "Aim/Aim.hpp"
#include "task/cvDirectory.h"

#define TASK 4

#if TASK == 1
int main() {
  std::string InputPath = "../task/img_zadan/allababah/";
  auto NameList = cv::Directory::GetListFiles(InputPath, ".jpg");

  cv::Aim aim;
  aim.SetFrame(cv::imread(InputPath + NameList[0]));
  aim.SetupTreshold();
  for (auto& Name : NameList) {
    cv::Mat InputImg = cv::imread(InputPath + Name);
    aim.SetFrame(InputImg);
    cv::Mat ToShow = aim.FrameWithMassCenters(3);
    cv::imshow("result", ToShow);
    cv::waitKey(-1);
  }

  return 0;
}
#elif TASK == 2
int main() {
  cv::Scalar VioletColor = {94, 4, 86};
  cv::Scalar BlueColor = {255, 0, 0};
  std::string InputPath = "../task/img_zadan/teplovizor/";
  auto NameList = cv::Directory::GetListFiles(InputPath);

  cv::Aim aim;
  for (auto& Name : NameList) {
    cv::Mat InputImg = cv::imread(InputPath + Name);
    aim.SetFrameInRange(InputImg, cv::Scalar(0, 0, 0),
                        cv::Scalar(10, 255, 255));
    aim.DialateBinFrame(cv::getStructuringElement(cv::MorphShapes::MORPH_CROSS,
                                                  cv::Size(35, 35)));
    cv::Mat ToShow = aim.FrameWithMassCenters(10, VioletColor, 3);
    cv::imshow("result", ToShow);
    cv::waitKey(-1);
  }

  return 0;
}
#elif TASK == 3

cv::Scalar RedColor(0, 0, 255);
cv::Scalar GreenColor(0, 255, 0);
cv::Scalar BlueColor(255, 0, 0);

cv::Scalar BlueMin(80, 50, 0);
cv::Scalar BlueMax(100, 255, 255);

cv::Scalar GreenMin(59, 50, 0);
cv::Scalar GreenMax(77, 255, 255);

cv::Scalar RedMin_0(0, 40, 59);
cv::Scalar RedMax_0(10, 168, 255);
cv::Scalar RedMin_1(170, 0, 0);
cv::Scalar RedMax_1(180, 255, 255);

enum Team { Red, Green, Blue };

int Ro(cv::Point p1, cv::Point p2) {
  cv::Point Diff = p2 - p1;
  double dist =
      std::sqrt(static_cast<double>(Diff.x) * static_cast<double>(Diff.x) +
                static_cast<double>(Diff.y) * static_cast<double>(Diff.y));
  return static_cast<int>(dist);
}

int FindMinRo(cv::Aim::VecOfPoints robots, cv::Point light) {
  int CurrRo;
  int MinRoIndex = 0;
  int MinRo = Ro(robots[0], light);
  for (int i = 1; i < robots.size(); ++i) {
    CurrRo = Ro(robots[i], light);
    MinRo = (CurrRo <= MinRo) ? CurrRo : MinRo;
    MinRoIndex = (CurrRo <= MinRo) ? i : MinRoIndex;
  }
  return MinRoIndex;
}

cv::Aim::VecOfPoints DrawTeam(cv::Mat image, Team team, cv::Point light) {
  cv::Aim aim;
  cv::Mat Frame = image.clone();
  cv::Scalar Color;
  switch (team) {
    case Red: {
      aim.SetFrameInRange(Frame, RedMin_0, RedMax_0);
      aim.AddFrameInRange(Frame, RedMin_1, RedMax_1);
      aim.DialateBinFrame(cv::getStructuringElement(
          cv::MorphShapes::MORPH_ELLIPSE, cv::Size(13, 13)));
      aim.ErodeBinFrame(cv::getStructuringElement(
          cv::MorphShapes::MORPH_ELLIPSE, cv::Size(15, 15)));
      Color = RedColor;
      break;
    }
    case Blue: {
      aim.SetFrameInRange(Frame, BlueMin, BlueMax);
      aim.ErodeBinFrame(cv::getStructuringElement(
          cv::MorphShapes::MORPH_ELLIPSE, cv::Size(13, 13)));
      aim.DialateBinFrame(cv::getStructuringElement(
          cv::MorphShapes::MORPH_ELLIPSE, cv::Size(13, 13)));
      Color = BlueColor;
      break;
    }
    case Green: {
      aim.SetFrameInRange(Frame, GreenMin, GreenMax);
      aim.ErodeBinFrame(cv::getStructuringElement(
          cv::MorphShapes::MORPH_ELLIPSE, cv::Size(13, 13)));
      aim.DialateBinFrame(cv::getStructuringElement(
          cv::MorphShapes::MORPH_ELLIPSE, cv::Size(13, 13)));
      Color = GreenColor;
      break;
    }
    default:
      break;
  }
  auto Res = aim.GetMassCenters();
  int ClosestRobot = FindMinRo(Res, light);
  cv::Mat ToShow = aim.FrameWithContours(Color, 2);
  cv::circle(ToShow, Res[ClosestRobot], 6, cv::Scalar(0, 255, 255), 4);
  cv::imshow("result", ToShow);
  cv::waitKey(-1);
  return Res;
}

cv::Point DrawLight(cv::Mat& image) {
  cv::Aim aim(250);
  aim.SetFrame(image);
  image = aim.FrameWithMassCenters(5, cv::Scalar(100, 100, 100), 5);
  auto Centers = aim.GetMassCenters();

  return Centers[0];
}

int main() {
  std::string InputPath = "../task/img_zadan/roboti/";
  auto NameList = cv::Directory::GetListFiles(InputPath);

  for (auto& Name : NameList) {
    cv::Mat InputImg = cv::imread(InputPath + Name);
    auto LightPoint = DrawLight(InputImg);
    auto RedPoints = DrawTeam(InputImg, Team::Red, LightPoint);
    auto GreedPoints = DrawTeam(InputImg, Team::Green, LightPoint);
    auto BluePoints = DrawTeam(InputImg, Team::Blue, LightPoint);
  }

  return 0;
}

#elif TASK == 4
int main() {
  std::string InputPath = "../task/img_zadan/gk/";
  cv::Mat GkTemplate = cv::imread(InputPath + "gk_tmplt.jpg");
  cv::Mat GkList = cv::imread(InputPath + "gk.jpg");

  cv::Aim GkTemplateAim(140);
  GkTemplateAim.SetFrame(GkTemplate);
  GkTemplateAim.SetupTreshold();
  auto GkTemplateContours = GkTemplateAim.GetContours();
  auto img1 = GkTemplateAim.FrameWithContours();

  cv::Aim GkListAim(140);
  GkListAim.SetFrame(GkList);
  GkListAim.SetupTreshold();
  auto GkListContours = GkListAim.GetContours();
  auto img2 = GkListAim.FrameWithContours();


  return 0;
}
#endif