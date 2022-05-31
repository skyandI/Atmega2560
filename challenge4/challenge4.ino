bool Random_Mode=false; //랜덤 모드 활성화 여부
bool Before_sw=true; //true 5V신호를 받는다 false 5V신호 안받는다
void SW(){
  int A=PINC>>4;//상위니블은 입력이라 값을 알지만 하위니블은 출력이라 값이 랜덤이므로 쉬프트로 날려버렸다
  if(A==0x01)
    if(Before_sw)
    {
      //스위치가 0V에서 5V가 되는 순간
      Random_Mode=!Random_Mode;   //랜덤 모드 반전
      Before_sw=false;            //5V신호 안받는다 
    }
  if(!Before_sw)//보통 Before_sw가 true때가 많아서
  {
    if(A==0x00)//위에 둬서 2번째 if를 적게 한다
    {
      //스위치가 5V에서 0V가 되는 순간
      Before_sw=true;             //5V신호 받는다
    }
  }
}

void LED(int Random_Num){    //10진수->2진수 변환후 LED출력
  int LED=0x00;
  for(int i=8;i>=1;i/=2)     //8,4,2,1
  {
    if(Random_Num/i)         //몫이 1이면
      LED|=0x01;             //몫이 1이면 1비트 추가
    Random_Num%=i;           //나머지 저장
    LED<<=1;
  }
  LED>>=1;
  PORTC=LED;                 //LED 출력
}


void setup() {
  // put your setup code here, to run once:
  DDRA=0xFF;  //세그먼트 출력
  DDRC=0x0F;  //상위 니블(4bit) 입력, 하위 니블 출력
  PORTA=0x3F; //초기 출력 세그먼트 0
  PORTC=0x00; //초기 입력 모두 off
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned int LED_Segment[10]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7C,0x27,0x7F,0x67};//0~9
  SW();                             //스위치 입력 감지
  if(Random_Mode)
  {
    int Random_Num=random(9);       //0~9까지 랜덤
    //Serial.print(Random_Num);
    PORTA=LED_Segment[Random_Num];  //세그먼트 출력
    LED(Random_Num);                //LED 출력
    delay(5);
  }
}
