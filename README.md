# findcontours_explained
Understanding the algorithm behind findContours from OpenCV based on the paper "Topological structural analysis of digitized binary images by border following."

https://github.com/yuelinn/findcontours_explained/assets/19858677/88b1444a-f6d1-4327-bf8b-7362895928d0

Some words explaining the gif:
+ findContours is a border following algorithm for finding outer and inner (holes) borders of all components in a binary image. 
+ We look through the image sequencially using raster scan and update the pixel values to with the unique border id. 
+ The binary image is shown in grey. The pixel in focus is shown as purple; light purple during raster scan, and darker purple during border following.
+ The yellow pixel is the pixel is where we start to 'look'. The direction of 'looking' is given by the algorithm. 
+ The green pixel is the pixel being 'looked' at when searching for the next pixel.


TODO:
+ Make graph for the relations between borders
+ Make D4 (now code only works for D8)
