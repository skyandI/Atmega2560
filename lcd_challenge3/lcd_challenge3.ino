#include <LiquidCrystal.h>

LiquidCrystal lcd(22,23,24,25,26,27);

//char ch;//str이 제대로 안되면 이걸 해재하고 Word_Read에 있는 char 선언을 없애라
String str;

void Word_Read(){
  delay(100);         //문자 수신을 위한 delay
  lcd.clear();
  while(Serial.available()>0){
    char ch=Serial.read(); 
    str.concat(ch);   //문자 받은것들을 열로 변환한다
  }
}

void LCD_Display(){   //시리얼포트가 새줄 일때 \n, line-ending 없음 하면 \n 없어도 된다
  if(str=="KOREA Fighting!"){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(str);
    return 2;
  }
  if(str=="Induk University!"){
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print(str);
    return 3;
  }
}

void setup() {
  // put your setup code here, to run once:
  lcd.begin(16,2);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
 /* if(Serial.available()){   //Word_Read가 제대로 동작 안될때
    delay(100);         //문자 수신을 위한 delay
    lcd.clear();
    while(Serial.available()>0){
      ch=Serial.read();
      str.concat(ch);   //문자 받은것들을 열로 변환한다
    }*/    
  if(Serial.available()){
    str="";
    //str를 NULL로 초기화 , 이걸안하면 concat를 해서 기존입력을 계속 합하기 때문에 초기화해야한다 
    Word_Read(); 
    Serial.println(str);
    LCD_Display();
  }
}
