#include <esp_now.h>
#include <WiFi.h>


uint8_t receiverAddress[] = {0xB0, 0xB2, 0x1C, 0xF8, 0x51, 0x18}; // indirizzo MAC 
const int motionSensor = 27; // PIR Motion Sensor
bool motionDetected = false;
// PMK and LMK keys
static const char* PMK_KEY_STR = "16_bytes_pmkey_here";
static const char* LMK_KEY_STR = "16_bytes_lmkey_here";

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
    String x;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// Counter variable to keep track of number of sent packets
void IRAM_ATTR detectsMovement() {
  //Serial.println("MOTION DETECTED!!!");
  motionDetected = true;
}
esp_now_peer_info_t peerInfo;
// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
  pinMode(motionSensor, INPUT_PULLUP);
  // Set motionSensor pin as interrupt, assign interrupt function and set RISING mode
  attachInterrupt(digitalPinToInterrupt(motionSensor), detectsMovement, RISING);


  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  
  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("There was an error initializing ESP-NOW");
    return;
  }
  
  // Set PMK key
  esp_now_set_pmk((uint8_t *)PMK_KEY_STR);
  
  // Register the receiver board as peer
  
  memcpy(peerInfo.peer_addr, receiverAddress, 6);
  peerInfo.channel = 0;
  //Set the receiver device LMK key
  for (uint8_t i = 0; i < 16; i++) {
    peerInfo.lmk[i] = LMK_KEY_STR[i];
  }
  // Set encryption to true
  peerInfo.encrypt = true;
  
  // Add receiver as peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of transmitted packet
  esp_now_register_send_cb(OnDataSent);
}
void loop() {
  static unsigned long lastEventTime = millis();
  static const unsigned long EVENT_INTERVAL_MS = 500;
  if ((millis() - lastEventTime) > EVENT_INTERVAL_MS) {
    if(motionDetected){
    //bot.sendMessage(CHAT_ID, "Motion detected!!", "");
    Serial.println("Rilevato movimento");
    motionDetected = false;
    myData.x = "Movimento";
  }
  else {
    myData.x = "";
  }
    lastEventTime = millis();
    
    // Set values to send
    
    
  
    // Send message via ESP-NOW
    esp_err_t result = esp_now_send(0, (uint8_t *) &myData, sizeof(myData));
    /*if (result == ESP_OK) {
      Serial.println("Sent with success");
    }
    else {
      Serial.println("Error sending the data");
    }  */
  }
  delay(10000);
}
