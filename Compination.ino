#include <WiFi.h>
#include <MQUnifiedsensor.h>
#include "MQ135m.h"
#include "DHT.h"
#include <HTTPClient.h>
#include "ThingSpeak.h"

#define Board "ESP32"
#define Pin_mq4 35

#define Type_mq4 "MQ-4"//MQ4
#define Voltage_Resolution 5
#define ADC_Bit_Resolution 12 
#define RatioMQ4CleanAir 4.4 //RS / R0 = 60 ppm 

#define esp32_sample_rate 4096
#define mq135_pin 34

#define DHTPIN 14     
#define DHTTYPE DHT22


DHT dht(DHTPIN, DHTTYPE);
MQUnifiedsensor MQ4(Board, Voltage_Resolution, ADC_Bit_Resolution, Pin_mq4, Type_mq4);
int test = 0;
float CO2;
float CH4;
float temp;
float hum;

unsigned long myChannelNumber = 1;
const char * myWriteAPIKey = "6Y3ZVQVGBXUK4XEW";

unsigned long lastTime = 0;
unsigned long timerDelay = 30000;

const char *ssid = "ELMEKAWY 1624";      // replace with your wifi ssid and wpa2 key
const char *pass = "C2w8815@";
HTTPClient http;
WiFiClient  client;

void setup() {

  Serial.begin(9600);
  dht.begin();

  WiFi.mode(WIFI_STA);

  ThingSpeak.begin(client);

  //**************************************
   //MQ135 *********************************
  int avg = 0;
  for(int i=0; i<10;i++){
  avg += analogRead(mq135_pin);
  Serial.print(".");
  delay(2000);
  }
  avg = avg/10;
  MQ135_init(esp32_sample_rate,avg);
  //**************************************

  //MQ4 **********************************
  MQ4.setRegressionMethod(1); //_PPM =  a*ratio^b
  MQ4.setA(1012.7); MQ4.setB(-2.786); // Configure the equation to to calculate CH4 concentration
  MQ4.init(); 
  Serial.print("Calibrating please wait.");
float calcR01 = 0;
    for(int i = 1; i<=10; i ++){
      MQ4.update(); // Update data, the arduino will be read the voltage on the analog pin
      calcR01 += MQ4.calibrate(RatioMQ4CleanAir);
      Serial.print(".");
    }
    MQ4.setR0(calcR01/10);
    Serial.println("  done!.");
    
    if(isinf(calcR01)) {Serial.println("Warning: Conection issue founded, R0 is infite (Open circuit detected) please check your wiring and supply"); while(1);}
    if(calcR01 == 0){Serial.println("Warning: Conection issue founded, R0 is zero (Analog pin with short circuit to ground) please check your wiring and supply"); while(1);}
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {

    //WIFI **********************************
    if(WiFi.status() != WL_CONNECTED){
      Serial.print("Attempting to connect");
      while(WiFi.status() != WL_CONNECTED){
        WiFi.begin(ssid, pass); 
        delay(5000);     
      } 
      Serial.println("\nConnected.");
    }
  
    //MQ135 ********************************
    int adc = 0;
    for(int i=0; i<8;i++){
    adc += analogRead(mq135_pin);
    }
    adc = adc/8;
    CO2 = read_CO2(esp32_sample_rate,adc);
    Serial.print("CO2_PPM:  ");
    Serial.println(CO2);
  

    // *************************************

    //MQ4 **********************************
    CH4=0;
    int avg = 0;
    for(int i=0; i<3;i++){
      MQ4.update(); // Update data, the arduino will read the voltage from the analog pin
      avg += MQ4.readSensor(); 
      delay(20);
    }
    CH4 = avg/10;
    Serial.print("CH4_PPM:  ");
    Serial.println(CH4);  
    //**************************************
    
    //DHT22 ********************************
    hum =  dht.readHumidity();   
    temp= dht.readTemperature();   
    Serial.print("temp:");
    Serial.println(temp);
    Serial.print("hum:");
    Serial.println(hum);
    Serial.println("*****           *****");  
  

  //Sending Data *****************************
    //Check WiFi connection status
    String URL = "https://gwi12327.000webhostapp.com/database.php?request_type=add&temp="+String(temp)+"&hum="+String(hum)+"&co2="+String(CO2)+"&ch4="+String(CH4);
    if (WiFi.status()== WL_CONNECTED) {
      bool http_begin = http.begin(URL);
      String payload_request = "";//Combine the name and value
      http.addHeader("Content-Type", "application/x-www-form-uU rlencoded");
      int httpResponseCode = http.sendRequest("GET", payload_request);
      String payload_response = http.getString();
      Serial.println(payload_response);
    }

  //Graphs *****************************
    // Write to ThingSpeak 
    ThingSpeak.setField(1, temp);
    ThingSpeak.setField(2, hum);
    ThingSpeak.setField(3, CO2);
    ThingSpeak.setField(4, CH4);
    
    int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

    if(x == 200){
      Serial.println("Channel update successful.");
    }
    else{
      Serial.println("Problem updating channel. HTTP error code " + String(x));
    }
    Serial.println("................................................................................");
    lastTime = millis();
  }

}
