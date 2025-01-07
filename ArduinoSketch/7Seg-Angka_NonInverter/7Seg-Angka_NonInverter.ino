#include <TimerOne.h>
#include <Wire.h>

#define LATCH2 7
#define CLK2 6
#define DATA2 5
#define LATCH1 4
#define CLK1 3
#define DATA1 2

//Deklarasi Variabel Parsing
String dataIn;
String dt[10];
int p;
boolean parsing=false;
char inChar;
long target=0;
long lasttarget=0;
long actual=0;
long lastactual=0;

//bilangan HEX Array
byte digit[10]= {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x7, 0x7F, 0x6F};

long adigit1=0;
int adigit2=0;
int adigit3=0;
int adigit4=0;
int adigit5=0;
int adigit6=0;

long tdigit1=0;
int tdigit2=0;
int tdigit3=0;
int tdigit4=0;
int tdigit5=0;
int tdigit6=0;

int i;

void sixSegmentWriteAct(long nilai)
{
  adigit1 = nilai/100000;
  adigit2 = (nilai-(adigit1*100000))/10000;
  adigit3 = (nilai-((adigit1*10)+adigit2)*10000)/1000;
  adigit4 = (nilai-((adigit1*100)+(adigit2*10)+adigit3)*1000)/100;
  adigit5 = (nilai-((adigit1*1000)+(adigit2*100)+(adigit3*10)+adigit4)*100)/10;
  adigit6 = (nilai-((adigit1*10000)+(adigit2*1000)+(adigit3*100)+(adigit4*10)+adigit5)*10);
  lastactual=actual;
}

void sixSegmentWriteTrg(long nilai)
{
  tdigit1 = nilai/100000;
  tdigit2 = (nilai-(tdigit1*100000))/10000;
  tdigit3 = (nilai-((tdigit1*10)+tdigit2)*10000)/1000;
  tdigit4 = (nilai-((tdigit1*100)+(tdigit2*10)+tdigit3)*1000)/100;
  tdigit5 = (nilai-((tdigit1*1000)+(tdigit2*100)+(tdigit3*10)+tdigit4)*100)/10;
  tdigit6 = (nilai-((tdigit1*10000)+(tdigit2*1000)+(tdigit3*100)+(tdigit4*10)+tdigit5)*10);
  lasttarget=target;
}

void setup()
{
  Serial.begin(9600);
  
  pinMode(LATCH1, OUTPUT);
  pinMode(CLK1, OUTPUT);
  pinMode(DATA1, OUTPUT);
  pinMode(LATCH2, OUTPUT);
  pinMode(CLK2, OUTPUT);
  pinMode(DATA2, OUTPUT);

  Timer1.initialize(500000);
  Timer1.attachInterrupt(Sview);
}

void loop()
{
  if(Serial.available()>0) 
  {
    inChar = Serial.read();
    dataIn += inChar;
    if (inChar == '#') 
    {
      parsing = true;
    }
  }
  if(parsing)
  {
    parsingData();
    parsing=false;
    dataIn="";
    sixSegmentWriteAct(actual);
    sixSegmentWriteTrg(target);
    Serial.print(actual);
    Serial.print("  ");
    Serial.println(target);
  }
}

void Sview()
{
  digitalWrite(LATCH1, LOW);
  shiftOut(DATA1, CLK1, MSBFIRST, ~digit[adigit6]); // digitenam
  shiftOut(DATA1, CLK1, MSBFIRST, ~digit[adigit5]);  // digitlima
  shiftOut(DATA1, CLK1, MSBFIRST, ~digit[adigit4]); // digitempat
  shiftOut(DATA1, CLK1, MSBFIRST, ~digit[adigit3]);  // digittiga
  shiftOut(DATA1, CLK1, MSBFIRST, ~digit[adigit2]);   // digitdua
  shiftOut(DATA1, CLK1, MSBFIRST, ~digit[adigit1]);  // digitsatu
  digitalWrite(LATCH1, HIGH);

  digitalWrite(LATCH2, LOW);
  shiftOut(DATA2, CLK2, MSBFIRST, ~digit[tdigit6]); // digitenam
  shiftOut(DATA2, CLK2, MSBFIRST, ~digit[tdigit5]);  // digitlima
  shiftOut(DATA2, CLK2, MSBFIRST, ~digit[tdigit4]); // digitempat
  shiftOut(DATA2, CLK2, MSBFIRST, ~digit[tdigit3]);  // digittiga
  shiftOut(DATA2, CLK2, MSBFIRST, ~digit[tdigit2]);   // digitdua
  shiftOut(DATA2, CLK2, MSBFIRST, ~digit[tdigit1]);  // digitsatu
  digitalWrite(LATCH2, HIGH);
}

/*Parsing Data*/
void parsingData()
{
  int j=0;
   
  //inisialisasi variabel, (reset isi variabel)
  dt[j]="";
  
  //proses parsing data
  for(p=1;p<dataIn.length();p++)
  {
    //pengecekan tiap karakter dengan karakter (#) dan (,)
    if ((dataIn[p] == '#') || (dataIn[p] == ','))
    {
      //increment variabel j, digunakan untuk merubah index array penampung
      j++;
      dt[j]="";       //inisialisasi variabel array dt[j]
    }
    else
    {
      //proses tampung data saat pengecekan karakter selesai.
      dt[j] = dt[j] + dataIn[p];
    }
  }

  //Memasukan Hasil Parsing ke dalam Variabel
  target=dt[1].toDouble();
  actual=dt[2].toDouble();
}
/*END*/
