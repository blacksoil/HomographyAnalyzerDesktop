//
//  ImageDrawingTools.h
//  RANSAC_AR
//
//  Created by Michael Hotan on 5/13/12.
//  Copyright (c) 2012 University of Washington. All rights reserved.
//

#ifndef RANSAC_AR_ImageDrawingTools_h
#define RANSAC_AR_ImageDrawingTools_h

#include <iostream>
#include <string>
#include <cstdlib>
#include <vector> 
#include <cmath>
#include <opencv2/opencv.hpp>

using namespace cv;

//Draw Circle image at coordinates identified in pts
void drawCircleOnSelected(const vector <Point2f> &pts, cv::Mat &img, CvScalar color);

//Draw X on all points identified in pts
void drawXOnSelected(const vector <Point2f> &pts, cv::Mat &img, CvScalar color);

//Draws lines from one set to another
//void drawLinesOnSelected(const vector <cv::Point2f> &pts1,
//                         const cv::Mat &pts2, cv::Mat img, CvScalar color);
void drawLinesOnSelected(const vector <Point2f> &pts1,
                         const vector <Point2f> &pts2, cv::Mat &img, CvScalar color);

//Converts input data types to vector of Point2f
vector <Point2f> toPoint2fVector(const cv::Mat &mat);

#endif
