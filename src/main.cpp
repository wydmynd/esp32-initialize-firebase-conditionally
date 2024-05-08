#include <Arduino.h>
/**
 * Based on code created by K. Suwatchai (Mobizt)
 * https://github.com/mobizt/Firebase-ESP-Client/blob/main/examples/RTDB/DataChangesListener/Callback/Callback.ino
 */


#include <WiFi.h>

#include "secrets.h"

#include <Firebase_ESP_Client.h>

// Provide the token generation process info.
#include <addons/TokenHelper.h>

// Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

// Define Firebase Data object
FirebaseData stream;
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;

int count = 0;

volatile bool dataChanged = false;

void setup()
{

  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the user sign in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  // To connect without auth in Test Mode, see Authentications/TestMode/TestMode.ino

  Firebase.begin(&config, &auth);

  Firebase.reconnectWiFi(true);

}

void loop()
{

  Firebase.ready(); // should be called repeatedly to handle authentication tasks.

  // This is a demo of POSTING data to RTDB using setJSON

  if (Firebase.ready() && (millis() - sendDataPrevMillis > 10000 || sendDataPrevMillis == 0))
  {
    Serial.printf("Available heap: %d bytes\n", ESP.getFreeHeap());
    sendDataPrevMillis = millis();
    count++; 
    FirebaseJson json;
    json.add("data", "hello");
    json.add("num", count);
    Serial.printf("Set json... %s\n\n", Firebase.RTDB.setJSON(&fbdo, "/counter/data/json", &json) ? "ok" : fbdo.errorReason().c_str());
  }

}
