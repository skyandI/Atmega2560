int LED_While=0;//반복횟수
#define Delay 300 //속도 조절
int type;

int Mode(){//반복횟수 설정
  if(digitalRead(22)){
    LED_While=3;
    type=0;
    return 0;
    Serial.print("a");
  }
  if(digitalRead(23)){
    LED_While=2;
    type=1;
    return 1;
  }
  if(digitalRead(24)){
    LED_While=3;
    type=2;
    return 2;
  }
  if(digitalRead(25)){
    LED_While=2;
    type=3;
    return 3;
  }
  type=4;
}

void setup() {
  // put your setup code here, to run once:
  pinMode(22,INPUT);
  pinMode(23,INPUT);
  pinMode(24,INPUT);
  pinMode(25,INPUT);
  pinMode(26,OUTPUT);
  pinMode(27,OUTPUT);
  pinMode(28,OUTPUT);
  pinMode(29,OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Mode();
  if(type==0|type==2){
    for(int i=0;i<LED_While;i++){
      for(int j=24;j<=28;){
       if(type==0)
          digitalWrite(j,LOW);
       digitalWrite(++j,HIGH);
       delay(Delay);
      }
      for(int k=26;k<=29;k++)
        digitalWrite(k,LOW);
      delay(Delay);
    }
  }
  if(type==1|type==3){
    for(int i=0;i<LED_While;i++){
      for(int j=30;j>=27;){
       if(type==1)
          digitalWrite(j,LOW);
       digitalWrite(--j,HIGH);
       delay(Delay);
      }
      for(int k=29;k>=26;k--)
        digitalWrite(k,LOW);
      delay(Delay);
    }
  }
}
