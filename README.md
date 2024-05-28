# MotionGram - IoT Security project.
The master is connected to a PIR Motion Sensor (HC-SR501), as soon as a movement gets detected it notifies the ESP32-CAM. The cam module sends a message and a photo to the specified Telegram user. The user can also request a photo using the /photo command even if there's no movement.

Master (ESP32-WROOM-32) and slave (ESP32-CAM) communicate via [ESP-NOW](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/network/esp_now.html?highlight=esp_now_set_pmk#esp-now). The ESP32-CAM and Telegram communication is secured through TLS.

## How-to
1. First you need to get your devices' MAC addresses:
   ```
   #include "WiFi.h"
   void setup(){
   Serial.begin(115200);
   WiFi.mode(WIFI_MODE_STA);
   Serial.println(WiFi.macAddress());
   } 
   void loop(){}
2. You can generate the keys needed for encrypted ESP-NOW communication using OpenSSL or PGP.
3. Edit master_esp32 with the receiver MAC address and your own keys:
   ```
   uint8_t receiverAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
   static const char* PMK_KEY_STR = "16_bytes_pmkey_here";
   static const char* LMK_KEY_STR = "16_bytes_lmkey_here";
   ```
4. Edit slave_esp32 with the master MAC address and the keys:
    ```
   uint8_t masterMacAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
   static const char* PMK_KEY_STR = "pmkey_used_in_master";
   static const char* LMK_KEY_STR = "lmkey_used_in_master";
   ```
   Then the ESP32_CAM must connect to the internet:
   ```
   const char* ssid = "wifissidhere";
   const char* password = "wifipasswordhere";
   ```
   Before interacting with the Telegram bot you have to create a new bot using [BotFather](@BotFather). It will give you a token in order to access the API. The bot needs your client ID (use [IDBot](@myidbot)), this means that nobody except you can use the bot. You can then edit these lines with your own token and user ID.
   ```
   String BOTtoken = "XXXXXXXXXX:XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
   String CHAT_ID = "XXXXXXXXXX";
   ```
