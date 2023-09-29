# Traffic_Light_Controller
## Hardware  Used :
1. TM4C123 Tiva C Series Launchpad.
2. Resistors of 1.5k
3. 3 Switches
4. Red, Yellow, Green LEDs

## Software Used :
[ARM Keil MDK](https://developer.arm.com/Tools%20and%20Software/Keil%20MDK)

## Working
Designed a traffic light controller for the intersection of two equally busy one-way streets to maximize traffic flow, minimize waiting time at a red light, and avoid accidents.
In this Moore FSM in such a way so the light pattern defines which road has right of way over the other.
Since an output pattern to the lights is necessary to remain in a state for 30s, 
It have two inputs (car sensors on North(N) and East(E) roads) and six outputs (one for each light in the traffic signal. and 2 for walk signal)

 ## Deployment 
First of all Make a new project and add Startup.s file to your project or the startupfile provided by your tool for yours praticular processor Model. Now add .c file , .c file consists of main C code of your Machine now compile and debug your code using your Embedded Designing Tool. After Succesful Debugging Burn the code onto your Machine and your Prototype is ready to use

## Video
 [Simulation](https://www.youtube.com/watch?v=zHNMw0erDD4&list=PLmg5qlXcbaf9ngrt8nHffRMJF5z8Mb-8w&index=17)
 
 [Hardware](https://www.youtube.com/watch?v=Yc7omlhZpj8&list=PLmg5qlXcbaf9ngrt8nHffRMJF5z8Mb-8w&index=16)
      
