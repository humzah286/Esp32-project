
/**
 * Created by K. Suwatchai (Mobizt)
 *
 * Email: k_suwatchai@hotmail.com
 *
 * Github: https://github.com/mobizt/Firebase-ESP32
 *
 * Copyright (c) 2022 mobizt
 *
 */

#include <WiFi.h>
#include <FirebaseESP32.h>

// Provide the token generation process info.
//#include <addons/TokenHelper.h>

// Provide the RTDB payload printing info and other helper functions.
//#include <addons/RTDBHelper.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "Humzah_2.4"
#define WIFI_PASSWORD "rehan1234"

// For the following credentials, see examples/Authentications/SignInAsUser/EmailPassword/EmailPassword.ino

/* 2. Define the API Key */
#define API_KEY "AIzaSyBQEh3JRtfHdsK0uq6TtyvrL-CTuVjp7Uk"

/* 3. Define the RTDB URL */
#define DATABASE_URL "esp32-firebase-demo-1c538-default-rtdb.europe-west1.firebasedatabase.app" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "humzah286@gmail.com"
#define USER_PASSWORD "28June2002"

// Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;

unsigned long count = 0;

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

  /* Assign the callback function for the long running token generation task */
//  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h

  Firebase.begin(&config, &auth);

  // Comment or pass false value when WiFi reconnection will control by your code or third party library
  Firebase.reconnectWiFi(true);

  Firebase.setDoubleDigits(5);
}

bool bool1;
String str1; 

void loop()
{

  // Firebase.ready() should be called repeatedly to handle authentication tasks.

//  if (Firebase.ready() && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0))
//  {
//    sendDataPrevMillis = millis();

      if (!Firebase.getBool(fbdo, F("/test/bool"), &bool1))
        Serial.printf(fbdo.errorReason().c_str());

      if (!Firebase.getString(fbdo, F("/test/string1")))
        Serial.printf(fbdo.errorReason().c_str());

      Serial.printf("String : %s , bool : %s\n", fbdo.to<const char *>(), bool1 ? "true" : "false");

      

//    Serial.printf("Get bool ref... %s\n", Firebase.getBool(fbdo, F("/test/bool"), &bVal) ? bVal ? "true" : "false" : fbdo.errorReason().c_str());

//    Serial.printf("Set string... %s\n", Firebase.setString(fbdo, F("/test/string"), "writing from esp32 cam") ? "ok" : fbdo.errorReason().c_str());

//    Serial.printf("Get string... %s\n", Firebase.getString(fbdo, F("/test/string")) ? fbdo.to<const char *>() : fbdo.errorReason().c_str());

//    Serial.printf("Set bool... %s\n", Firebase.setBool(fbdo, F("/test/bool"), count % 2 == 0) ? "ok" : fbdo.errorReason().c_str());
//  }
}
