void setup() {
  // put your setup code here, to run once:
  pinMode(37,INPUT);
  pinMode(33,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
if(digitalRead(37))
  digitalWrite(33,HIGH);
if(!digitalRead(37))
  digitalWrite(33,LOW);
}
