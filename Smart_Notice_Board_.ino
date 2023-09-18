#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
LiquidCrystal_I2C lcd(0x3F, 16, 2);
AsyncWebServer server(80);
 
const char* ssid = "shahadat";  
const char* password = "aaaaaaaa";   
String message;
String inputParam;
const char* PARAM_INPUT_1 = "input1";
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>Smart Notice Board</title>
  <meta name="viewport" content="width=device-width, initial-scale=5">
<p> <font size="10" face="sans-serif"> <marquee> Smart Notice Board By Team C </marquee> </font> </p>
  </head><body><center>
  <form action="/get">
    Enter the Text to Display: <input type="text" name="input1">
    <input type="submit" value="Send">

  </form><br>
</center></body></html>)rawliteral";
void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}
int ldrPin = D0;      
int ledPin = D5;    

void setup()
{
   pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
 Wire.begin();
 lcd.begin();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(4,0);
  lcd.print("Team C -> Shahadat,Sweety,Shakil,Supria,Abir,Jony");

  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed!");
    return;
  }
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
 
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });
 
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    
    if (request->hasParam(PARAM_INPUT_1)) {
      message = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;
       lcd.clear();
       lcd.setCursor(0,0);
       lcd.print(message);
    }
    else {
      message = "No message sent";
      inputParam = "none";
    }
    Serial.println(message);
   
  request->send(200, "text/html", index_html);
  });
  server.onNotFound(notFound);
  server.begin();
}

void loop()
{
    int ldrValue = analogRead(ldrPin);  // Read LDR value
  //Serial.println(ldrValue);           // Print LDR value to serial monitor
  
  if (ldrValue > 500) {               // If it's day time (LDR value is high)
    digitalWrite(ledPin, HIGH);       // Turn on LED
  } else {                            // If it's night time (LDR value is low)
    digitalWrite(ledPin, LOW);        // Turn off LED
  }    
    for (int positionCounter = 0; positionCounter < 29; positionCounter++) {
    lcd.scrollDisplayLeft();

  }
}
