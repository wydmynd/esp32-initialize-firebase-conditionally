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

#define TRIGGER_PIN 13

// Define Firebase Data object
FirebaseData *stream=NULL;
FirebaseData *fbdo=NULL;

FirebaseAuth *auth=NULL;
FirebaseConfig *config=NULL;

unsigned long sendDataPrevMillis = 0;

int count = 0;

volatile bool dataChanged = false;

bool config_mode = false;

void setup()
{
  pinMode(TRIGGER_PIN, INPUT_PULLUP);

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

  // To connect without auth in Test Mode, see Authentications/TestMode/TestMode.ino
  if (digitalRead(TRIGGER_PIN) == LOW)
  {
    Serial.println("Config mode");
    config_mode = true;
  }
  else
  {
    Serial.println("Normal mode");

    /* Assign the api key (required) */
    config = new FirebaseConfig();
    config->api_key = API_KEY;

    /* Assign the user sign in credentials */
    auth = new FirebaseAuth();
    auth->user.email = USER_EMAIL;
    auth->user.password = USER_PASSWORD;

    /* Assign the RTDB URL (required) */
    config->database_url = DATABASE_URL;

    /* Assign the Firebase objects */
    stream = new FirebaseData();
    fbdo = new FirebaseData();

    Firebase.begin(config, auth);
    Firebase.reconnectWiFi(true);
  }
}


void loop()
{

  // This is a demo of POSTING data to RTDB using setJSON
  if(!config_mode) 
  {
    Firebase.ready(); // should be called repeatedly to handle authentication tasks.

    if (Firebase.ready() && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0))
    {
      Serial.printf("Available heap: %d bytes\n", ESP.getFreeHeap());
      sendDataPrevMillis = millis();
      count++; 
      FirebaseJson json;
      json.add("data", "hello");
      json.add("num", count);
      Serial.printf("Set json... %s\n\n", Firebase.RTDB.setJSON(fbdo, "/counter/data/json", &json) ? "ok" : fbdo->errorReason().c_str());
    }
  } 
  else 
  {
    Serial.println("Config mode running");
    Serial.printf("Available heap: %d bytes\n", ESP.getFreeHeap());
    delay(1000);
  }

}
