The following are what we have done so far (all in OpenCV):
1. Being able to find a keypoint/feature points of the given image using SIFT feature detector.
   (Refer to FindCorrespondences() in CVUtilities.cc, up to detect() function call)

2. Given 2 set of keypoints/feature points of different images, compute the matches between the 2 images. 
   (Refer to FindCorrespondences() in CVUtilities.cc, starting from match())

OpenCV function call relevant for drawing
Keypoints: drawKeypoints()
Matches: drawMatches()

3. Given the 2 set of keypoints/feature points, compute the homography/transformation matrix. And after we have the homography, we can then transform the input image to look like the reference image. (Refer to HomAdapter.cc constructor and GetProjectedWarpImage())

4. To compute the homography, RANSAC algorithm is used by the OpenCV built-in function. RANSAC basically estimates the best matches between two images to solve for the transformation matrix. We were able to figure out which feature points were picked by RANSAC by essentially transforming one set of keypoints, then check the distance between the transformed keypoints and the other set of keypoints. If two points are within a certain tweakable treshold (RANSAC treshold) than the point is an inlier.
(Refer to DoTransformation() in main.cc)


What we've worked on so far is summarized as C++ project files.
To program command line argument is as follow:

[path to a folder where input images are placed] [path to where output images will be created] [path to a base image]

Make sure to create the output folder as the program won't run if it doesn't exists

eg.
/Users/antoniusdennyharijanto/Desktop/dummy/input /Users/antoniusdennyharijanto/Desktop/dummy/output /Users/antoniusdennyharijanto/Desktop/dummy/base.jpg
