#include "ClickButton.h"
#include "OneWire.h"
#include "DallasTemperature.h"
#include "ds3231.h"
#include <Wire.h>

#define ONE_WIRE_BUS 4 

struct ts currentTime;
struct ts lastTime;

int statusLeds[] = {7,6};
int numberLeds[] = {11,10,9,8};
const int buttonPin1 = 2;
ClickButton button1(buttonPin1, LOW, CLICKBTN_PULLUP);
int state = 0;
int minTemp = 3;
int maxTemp = 10;
int currentTemp = 13;
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup()
{
  Serial.begin(9600);
  for(int i=0; i<sizeof(statusLeds) / sizeof(int);i++)
  {
    pinMode(statusLeds[i], OUTPUT);    
  }
  for(int i=0; i<sizeof(numberLeds) / sizeof(int);i++)
  {
    pinMode(numberLeds[i], OUTPUT);    
  }
  refreshTemperature();
  state0();

  Wire.begin();
  DS3231_init(DS3231_CONTROL_INTCN);

  button1.debounceTime   = 20;   // Debounce timer in ms
  button1.multiclickTime = 250;  // Time limit for multi clicks
  button1.longClickTime  = 1000; // time until "held-down clicks" register
  sensors.begin();
}


void loop()
{
  delay(10);
  if(timeToReadTemperature()){
    refreshTemperature();
  }
  button1.Update();

  // Save click codes in LEDfunction, as click codes are reset at next Update()
  if (button1.clicks != 0){
    int currentClicks = button1.clicks;

    if(state == 0){
      if(currentClicks == 1){
        state1();
      } else if(currentClicks == 2){
        state2();
      } else if(currentClicks == 3){
        state4();
      }
    } else if(state == 1){
      if(currentClicks == 1){
        state0();
      }
    } else if(state == 2){
      if(currentClicks == 1){
        state3();
      }
    } else if(state == 3){
      if(currentClicks == 1){
        state0();
      }
    } else if(state == 4){
      if(currentClicks == 1){
        state6();
      } else if(currentClicks == 2){
        state5();
      } else if(currentClicks == 3){
        state7();
      }
    } else if(state == 5){
      if(currentClicks == 1){
        state6();
      } else if(currentClicks == 2){
        state5();
      } else if(currentClicks == 3){
        state7();
      }
    } else if(state == 6){
      if(currentClicks == 1){
        state6();
      } else if(currentClicks == 2){
        state5();
      } else if(currentClicks == 3){
        state7();
      }
    } else if(state == 7){
      if(currentClicks == 1){
        state9();
      } else if(currentClicks == 2){
        state8();
      } else if(currentClicks == 3){
        state0();
      }
    } else if(state == 8){
      if(currentClicks == 1){
        state9();
      } else if(currentClicks == 2){
        state8();
      } else if(currentClicks == 3){
        state0();
      }
    } else if(state == 9){
      if(currentClicks == 1){
        state9();
      } else if(currentClicks == 2){
        state8();
      } else if(currentClicks == 3){
        state0();
      }
    }
  }

}

void stateStart(int stateNumber){
    Serial.print("[STATE] " + stateNumber);
    state = stateNumber;
    updateStatusLED(state);
}

void stateEnd(int stateNumber){
    Serial.println(" end");
}

void state0(){
  stateStart(0);
  clearNumber();
  stateEnd(0);
}

void state1(){
  stateStart(1);
  showNumber(currentTemp);
  stateEnd(1);
}

void state2(){
  stateStart(2);
  showNumber(minTemp);
  stateEnd(2);
}

void state3(){
  stateStart(3);
  clearNumber();
  showNumber(maxTemp);
  stateEnd(3);
}

void state4(){
  stateStart(4);
  clearNumber();
  showNumber(minTemp);
  stateEnd(4);
}

void state5(){
  Serial.print("[STATE]  5");
  state = 5;
  updateStatusLED(state);
  clearNumber();
  minTemp --;
  showNumber(minTemp);
  Serial.println(" end");
}

void state6(){
  stateStart(6);
  clearNumber();
  if(maxTemp > minTemp+1){
    minTemp ++;
  } else {
    showError();
  }
  showNumber(minTemp);
  stateEnd(6);
}

void state7(){
  stateStart(7);
  clearNumber();
  showNumber(maxTemp);
  stateEnd(7);
}

void state8(){
  stateStart(8);
  clearNumber();
  if(maxTemp > minTemp+1){
    maxTemp --;
  } else {
    showError();
  }
  showNumber(maxTemp);
  stateEnd(8);
}

void state9(){
  stateStart(9);
  clearNumber();
  maxTemp ++;
  showNumber(maxTemp);
  stateEnd(9);
}

void updateStatusLED(int state){
  int mode;
  switch(state) {
    case 0:
      mode = -1;
      break;
    case 1:
      mode = 0;
      break;
    case 2:
    case 3:
      mode = 1;
      break;
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
      mode = 2;
      break;

  }

  switch (mode) {
    case 0:
      digitalWrite(statusLeds[0], LOW);    
      digitalWrite(statusLeds[1], HIGH);
      break;
    case 1:
      digitalWrite(statusLeds[0], HIGH);    
      digitalWrite(statusLeds[1], HIGH);
      break;
    case 2:
      digitalWrite(statusLeds[0], HIGH);    
      digitalWrite(statusLeds[1], LOW);
      break;
    default:
      digitalWrite(statusLeds[0], LOW);    
      digitalWrite(statusLeds[1], LOW);
      break;
  }
}


void showNumber(int number){
  if(number<0){
    showNegativeNumber();
    number = abs(number);
  }
  while(number>15){
    showTooBigNumber();
    number -= 16;
  } 

  for(int i=sizeof(numberLeds)/ sizeof(int); i>=0 ;i--)
  {
    if(number>=pow(2,i)){
      analogWrite(numberLeds[i], 255);
      number-=pow(2,i);
    } else {
      digitalWrite(numberLeds[i], LOW);
    }
  }
  
}

boolean timeToReadTemperature(){
  DS3231_get(&currentTime);
  int lastTimeInSeconds = lastTime.mday*24*60*60+lastTime.hour*60*60+lastTime.min*60+lastTime.sec;
  int currentTimeInSeconds = currentTime.mday*24*60*60+currentTime.hour*60*60+currentTime.min*60+currentTime.sec;
  return (lastTimeInSeconds <= currentTimeInSeconds - 10);
}

void refreshTemperature(){
   sensors.requestTemperatures();
   currentTemp = sensors.getTempCByIndex(0);
   DS3231_get(&lastTime);
   Serial.print("[UPDATE] ");
   Serial.print(lastTime.mday);
   Serial.print("/");
   Serial.print(lastTime.mon);
   Serial.print("/");
   Serial.print(lastTime.year);
   Serial.print(" ");
   Serial.print(lastTime.hour);
   Serial.print(":");
   Serial.print(lastTime.min);
   Serial.print(".");
   Serial.print(lastTime.sec);
   Serial.print(" - ");
   Serial.print(currentTemp);
   Serial.println("C");

}

void showTooBigNumber(){
  for(int i=sizeof(numberLeds)/ sizeof(int); i>=0 ;i--){      
    digitalWrite(numberLeds[i], HIGH);
    delay(100);
    digitalWrite(numberLeds[i], LOW);
  }
}

void showNegativeNumber(){
  for(int i=0; i<sizeof(numberLeds)/ sizeof(int); i++){      
    digitalWrite(numberLeds[i], HIGH);
    delay(100);
    digitalWrite(numberLeds[i], LOW);
  }
}

void showError(){
  clearNumber();
  delay(300);
  for(int i=0; i<5; i++){
    for(int i=0; i<sizeof(numberLeds)/ sizeof(int); i++){      
      digitalWrite(numberLeds[i], HIGH); 
    }
    delay(300);
    for(int i=0; i<sizeof(numberLeds)/ sizeof(int); i++){      
      digitalWrite(numberLeds[i], LOW); 
    }
    delay(300);
  }

}

void clearNumber(){
  showNumber(0);
}
