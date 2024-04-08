//RTC_DATA_ATTR - ovaj atribut sluzi da se promenljiva smesti u memoriju koja pripada RTC-u
//brojac koliko se puta kontroler resetovao 
RTC_DATA_ATTR int bootCount = 0; 
//int brojac = 0;

void setup(){
  Serial.begin(115200);
  delay(1000); 

  //inkrementacija i prikaz brojaca ciklusa budjenja
  ++bootCount;
  //brojac++;
  //Serial.println("Budjenje br: " + String(bootCount) + " brojac = " + String(brojac));
  Serial.println("Budjenje br: " + String(bootCount));
  
  //prikazi razlog budjenja
  print_wakeup_reason();

  //konfigurisi GPIO0 kao ext0 okidac za budjenje na nizak logicki nivo
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_0,LOW); // budi se samo na promenu stanja pina GPIO-0 na koji je povezan taster 

  //spavaj
  esp_deep_sleep_start();
}

void loop(){}

//prikaz razloga budjenja
void print_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;
  // esp_sleep_get_wakeup_cause() - ova funkcija koja je ugradjena ona nama vraca razlog budjenja
  wakeup_reason = esp_sleep_get_wakeup_cause(); 
  switch(wakeup_reason)
  {
    case 1: 
	  Serial.println("Budjenje uzrokovano spoljnim signalom koji koristi RTC_IO"); 
	  break;
    case 2: 
	  Serial.println("Budjenje uzrokovano spoljnim signalom koji koristi RTC_CNTL"); 
	  break;
    case 3: 
	  Serial.println("Budjenje uzrokovano tajmerom"); 
	  break;
    case 4: 
	  Serial.println("Budjenje uzrokovano touchpadom"); 
	  break;
    case 5: 
	  Serial.println("Budjenje uzrokovano ULP programom"); 
	  break;
    default: 
	  Serial.println("Budjenje nije uzrokovano dubokim snom"); 
	  break;
  }
}
