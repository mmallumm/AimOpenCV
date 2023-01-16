#include "Aim.hpp"

#include <opencv2/opencv.hpp>

using cv::Aim;

void cv::ProcImg(int, void* userData) {
  Aim* CurrentAim = reinterpret_cast<Aim*>(userData);
  cv::threshold(CurrentAim->mCurrentFrameGray, CurrentAim->mCurrentFrameBin,
                CurrentAim->mTreshold, 255, cv::THRESH_BINARY);
  cv::imshow("BinFrame", CurrentAim->mCurrentFrameBin);
}

Aim::Aim(int treshold) : mTreshold(treshold) {}

void Aim::SetFrame(cv::Mat frame) {
  mCurrentFrame = frame;
  cv::cvtColor(mCurrentFrame, mCurrentFrameGray, CV_BGR2GRAY);
  cv::threshold(mCurrentFrameGray, mCurrentFrameBin, mTreshold, 255,
                cv::THRESH_BINARY);
}

void Aim::SetupTreshold() {
  ProcImg(0, this);
  cv::createTrackbar("TresholdBar", "BinFrame", &mTreshold, 255, ProcImg, this);
  cv::waitKey(-1);
}

Aim::VecOfLines Aim::GetContours() {
  Aim::VecOfLines Contours;
  cv::findContours(mCurrentFrameBin.clone(), Contours, CV_RETR_EXTERNAL,
                   CV_CHAIN_APPROX_NONE);
  return Contours;
}

cv::Mat Aim::FrameWithContours(cv::Mat image) {
  cv::Mat Result = image.empty() ? mCurrentFrame.clone() : image.clone();
  VecOfLines Contours = GetContours();
  cv::polylines(Result, Contours, true, cv::Scalar(255, 0, 0));
  return Result;
}

Aim::VecOfPoints Aim::GetMassCenters() {
  VecOfPoints Result;
  VecOfLines Contours = GetContours();
  for (auto& Contour : Contours) {
    cv::Moments CurrentMoments = cv::moments(Contour, true);
    double X = CurrentMoments.m10 / CurrentMoments.m00;
    double Y = CurrentMoments.m01 / CurrentMoments.m00;
    Result.push_back(cv::Point(static_cast<int>(X), static_cast<int>(Y)));
  }
  return Result;
}

cv::Mat Aim::FrameWithMassCenters(cv::Mat image) {
  cv::Mat Result = image.empty() ? mCurrentFrame.clone() : image.clone();
  VecOfPoints Points = GetMassCenters();
  for (auto& Point : Points) {
    cv::circle(Result, Point, 3, cv::Scalar(0, 0, 255));
  }
  return Result;
}