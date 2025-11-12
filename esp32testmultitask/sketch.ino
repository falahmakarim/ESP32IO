#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP32Servo.h>

// =================== PIN DEFINITIONS ===================
#define LED1_PIN 2        // LED Wokwi
#define LED2_PIN 2        // LED Wokwi (gunakan satu LED atau sesuaikan)
#define LED3_PIN 39

#define BUZZER_PIN 46

// Stepper 4-wire
#define STEP_A_PLUS 7
#define STEP_A_MINUS 8
#define STEP_B_PLUS 9
#define STEP_B_MINUS 4

#define SERVO_PIN 18
#define BUTTON1_PIN 35
#define BUTTON2_PIN 19
#define POT_PIN 5

// Rotary Encoder
#define CLK_PIN 40
#define DT_PIN 41

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

// Stepper
#define STEPS_PER_REV 200

// =================== GLOBAL OBJECTS ===================
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Servo myServo;
SemaphoreHandle_t i2cMutex;
SemaphoreHandle_t setupCompleteSemaphore;

volatile int potValue = 0;
volatile bool button1Pressed = false;
volatile bool button2Pressed = false;
volatile int lastStateCLK = LOW;
volatile int direction = 0;

// =================== LED TASK ===================
void ledBlinkTask(void * pvParameters) {
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);

  for (;;) {
    digitalWrite(LED1_PIN, HIGH);
    digitalWrite(LED2_PIN, HIGH);
    digitalWrite(LED3_PIN, HIGH);
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);
    digitalWrite(LED3_PIN, LOW);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

// =================== BUZZER TASK ===================
void buzzerTask(void *pvParameters) {
  pinMode(BUZZER_PIN, OUTPUT);
  while (1) {
    tone(BUZZER_PIN, 3000);
    vTaskDelay(800 / portTICK_PERIOD_MS);
    noTone(BUZZER_PIN);
    vTaskDelay(200 / portTICK_PERIOD_MS);
  }
}

void oledTask(void * pvParameters) {
  String coreID = String(xPortGetCoreID());
  for (;;) {
    if (xSemaphoreTake(i2cMutex, portMAX_DELAY) == pdTRUE) {
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(0, 0);
      display.println("Integrasi 8-Periferal");
      display.setTextSize(2);
      display.setCursor(0, 20);
      display.print("CORE: ");
      display.println(coreID);
      display.display();
      xSemaphoreGive(i2cMutex);
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
// =================== STEPPER TASK (4-WIRE) ===================
void stepperTask(void * pvParameters) {
  pinMode(STEP_A_PLUS, OUTPUT);
  pinMode(STEP_A_MINUS, OUTPUT);
  pinMode(STEP_B_PLUS, OUTPUT);
  pinMode(STEP_B_MINUS, OUTPUT);

  int stepSequence[4][4] = {
    {HIGH, LOW, HIGH, LOW},
    {LOW, HIGH, HIGH, LOW},
    {LOW, HIGH, LOW, HIGH},
    {HIGH, LOW, LOW, HIGH}
  };

  for (;;) {
    // Putar maju
    for (int i = 0; i < STEPS_PER_REV; i++) {
      for (int j = 0; j < 4; j++) {
        digitalWrite(STEP_A_PLUS, stepSequence[j][0]);
        digitalWrite(STEP_A_MINUS, stepSequence[j][1]);
        digitalWrite(STEP_B_PLUS, stepSequence[j][2]);
        digitalWrite(STEP_B_MINUS, stepSequence[j][3]);
        delayMicroseconds(500);
      }
    }
    vTaskDelay(2000 / portTICK_PERIOD_MS);

    // Putar mundur
    for (int i = 0; i < STEPS_PER_REV; i++) {
      for (int j = 3; j >= 0; j--) {
        digitalWrite(STEP_A_PLUS, stepSequence[j][0]);
        digitalWrite(STEP_A_MINUS, stepSequence[j][1]);
        digitalWrite(STEP_B_PLUS, stepSequence[j][2]);
        digitalWrite(STEP_B_MINUS, stepSequence[j][3]);
        delayMicroseconds(500);
      }
    }
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}

// =================== SERVO TASK ===================
void servoTask(void * pvParameters) {
  myServo.attach(SERVO_PIN);
  for (;;) {
    myServo.write(0);
    vTaskDelay(1500 / portTICK_PERIOD_MS);
    myServo.write(180);
    vTaskDelay(1500 / portTICK_PERIOD_MS);
  }
}

// =================== BUTTON TASKS ===================
void readButton1Task(void *pvParameters) {
  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  for (;;) {
    button1Pressed = (digitalRead(BUTTON1_PIN) == LOW);
    vTaskDelay(50 / portTICK_PERIOD_MS);
  }
}

void readButton2Task(void *pvParameters) {
  pinMode(BUTTON2_PIN, INPUT_PULLUP);
  for (;;) {
    button2Pressed = (digitalRead(BUTTON2_PIN) == LOW);
    vTaskDelay(50 / portTICK_PERIOD_MS);
  }
}

// =================== POTENTIOMETER TASK ===================
void readPotTask(void *pvParameters) {
  pinMode(POT_PIN, INPUT);
  for (;;) {
    potValue = analogRead(POT_PIN);
    vTaskDelay(50 / portTICK_PERIOD_MS);
  }
}

// =================== ENCODER TASK ===================
void readEncoderTask(void *pvParameters) {
  pinMode(CLK_PIN, INPUT_PULLUP);
  pinMode(DT_PIN, INPUT_PULLUP);

  lastStateCLK = digitalRead(CLK_PIN);

  for (;;) {
    int currentStateCLK = digitalRead(CLK_PIN);
    if (currentStateCLK != lastStateCLK) {
      if (digitalRead(DT_PIN) != currentStateCLK) {
        direction = 1;
      } else {
        direction = -1;
      }
    }
    lastStateCLK = currentStateCLK;
    vTaskDelay(2 / portTICK_PERIOD_MS);
  }
}

// =================== LOGGING TASK ===================
void loggingTask(void * pvParameters) {
  xSemaphoreTake(setupCompleteSemaphore, portMAX_DELAY);

  for (;;) {
    int currentPot = potValue;
    bool btn1 = button1Pressed;
    bool btn2 = button2Pressed;
    int encDir = direction;

    Serial.printf("%-10d| %-6s| %-6s| %-12d\n",
                  currentPot,
                  btn1 ? "ON" : "OFF",
                  btn2 ? "ON" : "OFF",
                  encDir);

    direction = 0;
    vTaskDelay(250 / portTICK_PERIOD_MS);
  }
}

// =================== SETUP ===================
void setup() {
  Serial.begin(115200);
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println("===== MEMULAI PROGRAM INTEGRASI 8-PERIFERAL =====");

  setupCompleteSemaphore = xSemaphoreCreateBinary();
  i2cMutex = xSemaphoreCreateMutex();

  // Inisialisasi OLED
  if (xSemaphoreTake(i2cMutex, portMAX_DELAY) == pdTRUE) {
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
      Serial.println(F("Alokasi SSD1306 gagal"));
    } else {
      Serial.println("OLED SSD1306 ditemukan!");
      display.clearDisplay();
      display.display();
    }
    xSemaphoreGive(i2cMutex);
  }

  // Membuat task
  xTaskCreatePinnedToCore(readEncoderTask, "EncoderReader", 2048, NULL, 3, NULL, 0);
  xTaskCreatePinnedToCore(readButton1Task, "Button1", 2048, NULL, 2, NULL, 0);
  xTaskCreatePinnedToCore(readPotTask, "ReadPot", 2048, NULL, 1, NULL, 0);

  xTaskCreatePinnedToCore(stepperTask, "Stepper", 2048, NULL, 2, NULL, 1);
  xTaskCreatePinnedToCore(readButton2Task, "Button2", 2048, NULL, 2, NULL, 1);
  xTaskCreatePinnedToCore(ledBlinkTask, "LEDs", 2048, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(buzzerTask, "Buzzer", 2048, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(oledTask, "OLED", 4096, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(servoTask, "Servo", 2048, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(loggingTask, "Logging", 4096, NULL, 1, NULL, 1);

  Serial.println("===== Inisialisasi Selesai =====");
  Serial.println("Pot(4095) | Btn 1 | Btn 2 | Encoder Dir");
  Serial.println("----------|-------|-------|-------------");

  xSemaphoreGive(setupCompleteSemaphore);
}

// =================== LOOP ===================
void loop() {
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}
