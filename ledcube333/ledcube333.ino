#include <MsTimer2.h>
#define SENSOR_MAX 255

int led[] = {  
  3,4,5,6,7,8,9,10,11};
int layer[] = {  
  2,13,12};
//int cycletime=200;   //コマ間の時間1000ms

//template
int TEMP_LED[]={
  0x000, 0x000, 0x000}; //左からトップ層ミドル層ボトム層
//template


int ANIM_1_TOP[]={
  0x124, 0x092, 0x049, 0x049, 0x092,0x124,0x124,0x1C0,0x038,0x007,0x007,0x038,0x1C0,0x1C0,
  0x1ff,0x000,0x000,0x000,0x000,0x1ff,0x1ff
};
int ANIM_1_MID[]={
  0x124, 0x092, 0x049, 0x049, 0x092,0x124,0x124,0x1C0,0x038,0x007,0x007,0x038,0x1C0,0x1C0,
  0x000,0x1ff,0x000,0x000,0x1ff,0x000,0x000
};
int ANIM_1_BOT[]={
  0x124, 0x092, 0x049, 0x049, 0x092,0x124,0x124,0x1C0,0x038,0x007,0x007,0x038,0x1C0,0x1C0,
  0x000,0x000,0x1ff,0x1ff,0x000,0x000,0x000
};
int ANIM_2_TOP[]={
  0x111,0x092,0x054,0x038,0x111,0x092,0x054,0x038,0x111,0x000,0x049,0x092,
  0x124,0x000,0x049,0x092,0x124,0x000,0x000,0x007,0x038,0x1c0,0x000,0x007,0x038,0x1c0,0x000};
int ANIM_2_MID[]={
  0x111,0x092,0x054,0x038,0x111,0x092,0x054,0x038,0x111,0x1ff,0x098,0x092,
  0x092,0x1ff,0x098,0x092,0x092,0x1ff,0x1ff,0x038,0x038,0x038,0x1ff,0x038,0x038,0x038,0x1ff};
int ANIM_2_BOT[]={
  0x111,0x092,0x054,0x038,0x111,0x092,0x054,0x038,0x111,0x000,0x124,0x092,
  0x049,0x000,0x124,0x092,0x049,0x000,0x000,0x1c0,0x038,0x007,0x000,0x1c0,0x038,0x007,0x000};

void AllOFF(){
  for(int i=0;i<(sizeof(led)/sizeof(int));i++){   //sizeof()はarray.length
    digitalWrite(led[i], LOW);
  }
  for(int i=0;i<(sizeof(layer)/sizeof(int));i++){  
    digitalWrite(layer[i], HIGH);
  }
}
void DynaDrive(int pat[],int cycletime){ //1msで上下のレイヤーを点灯消灯  配列の先頭のポインタが与えられる
  for(int n=0; n<cycletime/3;n++){
    for(int L=0; L < (sizeof(layer)/sizeof(int)); L++){
      for(int temp=0; temp<(sizeof(led)/sizeof(int)); temp++){
        if(bitRead(pat[L],temp) == B01){
          digitalWrite(led[temp],HIGH);
        }
        else{
          digitalWrite(led[temp],LOW);
        }
      }
      digitalWrite(layer[L],LOW);
      delayMicroseconds(500);
      digitalWrite(layer[L],HIGH);
      delayMicroseconds(500);
    }
  }
}

int lightVal = 0;
void readSensor()
{
  int val = analogRead(0);
  if ( val > SENSOR_MAX )
    lightVal = SENSOR_MAX;
  else
    lightVal = val;
}

void setup() {
  for(int i=0;i<(sizeof(led)/sizeof(int));i++){  
    pinMode(led[i], OUTPUT);
  }     
  for(int i=0;i<(sizeof(layer)/sizeof(int));i++){  
    pinMode(layer[i], OUTPUT);
  } 
  AllOFF(); 
  
  MsTimer2::set(25,readSensor);
  MsTimer2::start();

  Serial.begin(9600);
  Serial.print('\r');

}

char buf[256];

void loop() {

  for(int n=0; n<(sizeof(ANIM_1_TOP)/sizeof(int));n++){
    TEMP_LED[0]=ANIM_1_TOP[n];
    TEMP_LED[1]=ANIM_1_MID[n];
    TEMP_LED[2]=ANIM_1_BOT[n];
    int cyctime = map(lightVal,0,SENSOR_MAX,200,10);
    sprintf(buf,"vv:: %d %d",lightVal,cyctime);
    Serial.println(buf);
   
    DynaDrive(TEMP_LED,cyctime); //配列の先頭のポインタのみ与えられる。
    AllOFF();
  }
  for(int n=0; n<(sizeof(ANIM_2_TOP)/sizeof(int));n++){
    TEMP_LED[0]=ANIM_2_TOP[n];
    TEMP_LED[1]=ANIM_2_MID[n];
    TEMP_LED[2]=ANIM_2_BOT[n];
    int cyctime = map(lightVal,0,SENSOR_MAX,200,10);
    
    DynaDrive(TEMP_LED,cyctime); //配列の先頭のポインタのみ与えられる。
    AllOFF();
  }
}

