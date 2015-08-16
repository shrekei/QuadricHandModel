In many researches related to hand pose estimation and gesture recognition, a realistic 3D hand model is needed to synthesize the training samples, to measure the compatibility between input and hypothesized hand poses, and to visualize the prediction. 

This is a 3D hand model developed mainly for vision-based hand motion analysis. The hand model is modeled as a set of rigid quadratic surfaces, and has 27 degrees-of-freedom (DoFs) including 6 DoFs of global motion and 21 DoFs of local motion. The hand skeleton is modeled as a kinematic chain of 20 joints connected by bones in a tree structure with the root at the wrist. In this program you can view the model from different positions and perspectives, and manipulate each DoF separately. More importantly, to assist research in this area, we also provide functions to generate the depth image, silhouette image and edge images, which can be easily used for model-based matching between the input and the 3D hand model. For synthesis of these images, it is implemented by a pin-hole camera model to project the hand model onto the image plane. Our implementation of the hand model is largely inspired by the model described in B. Stenger's paper. 

B. Stenger, et.al, Model-Based Hand Tracking Using an Unscented Kalman Filter, in BMVC, 2001.

This project is developed using C++/MFC/OpenGL/OpenCV. So make sure these libraries are installed before you run the project. 

In the menu "Console", you can open Console to assist debug and test the program. 

In the menu "Debug", you can find two functions. First is "show projection", and you can use it to generate the projected depth/silhouette/edge images for the current configuration of the hand. Second is "normalize model", and you can use it to move the hand model at a specific distance before the camera (Camera is assumed to be at the origin and pointing at (0, 0, 1)), so that the size of the synthesis images just fit the image. Note!! Make sure you move the hand model in front the camera when you generate the synthesis images.  

In the main panel, use Ctrl+mouse wheel to change distance, and use left mouse to change the viewpoint. 

You can also change the values of the 27 DoFs of the hand model and generate the synthesis images. 0-2 is for hand translation, 3-5 is for global hand rotation and 6-26 is for local finger articulations. You can either increase or decrease their values in the panel. 

For questions and comments: hliang1@e.ntu.edu.sg or shrekei@hotmail.com
