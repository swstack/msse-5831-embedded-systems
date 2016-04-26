# Lab 2: PD Motor Control

## Experiment with your system to answer the following questions.

**What is the degree accuracy of the encoder?**

* Encoder has a resolution of 2248.86 ticks per revolution. So in theory you could turn the motor .00045 of a revolution.

**What is the minimum drive signal required to move your motor from a stopped position?**

* Somewhere less than 5% duty cycle.

**What is the approximate frequency of the encoder task at 25% duty cycle?**

* About 7 ticks of ISR per 5 milliseconds. Which is 1 tick per ~714 microseconds for a frequency of **~1396 hz**.

**What is the approximate frequency of the encoder task at 75% duty cycle?**

* About 21 ticks of ISR per 5 milliseconds. Which is 1 tick per ~238 microseconds for a frequency of **~4201 hz**.

**At what frequency of the PWM signal do you start to notice the “off” and “on” portions of the wave?**

* Somewhere around around 10 to 20hz.

## Implement the controller to maintain position.

Use the information from the first exercises to set the frequency of the controller.

* At its fastest under no load, the motor will go about 150 RPMs (28 encoder ticks every 5ms). We will start with a control loop frequency of 1KHz (1ms).

Instructions for tuning gains:

Use only P. Start with a very low gain so that the motor runs very slowly and takes its time getting to the set position. Increase the gain to get a fast response (i.e. get to your set point quickly). Then increase it a little more to oscillate. Add in D to dampen the response and eliminate oscillation.

**Report your final gain values.**

* Ideal gains: Kp=60 Kd=30. Oscillation starts to occur above this.

## Experiment with the speed and the range of reference position.

Pick a “low,” “high,” and “ideal” gain based on above experiments. Use each gain value to reach a reference position that is 2 full rotations from current position. Use each gain value to reach a reference position that is 5 degrees from current position.

**Report on your observations, listing your “low,” “high,” and “ideal” gains.**


**Gains Table (Low, ideal, high)**

|       | Kp  | Kd  |
|-------|-----|-----|
| Low   | 10  | 2   |
| Ideal | 60  | 30  |
| High  | 200 | 100 |

**Results: Two full rotations**

* For two full rotations, each gains setting worked noticeably differently. The low gains were full speed for the first rotation and then quickly started dropping speed in the 2nd rotation and it approached the setpoint very slowly. The ideal gains were full speed for the majority of both rotations and almost unnoticeably slowed down when it hit the setpoint, but still no oscillation. The high gains I saw visible oscillation.

**Results: 5 degree setpoint**

* Suprisingly, i could not tell a difference between any of the gains settings for this experiement! Even with the high gains I witness no oscillation.

Now run the same experiments, except slow your controller task frequency to 2 Hz (or 5 Hz if this is too disruptive).

**Report on your observations.**

* When dropping the frequency of the control loop to 2-5hz...the motor basically never reaches it's setpoint and oscillates forever.

## Implement the interpolator and execute the trajectory

Using your tuned gains (i.e. those that achieve good control while maintaining good speed) and the optimal frequency of the controller based on above experiment, implement the interpolator and execute the trajectory:

1. rotate the motor forward 90 degrees
1. hold for .5 seconds (have the system precisely measure this time period)
1. rotate in reverse for 360 degrees
1. hold for .5 seconds
1. rotate forwards for 5 degrees

## Graph Pm, Pr and T that was recorded while executing the trajectory.

Be sure to graph the entire trajectory. “While” means to record while executing but you don’t need to graph in real-time. You can do a screen dump of the data, then cut and paste into a file or send to a file. Use Excel or Open Office or whatever is your favorite/easiest graphing app to generate the plots.

Execute the same trajectory described above, except run your PD controller at a "slow" rate and at a "very slow" rate while graphing the same variables. Discuss the results.

Explain the observed behavior.
