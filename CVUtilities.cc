//
//  Utilities.cc
//  SIFT
//
//  Created by Antonius Harijanto on 5/15/12.
//  Copyright (c) 2012 University Of Washington. All rights reserved.
//

#ifndef _CV_UTIL_
#define _CV_UTIL_

#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <string.h>
#include <fstream>
#include <dirent.h>
#include <vector>
#include <sstream>
#include <opencv2/opencv.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include "CVUtilities.h"
#include <cmath>

using namespace cv;
using namespace std;

#define EPSILON 0.05
bool Equal(double &f1, double &f2){
    return abs(f1-f2) < EPSILON;
}

double ComputeDiagonal(cv::Mat &img){
    Size s1 = img.size();
    return sqrt(pow(s1.height, 2.0) + pow(s1.width, 2.0));
}

void ScaleImage(std::string image1_path, std::string image2_path, Mat *out1, Mat *out2){
    //Original images
    Mat ori1 = imread(image1_path);
    Mat ori2 = imread(image2_path);
    Size s1 = ori1.size();
    Size s2 = ori2.size();
    
    double diag1 = sqrt(pow(s1.height, 2.0) + pow(s1.width, 2.0));
    double diag2 = sqrt(pow(s2.height, 2.0) + pow(s2.width, 2.0));    
    
    if(Equal(diag1, diag2)){
        ori1.copyTo(*out1);
        ori2.copyTo(*out2);
    }
    else if(diag1 < diag2){
        resize(ori2, *out2, s1);
        ori1.copyTo(*out1);
    }
    else{
        resize(ori1, *out1, s2);
        ori1.copyTo(*out2);
    }
    
}


vector<DMatch> FindCorrespondences(Mat &input1, Mat &input2, Correspondences *output, vector<KeyPoint> &keypoints1, vector<KeyPoint> &keypoints2, int matches_num){
    //descriptor matrix
    Mat descriptor1;
    Mat descriptor2;
    
    //sift detector
    SiftFeatureDetector detector;
    //sift feature extractor
    SiftDescriptorExtractor extractor;
    
    //vector of matches
    vector<DMatch> matches;
    
    //Nearest neighbhor matcher
    FlannBasedMatcher matcher;
    
    //compute keypoints
    detector.detect(input1, keypoints1);
    detector.detect(input2, keypoints2);
    
    //compute descriptor matrix
    extractor.compute(input1, keypoints1, descriptor1);
    extractor.compute(input2, keypoints2, descriptor2);
    
    //find matches
    matcher.match(descriptor1, descriptor2, matches);
    //uses knn to find the best n matches
    //matcher.knnMatch(descriptor1, descriptor2, matches, 3);
    
    
    // number of matches, -1 indicate all matches
    int num_matches = (((matches_num > matches.size()) || (matches_num == -1))? matches.size(): matches_num);
    // get only matches_num number of matches
    for(int i = 0;i < num_matches;i++){
        output->points_A.push_back(keypoints1[matches[i].queryIdx].pt);
        output->points_B.push_back(keypoints2[matches[i].trainIdx].pt);
    }
    //Can use manual looping as above or this shortcut
    //KeyPoint::convert(keypoints1, output->points_A, queryIdxs);
    //Keypoint::convert(keypoints2, output->points_B, trainIdxs);
    
    
    return matches;
}

/* given an input path, returns vector of string of the image files in that path */
vector<string> listImageFile(string path){
    DIR *pDIR;
    struct dirent *entry;
    /* individual file name */
    string fname;
    /* the image file name */
    vector<string> output;
    
    if( (pDIR=opendir(path.c_str())) ){
        while((entry = readdir(pDIR))){
            fname = entry->d_name;
            /* image file extension */
            if(fname.find(".JPG") != string::npos || 
               fname.find(".JPEG") != string::npos || 
               fname.find(".PNG") !=string::npos || 
               fname.find(".png") !=string::npos || 
               fname.find(".jpg") !=string::npos) {
                output.insert(output.end(), fname);
            }
        }
        closedir(pDIR);
    }
    
    return output;
}

#endif
