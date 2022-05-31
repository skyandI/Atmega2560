#define No    475
#define Low   480 
#define Mid   485

#define Mic A15

void setup() {
  pinMode(Mic,INPUT);
  DDRA=0x0F;//A포트 모두 출력
  Serial.begin(9600);
}

void loop() {
  int Sound=analogRead(Mic);
  Serial.print("소리크기 :");
  Serial.print(Sound);
  Serial.print("     ");
  int i=0;
  if(digitalRead(26))i=1; 
  Serial.print(i);
  if(Sound<=No){
    PORTA=0x00;
  }
  else if(Sound<=Low){
    PORTA=0x01;
    Serial.println("    음 : 저음");
  }
  else if(Sound<=Mid){
    PORTA=0x02;
    Serial.println("    음 : 중간음");
  }
  else{
    PORTA=0x04;
    Serial.println("    음 : 고음");
  }
  //delay(100);
}
