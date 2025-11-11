int buzzerPin = 4;
int CORE_ID = 0;

void buzzerTask(void *pvParameters) {
  pinMode(buzzerPin, OUTPUT);
  while (1) {
    tone(buzzerPin, 1000); // bunyi 1kHz
    vTaskDelay(500 / portTICK_PERIOD_MS);
    noTone(buzzerPin);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void setup() {
  xTaskCreatePinnedToCore(buzzerTask, "Buzzer Task", 2048, NULL, 1, NULL, CORE_ID); // Core 1
}

void loop() {}
