
//사운드는 C포트 0x10
//LED는 C포트 0x03


#include <LiquidCrystal.h>

 
LiquidCrystal lcd(22,23,24,25,26,27);


bool Buzzer_active=true;
bool Sound_active=true;

int Sound_Count=0;
int Before_Mode=0;


void Mode(){
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.write("Count : ");
  lcd.print(Sound_Count);
  Before_Mode=Sound_Count;   
  
  if(Sound_Count==1){
    PORTC=0x01;
    Buzzer_active=true;
    lcd.setCursor(0,0);
    lcd.print("Light 1 ON");
    Serial.println("Light 1 ON");
  }

  if(Sound_Count==2){
    PORTC=0x02;
    Buzzer_active=true;
    lcd.setCursor(0,0);
    lcd.print("Light 2 ON");
    Serial.println("Light 2 ON");
    
  }

  if(Sound_Count==3){
    if(Buzzer_active){//시끄러워서 짧게 동작
      lcd.setCursor(0,0);
      lcd.print("Sound ON");
      Serial.println("Sound ON");
      PORTC=0x04; 
      delay(1000);
      PORTC=0x00;
      Buzzer_active=false;
    }
  }
}

void stop_timer(){
  delay(300);
  Sound_active=false;
}
 

void setup() {
  DDRC=0x0F;
  lcd.begin(16,2);
  Serial.begin(9600);
}

 

void loop() {
  if(PINC&0x10){
    Sound_Count++;
    Sound_active=true;
  }
  if(Sound_Count!=Before_Mode) Mode();
  //여러번 쓰기 방지
  if(Sound_active)  stop_timer();
}
