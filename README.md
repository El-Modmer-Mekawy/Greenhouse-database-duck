# Global warming Inspectorem
This res contains all the code for the GWI.
You can find more info by visting our website: https://gwi12327.000webhostapp.com/ 
# Background:
With the rapid increase of global warming mainly due to the increase in greenhouse gases, it became important to study and monitor those increases and the relation between them. We - a team of three seniors - decided to tackle this problem by building an automated IoT system that can collect data about the temperature, humidity, and CO2, store it and methane and display it live for small regions. We believe that this system could help scientists monitor global warming in small areas and use the data to improve scientific research towards solving global warming. 
# How to use:
1. Download this [file](https://gwi12327.000webhostapp.com/12327%20Portfolio.pdf) to learn more about the project.
2. Download the files as zip file and extract them as they are.
3. Open Compination.ino and download the libraries found at the first of the code.
4. Follow the code comments and upload it to your ESP32.
5. To use thingspeak go to the [website](https://thingspeak.com/) make and account and follow the instructions in the code and on the website.
**6. You can use this section to upload the data to your database:**
```  
String URL = "(Change this to your datatbase loaction)?request_type=add&temp="+String(temp)+"&hum="+String(hum)+"&co2="+String(CO2)+"&ch4="+String(CH4);
    if (WiFi.status()== WL_CONNECTED) {
      bool http_begin = http.begin(URL);
      String payload_request = "";//Combine the name and value
      http.addHeader("Content-Type", "application/x-www-form-uU rlencoded");
      int httpResponseCode = http.sendRequest("GET", payload_request);
      String payload_response = http.getString();
      Serial.println(payload_response);
    }
```
If you need anything feel free to contact me at: mekyyousef@gmail.com 
