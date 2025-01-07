#include <Wire.h>
#include <TimerOne.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x20, 20, 4);

#define WRITE 3
#define SENSOR 2
#define BUZZER 11

int romstat=0;

int count=0;
float countt=0;
int fcountv=0;
int countv=0;
long lastcount=0;

long target=0;
long ftarget=0;
long lasttarget=0;

int blister=0;
int fblister=0;
float bundle=0;

float prc=0;
float bpm=0;

char SeKey;
char lastSeKey;
int x=0;

long Day=0;
int Hour =0;
int LTMinute=0;
int TMinute=0;
int Minute=0;
int Second=0;
int HighMillis=0;
int Rollover=0;
long lastupdate=0;
long uptimes=0;

char cbls[15];
char ctrg[15];
char ccnt[15];

/*----START KEYPAD----*/
//deklarasi akan jumlah baris dan kolom
//4 baris
const byte ROWS = 4; 
//4 kolom
const byte COLS = 4; 

char keys[ROWS][COLS] = { //penggambaran kondisi keypad ke dalam program
{'1','2','3','A'}, //
{'4','5','6','B'}, //
{'7','8','9','C'}, //
{'*','0','#','D'}  //
};

//untuk pin arduino yang dihubungkan dengan keypad
byte rowPins[ROWS] = {7, 8, 9, 10}; //pin untuk kontrol baris
byte colPins[COLS] = {3, 4, 5, 6}; //pin untuk kontrol kolom

Keypad keypad=Keypad(makeKeymap(keys),rowPins,colPins,ROWS,COLS); 
/*----END KEYPAD----*/

//Deklarasi Variabel Parsing
String dt[10];
int p;
String datadead1 = "";
String datadead4 = "";

void setup() {
  Serial.begin(9600);
  Wire.begin();
  lcd.begin();
  lcd.clear();

  pinMode(BUZZER, OUTPUT);
  pinMode(SENSOR, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(SENSOR),counterd,RISING);

  Timer1.initialize(500000);
  Timer1.attachInterrupt(uptime);
}

void loop() {
SeKey = keypad.getKey();

countt=countv+(count*blister);
prc=(countt/target)*100;
bundle=countt/blister;

if(x == 0)
{
  if((TMinute>=1)&&(TMinute>LTMinute))
  {
    bpm=countt/TMinute;
    LTMinute=TMinute;
  }
  
  lcd.setCursor(0, 0);
  lcd.print("ACT= " + String((int)countt) + " - " + String ((int)bundle));

  lcd.setCursor(16, 0);
  lcd.print(String((int)prc)+"%");

  lcd.setCursor(0, 1);
  lcd.print("TRG= " + String((int)target));

  lcd.setCursor(11, 1);
  lcd.print("BPM= " + String((int)bpm) + " ");

  lcd.setCursor(0, 2);
  lcd.print("UPT= " + String((int)Hour) + ":" + String((int)Minute) + ":" + String((int)Second) + " ");

  lcd.setCursor(0, 3);
  lcd.print("PRESS A TO CONFIG");
}

if(x == 1)
{ 
  lcd.setCursor(0,0);
  lcd.print("CONFIGURATION");
  lcd.setCursor(0,1);
  lcd.print("TARGET= " + String((int)target));
  lcd.setCursor(0,2);
  lcd.print("BLISTER= " + String((int)blister));

  lcd.setCursor(0, 3);
  lcd.print("D=SELECT A=NXT B=PRV");
}

if(x == 2)
{ 
  lcd.setCursor(0,0);
  lcd.print("ADJ ACTUAL");
  lcd.setCursor(0,1);
  lcd.print("ACT =" + String((float)countt,0));

  lcd.setCursor(0, 3);
  lcd.print("D=SELECT A=NXT B=PRV");
}

if(x == 3)
{ 
  lcd.setCursor(0,0);
  lcd.print("ADJ TARGET");
  lcd.setCursor(0,1);
  lcd.print("TRG =" + String((float)target,0));

  lcd.setCursor(0, 3);
  lcd.print("D=SELECT A=NXT B=PRV");
}

if(x == 4)
{ 
  lcd.setCursor(0,0);
  lcd.print("ADJ BLISTER");
  lcd.setCursor(0,1);
  lcd.print("BLS =" + String((float)blister,0));

  lcd.setCursor(0, 3);
  lcd.print("D=SELECT A=NXT B=PRV");
}

if(x == 5)
{ 
  lcd.setCursor(0,0);
  lcd.print("RESET CONFIG");

  lcd.setCursor(0, 3);
  lcd.print("D=SELECT A=NXT B=PRV");
}
  
  switch(SeKey)
  {
    case '0' ... '9': 
      break;
      
    case '#':
    lcd.clear();
    mincount();
    break;
  
    case '*':
    break;
    
    case 'A':
    x++;  
    lcd.clear();
      break;
      
    case 'B':
    x--;
    lcd.clear();
      break;
      
    case 'C':
      break;
      
    case 'D':
    if(x == 1)
    {
      lcd.clear();
      target=0;
      ftarget=0;
      blister=0;
      fblister=0;
      count=0;
      countt=0;
      countv=0;
      fcountv=0;
      
      settarget();
      setblister();
      lastupdate=millis();
    }
    if(x == 2)
    {
      lcd.clear();
      count=0;
      countt=0;
      setact();
    }
    if(x == 3)
    {
      lcd.clear();
      ftarget=0;
      target=0;
      settarget();
    }
    if(x == 4)
    {
      lcd.clear();
      fblister=0;
      blister=0;
      setblister();
    }
    break;if(x == 4)
    {
      lcd.clear();
      count=0;
      countt=0;
      fcountv=0;
      countv=0;
      target=0;
      ftarget=0;
      blister=0;
      fblister=0;
      prc=0;
      bpm=0;
    }
    break;
  }

  if(x > 5)
  {
    x = 0;
  }
  
  if(x < 0)
  {
    x = 5;
  }
  
  if((countt>=target)&&(target!=0))
  {
    digitalWrite(BUZZER, HIGH);
  }
  else
  {
    digitalWrite(BUZZER, LOW);
  }  

}

void counterd(){
  lastcount=count;
  if(target!=0)
  {
    count+=1;
  }
}

void setact(){
  lcd.setCursor(0,0);
  lcd.print("SET ACTUAL  ");

  SeKey = keypad.getKey();
  
  if(SeKey >= '0' && SeKey <= '9')
    {
      fcountv = fcountv * 10 + (SeKey - '0');
      lcd.setCursor(0,1);
      lcd.print(fcountv);
    }
  if(SeKey == 'C')
    {         
      lcd.clear();
      fcountv = 0;
      lcd.setCursor(0,1);
      lcd.print(fcountv);
    }
   
    if(SeKey == '*'){          
      lcd.clear();
      delay(1000);
      countv = fcountv;      
      return;
    }

    lcd.setCursor(0, 3);
    lcd.print("*=NEXT C=CLEAR");

setact();
}

void settarget(){
  lcd.setCursor(0,0);
  lcd.print("SET TARGET  ");

  SeKey = keypad.getKey();
  
  if(SeKey >= '0' && SeKey <= '9')
    {
      ftarget = ftarget * 10 + (SeKey - '0');
      lcd.setCursor(0,1);
      lcd.print(ftarget);
    }
  if(SeKey == 'C')
    {         
      lcd.clear();
      ftarget = 0;
      lcd.setCursor(0,1);
      lcd.print(ftarget);
    }
   
    if(SeKey == '*'){          
      lcd.clear();
      delay(1000);
      target = ftarget;      
      return;
    }

    lcd.setCursor(0, 3);
    lcd.print("*=NEXT C=CLEAR");

settarget();
}

void setblister(){
  lcd.setCursor(0,0);
  lcd.print("SET BLISTER  ");

  SeKey = keypad.getKey();
  
  if(SeKey >= '0' && SeKey <= '9')
    {
      fblister = fblister * 10 + (SeKey - '0');
      lcd.setCursor(0,1);
      lcd.print(fblister);
    }
  if(SeKey == 'C')
    {         
      lcd.clear();
      fblister = 0;
      lcd.setCursor(0,1);
      lcd.print(fblister);
    }
   
    if(SeKey == '*'){          
      lcd.clear();
      delay(1000);
      blister = fblister;      
      return;
    }

    lcd.setCursor(0, 3);
    lcd.print("*=NEXT C=CLEAR");

setblister();
}

/*---UPTIME Millis---*/
void uptime()
{
  if(millis()>=3000000000)
  { 
    HighMillis=1;
  }

  if(millis()<=100000&&HighMillis==1)
  {
    Rollover++;
    HighMillis=0;
  }

  long secsUp = ((millis()-lastupdate)/1000);
  
  Second = secsUp%60;
  Minute = (secsUp/60)%60;
  TMinute = (secsUp/60);
  Hour = (secsUp/(60*60))%24;
  Day = (Rollover*50)+(secsUp/(60*60*24));
  
  senddata();
}
/*---UPTIME END---*/

/*---Decrease Bundle Confirmation---*/
void mincount()
{
  lcd.setCursor(0, 0);
  lcd.print("Are you sure?");
  lcd.setCursor(0, 1);
  lcd.print("A = YES");
  lcd.setCursor(0, 2);
  lcd.print("B = NO");

  SeKey = keypad.getKey();

  if(SeKey == 'A')
  {
    lcd.clear();
    count=count-1;
    return;
  }

  if(SeKey == 'B')
  {
    lcd.clear();
    return;
  }
mincount();
}
/*---Decrease END---*/

/*---SEND DATA---*/
void senddata()
{
  String dt = "*0," + String (target) + "," + String ((int)countt) + "#";
  Serial.println(dt);
}

/*---END---*/

/*Parsing Data*/
void parsingData()
{
  int j=0;
   
  //inisialisasi variabel, (reset isi variabel)
  dt[j]="";
  
  //proses parsing data
  for(p=1;p<datadead4.length();p++)
  {
    //pengecekan tiap karakter dengan karakter (#) dan (,)
    if ((datadead4[p] == '#') || (datadead4[p] == ','))
    {
      //increment variabel j, digunakan untuk merubah index array penampung
      j++;
      dt[j]="";       //inisialisasi variabel array dt[j]
    }
    else
    {
      //proses tampung data saat pengecekan karakter selesai.
      dt[j] = dt[j] + datadead4[p];
    }
  }

  //Memasukan Hasil Parsing ke dalam Variabel
  count=dt[1].toFloat();
  target=dt[2].toInt();
  blister=dt[3].toFloat();
}
/*END*/
