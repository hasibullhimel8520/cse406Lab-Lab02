// Global Variables Declare
int waterLevel = 0;
int waterLevelPrevious = 0;
float waterPercentage = 0;
float rateOfChange = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  waterLevel = analogRead(A0); // Read Analog Input
  waterPercentage = 100/1023.0*waterLevel; // Convert To Percentage
  // Conditions for LEVEL wise ALERTS
  if(waterPercentage>=95) {
    Serial.println("HIGH LEVEL");
  }
  if(waterPercentage>=75 && waterPercentage<95) {
    Serial.println("MEDIUM LEVEL");
  }
  if(waterPercentage>=25 && waterPercentage<75) {
    Serial.println("LOW LEVEL");
  }
  Serial.print("Water Level: ");
  Serial.print(waterPercentage);
  Serial.println("%");
  Serial.print("Rate of Change: ");
  Serial.print(rateOfChange);
  Serial.println("%");
  rateOfChange = (waterLevel-waterLevelPrevious)/(waterLevelPrevious*1.00)*100; // Rate of Change Calculatation
  delay(1000); // Delay between next read
  waterLevelPrevious = waterLevel;
  
}
