#include <EEPROMEx.h>

int count=0;
int blister=0;
long target=0;

//Deklarasi Variabel Parsing
String dt[10];
int p;
String datadead1 = "";
String datadead4 = "";

void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP);

  EEPROMReadData();
}

void loop() {
  if(digitalRead(2)==LOW)
  {
    count=100;
    target=12093;
    blister=20;

    datadead1 = "*0," + String(count) + "," + String(target) + "," + String(blister) + ",0#";
    EEPROMWriteData();
  }

}

/*---EEPROM Data---*/
void EEPROMWriteData()
{
  char datadead2[80];
  
  String dd = datadead1;
  
  dd.toCharArray(datadead2,80);
  
  EEPROM.writeBlock<char>(50, datadead2, 80); //simpan data char array (15 karakter) ke address 0
  delay(50);
}
  

void EEPROMReadData()
{
  char datadead3[] = " ";
   
  EEPROM.readBlock<char>(50, datadead3, 80); //baca data char array di alamat 0
  
  datadead4 = String (datadead3);
  
  parsingData();
  datadead4="";

  Serial.print(count);
  Serial.print("  ");
  Serial.print(target);
  Serial.print("  ");
  Serial.println(blister);
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
