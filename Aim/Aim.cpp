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

void Aim::SetFrameInRange(cv::Mat frame, cv::Scalar lowPoint,
                          cv::Scalar highPoint) {
  mCurrentFrame = frame;
  cv::Mat Temp = frame.clone();
  cv::cvtColor(Temp, Temp, cv::COLOR_BGR2HSV);
  cv::GaussianBlur(Temp, Temp, cv::Size(3, 3), 4.0);
  cv::inRange(Temp, lowPoint, highPoint, mCurrentFrameGray);
  cv::threshold(mCurrentFrameGray, mCurrentFrameBin, mTreshold, 255,
                cv::THRESH_BINARY);
}

void Aim::AddFrameInRange(cv::Mat frame, cv::Scalar lowPoint,
                          cv::Scalar highPoint) {
  cv::Mat Temp = frame.clone();
  cv::Mat Additional;
  cv::cvtColor(Temp, Temp, cv::COLOR_BGR2HSV);
  cv::GaussianBlur(Temp, Temp, cv::Size(3, 3), 4.0);
  cv::inRange(Temp, lowPoint, highPoint, Additional);
  cv::bitwise_or(mCurrentFrameGray, Additional, mCurrentFrameGray);
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

cv::Mat Aim::FrameWithContours(cv::Scalar color, int thickness, cv::Mat image) {
  cv::Mat Result = image.empty() ? mCurrentFrame.clone() : image.clone();
  VecOfLines Contours = GetContours();
  cv::polylines(Result, Contours, true, color, thickness);
  return Result;
}

Aim::VecOfPoints Aim::GetMassCenters() {
  VecOfPoints Result;
  VecOfLines Contours = GetContours();
  for (auto& Contour : Contours) {
    cv::Moments CurrentMoments = cv::moments(Contour, true);
    double X = CurrentMoments.m10 / CurrentMoments.m00;
    double Y = CurrentMoments.m01 / CurrentMoments.m00;
    if (X >= 0 && Y >= 0) {
      Result.push_back(cv::Point(static_cast<int>(X), static_cast<int>(Y)));
    }
  }
  return Result;
}

cv::Mat Aim::FrameWithMassCenters(int radius, cv::Scalar color, int thickness,
                                  cv::Mat image) {
  cv::Mat Result = image.empty() ? mCurrentFrame.clone() : image.clone();
  VecOfPoints Points = GetMassCenters();
  for (auto& Point : Points) {
    cv::circle(Result, Point, radius, color, thickness);
  }
  return Result;
}

void Aim::DialateBinFrame(cv::Mat mask) {
  mask = mask.empty() ? cv::getStructuringElement(cv::MorphShapes::MORPH_RECT,
                                                  cv::Size(3, 3))
                      : mask;
  cv::dilate(mCurrentFrameBin, mCurrentFrameBin, mask);
}

void Aim::ErodeBinFrame(cv::Mat mask) {
  mask = mask.empty() ? cv::getStructuringElement(cv::MorphShapes::MORPH_RECT,
                                                  cv::Size(3, 3))
                      : mask;
  cv::erode(mCurrentFrameBin, mCurrentFrameBin, mask);
}