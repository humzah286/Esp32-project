//#include <WiFi.h>
#include <FirebaseESP32.h>

#include "esp_camera.h"
#include "fd_forward.h"
#include "fr_forward.h"

// MicroSD Libraries
#include "FS.h"
#include "SD_MMC.h"

// EEPROM Library
#include "EEPROM.h"

#include <WiFi.h>
#include <FirebaseESP32.h>

// Provide the token generation process info.
//#include <addons/TokenHelper.h>

// Provide the RTDB payload printing info and other helper functions.
//#include <addons/RTDBHelper.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "Humzah_2.4"
#define WIFI_PASSWORD "rehan1234"

int touchpin = 2;
int val = 0;
bool bool1;
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

#define EEPROM_SIZE 1

// Counter for picture number
unsigned int pictureCount = 0;

void initMicroSDCard() {
  // Start the MicroSD card

  Serial.println("Mounting MicroSD Card");
  if (!SD_MMC.begin()) {
    Serial.println("MicroSD Card Mount Failed");
    return;
  }
  uint8_t cardType = SD_MMC.cardType();
  if (cardType == CARD_NONE) {
    Serial.println("No MicroSD Card found");
    return;
  }

}

#define ENROLL_CONFIRM_TIMES 5

#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

face_id_list id_list = {0};

bool initCamera() {

  camera_config_t config;

  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_QVGA;
  config.jpeg_quality = 10;
  config.fb_count = 1;

  esp_err_t result = esp_camera_init(&config);

  if (result != ESP_OK) {
    return false;
  }

  return true;
}

mtmn_config_t mtmn_config = {0};
int detections = 0;

void read_face_id_list_sdcard(face_id_list *l, const char *path) {
  face_id_init(&id_list, 7, 5);
  if (!SD_MMC.exists(path)) {
    Serial.println("file doesnt exists");
    return;
  }
  File file = SD_MMC.open(path, FILE_READ);
  l->head =   file.read();
  l->tail =   file.read();
  l->count =  file.read();
  l->size =   file.read();
  l->confirm_times = file.read();
  dl_matrix3d_t **list = l->id_list;
  const int block_len = FACE_ID_SIZE * sizeof(float);
  for (uint8_t i = 0; i < l->count; i++)
  {
    list[i] = dl_matrix3d_alloc(1, 1, 1, FACE_ID_SIZE);
    file.read((uint8_t*)list[i]->item, block_len);
  }
  file.close();
}

void write_face_id_list_sdcard(face_id_list *l, const char* path) {
  File file = SD_MMC.open(path, FILE_WRITE);
  file.write(l->head);
  file.write(l->tail);
  file.write(l->count);
  file.write(l->size);
  file.write(l->confirm_times);
  const int block_len = FACE_ID_SIZE * sizeof(float);
  for (uint8_t i = 0; i < l->count; i++) {
    file.write((uint8_t*)l->id_list[i]->item, block_len);
  }
  file.close();
}

void setup() {

  bool1 = false;
  pinMode(touchpin, INPUT);

  initMicroSDCard();
  Serial.begin(115200);

  if (!initCamera()) {

    Serial.printf("Failed to initialize camera...");
    return;
  }

  mtmn_config = mtmn_init_config();

  //  face_id_init(&id_list, 7, 5);
  read_face_id_list_sdcard(&id_list, "/file.dat");

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

void loop() {

  //if button touched or enroll == 1 in firebase
  val = digitalRead(touchpin);
  if (!Firebase.getBool(fbdo, F("/Control/enroll"), &bool1))
    Serial.printf(fbdo.errorReason().c_str());

  if (val == 1 || bool1) {

    camera_fb_t * frame;
    frame = esp_camera_fb_get();

    dl_matrix3du_t *image_matrix = dl_matrix3du_alloc(1, frame->width, frame->height, 3);
    bool s = fmt2rgb888(frame->buf, frame->len, frame->format, image_matrix->item);

    esp_camera_fb_return(frame);

    if (!s) {
      dl_matrix3du_free(image_matrix);
      Serial.println("to rgb888 failed");
      Serial.printf("%s", ESP_FAIL);
    }

    box_array_t *boxes = face_detect(image_matrix, &mtmn_config);

    if (boxes != NULL) {
      detections = detections + 1;
      //    Serial.printf("Faces detected %d times \n", detections);

      //    Serial.printf("Enrolling face");
      enroll_face00(image_matrix, boxes);

      free(boxes->score);
      free(boxes->box);
      free(boxes->landmark);
      free(boxes);
    }

    dl_matrix3du_free(image_matrix);
  }
}

void enroll_face00(dl_matrix3du_t *image_matrix, box_array_t *boxes) {
  dl_matrix3du_t *aligned_face = NULL;

  int matched_id;

  aligned_face = dl_matrix3du_alloc(1, FACE_WIDTH, FACE_HEIGHT, 3);   //matrix only for face
  if (!aligned_face) {
    Serial.println("Could not allocate face recognition buffer");
    //        return matched_id;
  }


  if (align_face(boxes, image_matrix, aligned_face) == ESP_OK) {

    matched_id = recognize_face(&id_list, aligned_face);

    //      Serial.printf("matched_id =

    if (matched_id < 0 && bool1) {

      int8_t left_sample_face = enroll_face(&id_list, aligned_face);  //enrolls face on last element of id_list


      if (left_sample_face == (ENROLL_CONFIRM_TIMES - 1)) {
        Serial.printf("Enrolling Face ID: %d\n", id_list.tail);
      }
      Serial.printf("Enrolling Face ID: %d sample %d\n", id_list.tail, ENROLL_CONFIRM_TIMES - left_sample_face);
      //            rgb_printf(image_matrix, FACE_COLOR_CYAN, "ID[%u] Sample[%u]", id_list.tail, ENROLL_CONFIRM_TIMES - left_sample_face);
      if (left_sample_face == 0) {
        //                is_enrolling = 0;
        Serial.printf("Enrolled Face ID: %d\n", id_list.tail);

        write_face_id_list_sdcard(&id_list, "/file.dat");

        if (!Firebase.getString(fbdo, F("/Control/name_tmp")))
        Serial.printf(fbdo.errorReason().c_str());

        const char* t = fbdo.to<const char *>();

        if (!Firebase.setInt(fbdo, F("/Control/matched_id"), id_list.tail - 1)) 
        Serial.printf(fbdo.errorReason().c_str());

        if (!Firebase.setBool(fbdo, F("/Control/enroll"), false == 0))
        Serial.printf(fbdo.errorReason().c_str());
      }
    }
    else if (matched_id > -1) {
      Serial.printf("Face matched, matched id = %d\n" , matched_id);

      //      Firebase.getBool(fbdo, F("/Control/enroll"), &bool1)

      if (Firebase.setInt(fbdo, F("/Control/matched_id"), matched_id)) {
        Serial.printf(fbdo.errorReason().c_str());
      }
    }

  }
  else {
    Serial.println("Face Not Aligned");
    //rgb_print(image_matrix, FACE_COLOR_YELLOW, "Human Detected");
  }
  dl_matrix3du_free(aligned_face);
}
