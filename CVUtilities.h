//
//  CVUtilities.h
//  SIFT
//
//  Created by Antonius Harijanto and Stephen Joe Jonany on 5/15/12.
//  Copyright (c) 2012 University Of Washington. All rights reserved.

#include <vector>
#include <opencv2/opencv.hpp>
#include <string>

#ifndef SIFT_CVUtilities_h
#define SIFT_CVUtilities_hs

/*
 Structure that interfaces HomAdapter and SIFT Feature points
 */
typedef struct {
    std::vector<cv::Point2f> points_A;
    std::vector<cv::Point2f> points_B;
} Correspondences;

/*
    Given 2 images, output a vector of Point2D indicating the correspondences points
    ARG:
    input1_path: path to image 1
    input2_path: path to image 2
    *output: output parameter
    kp1, kp2 : output parameter, which are the keypoint
    matches_num: number of matching points to return, -1 to return all points
    
 
 
    Return: Matches from image 1 to image2
 */
std::vector<cv::DMatch> FindCorrespondences(cv::Mat &input1, cv::Mat &input2, Correspondences *output, std::vector<cv::KeyPoint>  &kp1,std::vector<cv::KeyPoint> &kp2, int matches_num = -1);

/*
    Given a path to directory, spits out the image file (.JPG / .PNG) on that folder
    vector of size 0 is returned when no file.
 */
std::vector<std::string> listImageFile(std::string path);

/*
    Given 2 image path, return a Mat representation of those two image with the bigger image
    scaled to be as big as the smaller image
 */
void ScaleImage(std::string image1_path, std::string image2_path, cv::Mat *out1, cv::Mat *out2);

/*
    Compute the diagonal of two Mat images
 */
double ComputeDiagonal(cv::Mat &img);

#endif
