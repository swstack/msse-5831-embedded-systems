# Lab 2: PD Motor Control

## Experiment with your system to answer the following questions.

* What is the degree accuracy of the encoder?

> Encoder has a resolution of 2248.86 ticks per revolution. So in theory you could turn the motor .00045 of a revolution.

* What is the minimum drive signal required to move your motor from a stopped position?

> Somewhere less than 5% duty cycle.

* What is the approximate frequency of the encoder task (i.e. PCINT ISR) at 25% duty cycle and 75% duty cycle?

**25% Duty Cycle:**

> About 7 ticks of ISR per 5 milliseconds. Which is 1 tick per ~714 microseconds for a frequency of **~1396 hz**.

**75% Duty Cycle:**

> About 21 ticks of ISR per 5 milliseconds. Which is 1 tick per ~238 microseconds for a frequency of **~4201 hz**.

* At what frequency of the PWM signal do you start to notice the “off” and “on” portions of the wave?

> TODO

## Implement the controller to maintain position.

Use the information from the first exercises to set the frequency of the controller. It might be useful to do the following to tune your gains.

* Use only P. Start with a very low gain so that the motor runs very slowly and takes its time getting to the set position.
* Increase the gain to get a fast response (i.e. get to your set point quickly). Then increase it a little more to oscillate.
* Add in D to dampen the response and eliminate oscillation.
* Report your final gain values.

> TODO

## Experiment with the speed and the range of reference position.

* Pick a “low,” “high,” and “ideal” gain based on above experiments.
* Use each gain value to reach a reference position that is 2 full rotations from current position.
* Use each gain value to reach a reference position that is 5 degrees from current position.
* Report on your observations, listing your “low,” “high,” and “ideal” gains.

> TODO

* Now run the same experiments, except slow your controller task frequency to 2 Hz (or 5 Hz if this is too disruptive).
* Report on your observations.

> TODO


## Implement the interpolator and execute the trajectory

Using your tuned gains (i.e. those that achieve good control while maintaining good speed) and the optimal frequency of the controller based on above experiment, implement the interpolator and execute the trajectory:

* rotate the motor forward 90 degrees
* hold for .5 seconds (have the system precisely measure this time period)
* rotate in reverse for 360 degrees
* hold for .5 seconds
* rotate forwards for 5 degrees

## Graph Pm, Pr and T that was recorded while executing the trajectory.

Be sure to graph the entire trajectory. “While” means to record while executing but you don’t need to graph in real-time. You can do a screen dump of the data, then cut and paste into a file or send to a file. Use Excel or Open Office or whatever is your favorite/easiest graphing app to generate the plots.

Execute the same trajectory described above, except run your PD controller at a "slow" rate and at a "very slow" rate while graphing the same variables. Discuss the results.

Explain the observed behavior.
