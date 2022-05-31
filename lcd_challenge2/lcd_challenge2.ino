 #include <LiquidCrystal.h>

LiquidCrystal lcd(22,23,24,25,26,27);

void setup() {
  lcd.begin(16,2);
}

void loop() {
  for(int i=16;i>=0;i--){
    lcd.setCursor(i,1);
    lcd.print("induk university!");
    lcd.setCursor(16-i,0);
    lcd.print("KOREA Fighting!");
    delay(500);
    lcd.clear();
  } 
}
