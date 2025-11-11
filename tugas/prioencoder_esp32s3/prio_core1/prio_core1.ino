int pinA = 16, pinB = 17;
int counter = 0, lastStateA;
int CORE_ID = 1;

void encoderTask(void *pvParameters) {
  pinMode(pinA, INPUT);
  pinMode(pinB, INPUT);
  lastStateA = digitalRead(pinA);
  while (1) {
    int stateA = digitalRead(pinA);
    if (stateA != lastStateA) {
      if (digitalRead(pinB) != stateA) counter++;
      else counter--;
      Serial.println(counter);
    }
    lastStateA = stateA;
    vTaskDelay(1);
  }
}

void setup() {
  Serial.begin(115200);
  xTaskCreatePinnedToCore(encoderTask, "Encoder Task", 4096, NULL, 1, NULL, CORE_ID);
}

void loop() {}
