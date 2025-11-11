int ledPin = 5;  // LED eksternal di GPIO 2
int CORE_ID = 0; // ubah nilainya jadi 0 atau 1

void ledTask(void *pvParameters) {
   pinMode(ledPin, OUTPUT);
  while (1) {
    digitalWrite(ledPin, HIGH);  // LED ON
    vTaskDelay(500 / portTICK_PERIOD_MS);
    digitalWrite(ledPin, LOW);   // LED OFF
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void setup() {
  xTaskCreatePinnedToCore(ledTask, "LED Task", 2048, NULL, 1, NULL, CORE_ID); // Core 0
}

void loop() {}
