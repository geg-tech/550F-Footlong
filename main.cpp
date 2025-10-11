/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       550F                                                      */
/*    Created:      10/8/2025, 12:54:28 PM                                    */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here
  // brain
brain Brain;
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
motor MotorIntakeMain = motor(PORT1, ratio18_1);
motor MotorIntakeTop = motor(PORT8, ratio18_1);
    // motor groups
motor_group DrivetrainLeft = motor_group(MotorL1, MotorL2, MotorL3);
motor_group DrivetrainRight = motor_group(MotorR1, MotorR2, MotorR3);
motor_group BothIntakes = motor_group(MotorIntakeMain, MotorIntakeTop);
  // define drivetrain object
drivetrain Drivetrain = drivetrain(DrivetrainLeft, DrivetrainRight, 12.5, 14, 14, inches, 0.75);

// pneumatic class.
digital_out pneumaticMain = digital_out(Brain.ThreeWirePort.A); 
digital_out pneumaticFront = digital_out(Brain.ThreeWirePort.B);

// NEW: Global boolean to track the state of the middle pneumatic.
bool isMiddlePneumaticExtended = false;
// NEW: Global boolean for the latch to handle button presses.
bool middleButtonLatch = false;


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

  pneumaticMain.set(false);
  pneumaticFront.set(false);

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
//Define auton (Farton)
  Drivetrain.driveFor(42, inches, 150, rpm);
  Drivetrain.turnFor(right,52, degrees); //SWITCH TO LEFT FOR LEFT AUTON
  Drivetrain.driveFor(-25, inches, 100, rpm);
  BothIntakes.spinFor(reverse, 5, sec, 200, rpm);


  //MotorIntakeMain.spinFor(forward, 5, sec);
  //Drivetrain.stop();
  // ..........................................................................
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void usercontrol(void) {
  // User control code here, inside the loop
  while (1) {
    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo
    // values based on feedback from the joysticks.
    
    // ........................................................................
        // Drivetrain movement
      // get joystick values
    int leftY = 6 * Controller.Axis1.position(); // multiply percent by 6 to get rpm, Controller.AxisNUMBER.position(); returns numbers -100 to 100
    int rightX = 6 * Controller.Axis3.position();
      // move the left side of the robot
    DrivetrainLeft.spin(forward, leftY + rightX, rpm);

      // move the right side of the robot 
    DrivetrainRight.spin(forward, rightX - leftY, rpm);

    bool Frontpneumatic = Controller.ButtonX.pressing();
    
    if (Frontpneumatic == true){
      pneumaticFront.set(true);
    } else {
      pneumaticFront.set(false);
    }

    // NEW: Toggle logic for the middle pneumatic
    if (Controller.ButtonY.pressing() && !middleButtonLatch) {
      isMiddlePneumaticExtended = !isMiddlePneumaticExtended;
      middleButtonLatch = true; // Set the latch
    } else if (!Controller.ButtonY.pressing()) {
      middleButtonLatch = false; // Release the latch
    }
    pneumaticMain.set(isMiddlePneumaticExtended);


    // bool for both intakes
    bool pressy = Controller.ButtonR1.pressing();
    bool pressylower = Controller.ButtonR2.pressing();
    // set up speed for intake motors
    BothIntakes.setVelocity(200,rpm);
    // if controller button R1 is presse motors spin reversed, if R2 is pressed motors spin out, if else stop
    if (pressy == true){
      BothIntakes.spin(reverse);
    } else if (pressylower == true) {
      BothIntakes.spin(forward);
    } else {
      BothIntakes.stop();
    }

    // variable fo Main intake
    bool Verypressed = Controller.ButtonL1.pressing();
    bool NotVerypressed = Controller.ButtonL2.pressing();
    // set up speed for main intake motor
    MotorIntakeMain.setVelocity(200,rpm);
    // if controller bumper is presse motor go spin
    if (Verypressed == true){
      MotorIntakeMain.spin(reverse);
    } else if (NotVerypressed == true){
      MotorIntakeMain.spin(forward);
    } else {
      MotorIntakeMain.stop();
    }
    
    // ........................................................................

    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
  }
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
