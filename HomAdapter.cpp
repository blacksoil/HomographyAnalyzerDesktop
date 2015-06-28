//
//  HomAdapter.cpp
//  RANSAC_AR
//
//  Created by Michael Hotan on 5/12/12.
//  Copyright (c) 2012 University of Washington. All rights reserved.
//
#include <iostream>
#include <exception>
#include <string>
#include <cstdlib>
#include <vector>
#include <opencv2/opencv.hpp>

#include "HomAdapter.h"
#include "ImageDrawingTools.h"

#define RED CV_RGB(250,0,0)
#define GREEN CV_RGB(0,250,0)
#define BLUE CV_RGB(0,0,250)
#define WHITE CV_RGB(250,250,250)
#define BLACK CV_RGB(0,0,0)

using namespace cv;

Mat HomAdapter::GetHomography() const{
    return Homography_;
}

Mat HomAdapter::GetInverseHomography() const{
    Mat inverse;
    invert(GetHomography(), inverse);
    return inverse;
}

Mat HomAdapter::GetProjectedPoints(const vector<CvPoint2D32f> &ptList, int image_num) const{
    
    vector<Point2f> points;
    int i;
    for (i = 0; i < ptList.size(); ++i){
        Point2f temp;
        temp.x = ptList[i].x;
        temp.y = ptList[i].y;
        points[i] = temp;
    }
    
    return GetProjectedPoints(ptList, image_num);
}

Mat HomAdapter::GetProjectedPoints(const vector<Point2f> &ptList, int image_num) const{
    
    Mat proj;
    
    switch(image_num){
        case 1:
            //pt List are points on the second image
            //Map these points back to the first image
            perspectiveTransform(Mat(ptList), proj, GetInverseHomography());
            break;
        case 2:
            //pt List are points on the first image
            //Map these points back to the second image
            perspectiveTransform(Mat(ptList), proj, GetHomography());
            break;
        default:
            throw "Illegal Image Number";
    }
    
    return proj;
}

void HomAdapter::PrintCorrespondingPoints(){
    int i;
    int size = matchedPts1_.size();
    for(i=0; i<size; ++i){
        cout << "Image 1 Point [ X: " << matchedPts1_[i].x << ", Y: " << matchedPts1_[i].y << " ]";
        cout << " Image 2 Point [ X: " << matchedPts2_[i].x << ", Y: " << matchedPts2_[i].y << " ]";        
        cout << endl;
    }
}

cv::Mat HomAdapter::GetProjectedWarpImage(const cv::Mat &img1, const cv::Mat &img2, int image_num){
    //Make tmeporary copies of MAt
    Mat src, dest;
    Mat HH; //Homography holder
    cv::Size size;
    
    //Set parameters
    switch (image_num){
        case 1: //Project warped first image
            src = img1;
            size.width = img2.cols;
            size.height = img2.rows;
            HH = GetHomography();
            break;
        case 2: //Project second image
            src = img2;
            HH = GetInverseHomography();
            size.width = img1.cols;
            size.height = img1.rows;
            break;
        default:
            throw "Illegal image number, Image number needs to be 1 or 2 ";
            //Returns empty Mat
    }
    
    //Warp using Perspective
    cv::warpPerspective(src, dest, HH, size);
    
    return dest;
}

cv::Mat HomAdapter::GetProjectedPoints(const cv::Mat &img1, const cv::Mat &img2, int image_num){
    Mat select_img, proj;
    vector<Point2f> plist;
    
    //Pending on the image number draw imaeg accordingly
    switch (image_num){
        case 1:
            plist = GetMatchedPts1();
            proj = GetProjectedPoints(GetMatchedPts2(), image_num);
            select_img = img1;
            break;
        case 2: 
            plist = GetMatchedPts2();
            proj = GetProjectedPoints(GetMatchedPts1(), image_num);
            select_img = img2;
            break;
        default:
            throw "Illegal image number, Image number needs to be 1 or 2 ";
            //Returns empty Mat
    }
    
    //Label each circle
    assert(plist.size() == proj.rows);
    
    //Draw all the selections on the image
    drawCircleOnSelected(plist, select_img, BLUE);
    drawXOnSelected(proj, select_img, RED);
    drawLinesOnSelected(plist, toPoint2fVector(proj), select_img, RED);

    return select_img;
}


