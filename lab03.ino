// DHT Temperature & Humidity Sensor
// Unified Sensor Library Example
// Written by Tony DiCola for Adafruit Industries
// Released under an MIT license.

// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 2     // Digital pin connected to the DHT sensor 

// Uncomment the type of sensor in use:
#define DHTTYPE    DHT11     // DHT 11
//#define DHTTYPE    DHT22     // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)

DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;

// Set temperature threshold (in Celsius)
float tempThreshold = 20.0; // Initial threshold value

// Define the number of samples for the moving average
const int NUM_SAMPLES = 10;  
float tempReadings[NUM_SAMPLES];  // Array to store temperature readings
int currentIndex = 0;            // Current index in the array
float total = 0.0;               // Sum of the readings for averaging
float filteredTemp = 0.0;        // Filtered temperature value

void setup() {
  Serial.begin(9600);
  // Initialize device.
  dht.begin();
  
  // Initialize all readings to 0
  for (int i = 0; i < NUM_SAMPLES; i++) {
    tempReadings[i] = 0.0;
  }
  
  Serial.println(F("DHTxx Unified Sensor Example"));
  
  // Print initial temperature threshold
  Serial.print(F("Initial Temperature Threshold: "));
  Serial.print(tempThreshold);
  Serial.println(F("°C"));
  
  // Print temperature sensor details.
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  Serial.println(F("------------------------------------"));
  Serial.println(F("Temperature Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("°C"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("°C"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("°C"));
  Serial.println(F("------------------------------------"));
  
  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
  Serial.println(F("Humidity Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("%"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("%"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("%"));
  Serial.println(F("------------------------------------"));
  
  // Set delay between sensor readings based on sensor details.
  delayMS = sensor.min_delay / 1000;
}

void loop() {
  // Delay between measurements.
  delay(delayMS);
  
  // Get temperature event and print its value.
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {
    // Raw temperature in Celsius
    float rawTemp = event.temperature;
    
    // Update the total by removing the oldest reading and adding the new one
    total -= tempReadings[currentIndex];           // Subtract the oldest reading
    tempReadings[currentIndex] = rawTemp;          // Store the new reading
    total += tempReadings[currentIndex];           // Add the new reading to the total
    
    // Move to the next index, looping back to 0 if necessary
    currentIndex = (currentIndex + 1) % NUM_SAMPLES;
    
    // Calculate the moving average (filtered temperature)
    filteredTemp = total / NUM_SAMPLES;
    
    // Convert temperature from Celsius to Fahrenheit
    float fahrenheit = (rawTemp * 9.0 / 5.0) + 32;
    float filteredFahrenheit = (filteredTemp * 9.0 / 5.0) + 32;
    
    // Print both raw and filtered temperature values
    Serial.print(F("Raw Temp: "));
    Serial.print(rawTemp);
    Serial.print(F("°C, Filtered Temp: "));
    Serial.print(filteredTemp);
    Serial.print(F("°C / "));
    Serial.print(fahrenheit);
    Serial.print(F("°F, "));
    Serial.print(filteredFahrenheit);
    Serial.println(F("°F"));
  }
  
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  }
  else {
    // Print humidity
    Serial.print(F("Humidity: "));
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
  }
}
