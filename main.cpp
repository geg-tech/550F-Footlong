/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       4804195893                                                */
/*    Created:      8/29/2025, 8:51:54 AM                                     */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;

//-----------------------------------------------------------------------------------------------------

// define your global instances of motors and other devices here
  // define controller
controller Controller = controller();
  // define drivetrain motors/motor groups
    // motors
motor MotorL1 = motor(PORT11, ratio6_1,true); //second parameter dictates motor cartridge, ratio6_1 is blue speed cartridge
motor MotorL2 = motor(PORT12, ratio6_1,true);
motor MotorL3 = motor(PORT13, ratio6_1,true);
motor MotorR1 = motor(PORT9, ratio6_1);
motor MotorR2 = motor(PORT19, ratio6_1);
motor MotorR3 = motor(PORT18, ratio6_1);
    // motor for intake/conveyor
motor MotorIntake = motor(PORT1,ratio18_1);
    // motor groups
motor_group DrivetrainLeft = motor_group(MotorL1, MotorL2, MotorL3);
motor_group DrivetrainRight = motor_group(MotorR1, MotorR2, MotorR3);
  // define drivetrain object
drivetrain Drivetrain = drivetrain(DrivetrainLeft, DrivetrainRight, 12.5, 14, 14, inches, 0.75);

//-----------------------------------------------------------------------------------------------------


/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void autonomous(void) {
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................
}


void autotototot(){
  Drivetrain.driveFor(56, inches, 200, rpm);
  Drivetrain.turnFor(left,30, degrees);
  Drivetrain.driveFor(10, inches, 200, rpm);
  MotorIntake.spinFor(forward, 5, sec);
  Drivetrain.stop();
  // Drivetrain.driveFor(10, inches, 200, rpm);a
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                 2         */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void usercontrol() {
  // User control code here, inside the loop
      
    
    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.

    // ........................................................................
    // Insert user code here. This is where you use the joystick values to
    // update your motors, etc.
    // ........................................................................

    // Drivetrain movement
      // get joystick values
    int leftY = 6 * Controller.Axis1.position(); // multiply percent by 6 to get rpm, Controller.AxisNUMBER.position(); returns numbers -100 to 100
    int rightX = 6 * Controller.Axis3.position();
      // move the left side of the robot
    DrivetrainLeft.spin(forward, leftY - rightX, rpm);
      // move the right side of the robot 
    DrivetrainRight.spin(forward, leftY + rightX, rpm);

    // variable fo button bumper
    bool pressy = Controller.ButtonR1.pressing();
    bool pressylower = Controller.ButtonR2.pressing();
    // set up speed for intake motor
    MotorIntake.setVelocity(200,rpm);
    // if controller bumper is presse motor go spin
    if (pressy == true){
      MotorIntake.spin(reverse);
    } else if (pressylower == true) {
      MotorIntake.spin(forward);
    } else {
      MotorIntake.stop();
    }
    Controller.ButtonY.pressed(autotototot);

    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.

}

//
// Main will set up the competition functions and callbacks.
//
int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
