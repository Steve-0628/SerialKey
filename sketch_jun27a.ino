void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("INIT");
  Serial.println("");
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print(!digitalRead(4));
  Serial.print(!digitalRead(5));
  Serial.print(!digitalRead(6));
  Serial.print(!digitalRead(7));
//Serial.print(1234);
  delay(50);
}
