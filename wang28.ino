#include <RCSwitch.h>
#include <ResponsiveAnalogRead.h>
RCSwitch mySwitch = RCSwitch();       //przewód sygnałowy w pinie 2

long przyciskBrightPlus = 5895685;       //przyciski pilota
long przyciskBrightMinus = 5895686;
long przyciskOnOff = 5895681;
long przycisk100 = 5895687;
long przycisk50 = 5895688;
long przycisk25 = 5895689;
long przyciskLightPilot = 5895684;
long przyciskModePlus = 5895691;
long przyciskModeMinus = 5895697;
long przyciskSpeedPlus = 5895695;
long przyciskSpeedMinus = 5895693;

#define dioda01 3  //diody na modelu
#define dioda02 5
#define dioda03 6 

#define diodaTryb1 A0  //diody trybów
#define diodaTryb2 A1
#define diodaTryb3 A2 

#define diodyLogo 9  // podświetlana nazwa modelu

#define przyciskLight 4 // przycisk trybu na modelu
#define przyciskPlus 7 // przycisk + na modelu
#define przyciskMinus 8 // przycisk - na modelu

byte jasnoscDiody01 = 0;   
byte jasnoscDiody02 = 0;
byte jasnoscDiody03 = 0;
byte jasnoscLogo = 0;
// byte jasnoscDiodyTrybu = 5;  
boolean dioda01Stan = true;  // stan diod wł./wył.
boolean dioda02Stan = true;
boolean dioda03Stan = true;
boolean diodyLogoStan = true;
int tryb = 0;   //tryb regulacji jasności
int mode = 1;   //tryb po nacisnięciu na pilocie mode
int fadeMode = 1; //zmienna do trybu fade
int modeSpeed = 30; //szybkość fade
//const int potencjometr = A1;
int potJasnosc = 0;
boolean DiodyTrybow = true;

//ResponsiveAnalogRead analog(potencjometr, true);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////  SETUP  ///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(9600);
  mySwitch.enableReceive(0);

  pinMode(dioda01, OUTPUT);
  pinMode(dioda02, OUTPUT);
  pinMode(dioda03, OUTPUT);

  pinMode(diodaTryb1, OUTPUT);
  pinMode(diodaTryb2, OUTPUT);
  pinMode(diodaTryb3, OUTPUT);

  pinMode(diodyLogo, OUTPUT);
  
  pinMode(przyciskLight, INPUT_PULLUP);
  pinMode(przyciskPlus, INPUT_PULLUP);
  pinMode(przyciskMinus, INPUT_PULLUP);
  
  for(int x=0; x<250; x=x+3){               //początkowe rozświetlenie logo
    jasnoscLogo = jasnoscLogo +3;
    przypisywanieJasnosci();
    delay(5);
  }

  for(int x=0; x<250; x++){               //początkowe rozświetlenie
    jasnoscDiody01 = jasnoscDiody01 +1;
    jasnoscDiody02 = jasnoscDiody02 +1;
    jasnoscDiody03 = jasnoscDiody03 +1;
    przypisywanieJasnosci();
    delay(5);
  }

  digitalWrite (diodaTryb1, 1);   //początkowe włączenie diod trybów
  digitalWrite (diodaTryb2, 1);
  digitalWrite (diodaTryb3, 1);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////  LOOP  ////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
void loop() {
  //analog.update();
  
/////////////////////////////////////////////////// STEROWANIE PILOTEM  ///////////////////////////////////////////////////////////
  if (mySwitch.available()){     
    if (mySwitch.getReceivedValue() == przyciskLightPilot){
      tryb = tryb +1;
      if(tryb == 5) tryb = 0;
      if(tryb == 0 && DiodyTrybow == true){
        digitalWrite (diodaTryb1,1);
        digitalWrite (diodaTryb2,1);
        digitalWrite (diodaTryb3,1);
      }
        if(tryb == 1 && DiodyTrybow == true){
        digitalWrite (diodaTryb1,1);
        digitalWrite (diodaTryb2,0);
        digitalWrite (diodaTryb3,0);
      }
        if(tryb == 2 && DiodyTrybow == true){
        digitalWrite (diodaTryb1,0);
        digitalWrite (diodaTryb2,1);
        digitalWrite (diodaTryb3,0);
      }
        if(tryb == 3 && DiodyTrybow == true){
        digitalWrite (diodaTryb1,0);
        digitalWrite (diodaTryb2,0);
        digitalWrite (diodaTryb3,1);
      }
        if(tryb == 4){
        digitalWrite (diodaTryb1,0);
        digitalWrite (diodaTryb2,0);
        digitalWrite (diodaTryb3,0);
      }
      delay(300);
    }

    if(tryb == 0) sterowaniePilotemTryb0();
    if(tryb == 1) sterowaniePilotemTryb1();
    if(tryb == 2) sterowaniePilotemTryb2();
    if(tryb == 3) sterowaniePilotemTryb3();
    if(tryb == 4) sterowaniePilotemTryb4();

    if (mySwitch.getReceivedValue() == przyciskModePlus) {
      mode = mode+1;
      if(mode>2){
        mode=1;
        jasnoscDiody01 = 250;
        jasnoscDiody02 = 250;
        jasnoscDiody03 = 250;
        przypisywanieJasnosci();
      }
      delay(300);
    }

    if (mySwitch.getReceivedValue() == przyciskModeMinus) {
      mode = mode-1;
      if(mode<1) {
        mode=2;
      }
      if(mode==1){
        jasnoscDiody01 = 250;
        jasnoscDiody02 = 250;
        jasnoscDiody03 = 250;
        przypisywanieJasnosci();
      }
      delay(300);
    }
      
    if (mySwitch.getReceivedValue() == przyciskSpeedPlus && modeSpeed>1){ // zmiana szybkości automatycznego rozświetlania
      modeSpeed = modeSpeed -1;
      delay(10);
    }
    if (mySwitch.getReceivedValue() == przyciskSpeedMinus && modeSpeed<60){
      modeSpeed = modeSpeed +1;
      delay(10);
    }
    mySwitch.resetAvailable();
  }
 /////////////////////////////////////////////////  PRZYCISK ZMIANY TRYBU NA MODELU  ///////////////////////////////////////////////
  
  if (!digitalRead(przyciskLight)){ //przełączanie trybu zmiany jasności
    tryb = tryb +1;
    if(tryb == 5) tryb = 0;
    if(tryb == 0 && DiodyTrybow == true){
      digitalWrite (diodaTryb1,1);
      digitalWrite (diodaTryb2,1);
      digitalWrite (diodaTryb3,1);
    }
    if(tryb == 1 && DiodyTrybow == true){
      digitalWrite (diodaTryb1,1);
      digitalWrite (diodaTryb2,0);
      digitalWrite (diodaTryb3,0);
    }
    if(tryb == 2 && DiodyTrybow == true){
      digitalWrite (diodaTryb1,0);
      digitalWrite (diodaTryb2,1);
      digitalWrite (diodaTryb3,0);
    }
    if(tryb == 3 && DiodyTrybow == true){
      digitalWrite (diodaTryb1,0);
      digitalWrite (diodaTryb2,0);
      digitalWrite (diodaTryb3,1);
    }
    if(tryb == 4 && DiodyTrybow == true){
      digitalWrite (diodaTryb1,0);
      digitalWrite (diodaTryb2,0);
      digitalWrite (diodaTryb3,0);
    }
    delay(300);
    while(digitalRead(przyciskLight) == LOW){}
  }


/////////////////////////////////////  STEROWANIE JASNOŚCIĄ PRZEZ PRZYCISKI NA MODELU  ////////////////////////////////////////////////////
                                                                       
  if (!digitalRead(przyciskPlus)){
    if (tryb == 0 || tryb == 1){
      if (jasnoscDiody01 < 250) jasnoscDiody01 = jasnoscDiody01 +1;
    }
    if (tryb == 0 || tryb == 2){
      if (jasnoscDiody02 < 250) jasnoscDiody02 = jasnoscDiody02 +1;
    }
    if (tryb == 0 || tryb == 3){
      if (jasnoscDiody03 < 250) jasnoscDiody03 = jasnoscDiody03 +1;
    }
    if (tryb == 4){
      if (jasnoscLogo < 250) jasnoscLogo = jasnoscLogo +1;
    }
    delay(20);
    przypisywanieJasnosci();
  }

  if (!digitalRead(przyciskMinus)){
    if (tryb == 0 || tryb == 1){
      if (jasnoscDiody01 > 0) jasnoscDiody01 = jasnoscDiody01 -1;
    }
    if (tryb == 0 || tryb == 2){
      if (jasnoscDiody02 > 0) jasnoscDiody02 = jasnoscDiody02 -1;
    }
      if (tryb == 0 || tryb == 3){
    if (jasnoscDiody03 > 0) jasnoscDiody03 = jasnoscDiody03 -1;
    }
    if (tryb == 4){
      if (jasnoscLogo > 0) jasnoscLogo = jasnoscLogo -1;
    }
  delay(20);
  przypisywanieJasnosci();
  }
   
/*
/////////////////////////////////////  STEROWANIE JASNOŚCIĄ PRZEZ POTENCJOMETR  ////////////////////////////////////////////////////

potJasnosc = map (analog.getValue(),0,1023,0,250 );

 if (analog.hasChanged() && tryb == 0){ 
   if (jasnoscDiody01 < 250)jasnoscDiody01 = potJasnosc;      
   if (jasnoscDiody02 < 250)jasnoscDiody02 = potJasnosc;
   if (jasnoscDiody03 < 250)jasnoscDiody03 = potJasnosc;            
    przypisywanieJasnosci();
    delay(1);}

 if (analog.hasChanged() && tryb == 0){
   if (jasnoscDiody01 > 0)jasnoscDiody01 = potJasnosc;
   if (jasnoscDiody02 > 0)jasnoscDiody02 = potJasnosc;
   if (jasnoscDiody03 > 0)jasnoscDiody03 = potJasnosc;       
    przypisywanieJasnosci();
    delay(1);}

 if (analog.hasChanged() && tryb == 1){ 
  jasnoscDiody01 = potJasnosc;                
  przypisywanieJasnosci();
  delay(1);}
 if (analog.hasChanged() && tryb == 1){
  jasnoscDiody01 = potJasnosc;                
  przypisywanieJasnosci();
  delay(1);}
 if (analog.hasChanged() && tryb == 2){
  jasnoscDiody02 = potJasnosc;                
  przypisywanieJasnosci();
  delay(1);}
 if (analog.hasChanged() && tryb == 2){
  jasnoscDiody02 = potJasnosc;                
  przypisywanieJasnosci();
  delay(1);}
 if (analog.hasChanged() && tryb == 3){ 
  jasnoscDiody03 = potJasnosc;                
  przypisywanieJasnosci();
  delay(1);}
 if (analog.hasChanged() && tryb == 3){ 
  jasnoscDiody03 = potJasnosc;                
  przypisywanieJasnosci();
  delay(1);}
*/

//////////////////////////////////////////////// tryb automatycznego rozświetlania /////////////////////////////////////////////////

  if (mode == 2 && fadeMode == 1)automatyczneRozswietlanie(); 
  if (mode == 2 && fadeMode == 2)automatycznePrzygaszanie(); 
   
//////////////////////////////////////////////// Zgaszenie diod trybów na modelu ///////////////////////////////////////////////////

  if (!digitalRead(przyciskPlus) && !digitalRead(przyciskMinus)){
    digitalWrite (diodaTryb1,0);
    digitalWrite (diodaTryb2,0);
    digitalWrite (diodaTryb3,0);
    DiodyTrybow = !DiodyTrybow;
    if(tryb == 0 && DiodyTrybow == true){
      digitalWrite (diodaTryb1,1);
      digitalWrite (diodaTryb2,1);
      digitalWrite (diodaTryb3,1);
    }
    if(tryb == 1 && DiodyTrybow == true){
      digitalWrite (diodaTryb1,1);
      digitalWrite (diodaTryb2,0);
      digitalWrite (diodaTryb3,0);
    }
    if(tryb == 2 && DiodyTrybow == true){
      digitalWrite (diodaTryb1,0);
      digitalWrite (diodaTryb2,1);
      digitalWrite (diodaTryb3,0);
    }
    if(tryb == 3 && DiodyTrybow == true){
      digitalWrite (diodaTryb1,0);
      digitalWrite (diodaTryb2,0);
      digitalWrite (diodaTryb3,1);
    }
    delay(100);
  }

//////////////////////////////////////////////////////  SERIAL PRINT  //////////////////////////////////////////////////////////////
  // Serial.println(mySwitch.getReceivedValue());
  // Serial.println(jasnoscLogo);
  /*Serial.print("\t");
  
  // Serial.println(jasnoscDiody01);
  Serial.print("\t");
  Serial.print(jasnoscDiody02);
    Serial.print("\t");
  Serial.print(jasnoscDiody03);
  Serial.print("\t");
  //Serial.print("   Fade delay - ");    
  //Serial.println(modeSpeed);
  // Serial.print("\t");
      
  // if(analog.hasChanged()) {
      Serial.print("\tchanged");}
  */

}

/////////////////////////////////////////////////////////  FUNKCJE  ////////////////////////////////////////////////////////////////

  void automatyczneRozswietlanie(){                  //tryb automatycznego rozświetlania
    jasnoscDiody01 = jasnoscDiody01 +1;
    jasnoscDiody02 = jasnoscDiody02 +1;
    jasnoscDiody03 = jasnoscDiody03 +1;
    przypisywanieJasnosci();
    delay(modeSpeed);
    if(jasnoscDiody01 > 250) fadeMode=2;
  }

  void automatycznePrzygaszanie(){                    //tryb automatycznego przygaszania
    jasnoscDiody01 = jasnoscDiody01 -1;
    jasnoscDiody02 = jasnoscDiody02 -1;
    jasnoscDiody03 = jasnoscDiody03 -1;
    przypisywanieJasnosci();
    delay(modeSpeed);
    if(jasnoscDiody01 < 3) fadeMode=1;
  }

  void przypisywanieJasnosci(){
    if (dioda01Stan== true) analogWrite(dioda01, jasnoscDiody01);  
    else analogWrite(dioda01, 0);                                          // przypisywanie jasności
    if (dioda02Stan== true) analogWrite(dioda02, jasnoscDiody02);
    else analogWrite(dioda02, 0);
    if (dioda03Stan== true) analogWrite(dioda03, jasnoscDiody03);
    else analogWrite(dioda03, 0);
    if (diodyLogoStan== true) analogWrite(diodyLogo, jasnoscLogo);
    else analogWrite(diodyLogo, 0);
  }
 
//////////////////////////////////////////////////////////  TRYB 0  ////////////////////////////////////////////////////////////////

  void sterowaniePilotemTryb0(){                                                                           
    long value = mySwitch.getReceivedValue();
    if (value == przyciskBrightPlus && jasnoscDiody01 < 250) jasnoscDiody01 = jasnoscDiody01 +5;
    if (value == przyciskBrightPlus && jasnoscDiody02 < 250) jasnoscDiody02 = jasnoscDiody02 +5;
    if (value == przyciskBrightPlus && jasnoscDiody03 < 250) jasnoscDiody03 = jasnoscDiody03 +5;
    if (value == przyciskBrightMinus && jasnoscDiody01 > 0) jasnoscDiody01 = jasnoscDiody01 -5;
    if (value == przyciskBrightMinus && jasnoscDiody02 > 0) jasnoscDiody02 = jasnoscDiody02 -5;
    if (value == przyciskBrightMinus && jasnoscDiody03 > 0) jasnoscDiody03 = jasnoscDiody03 -5;      
    
    if (value == przyciskOnOff){
      dioda01Stan = !dioda01Stan;
      dioda02Stan = !dioda02Stan; 
      dioda03Stan = !dioda03Stan;
      przypisywanieJasnosci();
      delay(300);
    }
    if (value == przycisk50){
      jasnoscDiody01 = 100;
      jasnoscDiody02 = 100;  
      jasnoscDiody03 = 100;
      delay(100);
    }
    if (value == przycisk100){
      jasnoscDiody01 = 250;
      jasnoscDiody02 = 250;
      jasnoscDiody03 = 250;
      delay(100);
    }
    if (value == przycisk25){
      jasnoscDiody01 = 30;
      jasnoscDiody02 = 30;
      jasnoscDiody03 = 30;
      delay(100);
    }
    przypisywanieJasnosci();
  }
   
//////////////////////////////////////////////////////////  TRYB 1  ////////////////////////////////////////////////////////////////

  void sterowaniePilotemTryb1(){                                         
    long value = mySwitch.getReceivedValue();

    if (value == przyciskBrightPlus && jasnoscDiody01 < 250) jasnoscDiody01 = jasnoscDiody01 +5;
    if (value == przyciskBrightMinus && jasnoscDiody01 > 0) jasnoscDiody01 = jasnoscDiody01 -5;     

    if (value == przyciskOnOff){
      dioda01Stan = !dioda01Stan;
      delay(300);
    }
    if (value == przycisk50){
      jasnoscDiody01 = 100;
      delay(100);
    }
    if (value == przycisk100){
      jasnoscDiody01 = 250;
      delay(100);
    }
    if (value == przycisk25){
      jasnoscDiody01 = 30;
      delay(100);
    }
    przypisywanieJasnosci();
  }
   
//////////////////////////////////////////////////////////  TRYB 2  ////////////////////////////////////////////////////////////////

  void sterowaniePilotemTryb2(){                                        
    long value = mySwitch.getReceivedValue();

    if (value == przyciskBrightPlus && jasnoscDiody02 < 250) jasnoscDiody02 = jasnoscDiody02 +5;
    if (value == przyciskBrightMinus && jasnoscDiody02 > 0) jasnoscDiody02 = jasnoscDiody02 -5;     
    
    if (value == przyciskOnOff){
      dioda02Stan = !dioda02Stan;
      delay(300);
    }
    if (value == przycisk50){
      jasnoscDiody02 = 100;
      delay(100);
    }
    if (value == przycisk100){
      jasnoscDiody02 = 250;
      delay(100);
    }
    if (value == przycisk25){
      jasnoscDiody02 = 30;
      delay(100);
    }
    przypisywanieJasnosci();
  }

//////////////////////////////////////////////////////////  TRYB 3  ////////////////////////////////////////////////////////////////

  void sterowaniePilotemTryb3(){                                
    long value = mySwitch.getReceivedValue();

    if (value == przyciskBrightPlus && jasnoscDiody03 < 250) jasnoscDiody03 = jasnoscDiody03 +5;
    if (value == przyciskBrightMinus && jasnoscDiody03 > 0) jasnoscDiody03 = jasnoscDiody03 -5;     
    
    if (value == przyciskOnOff){
      dioda03Stan = !dioda03Stan;
      delay(300);
    }
    if (value == przycisk50){
      jasnoscDiody03 = 100;
      delay(100);
    }
    if (value == przycisk100){
      jasnoscDiody03 = 250;
      delay(100);
    }
    if (value == przycisk25){
      jasnoscDiody03 = 30;
      delay(100);
    }
    przypisywanieJasnosci();
  }
//////////////////////////////////////////////////////////  TRYB 4  ////////////////////////////////////////////////////////////////

  void sterowaniePilotemTryb4(){                                
    long value = mySwitch.getReceivedValue();

    if (value == przyciskBrightPlus && jasnoscLogo < 250) jasnoscLogo = jasnoscLogo +5;
    if (value == przyciskBrightMinus && jasnoscLogo > 0) jasnoscLogo = jasnoscLogo -5;     
    
    if (value == przyciskOnOff){
      diodyLogoStan = !diodyLogoStan;
      delay(300);
    }
    if (value == przycisk50){
      jasnoscLogo = 100;
      delay(100);
    }
    if (value == przycisk100){
      jasnoscLogo = 250;
      delay(100);
    }
    if (value == przycisk25){
      jasnoscLogo = 30;
      delay(100);
    }
    przypisywanieJasnosci();
  }

