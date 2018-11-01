#define LED 7

String incomingString = "none";

void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
  Serial.setTimeout(10);
  digitalWrite(LED, HIGH);
  delay(5000);
  digitalWrite(LED, LOW);
}

void loop() {
  if (Serial.available()){
    incomingString = Serial.readString();
    Serial.println(incomingString);
  }

  if(incomingString.indexOf("AAN") != -1){
      digitalWrite(LED, HIGH);
      delay(1500);
      digitalWrite(LED, LOW);
      incomingString = "";
  }
//  if(incomingString.indexOf("AAN") == -1){
//      digitalWrite(LED, LOW);
//  }
}
