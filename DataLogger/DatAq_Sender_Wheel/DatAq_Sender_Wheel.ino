
/*
  Rui Santos & Sara Santos - Random Nerd Tutorials
  Complete project details at https://RandomNerdTutorials.com/esp32-esp-now-wi-fi-web-server/
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*/
#include <esp_now.h>
#include <esp_wifi.h>
#include <WiFi.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM6DSOX.h>

// Set your Board ID (ESP32 Sender #1 = BOARD_ID 1, ESP32 Sender #2 = BOARD_ID 2, etc)
#define BOARD_ID 1

// For Hardware or Software SPI mode, we need a CS pin
#define LSM_CS 9   //3 for Xiao C6, or any digital pin

// For software-SPI mode we need SCK/MOSI/MISO pins
//#define LSM_SCK 13   //8 for Xiao C6
//#define LSM_MISO 12  //9 for Xiao C6
//#define LSM_MOSI 11  //10 for Xiao C6


/*
Wiring for LSM6DSOX: https://learn.adafruit.com/lsm6dsox-and-ism330dhc-6-dof-imu/arduino
SCK = SCL
D0 = MISO  "Microcontroller In Sensor Out"
SDA = MOSI
CS = CS / SS

Connect Vin to the power supply, 3V or 5V is fine. Use the same voltage that the microcontroller logic is based off of
Connect GND to common power/data ground
Connect the SCL pin to Digital #13 but any pin can be used later
Connect the DO pin to Digital #12 but any pin can be used later
Connect the SDA pin to Digital #11 but any pin can be used later
Connect the CS pin Digital #10 but any pin can be used later
*/



//MAC Address of the receiver 
uint8_t broadcastAddress[] = {0x30, 0x83, 0x98, 0x54, 0x09, 0x80};

// Insert your SSID
constexpr char WIFI_SSID[] = "SM-G935W85092";

int32_t getWiFiChannel(const char *ssid) {
  if (int32_t n = WiFi.scanNetworks()) {
      for (uint8_t i=0; i<n; i++) {
          if (!strcmp(ssid, WiFi.SSID(i).c_str())) {
              return WiFi.channel(i);
          }
      }
  }
  return 0;
}

// Create Peer Interface
esp_now_peer_info_t peerInfo;

//Create object for sensor
Adafruit_LSM6DSOX sox;

sensors_event_t a, g;

float acc_x, acc_y, acc_z;
float gyro_x, gyro_y, gyro_z;


/*
// Init LSM6DSOX IMU
void init_sox(){
  if (!sox.begin()) {
    Serial.println("Failed to find LSM6DSOX chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("LSM6DSOX Found!");
}
*/

// Gyro Error (just in case)
float gyro_x_error = 0.07;
float gyro_y_error = 0.07;
float gyro_z_error = 0.07;






//Structure example to send data
//Must match the receiver structure
typedef struct struct_message {
    int id;
    float acc_x;
    float acc_y;
    float acc_z;
    float gyro_x;
    float gyro_y;
    float gyro_z;
    //float mag_x;
    //float mag_y;
    //float mag_z;
    int readingId;
} struct_message;

//Create a struct_message called myData
struct_message myData;



// Timer Variables
unsigned long lastTime = 0;             
unsigned long lastTimeTemperature = 0;
unsigned long lastTimeAcc = 0;
unsigned long gyroDelay = 10;
unsigned long temperatureDelay = 1000;
unsigned long accelerometerDelay = 200;

unsigned long previousMillis = 0;   // Stores last time temperature was published
const long interval = 1000;        // Interval at which to publish sensor readings

unsigned int readingId = 0;         //Reading Id to be increased, starting at "0"



// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
 
void setup() {
  //Init Serial Monitor
  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  //Something about intitializing the sensor

 
  // Set device as a Wi-Fi Station and set channel
  WiFi.mode(WIFI_STA);

  int32_t channel = getWiFiChannel(WIFI_SSID);

  WiFi.printDiag(Serial); // Uncomment to verify channel number before
  esp_wifi_set_promiscuous(true);
  esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
  esp_wifi_set_promiscuous(false);
  WiFi.printDiag(Serial); // Uncomment to verify channel change after

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}
 
void loop() {
  //get gyro and acceleration data every "Interval" time has passed. Interval is set at the top of the code
  unsigned long currentMillis = millis();

   //  /* Get a new normalized sensor event */
  sensors_event_t a;
  sensors_event_t g;
  sensors_event_t t;

  if (currentMillis - previousMillis >= interval) {
    // Save the last time a new reading was published
    previousMillis = currentMillis;
    //Set values to send
    sox.getEvent(&a, &g, &t);
    myData.id = BOARD_ID;
    myData.acc_x = a.acceleration.x;
    myData.acc_y = a.acceleration.y;
    myData.acc_z = a.acceleration.z;
    myData.gyro_x = g.gyro.x;
    myData.gyro_y = g.gyro.y;
    myData.gyro_z = g.gyro.z;
    myData.readingId = readingId++;

    //Send message via ESP-NOW
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
    if (result == ESP_OK) {
      Serial.println("Sent with success");
    }
    else {
      Serial.println("Error sending the data");
    }
  }
}