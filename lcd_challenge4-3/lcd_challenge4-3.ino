//이번에는 코드를 짧게 짜는 것을 중점(if문을 추가해서라도 줄을 줄였다)
//한글 초를 출력하는 방법 https://www.kocoafab.cc/tutorial/view/495
//한글 글자 비트로 만들어주는 것  http://mikeyancey.com/hamcalc/lcd_characters.php
//4-2와 4-3차이점 알아볼것
//4-2는 땔때 제대로 되지만 3은 동시에 누르는게 안됨

#include <LiquidCrystal.h>

LiquidCrystal lcd(22,23,24,25,26,27);

bool before_sw=false;
bool lcd_write=true;//lcd가 여러번 쓰이는걸 방지
bool End_Trigger=false;
//bool이 1비트가 아니라 1바이트인 이유
//CPU가 메모리(RAM)의 주소에 접근하는데 하나의 메모리 주소가 가지는 데이터가 1bity이기 때문에
//CPU가 메모리 주소에 접근할 수 있는 최소단위가 1byte가 된다
//간단히 말해 CPU가 1바이트보다 작은 데이터 처리가 못한다
//단 1바이트이지만 범위는 0~1이다

unsigned int sec=0;
unsigned int last_sec=0;
unsigned int sec_10=0;
//int 대신 char를 사용해봄 int는 4바이트라 16진수 제어에는 너무 큰것 같다
//char는 1바이트이므로 unsigned로 부호를 없애 8bit사용한다
//C#을 가면 char에 숫자를 못넣게 막여 있다
//char : 1바이트 문자 자료형  byte : 1바이트 숫자 자료형
//따라서 byte를 사용하는데 byte는 처음부터 부호가 없어 unsigned를 안써도 된다
//byte 설명 https://www.arduino.cc/reference/ko/language/variables/data-types/byte/

/*byte sec_Kr[8] = {//초를 출력하기 위한 노력... Ver.2진수
        B00100,   //1의 위치를 합쳐서 보면 초라는 글씨가 된다
        B11111,   //1은 색출력O, 0은 색출력X
        B00100,   //B는 2진수 출력
        B01010,
        B10001,
        B00100,
        B00100,
        B11111.
};*/

byte sec_Kr[8] = {//초를 출력하기 위한 노력... Ver.16진수
        0x04,0x1F,0x04,0x0A,0x11,0x04,0x04,0x1F
};

unsigned int SW(){
  if(PINC&0x30){//스위치 하나만 눌리면 동작되는 것
    lcd_write=true;
    sec_10=millis()/1000;//둘이 동시에 눌렀을때 시간을 기억한
  }
  if((PINC&0x30)==0x30){  
    delay(100);//때는 시간 여유
    return 0x03;
  }
    //우선 순위가 == 다음에 &라서 ()를 사용
    //스위치 동시 입력 ==0x30을 해준 이유는 하나만 눌려도 동작되므로
    //동시에 누르는걸 제일 위에 넣어야 우선인식후 reture한다 밑에 넣으면 PINC&0x10이 먼저 실행
  if(PINC&0x10){
    delay(100);    
    return 0x01;//1번 스위치 입력
  }
  if(PINC&0x20){    
    delay(100);
    return 0x02;//2번 스위치 입력
  }
  return 0x00;//두개 눌렀을 때 루프 탈출용인데 없어도 되는지 확인해볼것 
}

void LCD_Clear(){//범위 지정 클리어 타이머를 계속 살리기 위해서
  lcd.setCursor(0,0);
  lcd.print("                ");
  lcd.setCursor(0,1);
  lcd.print("         ");
}

void Display(unsigned int Mode){
  //if가 3개 이하면 if가 연산이 빠르지만 4개부턴 스위치가 소모 클럭이 적다고 한다 따라서 if문 사용
  //단 컴파일러별로 방법이 다르므로 정확한건 아니다
  if(Mode==0x03){
    if(sec-sec_10>=10){//10초가 지나면
      lcd_write=false;
      End_Trigger=true;      
    }
    return 0x04; //리턴 넣어준 이유는 동시에 눌릴때(0x03) 밑에것이 동작이 안되게
  }
  if(Mode==0x01||Mode==0x02){ //mode가 1,2일때 동작
    LCD_Clear();
    lcd.setCursor(0,0);
    lcd.print("Switch ");
    lcd.print(Mode);//출력이 잘 안되면 1일때와 2일때 분리
    lcd.print(" ON");
    lcd.setCursor(0,1);
    lcd.print("Lamp ");
    lcd.print(Mode);
    lcd.print(" ON");
    lcd_write=false;
  }
}

void timer(){//해당자리를 계속 사용하는거라 별도의 clear필요없음
  sec=millis()/1000;
  if(sec!=last_sec){//초가 바뀌면실행  lcd 출력 횟수를 줄이기 위해서
    last_sec=sec;
    int row=1;
    for(int i=10;i<=sec;i*=10)  row++;
    //10의n승 초일때 열 한칸 이동한다
    //단 0~1ms일때 row는 0이라 위치가 올바르지 않음 하지만 미세한 시간이라 사람이 보지 못하므로 방치함
    lcd.setCursor(15-row,1);
    lcd.print(sec);
    lcd.write(byte(0));//초를 쓰기 위한 노력 print는 0이라고 인식 
    //byte 0을 byte데이터 유형으로 바꾼다 createChar에서 사용자 정의를 바이트로 불러오라고 되어있음
    //byte() 설명 https://www.arduino.cc/reference/ko/language/variables/conversion/bytecast/
  }
}

void End(){
  PORTC=0x00;
  for(int i=0; i<=2; i++){
    //set커서로 하는 이유는 scrollDispaly는 모든 글자가 밀려 timer가 이상해짐
    Enter_Display(i);
    for(int s=0; s<50; s++){
      timer();
      delay(10);
      if(SW()) return 5;
    }
  }
  for(int j=3; j>=1; j--){
    //i와 j가 2,1인 이유는 마지막은 다른 것이 출력한다 라는 의미
    //i와 j를 3,0으로 하면 마지막에 한번씩 출력함
    Enter_Display(j);
    for(int s=0; s<50; s++){
      timer();
      delay(10);
      if(SW()) return 6;
    }
  }
  End_Trigger=false;
}

void Enter_Display(int k){
  LCD_Clear(); //기존 글씨가 남기때문에 clear
  lcd.setCursor(k,0);
  lcd.print("Enter Next...");
  lcd.setCursor(1,0);
}

void setup() {
  // put your setup code here, to run once:
  DDRC = 0x0F; //상위니블 입력, 하위니블 출력
  PORTC = 0x00;//초기화
  lcd.begin(16,2);
  lcd.createChar(0, sec_Kr);//초를 쓰기 위한 노력 1번째 인자는 0~7까지 사용자 정의
  
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned int Mode=SW();
  timer();
  if(lcd_write)//스위치를 계속 누를때 계속 갱신되는 것을 방지 LCD는 연산이 많이 들어가므로 연산 절약용
    if(!End_Trigger){
      PORTC=Mode;
      Display(Mode);
    }
    
  if(End_Trigger) End();
  //if(sec-sec_10>=10)로 하지 않은 이유는 last_sec가 갱신이 안되서 계속된다
  //sec_10를 매번 현재값에 대입하는 것 보다 변수를 이용해서 활성화여부를 하는 것이 좋다고 판단
}
