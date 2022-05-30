void setup() {
  pinMode(30,OUTPUT);
  pinMode(31,OUTPUT);
  pinMode(32,OUTPUT);
  pinMode(33,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(30,HIGH);
  digitalWrite(31,HIGH);
  digitalWrite(32,HIGH);
  digitalWrite(33,HIGH);
  delay(1000);
  for(int i=0;i<4;i++)
  {
    digitalWrite(30+i,LOW);
    delay(1000);
  }
}
