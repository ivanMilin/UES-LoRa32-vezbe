/*
  • SPIFFS (SPI Flash File System) organizuje memorijski prostor eksterne flash 
    memorije na sličan način kao na USB memorijskim modulima, ali jednostavnije 
    i sa više ograničenja (npr. nisu podržani direktorijumi) 
  
  • Osnovne funkcionalnosti uključuju čitanje, upis, zatvaranje i brisanje datoteka 
  • Najčešće primene SPIFFS na ESP32 ploči (kapaciteta 8MB): 
    oKreiranje konfiguracionih datoteka sa podešavanjima 
    oLogovanje podataka 
    oČuvanje HTML i CSS datoteka za realizaciju web servera 
    oČuvanje slika i ikonica
*/

#include "SPIFFS.h"
void setup()
{
  Serial.begin(115200);
  
  if(!SPIFFS.begin(true))
  {
      Serial.println("Greska prilikom inicijalizacije SPIFFS");
      return;
  }
  
  File file = SPIFFS.open("/primer.txt");
  
  if(!file)
  {
      Serial.println("Greska prilikom otvaranja datoteke");
      return;
  }
  
  Serial.println("Sadrzaj datoteke:");
  while(file.available())
      Serial.write(file.read());
  
  file.close();
  Serial.println("");
}

void loop()
{
}
