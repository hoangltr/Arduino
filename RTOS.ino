// Include libraries
#include <Arduino_FreeRTOS.h> // FreeRTOS for Arduino Library
#include <semphr.h> //semaphore supoport library
// define pin of divices
#define sensor 2
#define buzzer 5
#define LED 4
#define light 6
#define lightcheck 7
//Declaring a global variable of type SemaphoreHandle_t 
SemaphoreHandle_t interruptSemaphore;

void TaskLed( void *pvParameters );
void TaskBuzzer( void *pvParameters );
void TaskLight( void *pvParameters );

void setup() {
  // Setting pin input, output
  pinMode(sensor,INPUT);
  pinMode(LED, OUTPUT);
  pinMode(buzzer, OUTPUT);
  //Set initial output
  digitalWrite(LED, LOW);
  digitalWrite(buzzer, LOW);
  //Create task for Arduino
  xTaskCreate(TaskLed, "LED", 128, NULL, 3, NULL);// Task LED 1Hz
  xTaskCreate(TaskBuzzer, "Buzzer", 128, NULL, 1, NULL);// Task control buzzer
  xTaskCreate(TaskLight, "Light", 128, NULL, 2, NULL);// Task control light
  //Create a binary semaphore.
  interruptSemaphore = xSemaphoreCreateBinary();
  if (interruptSemaphore != NULL) {
    attachInterrupt(digitalPinToInterrupt(sensor), interruptHandler, CHANGE);
  }
}

void loop() {}
//Give semaphore in the interrupt handler
void interruptHandler() {
  xSemaphoreGiveFromISR(interruptSemaphore, NULL);
}
//Task buzzer
void TaskBuzzer(void *pvParameters)
{
  (void) pvParameters;
  // Take the semaphore.
  for (;;) {
    if (xSemaphoreTake(interruptSemaphore, portMAX_DELAY) == pdPASS) {
      digitalWrite(buzzer, !digitalRead(buzzer));
    }
    vTaskDelay(350);
  }
}
//Task Led
void TaskLed(void *pvParameters)
{
  (void) pvParameters;

  for (;;) {
    digitalWrite(LED, HIGH);
    vTaskDelay(500/portTICK_PERIOD_MS);
    digitalWrite(LED, LOW);
    vTaskDelay(500/portTICK_PERIOD_MS);
  }
}
//Task Control Light
void TaskLight(void *pvParameters)
{
  (void) pvParameters;//pin of LightTask
  pinMode(lightcheck,INPUT);
  pinMode(light, OUTPUT);
  for (;;) {
    if(digitalRead(lightcheck)==1)
    {
    digitalWrite(light, HIGH);
    } else
    {
    digitalWrite(light, LOW);
    }
    vTaskDelay(10);
  }
}