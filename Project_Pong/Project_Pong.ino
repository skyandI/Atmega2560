//배열을 오버해서 사용하는 것을 방지하기 위해서 if문을 넣어줘 범

//배선 세팅
#define RS 22
#define E 23
#define DB4 24
#define DB5 25
#define DB6 26
#define DB7 27

#define SW1 28
#define SW2 29

//게임 기초 세팅
#define Limit_Row_Top 1

int Speed=500;
int Speed_Up=50;
int Limit_Col=5;

#include <LiquidCrystal.h>
#include<EEPROM.h>

int addr=0;

LiquidCrystal lcd(RS, E, DB4, DB5, DB6, DB7);

bool Game_Start=false;
bool Game_Over=false;
bool Row_Up=true;
bool Col_Left=true;
bool Bar_Lose_Flag=false;
bool SW=false;
bool Bar_Go_Up=false;
bool Fail_Detect=false;

int Big_Row=0;//큰 셀의 행(가로)
int Big_Col=Limit_Col/2;//큰 셀의 열(세로)
int Bar_1p_Row=0;//1p 막대기 행(가로)
int Bar_2p_Row=0;

int Row=3;
int Col=4;

int Bar_Center_1p=5;//원점은 위 끝으로 설정함
int Bar_Center_2p=5;
int Bar_Lose=0;//위쪽으로 바이동시 초과해서 버린 횟수
int Bar_Up_Lose_2p=0;//위쪽으로 바이동시 초과해서 버린 횟수
int Bar_Down_Lose_2p=0;//아래쪽으로 바이동시 초과해서 버린 횟수
int Go_Up=true;

int Score=0;

byte Ball_Location[8] = {
  B00000,//0
  B00000,//1
  B00000,//2
  B00000,//3
  B00000,//4
  B00000,//5
  B00000,//6
  B00000,//7
};

byte Bar_1p_Top[8] = {//1P 위쪽 바
  B00000,//0
  B00000,//1
  B00000,//2
  B00000,//3
  B00000,//4
  B00000,//5
  B00000,//6
  B00000,//7
};

byte Bar_1p_Bottom[8] = {//1P 아래쪽 바
  B00000,//0
  B00000,//1
  B00000,//2
  B00000,//3
  B00000,//4
  B00000,//5
  B00000,//6
  B00000,//7
};

byte Bar_2p_Top[8] = {//2P 위쪽 바
  B00000,//0
  B00000,//1
  B00000,//2
  B00000,//3
  B00000,//4
  B00000,//5
  B00000,//6
  B00000,//7
};

byte Bar_2p_Bottom[8] = {//2P 아래쪽 바
  B00000,//0
  B00000,//1
  B00000,//2
  B00000,//3
  B00000,//4
  B00000,//5
  B00000,//6
  B00000,//7
};

void Game_Start_Set(){//초기 세팅값
  randomSeed(analogRead(A0));//아날로그핀의 플로팅 현상으로 인해 0~1023사이 무작이 값 반환
  //아두이노 랜덤 시퀀스는 정해져 있으므로 난수 
  Big_Row=random(0,2);//초기 공 위치
  Big_Col=Limit_Col/2;//초기 공위치초기화
  Row=random(1,8);
  Serial.println(Row);
  Col=4;
  for(int i=0;i<=7;i++){//재시작시 내부 클리어
    Ball_Location[i]=0x00;
    Bar_1p_Top[i]=0x00;
    Bar_1p_Bottom[i]=0x00;
    Bar_2p_Top[i]=0x00;
    Bar_2p_Bottom[i]=0x00;
  }
  Ball_Location[Row]=0x01;//초기공 생성
  Bar_1p_Row=0;//1p 막대기 행(가로)
  Bar_2p_Row=0;
  Bar_Lose=0;//바이동시 초과해서 버린 횟수
  Bar_Center_1p=5;//원점은 위 끝으로 설정함
  Bar_Center_2p=5;
  for(int i=5;i<=7;i++){//바 초기위치
    Bar_1p_Top[i]=0x10;
  }
  Fail_Detect=false;
  Score=0;
  Speed=500;
  Game_Start=true;
  Game_Over=false;
}

int Move_Row(){
  if(Row_Up){
    if(Row==Limit_Row_Top){//작은셀이 맨 위일때 칸을 홀수로 만들어 패턴 변환
      if(!Big_Row){// (n,0)일때
        Row_Up=false;// 열이 아래로 이동한다
        if(Row+1<8) Ball_Location[Row+1]=Ball_Location[Row];
        Ball_Location[Row]=0x00;
        Row++;
        return 0;
      }
    }
    if(Row==0){//밑에는 0줄도 출력하기 때문에 따로 줌
      if(Big_Row){// (n,1)일때
        Big_Row=0;   // (n,0)으로 만들고
        Row=7;       // 작은셀을 맨 아래로 설정
        Ball_Location[7]=Ball_Location[0];
        Ball_Location[0]=0x00;
        return 0;
      }
     }
   //후 연산해서 조건이 맞으면 먼저 끝낸다
   //선 연산을 한 뒤 조건을 하면 동작이 꼬임 
   if(Row-1>=0) Ball_Location[Row-1]=Ball_Location[Row];//현재 열값을 다음열 값에 넣는데
   Ball_Location[Row]=0x00;//이전 Row 초기화
   Row--;//Row를 다음열로 이동하여 현재열을 갱신한다
  }
  
  if(!Row_Up){
    
   if(Row==7){//작은셀이 맨 아래일때
    if(Big_Row==1){// (n,1)일때
      Row_Up=true; // 열이 위로 이동한다
      if(Row-1>=0) Ball_Location[Row-1]=Ball_Location[Row];
      Ball_Location[Row]=0x00;
      Row--;
      return 0;    //Big_Row 실행 막기
    }
    
    if(Big_Row==0){// (n,0)일때
      Big_Row=true;   // (n,1)로 만들고
      Row=0;       // 작은셀을 맨 위로 설정
      Ball_Location[0]=Ball_Location[7];
      Ball_Location[7]=0x00;
      return 0;
    }
   }
   if(Row+1<8) Ball_Location[Row+1]=Ball_Location[Row];//Row값을 다음순서에 저장
   Ball_Location[Row]=0x00;
   Row++;//현재 열의 위치
  }
}

void Move_Col(){
  if(Col_Left){
    if(Col==0)
      if(Big_Col!=0){
       Ball_Location[Row]=0x01;
       Col=4;
       Big_Col--;
       return ;
    }
    Ball_Location[Row] <<=1;
    Col--;
  }
  if(!Col_Left){
   if(Col==4)
     if(Big_Col!=Limit_Col){
       Ball_Location[Row]=0x10;
       Col=0;
       Big_Col++;
       return ;
    }
    Ball_Location[Row] >>=1;
    Col++;
  }
  if(Big_Col==0)
    if(Col==1){
      Fail_Detect=true;
      Col_Left=false;
    }
  if(Big_Col==Limit_Col)
    if(Col==2)//칸을 홀수로 만들어 패턴 변환
      Col_Left=true;
}

void Bar_Lose_Set(){
  SW=false;
  if(Bar_Go_Up){
        Bar_Lose--;
        if(Bar_Lose==2){
          Bar_1p_Bottom[2]=0x00;
          Bar_1p_Bottom[1]=0x10;
          Bar_1p_Bottom[0]=0x10;
          Bar_1p_Top[7]=0x10;
          Bar_1p_Top[6]=0x00;
          Bar_1p_Top[5]=0x00;
        }
        if(Bar_Lose==1){
          Bar_1p_Bottom[2]=0x00;
          Bar_1p_Bottom[1]=0x00;
          Bar_1p_Bottom[0]=0x10;
          Bar_1p_Top[7]=0x10;
          Bar_1p_Top[6]=0x10;
          Bar_1p_Top[5]=0x00;
        }
        if(Bar_Lose==0){
          Bar_1p_Bottom[2]=0x00;
          Bar_1p_Bottom[1]=0x00;
          Bar_1p_Bottom[0]=0x00;
          Bar_1p_Top[7]=0x10;
          Bar_1p_Top[6]=0x10;
          Bar_1p_Top[5]=0x10;
          Bar_Center_1p=5;
          Bar_Lose=0;
          Bar_1p_Row=0;
          Bar_Lose_Flag=false;
        }
  }
  if(!Bar_Go_Up){
    Bar_Lose++;
        if(Bar_Lose==1){
          Bar_1p_Top[5]=0x00;
          Bar_1p_Top[6]=0x10;
          Bar_1p_Top[7]=0x10;
          Bar_1p_Bottom[0]=0x10;
          Bar_1p_Bottom[1]=0x00;
          Bar_1p_Bottom[2]=0x00;
        }
        if(Bar_Lose==2){
          Bar_1p_Top[5]=0x00;
          Bar_1p_Top[6]=0x00;
          Bar_1p_Top[7]=0x10;
          Bar_1p_Bottom[0]=0x10;
          Bar_1p_Bottom[1]=0x10;
          Bar_1p_Bottom[2]=0x00;
        }
        if(Bar_Lose==3){
          Bar_1p_Top[5]=0x00;
          Bar_1p_Top[6]=0x00;
          Bar_1p_Top[7]=0x00;
          Bar_1p_Bottom[0]=0x10;
          Bar_1p_Bottom[1]=0x10;
          Bar_1p_Bottom[2]=0x10;
          Bar_Center_1p=0;
          Bar_Lose=3;
          Bar_1p_Row=1;
          Bar_Lose_Flag=false;
        }
  }
}

void Move_Bar(){
  if(digitalRead(SW1)){//위로 가는 것
    Bar_Go_Up=true;
    SW=true;
    if(Bar_1p_Row==0){//0열에 있을때
      if(Bar_Center_1p!=Limit_Row_Top){
        if(Bar_Center_1p+2<8) 
          if(Bar_Center_1p+2>=0)
            Bar_1p_Top[Bar_Center_1p+2]=0x00;//이전값 지움
        Bar_Center_1p--;
      }
      if(Bar_Center_1p==Limit_Row_Top){//더이상 못가게 막음
        Bar_Center_1p=Limit_Row_Top;
      }
      if(Bar_Center_1p<8) Bar_1p_Top[Bar_Center_1p]=0x10;
      if(Bar_Center_1p+1<8) Bar_1p_Top[Bar_Center_1p+1]=0x10;
      if(Bar_Center_1p+2<8) Bar_1p_Top[Bar_Center_1p+2]=0x10;
    }
    if(Bar_1p_Row==1){//1열에 있을때
      if(Bar_Center_1p+2<8) 
          if(Bar_Center_1p+2>=0)
            Bar_1p_Bottom[Bar_Center_1p+2]=0x00;//이전값 지움
      Bar_Center_1p--;
      if(Bar_Center_1p<0){//바 중심이 7일때 한개가 버려짐
        Bar_Lose_Flag=true;
        return ;
      }
      if(Bar_Center_1p<8) Bar_1p_Bottom[Bar_Center_1p]=0x10;
      if(Bar_Center_1p+1<8) Bar_1p_Bottom[Bar_Center_1p+1]=0x10;
      if(Bar_Center_1p+2<8) Bar_1p_Bottom[Bar_Center_1p+2]=0x10;
    }
  }
  if(digitalRead(SW2)){//아래로 가는 것
    Bar_Go_Up=false;
    SW=true;
    if(Bar_1p_Row==1){//1열에 있을때
      if(Bar_Center_1p!=5){//맨 밑에 닿지 않았을때
        if(Bar_Center_1p<8) 
          if(Bar_Center_1p>=0)
            Bar_1p_Bottom[Bar_Center_1p]=0x00;//이전값 지움
        Bar_Center_1p++;
      }
      if(Bar_Center_1p==5){//더이상 못가게 막음
        Bar_Center_1p=5;
      }
      if(Bar_Center_1p>=0) Bar_1p_Bottom[Bar_Center_1p]=0x10;
      if(Bar_Center_1p+1>=0) Bar_1p_Bottom[Bar_Center_1p+1]=0x10;
      if(Bar_Center_1p+2>=0) Bar_1p_Bottom[Bar_Center_1p+2]=0x10;
    }
    if(Bar_1p_Row==0){//0열에 있을때
        if(Bar_Center_1p<8) 
          if(Bar_Center_1p>=0)
            Bar_1p_Top[Bar_Center_1p]=0x00;//이전값 지움
      Bar_Center_1p++;
      if(Bar_Center_1p>5){//바 중심이 5일때 한개가 버려짐
        Bar_Lose_Flag=true;  
        return ;      
      }
      if(Bar_Center_1p>=0)  Bar_1p_Top[Bar_Center_1p]=0x10;
      if(Bar_Center_1p+1>=0)  Bar_1p_Top[Bar_Center_1p+1]=0x10;
      if(Bar_Center_1p+2>=0)  Bar_1p_Top[Bar_Center_1p+2]=0x10;
    }
  }
}

void Move_CPU(){
  if(Big_Row==0){
    if(Row==Limit_Row_Top){
      Go_Up=false;
    }
  } 
  if(Big_Row==1){
    if(Row==7)  Go_Up=true;
    if(Row>=6)  return;
  }
  if(Go_Up){
    if(Big_Row==0){
      if(Row-1>=0)  Bar_2p_Top[Row-1]=0x00;
      if(Row>=0)  
        if(Row<8)
          Bar_2p_Top[Row]=0x02;
      if(Row+1<8)  Bar_2p_Top[Row+1]=0x02;
      if(Row+2<8)  Bar_2p_Top[Row+2]=0x02;
      if(Row+3<8)  Bar_2p_Top[Row+3]=0x00;
      if(Row==7)  Bar_2p_Bottom[2]=0x00;//칸이동
      if(Row==6)  Bar_2p_Bottom[1]=0x00;
      if(Row==5)  Bar_2p_Bottom[0]=0x00;
    }
    if(Big_Row==1){
      if(Row-1>=0)  Bar_2p_Bottom[Row-1]=0x00;
      if(Row>=0)
        if(Row<8)  
          Bar_2p_Bottom[Row]=0x02;
      if(Row+1<8)  Bar_2p_Bottom[Row+1]=0x02;
      if(Row+2<8)  Bar_2p_Bottom[Row+2]=0x02;
      if(Row+3<8)  Bar_2p_Bottom[Row+3]=0x00;
    }
  }
  if(!Go_Up){
    if(Big_Row==1){
      Bar_2p_Top[7]=0x00;
      if(Row-1>=0)  Bar_2p_Bottom[Row-1]=0x00;
      if(Row>=0)
        if(Row<8)
          Bar_2p_Bottom[Row]=0x02;
      if(Row+1<8)  Bar_2p_Bottom[Row+1]=0x02;
      if(Row+2<8)  Bar_2p_Bottom[Row+2]=0x02;
      if(Row+3<8)  Bar_2p_Bottom[Row+3]=0x00;
    }
    if(Big_Row==0){
      if(Row-1>=0)  Bar_2p_Top[Row-1]=0x00;
      if(Row>=0)  
        if(Row<8)  
          Bar_2p_Top[Row]=0x02;
      if(Row+1<8)  Bar_2p_Top[Row+1]=0x02;
      if(Row+2<8)  Bar_2p_Top[Row+2]=0x02;
      if(Row+3<8)  Bar_2p_Top[Row+3]=0x00;  
      if(Row==6)  Bar_2p_Bottom[0]=0x02;
      if(Row==7)  Bar_2p_Bottom[1]=0x02;
    }
  }
}

bool Fail(){
  if(Fail_Detect){
    Fail_Detect=false;
    Score++; 
    Speed-=Speed_Up;//속도증가
    if(Speed<=60)//너무 빨라져 공사라짐 방지
      Speed=50;
    byte test[8];
    if(Big_Row==0)
      for(int i=0;i<=7;i++){
        test[i]=Bar_1p_Top[i];
        test[i]|=Ball_Location[i];
      }
    if(Big_Row==1)
      for(int i=0;i<=7;i++){
        test[i]=Bar_1p_Bottom[i];
        test[i]|=Ball_Location[i];
      }
    if(!Bar_Lose_Flag){
      if(Big_Row==0)
        if(test[Row]!=0x18)
          return 1;
      if(Big_Row==1)
        if(test[Row]!=0x18)
          return 1;
    }
    if(Bar_Lose_Flag){//바가 중간에 있을때
      for(int i=5;i<=7;i++)
        if((test[i]&0x10)==0x10)//바가 있는것을 서치
          if(test[i]==0x18)//바가 있는 곳에 공이 있으면
            return 0;            //무사히 통과
      for(int i=0;i<=2;i++)
        if((test[i]&0x10)==0x10)//바가 있는것을 서치
        if(test[i]==0x18)//바가 있는 곳에 공이 있으면
            return 0;            //무사히 통과
      return 1;//여기까지 통과가 안됬으면 공이 없는거다
    }
  }
    return 0;
}

void Score_Count(){
    int Best_Score=EEPROM.read(addr);
    if(Best_Score<Score){//베스트 스코어 갱신{
      EEPROM.write(addr,Score);
      Best_Score=Score;
      lcd.setCursor(0,0);
      lcd.write("New Best Score");
      lcd.setCursor(0,1);
      lcd.write("Best Score: ");
      lcd.print(Best_Score);
      return ;
    }
    lcd.setCursor(0,0);
    lcd.write("Best Score: ");
    lcd.print(Best_Score);
    lcd.setCursor(0,1);
    lcd.write("Score : ");
    lcd.print(Score);
};

void Game_Display(bool Game_Over){
  if(!Game_Over){
  lcd.createChar(0,Ball_Location);
  if(Big_Col==0){//0번째 셀일 때 공이 안보이는 현상을 해소
    if(Big_Row==0){
      for(int i=0;i<8;i++)
        Bar_1p_Top[i]|=Ball_Location[i];
    }
    if(Big_Row==1)  {
      for(int i=0;i<8;i++)
        Bar_1p_Bottom[i]|=Ball_Location[i];
    }
  }
  if(Big_Col==Limit_Col){
    if(Big_Row==0){
      for(int i=0;i<8;i++)
        Bar_2p_Top[i]|=Ball_Location[i];
    }
    if(Big_Row==1)  {
      for(int i=0;i<8;i++)
        Bar_2p_Bottom[i]|=Ball_Location[i];
    }
  }
  lcd.createChar(1,Bar_1p_Top);
  lcd.createChar(2,Bar_1p_Bottom);
  lcd.createChar(3,Bar_2p_Top);
  lcd.createChar(4,Bar_2p_Bottom);
  for(int i=0;i<8;i++){//바만 살리고 나머지 초기화
    Bar_1p_Top[i]&=0x10;
    Bar_1p_Bottom[i]&=0x10;
    Bar_2p_Top[i]&=0x02;
    Bar_2p_Bottom[i]&=0x02;
  }
  lcd.begin(16,2);
  lcd.clear();
  lcd.setCursor(Big_Col,Big_Row);
  lcd.write(byte(0));
  //밑에는 바 출력 관한 식
  lcd.setCursor(0,0);
  lcd.write(byte(1));
  lcd.setCursor(0,1);
  lcd.write(byte(2));
  lcd.setCursor(Limit_Col,0);
  lcd.write(byte(3));
  lcd.setCursor(Limit_Col,1);
  lcd.write(byte(4));
  lcd.setCursor(9,0);
  lcd.write("Score");
  lcd.setCursor(9,1);
  lcd.print(Score);
  delay(Speed);
  }
  
  if(Game_Over){
    delay(500);//죽는 장면 보기 위해
    Score--;
    lcd.clear();
    Score_Count();
    delay(1000);
    Game_Start=false;
  }
}

void setup() {
  pinMode(SW1,INPUT);
  pinMode(SW2,INPUT);
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  lcd.write("Press SW");
  Serial.begin(9600);
}

void loop() {
  if(Game_Start){
    Move_Row();
    Move_Col();
    Move_Bar();
      if(Bar_Lose_Flag)
        if(SW)
          Bar_Lose_Set();
    Move_CPU();
    Game_Display(Game_Over);
    Game_Over=Fail();
  }
  if(!Game_Start)
    if(digitalRead(SW1)){//스위치1을 누르면 게임 재시작
      lcd.clear();
      Game_Over=false;
      Game_Start=true;
      Game_Start_Set();//초기 셋업
    }
}
        
