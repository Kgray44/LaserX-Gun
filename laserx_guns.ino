#include <Adafruit_NeoPixel.h>

#include <Arduino.h>
#include <IRsend.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>


const uint16_t RECV_PIN = 14;  //D10:17, D12:4, D6:14
#define LED_PIN    D9
#define LED_COUNT 9
#define Trigger D2
const uint16_t kIrLed = 16;//D11

IRrecv myReceiver(RECV_PIN);
IRsend irsend(kIrLed);

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

decode_results results;

static const long PURPLEir = 0x67228B44;
static const long REDir    = 0xC6D404B6; //0x78653B0E;
static const long BLUEir   = 0x2FFEA610;

// PURPLE
uint16_t rawDataPurple[] = {
  2888, 5987, 2893,
  2058, 848, 2052, 843, 2069, 795, 2104, 791,  // 0, 0, 0, 0
  2109, 1816, 2109, 837, 2063, 842, 2057, 848, // 1, 0, 0, 0
  2052, 1873, 2055, 840, 2059, 846, 2054, 841, // 1, 0, 0, 0
  2071, 794, 2106, 789, 2111, 795, 2105, 841,  // 0, 0, 0, 0
  1000
};

// RED
uint16_t rawDataRed[] = {
  2899, 5977, 2897,
  2065, 842, 2058, 849, 2051, 845, 2055, 842,  // 0, 0, 0, 0
  2058, 1869, 2056, 851, 2061, 826, 2074, 792, // 1, 0, 0, 0
  2108, 789, 2111, 836, 2064, 843, 2057, 840,  // 0, 0, 0, 0
  2060, 846, 2054, 843, 2057, 850, 2050, 847,  // 0, 0, 0, 0
  1000
};

// BLUE
uint16_t rawDataBlue[] = {
  2896, 5979, 2902,
  2060, 836, 2064, 843, 2057, 848, 2052, 844,  // 0, 0, 0, 0
  2055, 1870, 2055, 851, 2052, 844, 2065, 820, // 1, 0, 0, 0
  2080, 795, 2105, 1821, 2104, 792, 2111, 836, // 0, 1, 0, 0
  2063, 843, 2057, 838, 2062, 845, 2055, 840,  // 0, 0, 0, 0
  1000
};

int life = 2;

int myColor;

int timer1;
int timer2;
int timer3;

int trig;

int teamScan = 1;

void setup()
{
  Serial.begin(115200);
  Serial.println("Setting up IR...");
  myReceiver.enableIRIn(); // Start the receiver
  irsend.begin();
  
  delay(1000);

  
  Serial.println("Setting up trigger and LEDs...");
  pinMode(Trigger, INPUT);
  
  strip.begin();
  strip.show();
  strip.setBrightness(255);
  
  trig = digitalRead(Trigger);
  
  Serial.println("Waiting for team...");
  retry:
  while (digitalRead(Trigger) == LOW);
  Serial.println("Trigger pressed...");
  if (digitalRead(Trigger) == HIGH){
    Serial.println("Trigger high...");
    while (digitalRead(Trigger) == HIGH){
      Serial.println(timer1);
      timer1++;
      delay(1);
      digitalRead(Trigger);
      if (timer1 == 2000){
        timer1 = 0;
        goto complete;
      }
    }
    teamscan:
    Serial.println("teamScan...");
    if (teamScan == 4){teamScan=1;}
    if (teamScan == 1){
      for (int f=0;f<LED_COUNT;f++){
        strip.setPixelColor(f, strip.Color(255,0,255)); 
        strip.show();
        delay(20);
      }
      myColor = 1;
    }
    else if (teamScan == 2){
      for (int f=0;f<LED_COUNT;f++){
        strip.setPixelColor(f, strip.Color(255,0,0)); 
        strip.show();
        delay(20);
      }
      myColor = 3;
    }
    else if (teamScan == 3){
      for (int f=0;f<LED_COUNT;f++){
        strip.setPixelColor(f, strip.Color(0,0,255)); 
        strip.show();
        delay(20);
      }
      myColor = 2;
    }
    teamScan++;
    goto retry;
  }
  complete:
  if (myColor == 1){
    for (int k=0;k<5;k++){
      for (int f=0;f<LED_COUNT;f++){
        strip.setPixelColor(f, strip.Color(255,0,255)); 
        strip.show();
        delay(20);
      }
      delay(600);
      for (int f=0;f<LED_COUNT;f++){
        strip.setPixelColor(f, strip.Color(0,0,0)); 
        strip.show();
        delay(20);
      }
      delay(600);
    }
    for (int f=0;f<LED_COUNT;f++){
      strip.setPixelColor(f, strip.Color(255,0,255)); 
      strip.show();
      delay(20);
    }
    delay(3000);
    for (int f=LED_COUNT;f>1;f--){
      strip.setPixelColor(f, strip.Color(0,0,0)); 
      strip.show();
      delay(40);
    }
  }
  else if (myColor == 3){
    for (int k=0;k<5;k++){
      for (int f=0;f<LED_COUNT;f++){
        strip.setPixelColor(f, strip.Color(255,0,0)); 
        strip.show();
        delay(20);
      }
      delay(600);
      for (int f=0;f<LED_COUNT;f++){
        strip.setPixelColor(f, strip.Color(0,0,0)); 
        strip.show();
        delay(20);
      }
      delay(600);
    }
    for (int f=0;f<LED_COUNT;f++){
      strip.setPixelColor(f, strip.Color(255,0,0)); 
      strip.show();
      delay(20);
    }
    delay(3000);
    for (int f=LED_COUNT;f>1;f--){
      strip.setPixelColor(f, strip.Color(0,0,0)); 
      strip.show();
      delay(40);
    }
  }
  else if (myColor == 2){
    for (int k=0;k<5;k++){
      for (int f=0;f<LED_COUNT;f++){
        strip.setPixelColor(f, strip.Color(0,0,255)); 
        strip.show();
        delay(20);
      }
      delay(600);
      for (int f=0;f<LED_COUNT;f++){
        strip.setPixelColor(f, strip.Color(0,0,0)); 
        strip.show();
        delay(20);
      }
      delay(600);
    }
    for (int f=0;f<LED_COUNT;f++){
      strip.setPixelColor(f, strip.Color(0,0,255)); 
      strip.show();
      delay(20);
    }
    delay(3000);
    for (int f=LED_COUNT;f>1;f--){
      strip.setPixelColor(f, strip.Color(0,0,0)); 
      strip.show();
      delay(40);
    }
  }
  for (int f=2;f<LED_COUNT;f++){
    strip.setPixelColor(f, strip.Color(0,255,0)); 
    strip.show();
    delay(20);
  }
  delay(1000);
  myReceiver.resume();
}

void loop() {
  if (digitalRead(Trigger) == HIGH){
    Serial.println("Trigger pulled! Shooting...");
    for (int f=2;f<LED_COUNT;f++){
      strip.setPixelColor(f, strip.Color(0,0,0)); 
      strip.show();
    }
    shoot();
    for (int f=2;f<LED_COUNT;f++){
      strip.setPixelColor(f, strip.Color(0,255,0)); 
      strip.show();
      delay(90);
    }
  }

  if (myReceiver.decode(&results)){
    dump(&results);
    myReceiver.resume();
  }
}

void dump(decode_results *results) {
  serialPrintUint64(results->value, 16);
  Serial.println("");
  //normal mode
  if (results->value == PURPLEir && myColor == 1) {//PURPLEir
    Serial.println("Purple hit!!!!");
    life--;
    if (life == 0){
      die();
      life = 2;
    }
    delay(100);
  }
  else if (results->value == BLUEir && myColor == 3) {
    Serial.println("BLUE hit!!!!");
    life--;
    if (life == 0){
      die();
      life = 2;
    }
    delay(100);
  }
  else if (results->value == REDir && myColor == 2) {
    Serial.println("RED hit!!!!");
    life--;
    if (life == 0){
      die();
      life = 2;
    }
    delay(100);
  }
}

void die() {
  rainbow(9);
}

void rainbow(int wait) {
  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this outer loop:
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the single-argument hue variant. The result
      // is passed through strip.gamma32() to provide 'truer' colors
      // before assigning to each pixel:
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}

  
void shoot() {
  //irsend.sendRaw(rawDataBlue, 36, 38);//data, 67, 38

  if (myColor == 1){
    irsend.sendRaw(rawDataPurple, 36, 38);//data, 67, 38
  }
  else if (myColor == 3){
    irsend.sendRaw(rawDataRed, 36, 38);//data, 67, 38
  }
  else if (myColor == 2){
    irsend.sendRaw(rawDataBlue, 36, 38);//data, 67, 38
  }
}
