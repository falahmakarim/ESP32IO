#include <U8g2lib.h>
#include <Wire.h>
#include <Arduino.h>

int OLED_SDA = 21;
int OLED_SCL = 20;

// Konstruktor U8g2 (hardware I2C, full buffer)
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, OLED_SCL, OLED_SDA);

// Pilih core: 0 atau 1
int CORE_ID = 0;

void oledTask(void *pvParameters) {
  for (;;) {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB08_tr);

     // Baris pertama
    u8g2.drawStr(0, 12, "Hello ESP32-S3!");

    // Baris kedua (dinamis menampilkan core)
    String msg = "Running on core: " + String(xPortGetCoreID());
    u8g2.drawStr(0, 28, msg.c_str());

    u8g2.sendBuffer();
    delay(1000);
  }
}

void setup() {
  Serial.begin(115200);
  // Inisialisasi I2C
  Wire.begin(OLED_SDA, OLED_SCL);
  u8g2.begin();

  Serial.println("Starting OLED task on core: " + String(CORE_ID));
  Serial.println("Loop running on core: " + String(CORE_ID));


  // Buat task OLED di core yang dipilih
  xTaskCreatePinnedToCore(
    oledTask,        // fungsi task
    "OLEDTask",      // nama task
    5000,            // ukuran stack
    NULL,            // parameter task
    1,               // prioritas
    NULL,            // handle task
    CORE_ID        // core: 0 atau 1
  );
}

void loop() {
}
