//
//  main.cpp
//  SIFT
//
//  Created by Antonius Harijanto on 5/17/12.
//  Copyright (c) 2012 University Of Washington. All rights reserved.
//

#include <iostream>
#include <string>
#include <cassert>
#include "CVUtilities.h"
#include "HomAdapter.h"
#include <sys/time.h>
#include <sstream>

//command line argument
#define ARG_LENGTH 4
#define INPUT_PATH 1
#define OUTPUT_PATH 2
#define BASE_IMG_PATH 3


//Whether to display to screen or save or both
#define SHOW_TO_SCREEN 0
#define SAVE 1

//Whether inliers matches would be drawn or not
#define DRAW_FEATURE 1

#define RESULT_WINDOW "Input to Base"
#define INPUT_WINDOW "Input"
#define BASE_WINDOW "Base"
using namespace std;


/*
 Block until an ESC is hit
 */
void EscapeToQuit(){
    while (1) {
        if(cvWaitKey(100) == 27)
            break;
    }
}


/* Given two timeval, prints out the difference in milliseconds */
void PrintTiming(struct timeval &before, struct timeval &after){
    float timing = ((after.tv_sec*1000000 + after.tv_usec) - (before.tv_sec*1000000 + before.tv_usec))/1000.0; 
    cout << timing << " milliseconds." << endl << endl;
}



void DoTransformation(string base_path, string input_path, string save_path, int image_index){
    // Prints processing input file
    cout << "PROCESSING: " << input_path << endl;
    cout << "---------------------------------------------" << endl << endl;
    
    // To record the timing of the processing
    struct timeval tp_before, tp_after;
    

    //TODO: Compute the treshold using precentage
    //const float RANSAC_TRESHOLD_PERCENTAGE = 0.02;
    
    Mat image_base; //Reference image
    Mat image_other; //Image to be transformed to look like reference image
    Mat result;    //result of homography transformation
    Mat result_matches; //result with matches drawn
    Mat result_matches_feature; // result of feature detection with keypoints on it
    
    //Result with matches drawn
    Mat result_inlier_matches, result_all_matches;
    //Result with feature drawn
    Mat result_feature_base, result_feature_input;
    
    
    //Rescale the two images to be of the same size
    ScaleImage(base_path, input_path, &image_base, &image_other);
    
    float ransac_treshold = 3;//ComputeDiagonal(image_base) * RANSAC_TRESHOLD_PERCENTAGE;

    
    if(image_base.data == NULL){
        cout << "Invalid base image path: " << base_path << endl;
        return;
    }
    
    if(image_other.data == NULL){
        cout << "Invalid target image path: " << input_path << endl;
        return;
    }
    
    //Holds the correspondences points
    Correspondences corr;
    //Holds the correspondences in terms of DMatch
    vector<DMatch> matches;
    
    //Feature keypoints for base and output image
    vector<KeyPoint> kp_base;
    vector<KeyPoint> kp_out;
    
    cout << "Computing correspondences using SIFT" << endl;
    gettimeofday(&tp_before, NULL);
    gettimeofday(&tp_before, NULL);
    
    //Getting the correspondences points using SIFT
    matches = FindCorrespondences(image_base, image_other, &corr, kp_base, kp_out);
    gettimeofday(&tp_after, NULL);
    PrintTiming(tp_before, tp_after);
    

    cout << "Computing homography" << endl;
    gettimeofday(&tp_before, NULL);
    gettimeofday(&tp_before, NULL);
    //Getting the homography matrix
    HomAdapter ha(corr.points_A, corr.points_B, ransac_treshold);
    gettimeofday(&tp_after, NULL);
    PrintTiming(tp_before, tp_after);
    
    cout << "Producing transformed image" << endl;
    gettimeofday(&tp_before, NULL);
    gettimeofday(&tp_before, NULL);
    
    //Project the input image to the base image
    const int INPUT_TO_BASE = 2;
    result = ha.GetProjectedWarpImage(image_base, image_other, INPUT_TO_BASE);
    
    gettimeofday(&tp_after, NULL);
    PrintTiming(tp_before, tp_after);
    
    int numInliers = 0;
    //If an inlier matches wanted to be drawn
    if(DRAW_FEATURE){
        cout << "Producing feature points image" << endl;
        gettimeofday(&tp_before, NULL);
        gettimeofday(&tp_before, NULL);
        
        //To mask the outlier
        vector<char> matchesMask(matches.size(),0);
        
        //Transformation matrix
        Mat H = ha.GetHomography();
        
        // We compute the inlier by transforming the image then look at the
        // keypoints we found before the transformation
        // if the keypoints were of within ransac treshold, it is an inlier
        Mat outputPtTransformed;
        
        //Transform the base keypoints
        perspectiveTransform(Mat(corr.points_A), outputPtTransformed, H);
        

        //Iterate through the points and figure out the inliers
        for(int i=0 ; i < corr.points_B.size() ; i++){
            //inliers is the one that's within RansacTreshold
            //norm is just a fancy way for doing absolute
            if( norm(corr.points_B[i] - outputPtTransformed.at<Point2f>((int)i,0)) <= ransac_treshold) // inlier
            {
                matchesMask[i] = 1; // We include this keypoint
                numInliers++;
            }
                
        }
        

        //Draw the inlier matches
        drawMatches(image_base, kp_base, image_other, kp_out, matches, result_inlier_matches, CV_RGB(0, 0, 0), CV_RGB(255, 255, 255)/*Scalar::all(-1)*/, matchesMask, /*DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS*/DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
        
        //Draw all matches
        drawMatches(image_base, kp_base, image_other, kp_out, matches, result_all_matches, Scalar::all(-1), Scalar::all(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
        

        //Draw feature points
        //of the reference image
        drawKeypoints(image_base, kp_base, result_feature_base, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
        //of the input image
        drawKeypoints(image_other, kp_out, result_feature_input, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
        
        gettimeofday(&tp_after, NULL);
        PrintTiming(tp_before, tp_after);
    }
    
    
    if(SHOW_TO_SCREEN){
        namedWindow(RESULT_WINDOW);
        namedWindow(BASE_WINDOW);
        namedWindow(INPUT_WINDOW);
        imshow(BASE_WINDOW, image_base);
        imshow(INPUT_WINDOW, image_other);
        imshow(RESULT_WINDOW, result);
        cout << "Press Esc to process next image" << endl;
        EscapeToQuit();
    }
    if(SAVE){
        stringstream ss;
        ss << numInliers << "__" << image_index;
        save_path += ss.str();
        imwrite(save_path + "_homog_result.jpg", result);
        cout << "Image produced: " << save_path + "_homog_result.jpg" << endl;
        
        if(DRAW_FEATURE){
            /*
            //Result with matches drawn
            Mat result_inlier_matches, result_all_matches;
            //Result with feature drawn
            Mat result_feature_base, result_feature_input;
            */

            string name = "_matches_inlier_matches.jpg";
            imwrite(save_path + name, result_inlier_matches);
            cout << "Image produced: " << save_path + name << endl;   
            
            name = "_matches_all_matches.jpg";
            imwrite(save_path + name, result_all_matches);
            cout << "Image produced: " << save_path + name << endl;   
            
            name = "_result_feature_base.jpg";
            imwrite(save_path + name, result_feature_base);
            cout << "Image produced: " << save_path + name << endl;   
            
            name = "_result_feature_input.jpg";
            imwrite(save_path + name, result_feature_input);
            cout << "Image produced: " << save_path + name << endl;   
        }
    }
    
    cout << "---------------------------------------------" << endl << endl;
    
}

void FUsage(){
    cout << "./ProgramName [input-dir-path] [output-dir-path] [base-file-path]" << endl;
    cout << ".jpg or .png files are expected on the input-path" << endl << endl;
//    cout << "Be sure to have two folders on the output-dir-path: \"homography\" and \"Feature\"" << endl;
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv){
    if(argc != ARG_LENGTH){
        cout << "Arg length: " << argc << endl;
        FUsage();
    }
    
    /* Grab all input file in the input folder */
    vector<string> inputFile = listImageFile(argv[INPUT_PATH]);
    if(inputFile.size() == 0){
        cout << "Invalid input path? No file found!" << endl;
        exit(EXIT_FAILURE);
    }
    
    /* grab the input path and append / if necessary */
    string inputpath = argv[INPUT_PATH];
    if(inputpath[inputpath.length()-1] != '/')
        inputpath += '/';
    
    string outputdirpath = argv[OUTPUT_PATH];
    if(outputdirpath[outputdirpath.length()-1] != '/')
        outputdirpath += '/';
    
    /* base image path */
    string base_img_path = argv[BASE_IMG_PATH];
    
    /* input output path*/
    string ifpath;
    stringstream ofpath;
    string ofpath_homography;
    
    for(int i=0;i<inputFile.size();i++){
        ofpath.str("");
        ifpath =  inputpath + inputFile[i];
        ofpath << outputdirpath;
        /* be sure to append .extension to the file */
        ofpath_homography = ofpath.str();// + "_homog_result.JPG";
        DoTransformation(base_img_path, ifpath,ofpath_homography, i+1);
    }
    
    
    return 0;
}