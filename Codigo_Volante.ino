
//Si usan una placa de arduino uno cambien la siguiente linea por: #include "UnoJoy.h"
#include "MegaJoy.h"

int encoderI = 2;
int encoderQ = 3;

volatile int count;
volatile int Pulso0;
volatile int Pulso = 0;
int top = 255;
int off = 0;

void setup(){
  
  setupPins();
  setupMegaJoy();

  count=0;
  attachInterrupt(0,handleEncoder, CHANGE);

}

void loop(){
  megaJoyControllerData_t controllerData = getControllerData();
  setControllerData(controllerData);
}


// Esta parte es para configurar el encoder
void handleEncoder(){
  if(digitalRead(encoderI) == digitalRead(encoderQ)){
    count++;
  }else{
    count--;
  }
  Pulso0 = count*0.04;
  Pulso = Pulso0 + 128;
  if (Pulso > top) Pulso = top;
  if (Pulso < off) Pulso = off;
  
}
//Aqui termina la parte del encoder

void setupPins(void){
  // Set all the digital pins as inputs
  // with the pull-up enabled, except for the 
  // two serial line pins
  for (int i = 2; i <= 54; i++){
    pinMode(i, INPUT_PULLUP);
    digitalWrite(i, HIGH);
  }
}

megaJoyControllerData_t getControllerData(void){
  
  // Set up a place for our controller data
  //  Use the getBlankDataForController() function, since
  //  just declaring a fresh dataForController_t tends
  //  to get you one filled with junk from other, random
  //  values that were in those memory locations before
  megaJoyControllerData_t controllerData = getBlankDataForMegaController();
  // Since our buttons are all held high and
  //  pulled low when pressed, we use the "!"
  //  operator to invert the readings from the pins
  for (int i = 4; i < 54; i++){
    controllerData.buttonArray[(i - 2) / 8] |= (!digitalRead(i)) << ((i - 2) % 8);
  }
  
  // Set the analog sticks
  //  Since analogRead(pin) returns a 10 bit value,
  //  we need to perform a bit shift operation to
  //  lose the 2 least significant bits and get an
  //  8 bit number that we can use 
  controllerData.analogAxisArray[0] = Pulso;
  controllerData.analogAxisArray[1] = analogRead(A0);
  controllerData.analogAxisArray[2] = analogRead(A1); 
  controllerData.analogAxisArray[3] = analogRead(A2); 
  controllerData.analogAxisArray[4] = 0; 
  controllerData.analogAxisArray[5] = 0; 
  controllerData.analogAxisArray[6] = 0; 
  controllerData.analogAxisArray[7] = 0; 
  controllerData.analogAxisArray[8] = 0; 
  controllerData.analogAxisArray[9] = 0; 
  controllerData.analogAxisArray[10] = 0; 
  controllerData.analogAxisArray[11] = 0; 
  
  // And return the data!
  return controllerData;
}
