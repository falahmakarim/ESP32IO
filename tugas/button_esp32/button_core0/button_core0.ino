#include <Arduino.h>

int BUTTON_PIN = 17;
int CORE_ID = 0;
void taskButton(void *pvParameters) {
  Serial.print("Task Button berjalan di Core: ");
  Serial.println(xPortGetCoreID());

  // Gunakan internal pull-up resistor
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  bool lastState = HIGH;

  for (;;) {
    bool currentState = digitalRead(BUTTON_PIN);
    
    if (currentState == LOW && lastState == HIGH) {
      Serial.println("Tombol Ditekan!");
    }
    
    lastState = currentState;
    vTaskDelay(50 / portTICK_PERIOD_MS); // debounce
  }
}

void setup() {
  Serial.begin(115200);
  xTaskCreatePinnedToCore(taskButton, "Button Task", 2048, NULL, 1, NULL, CORE_ID);
}

void loop() {
  // kosong, semua dijalankan di FreeRTOS task
}
