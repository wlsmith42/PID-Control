#include "PID.h"
#include <limits>
#include <iostream>

PID::PID() {}

PID::~PID() {}

void PID::Init(double Kp_, double Ki_, double Kd_) {
  //Initializes PID coefficients (and errors, if needed)

   PID::Kp = Kp_;   //tau_p
   PID::Ki = Ki_;   //tau_i
   PID::Kd = Kd_;   //tau_d

   p_error = 0.0;   //cte
   i_error = 0.0;   //sum(cte)
   d_error = 0.0;   //cte_diff

   step_num = 1;
   best_err = std::numeric_limits<double>::max(); //Initialize to high value

   //twiddle updates every 200 steps
   numSteps = 200;

   //Store coefficients in array for twiddle
   p[0] = Kp*0.1;
   p[1] = Ki*0.1;
   p[2] = Kd*0.1;

   //Current index of twiddle coefficients
   p_index = 0;
}

void PID::UpdateError(double cte) {
   //Updates PID errors based on cte.

   //Set the intial p_error to the cte, which can be used as the prev_cte in the next cycle
   if(step_num == 1) {
     p_error = cte;
   }

   //PID equations
   d_error = cte - p_error;
   p_error = cte;
   i_error += cte;

  /**
    * Twiddle - Hyperparameter Tuning
    */

  //Get total error based on cross track error
  total_err += cte*cte;

  //Run the twiddle algorithm every 'n' evaluation steps
  if(step_num % numSteps == 0) {
    //if the current error is a new best, update
    if(total_err < best_err) {
      best_err = total_err;
      p[p_index] *= 1.1;

      //Setup for the twiddler
      p_add = p_sub = false;
    }

    if(!p_add && !p_sub) {
      //First iteration after start of cycle, add elements
      Twiddler(p_index, p[p_index]);
      p_add = true;
    } else if(p_add && !p_sub) {
      //Second iteration after cycle
      //No best error found,
      Twiddler(p_index, -2*p[p_index]);
      p_sub = true;
    } else {
      //Third iteration
      //No best error found after two attempts, time to try something new
      Twiddler(p_index, p[p_index]);
      p[p_index] *= 0.9;
      p_add = p_sub = false;

      //Cycle through the 3 hyperparameters
      p_index = (p_index + 1) % 3;
    }
    //Reset total error at end of cycle
    total_err = 0;

    //Debugging prompts
    std::cout << "Adjusted parameters ..." << "\n";
    std::cout << "Kp = " << Kp << " Ki = " << Ki << " Kd = " << Kd << "\n\n";
  }

   step_num++;
}

double PID::TotalError() {
  //Calculate and return the total error
  return -Kp * p_error - Kd * d_error - Ki * i_error;
}

//Twiddles variables based on index of hyperparameter and value determined above.
void PID::Twiddler(int index, double value) {
  switch(index) {
    case 0:
      Kp += value;
      break;
    case 1:
      Ki += value;
      break;
    case 2:
      Kd += value;
      break;
    default:
      break;
  }
}
