//
//  ImageDrawingTools.cpp
//  RANSAC_AR
//
//  Created by Michael Hotan on 5/13/12.
//  Copyright (c) 2012 University of Washington. All rights reserved.
//

#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <opencv2/opencv.hpp>
#include "ImageDrawingTools.h"
#include <stdio.h>

#define WHITE CV_RGB(250,250,250)

using namespace cv;

void drawCircleOnSelected(const vector <Point2f> &pts, Mat &img, CvScalar color){
    //Font parameters
    double hscale = 1.0;
	double vscale = 0.8;
	double shear = 0.2;
	int thickness = 1;
	int line_type = 8;
    
    //Declare and define font
    
	CvFont font1;
	cvInitFont(&font1,CV_FONT_HERSHEY_DUPLEX,hscale,vscale,shear,thickness,line_type);
    
    //Declare point 
    Point circ_pt, text_pt;
    
    //Declare Circle parameter
    int circ_radius = 5;
    
    int size = pts.size();
    int i; 
    
    for (i = 0; i < size; ++i){
        //Buffer to start character
        char buf[50] = {'0'}; //Zero out the buffer to ensure null termination
        
        //Get closest estimate for x and y coordinate
        int x_int = static_cast<int>( pts[i].x );
        int y_int = static_cast<int>( pts[i].y );
        
        circ_pt = Point(x_int, y_int);
        
        circle(img, circ_pt, circ_radius, color);
        
        //Turn index into Number
        sprintf(buf, "%d", i+1);
        
        //Off the point origin of text outside the circle
        text_pt = Point(circ_pt.x + circ_radius, circ_pt.y - circ_radius);
        string num(buf);
        putText(img, num, text_pt,CV_FONT_HERSHEY_SIMPLEX,1, WHITE);
        //TODO Draw selected number
    }
}

void drawXOnSelected(const vector <Point2f> &pts, Mat &img,  CvScalar color){
    
    //Declare point 
    CvPoint line1_pt_begin, line1_pt_end, line2_pt_begin, line2_pt_end;
    
    //Declare Circle parameter
    int circ_radius = 5;
    
    int size = pts.size();
    int i; 
    
    for (i = 0; i < size; ++i){
        
        //Get closest estimate for x and y coordinate
        int x_int = static_cast<int>( pts[i].x );
        int y_int = static_cast<int>( pts[i].y );
        
        line1_pt_begin = cvPoint(x_int - circ_radius, y_int - circ_radius);
        line1_pt_end = cvPoint(x_int + circ_radius, y_int + circ_radius);
        line2_pt_begin = cvPoint(x_int + circ_radius, y_int - circ_radius);
        line2_pt_end = cvPoint(x_int - circ_radius, y_int + circ_radius);
        
        line(img, line1_pt_begin, line1_pt_end, color);
        line(img, line2_pt_begin, line2_pt_end, color);
    }
}

void drawLinesOnSelected(const vector <Point2f> &pts1,
                         const vector <Point2f> &pts2, Mat &img,  CvScalar color){
    
    assert(pts1.size() ==pts2.size());
    
    //Declare point 
    CvPoint line_begin, line_end;
    
    int size = pts1.size();
    int i; 
    
    for (i = 0; i < size; ++i){
        
        //Get closest estimate for x and y coordinate
        int x1_int = static_cast<int>( pts1[i].x );
        int y1_int = static_cast<int>( pts1[i].y );
        int x2_int = static_cast<int>( pts2[i].x );
        int y2_int = static_cast<int>( pts2[i].y );
        
        //Draw Line
        line_begin = cvPoint(x1_int, y1_int);
        line_end = cvPoint(x2_int, y2_int);
        
        line(img, line_begin, line_end, color);
    }
}

vector <Point2f> toPoint2fVector(const cv::Mat &mat){
    vector <Point2f> copy;
    
    //Number of rows correlate to amount of points
    int size = mat.rows;
    int i;
    for (i = 0; i < size ; ++i){
        CvPoint2D32f temp;
        temp.x = mat.at<float>(i,0);
        temp.y = mat.at<float>(i,1);
        copy.push_back(temp);
    }
    
    return copy;
}
