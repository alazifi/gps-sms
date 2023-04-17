/*_____________________________________
Konfigurasi pin:

SIM800L <=> Arduino Nano
RX <=> 10
TX <=> 11

GPS <=> Arduino Nano
RX <=> 9
TX <=> 8
______________________________________*/

#include <TinyGPSPlus.h>
#include <AltSoftSerial.h>  
#include <TimeLib.h> 
#include <SoftwareSerial.h>
#include <Sim800L.h>      

#define RX_SIM 10
#define TX_SIM 11
#define NOMOR_TELEPON "+621111111"

TinyGPSPlus gps;
AltSoftSerial ssgps;
Sim800L GSM (TX_SIM, RX_SIM);

char  wjam[10],
      wtanggal[10],
      latitude[15],
      longitude[15],
      alt[15],
      hasil[150];
char* nomor;
bool kirim;


const int UTCkeWIB = 7;
time_t setting = 0;

void setup()
{
  Serial.begin(9600);
  ssgps.begin(9600);
  GSM.begin(4800);
}

void loop(){
  settingWaktuGPS();
  nomor = NOMOR_TELEPON;
  dataHasil().toCharArray(hasil, 150);
  kirim = GSM.sendSms(nomor, hasil);
  Serial.println(dataHasil());
}

void settingWaktuGPS() {
  while (ssgps.available()) {
    if (gps.encode(ssgps.read())) {
      setTime(gps.time.hour(), gps.time.minute(), gps.time.second(), gps.date.day(), gps.date.month(), gps.date.year());
      adjustTime(UTCkeWIB * SECS_PER_HOUR);
    }
  }
  if (timeStatus() != timeNotSet) {
    if (now() != setting) {
      setting = now();
      Serial.println(dataHasil());
      }
  }
}

String dataHasil(){
    dtostrf(gps.location.lat(),9,6,latitude);
    dtostrf(gps.location.lng(),10,6,longitude);
    dtostrf(gps.altitude.meters(),8,2,alt);

    sprintf (wjam, "%02d:%02d:%02d", hour(), minute(), second());
    String jam = String(wjam);
    
    sprintf (wtanggal, "%02d/%02d/%02d", day(), month(), year());
    String data = "SAYA BUTUH BANTUAN\nSaat ini saya berada di sini: \nwww.google.com/maps/place/"+String(latitude)+
                  ","+String(longitude)+
                  "\nPukul: "+jam+
                  "\nTanggal: "+String(wtanggal)+
                  //", Altitude: "+String(alt)+ //uncomment untuk mendapatkan nilai altitude
                  "";                          
    return data;
  }
