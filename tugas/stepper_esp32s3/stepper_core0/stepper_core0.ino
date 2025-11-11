#include <Stepper.h>
int CORE_ID = 0;

const int stepsPerRevolution = 200;
Stepper myStepper(stepsPerRevolution, 12, 13, 14, 15);

void stepperTask(void *pvParameters) {
  myStepper.setSpeed(60);
  while (1) {
    myStepper.step(stepsPerRevolution);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    myStepper.step(-stepsPerRevolution);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void setup() {
  xTaskCreatePinnedToCore(stepperTask, "Stepper Task", 4096, NULL, 1, NULL, CORE_ID);
}

void loop() {}
