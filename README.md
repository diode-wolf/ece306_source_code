# ECE 306 Project Code
### Source code for upgraded course project

**Overview**

NC State's Intro to Embedded Systems course (ECE 306) requires students to build a remote controlled toy car over the course of the semester. The car is based around Texas Insturment's MSP430 FR2355
microcontroller. Students write bare metal code using an infinite while loop plus interrupts (foreground-background) to handle scheduling. The car is controlled by sending messages via wifi to a tcp
server running on an ESP32. The ESP32 parses the incoming TCP packets and outputs received messages via UART. The MSP430 reads the UART messages and moves the car accordingly.

I completed all parts of the course project, but by the end of the semester my code was a disorganized pile of solutions to the subprojects we had been working on all semester. I wanted to
refactor it and investigate possible improvements and so the following semester I rewrote the majority of the project from scratch. I removed many of the global variables, removed deeply nested
function calls, and implemented a simple run to completion scheduler. I also changed the UI so that the car is now controlled by tilting one's phone rather than having to push buttons for specific 
movements. That rewritten code is what I have uploaded here. 


**Known Issues**
- There are still many global variables
- Header files are disorganized (eg. macro in init.h used in much more than init.c)
- SPI implementation for LCD could be improved (still same as provided code)
- Movement calculation is overly sensitive in phone x axis
- MSP430 plus external ESP32 is not the ideal way to accomplish the goals of this course project.

**Files**
- adc_isrs.c: Contains the analog to digital conversion interrupt service routines
- init.c: Contains initalization code
- init_clocks.c: Contains code for initalizing clock sources. Provided by professor. Should be combined with init.c
- LCD.c: Code for interfacing with the LCD display via spi
- main.c: Creates instances of each task and contains entry point for RTCS scheduler
- menu.c: Creates a menu on the LCD display allowing the user to select options and connect to wifi
- movement.c: Calculates vehicle movements from messages received over tcp server on ESP32
- ports.c: Initalizes port periphials on the MSP340
- rtcs.c: Contains the code for running the run to completion scheduler
- serial.c: Contains the UART port interrupt service routines
- switch_isrs.c: Contains the interrupt service routines for reading and debouncing switch presses
- system.c: Code provided by professor. At this point it only enables interrupts. Should be combined with init.c
- tasks.c: Contains the code for all tasks run by the scheduler. Somewhat disorganized. (Need to clean up header files or split this file up)
- timers_isrs.c: Timer periphial interrupt service routines.

