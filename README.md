# PID Controller
Self-Driving Car Engineer Nanodegree Program

---
[![Udacity - Self-Driving Car NanoDegree](https://s3.amazonaws.com/udacity-sdc/github/shield-carnd.svg)](http://www.udacity.com/drive)

The goals / steps of this project are the following:

* Create a PID control algorithm to guide a vehicle around a track.
* Tune PID hyperparameters so the vehicle smoothly follows the road, minimizing the cross-track error.
* Implement a twiddle algorithm to continuously tune the hyper parameters.

[//]: # (Image References)

[image1]: ./images/slow.gif "slow"
[image2]: ./images/fast.gif "fast"
[image3]: ./images/equation.gif "equation"



---

## Project Introduction

This project implements a PID controller with twiddle optimization of the PID hyperparameters. The PID control algorithm runs based on the cross-track error (CTE), provided by the simulator, which is calculated using the vehicle's distance from the center of the lane. The CTE is then used in a series of formulas to calculate the proportional (P), integral (I) and derivative (D) values which together can be used to calculate the total error, alpha. Ultimately, the total error is used as a steering angle and throttle measurement that corrects the vehicle's position and speed until it reaches the center of the lane and maximum acceleration. The equation for total error (alpha) is shown below.

![alt text][image3]

The first term (-taup * CTE) is the proportional component that allows the system to minimize the CTE, but it always overshoots the target resulting in an oscillation around the target. The second term (-taud * derivative of CTE) is the derivative term that counteracts the oscillation from P. Finally the third term (-taui * sum_CTE) is the integral term that corrects for the system if the PID controller is running parallel, but not equal to, the target. 

For more details on how the control algorithm works, view the comments in `PID.cpp`.

### Hyperparameter Tuning

This section discusses how the hyperparameters, also known as tau, were chosen for each term P, I and D. When tuning a hyperparameter, all others were set to zero to isolated the hyperparameter being tuned.

**P -** This was tuned by staring at a small value and gradually increasing until the vehicle began swerving back and forth, showing the oscillation of the P term. I chose `0.07` for the steering PID and `0.02` for the throttle PID.

**I -** This value was largely unused because after tuning P and D, the vehicle stayed in the center of the lane. I chose a value of `0.0001` in both the steering and throttle PID to minimize I's influence while still allowing for improvement through twiddle as the vehicle is driving.

**D -** This was tuned with the P value set, then I increased the D value until the oscillation stoped. I chose `3.0` for the steering PID to ensure a smooth ride and `0.03` for the throttle PID because otherwise the vehicle would take corners too fast and would rarely brake.


### Twiddle

The twiddle algorithm continuously tunes the PID controller's hyperparameters by analyzing the cross-track error and keeping track of the smallest CTE. If the vehicle starts to veer and the CTE increases, the twiddle algorithm will incrementally increase or decrease the hyperparameters until the CTE is minimized. 

### Other Safety Features

There are a few extra safety features implemented in `main.cpp` that help the vehicle's performance. The first is increasing the steering angle by 50% at high speeds. This helps the car to handle sharp turns without slowing down. The second safety features is inverted steering when the car is traveling in reverse. Although this feature isn't used, it was included to help the car recover if it goes outside of the lane.

## Results

There are two modes when running this PID controller: slow & steady and fast & furious.

### Slow & Steady

This mode uses a constant acceleration of 0.3, keeping the vehicle at or below 30 mph. This slower speed allows for a smooth ride with very minimal oscillation in the lane. 

![alt text][image1]

### Fast & Furious

This mode uses a PID controller as well as a default acceleration of 0.88 to acheive speeds up to 95 mph. While this is not as smooth as the the previous mode, it shows the limits of this PID controller. 

![alt text][image2]

## Limitations
Overall, the PID control algorithm performs very well. The main limitation is in the faster mode and is described below:

* After a turn the car begins to oscillate in the lane, this can be a result of the steering properties of the vehicle as well as the refresh rate of the PID control.

One solution may be to implement a computer vision algorithm that gives the vehicle a heads-up of upcoming sharp turns so that the vehicle can slow down and eliminated overshooting though turns.

## Dependencies

* cmake >= 3.5
 * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1(mac, linux), 3.81(Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools]((https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)
* Simulator. You can download these from the [project intro page](https://github.com/udacity/self-driving-car-sim/releases) in the classroom.


## Basic Build Instructions

This project involves the Term 2 Simulator which can be downloaded [here](https://github.com/udacity/self-driving-car-sim/releases)

Some scripts have been included to streamline the process of running this project, these can be leveraged by executing the following in the top directory of the project:

1. Clone this repo.
2. Run the build script using `./build.sh`.
4. Run the program: `./run.sh`.
5. Open the simulator and select "Project 4: PID Controller"

