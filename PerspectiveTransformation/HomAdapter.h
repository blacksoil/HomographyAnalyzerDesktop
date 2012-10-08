//
//  HomAdapter.h
//  RANSAC_AR
//
//  Created by Michael Hotan on 5/12/12.
//  Copyright (c) 2012 University of Washington. All rights reserved.
//

#ifndef RANSAC_AR_HomAdapter_h
#define RANSAC_AR_HomAdapter_h

#include <iostream>
#include <exception>
#include <string>
#include <cstdlib>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

/*
 Represents a 
 
 
 */
class HomAdapter {
public:
    /*
     Wrapper class to hold the homography of between two objects.  Homography is 
     determined by the list of corresponding points.
     If point list are to large then the largest list is trimmed off until is equal to the smallest
     Excess points are discarded
     
     Arguments
        m1- Vector of first set of correspondence points that should be correlated with image 1
        m2- Vector of Second set of correspondence points that should be correlated with image 2
        threshold- threshold for homographic correspondence
     */
    HomAdapter(vector<Point2f> m1, vector<Point2f> m2, double threshold) : matchedPts1_(m1) , matchedPts2_(m2) {
        while (matchedPts1_.size() > matchedPts2_.size()){
            matchedPts1_.pop_back();
        }
        
        while (matchedPts2_.size() > matchedPts1_.size()){
            matchedPts2_.pop_back();
        }
        
        Homography_ = findHomography(matchedPts1_, matchedPts2_, CV_RANSAC, threshold);
    }
    
    HomAdapter(vector<CvPoint2D32f> m1, vector<CvPoint2D32f> m2, double threshold) {
        int size = (m1.size() < m2.size()) ? m1.size(): m2.size();
        
        int i;
        for(i = 0; i < size; ++i){
            Point2f temp1, temp2;
            temp1.x = m1[i].x;
            temp1.y = m1[i].y;
            temp2.x = m2[i].x;
            temp2.y = m2[i].y;
            
            matchedPts1_.push_back(temp1);
            matchedPts2_.push_back(temp2);
        }
        
        Homography_ = findHomography(matchedPts1_, matchedPts2_, CV_RANSAC, threshold);
    }
    
    /*
     Returns the Homography Matrix of Corresponding points
     */
    cv::Mat GetHomography() const;

    /*
     Returns the Inverse Homography Matrix of Corresponding points
     */
    cv::Mat GetInverseHomography() const;
    
    /*
     Returns the projected points from a list of points stored in ptList.
     image_num represents 
     */
    cv::Mat GetProjectedPoints(const vector<CvPoint2D32f> &ptList, int image_num) const;
    
    /*
     Returns the projected points from a list of points stored in ptList.
     image_num represents 
     */
    cv::Mat GetProjectedPoints(const vector<Point2f> &ptList, int image_num) const;
    
    /*
     Returns copy of first set of correspeondence points
     */
    vector<Point2f> GetMatchedPts1() const {return matchedPts1_;}
    
    /*
     Returns copy of second set of correspondence points
     */
    vector<Point2f> GetMatchedPts2() const {return matchedPts2_;}
    
    /*
     Returns the projected image based off the image number
     if the image number is one.  It projects img2 onto Image one
     
     Arguments:
     img1- First image with respect to finding the homography. 
        img1 correlates to the first vector of matched points
     img2- second image with respect to finding the homography,
        img2 correlates to the second vector of matched points
     image_num - defines which image to project onto which Has to be 1 or 2;
     
     Returns 
     Image of the the new projection
     */
    cv::Mat GetProjectedWarpImage(const cv::Mat &img1, const cv::Mat &img2, int image_num);
    
    /*
     Returns the image of original points and the projections of the other images key points.
     Circles represent the original points while the Xs represent the projection mapped abck to the original 
     image.
     
     Arguments:
     img1- First image with respect to finding the homography. 
     img1 correlates to the first vector of matched points
     img2- second image with respect to finding the homography,
     img2 correlates to the second vector of matched points
     image_num - defines which image to project onto which, Has to be 1 or 2;
     
     Return:
     Images with projected points of image other then image_num onto image numbered with image_num
     */
    cv::Mat GetProjectedPoints(const cv::Mat &img1, const cv::Mat &img2, int image_num);
    
    void PrintCorrespondingPoints();
    
private:
    vector<Point2f> matchedPts1_;
    vector<Point2f> matchedPts2_;
    cv::Mat Homography_;
};


#endif
