# ESP32CAR
This is a very basic code of a 4WD RC car with ESP32 
Just install the ESP32 board in the arduino IDE 
Make connections to the 74HC595 shift register whose outputs are connected to the MOTOR inputs of the 2 motor Drivers(You can use a single motor driver at your own risk)
Make connections to the enable pins of the motor drivers
Pin numbers are:  
  74hc595 inputs:  
    data - 27  
    clk - 26  
    latch - 25  
  motor driver enable pins(PWM):  
    driver    (en1, en2)  
    driver 1 : 13, 4   
    driver 2: 33, 32  (interchange them as per you connections)  
    
    
    
   
