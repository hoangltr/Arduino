#include <DHT.h>
#include <FirebaseESP8266.h>
#include <ESP8266WiFi.h>
#include <Servo.h>
//Define for Firebase connection
#define FIREBASE_HOST "iotlab-be8c9-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "LaEmiPF5Bxrj6AHOHPJT2a0xICCNoUFlcYyirr9U"
#define WIFI_SSID "DEP LA" // Thay đổi tên wifi của bạn
#define WIFI_PASSWORD "999999999" // Thay đổi password wifi của bạn
//ESP8266 Pin
#define DHTTYPE DHT11 //Declare sensor type
#define DHTPIN 5  //D1 of ESP8266 
#define LED 12    //D6 of ESP8266
#define Fan 14    //D5 of ESP8266

//Declare myservo object in global scope
Servo myservo;

//Declare fbdo object in global scope
FirebaseData fbdo;

int motor_signal = D2;

float led;

float fan;

DHT dht(DHTPIN, DHTTYPE);
String path = "/";

void setup() {
  //Print data to screen monitor
  Serial.begin(115200);
  //Define LED pin and FAN pin as output pin
  pinMode(LED, OUTPUT);
  pinMode(Fan, OUTPUT);
  delay(1000);
  //Connect to Wifi
  WiFi.begin (WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Dang ket noi");
  while (WiFi.status() != WL_CONNECTED) { 
    Serial.print(".");
    delay(500);
  }
  Serial.println ("");
  Serial.println ("Da ket noi WiFi!");
  Serial.println(WiFi.localIP());
  //Firebase connection
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  //Attach servo signal
  myservo.attach(motor_signal);
  //Sensor DHT11 start
  dht.begin(); 
}

void loop() {

  //Sensor parameter datatype
  float h = dht.readHumidity(); 
  float t = dht.readTemperature();  
  float value = analogRead(A0); 
  //Send data to Firebase
  Firebase.setFloat(fbdo,"thongso/gas", value);
  Firebase.setFloat(fbdo,"thongso/nhietdo", t);  
  Firebase.setFloat (fbdo,"thongso/doam", h);
  //Get data from Firebase
  Firebase.getFloat (fbdo,"thietbi1/Fan");
  fan = fbdo.floatData();

  Firebase.getFloat (fbdo,"thietbi2/Led");
  led = fbdo.floatData();

  Firebase.getFloat(fbdo,"thietbi3/Motor");
  motor_signal = fbdo.floatData();
  //Print to Serial Monitor
  //Gas parameter
  Serial.print("Gas: ");
  Serial.print(value);
  Serial.print("PPM ");
  //Temperature parameter
  Serial.print("Nhiet do: ");
  Serial.print(t);
  Serial.print("*C ");
  //Humidity parameter
  Serial.print("Do am: ");
  Serial.print(h);
  Serial.println("%");
  //Motor parameter
  Serial.print("Motor: ");
  Serial.print(motor_signal);
  Serial.print("  ");
  //LED parameter
  Serial.print("LED: ");
  Serial.print(led);
  Serial.print("  ");
  //FAN parameter
  Serial.print("FAN: ");
  Serial.print(fan);
  Serial.println("  ");
  //LED CONTROL
  if (led == 1)
  {
    digitalWrite(LED, HIGH); //Output LED high level signal
  }
  else 
  {
    digitalWrite(LED, LOW); //Output LED low level signal
  }
  //FAN CONTROL
  if (fan == 1)
  {
    digitalWrite(Fan, HIGH); //Output FAN high level signal
  }
  else 
  {
    digitalWrite(Fan, LOW); //Output FAN low level signal
  }
  delay(500);
  //MOTOR CONTROL
  if(motor_signal == 1){
    for(int angle = 0; angle < 180; angle+=5){     // scan from 0 to 180 degrees                     
      myservo.write(angle);
      delay(5);                       
    } 
    delay(500);
    for(int angle = 180; angle>=1; angle-=5){     // now scan back from 180 to 0 degrees                        
      myservo.write(angle);
      delay(5);                       
    }
  }
}  