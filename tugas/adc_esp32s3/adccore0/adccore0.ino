#define POT_PIN 4
#define CORE_ID 0

void taskPotentiometer(void *pvParameters) {
  Serial.print("Task Potentiometer berjalan di Core: ");
  Serial.println(xPortGetCoreID());

  // ADC di ESP32-S3 tidak perlu pinMode
  
  for (;;) {
    int adcValue = analogRead(POT_PIN);
    Serial.printf("Nilai Pot: %d\n", adcValue);
    vTaskDelay(250 / portTICK_PERIOD_MS); // Baca setiap 250ms
  }
}

void setup() {
  Serial.begin(115200);
  xTaskCreatePinnedToCore(taskPotentiometer, "Pot Task", 2048, NULL, 1, NULL, CORE_ID); // Core 0
}

void loop() {}
