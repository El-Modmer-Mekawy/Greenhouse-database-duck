#include "MQ135m.h"
//Include the library
#include <MQUnifiedsensor.h>
/************************Hardware Related Macros************************************/
#define         Board                   ("Arduino UNO")
#define         Pin                     (35)  //Analog input 4 of your arduino
/***********************Software Related Macros************************************/
#define         Type                    ("MQ-4") //MQ4
#define         Voltage_Resolution      (5)
#define         ADC_Bit_Resolution      (12) // For arduino UNO/MEGA/NANO
#define         RatioMQ4CleanAir        (4.4) //RS / R0 = 60 ppm 
/*****************************Globals***********************************************/
//Declare Sensor

#define esp32_sample_rate 4096
#define esp32_pin_no 34

void setup() {
  Serial.begin(115200);
  //MQ135 *********************************
  int avg = 0;
  for(int i=0; i<10;i++){
  avg += analogRead(esp32_pin_no);
  delay(2000);
  }
  avg = avg/10;
  MQ135_init(esp32_sample_rate,avg);
  //**************************************


}


void loop() {
  //MQ135 ********************************
  int adc = 0;
  for(int i=0; i<5;i++){
  adc += analogRead(esp32_pin_no);
  }
  adc = adc/5;
  float CO2 = read_CO2(esp32_sample_rate,adc);
  float NH4 = read_NH4(esp32_sample_rate,adc);

  Serial.print("CO2_PPM:  ");
  Serial.println(CO2);
  Serial.print("NH4_PPM:  ");
  Serial.println(NH4);

  delay(1000);

}
