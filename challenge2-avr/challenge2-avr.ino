unsigned char led =0xff;

void setup() {
  // put your setup code here, to run once:
DDRC =0xF0;
}

void loop() {
  // put your main code here, to run repeatedly:
  PORTC =led;
  delay(1000);
  led>>=1;;
  if(led==0x0f)
  {
    PORTC=led;
    delay(1000);
    led=0xf0;
  }
}
