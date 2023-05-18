//huskylens
#include <HUSKYLENS.h>
#include <SoftwareSerial.h>

//nRF
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


//motor
const int fl_a=6, fl_b=5, fr_a=12, fr_b=11, bl_a=3, bl_b=4, br_a=10, br_b=9;
int fl_s=7, fr_s=13, bl_s=2, br_s=8, m_speed=150;
bool a=false;

//huskylens
HUSKYLENS huskylens;
int width, height, x, y;
void printResult(HUSKYLENSResult result);

//nRF(CE, CSN)
RF24 radio(SCE, SCSN);
const byte address[6] = "00001";


void setup() {
    //huskylens
    Serial.begin(115200);
    Wire.begin();
    while (!huskylens.begin(Wire))
    {
        Serial.println(F("Begin failed!"));
        Serial.println(F("1.Please recheck the \"Protocol Type\" in HUSKYLENS (General Settings>>Protocol Type>>I2C)"));
        Serial.println(F("2.Please recheck the connection."));
        delay(100);
    }
    
    //motor
    pinMode(fl_a, OUTPUT);
    pinMode(fl_b, OUTPUT);
    pinMode(fl_s, OUTPUT);
    pinMode(fr_a, OUTPUT);
    pinMode(fr_b, OUTPUT);
    pinMode(fr_s, OUTPUT);
    pinMode(bl_a, OUTPUT);
    pinMode(bl_b, OUTPUT);
    pinMode(bl_s, OUTPUT);
    pinMode(br_a, OUTPUT);
    pinMode(br_b, OUTPUT);
    pinMode(br_s, OUTPUT);

    //nRF
    //Serial.begin(9600);
    radio.begin();
    radio.openReadingPipe(0, address);
    radio.setPALevel(RF24_PA_MIN);
    radio.startListening();
}

void loop() {

  if (radio.available()) {
    char mode[32] = "";
    radio.read(&mode, sizeof(mode));
    Serial.println(mode);

    if(mode==1){
      while(mode==1){
        //huskylens request
        if (!huskylens.request()) {
        Serial.println(F("Fail to request data from HUSKYLENS, recheck the connection!"));
        car_stop();
        }
        else if(!huskylens.isLearned()) {
        Serial.println(F("Nothing learned, press learn button on HUSKYLENS to learn one!"));
        car_stop();
        }
        else if(!huskylens.available()) {
        Serial.println(F("No block or arrow appears on the screen!"));
        car_stop();
        }
        else
        {
          while (huskylens.available())
          {
            HUSKYLENSResult result = huskylens.read();
            printResult(result);  //sensor data - x,y,width(height)

            //car_algorithm
            if(width<200) {
              if( x<=180 and x>=140 ) car_forward();
              else if ( x<140 ) {
                while(x<=180 and x>=140){
                  car_leftturn();
                }        
              }
              else if ( x>180 ) {
                while(x<=180 and x>=140){
                  car_rightturn();
                } 
              }
            }
            else{
              car_stop();
            }
            delay(200);
        }    
      }
     }
      
    }
    else if (mode==2){
      if (radio.available()) {
      double x,y;
      radio.read(&x, sizeof(x));
      radio.read(&y, sizeof(y));
      Serial.println(x);
      Serial.println(y);
      }

      if(x>40 and y<40 and y>-40){
        car_forward();
      }
      else if(x<-40 and y<40 and y>-40){
        car_backward();
      }
      else if(y>40 and x<40 and x>-40){
        car_leftward();
      }
      else if(y<-40 and x<40 and x>-40){
        car_rightward();
      }
      else car_stop();
      
    }

    else{
      //huskylens request
    if (!huskylens.request()) {
      Serial.println(F("Fail to request data from HUSKYLENS, recheck the connection!"));
      car_stop();
    }
    else if(!huskylens.isLearned()) {
      Serial.println(F("Nothing learned, press learn button on HUSKYLENS to learn one!"));
      car_stop();
    }
    else if(!huskylens.available()) {
      Serial.println(F("No block or arrow appears on the screen!"));
      car_stop();
    }
    else
      {
        while (huskylens.available())
        {
            HUSKYLENSResult result = huskylens.read();
            printResult(result);  //sensor data - x,y,width(height)

            //car_algorithm
            if(width<200) {
              if( x<=180 and x>=140 ) car_forward();
              else if ( x<140 ) {
                while(x<=180 and x>=140){
                  car_leftturn();
                }        
              }
              else if ( x>180 ) {
                while(x<=180 and x>=140){
                  car_rightturn();
                } 
              }
            }
            else{
              car_stop();
            }
              delay(200);
        }    
      }
    }
    
  }
}

//huskylens sensor input
void printResult(HUSKYLENSResult result){
    if (result.command == COMMAND_RETURN_BLOCK){
        Serial.println(String()+F("Block:xCenter=")+result.xCenter+F(",yCenter=")+result.yCenter+F(",width=")+result.width+F(",height=")+result.height+F(",ID=")+result.ID);
        x=result.xCenter;
        y=re+sult.yCenter;
        width=result.width;
        height=result.height;
    }
    else if (result.command == COMMAND_RETURN_ARROW){
        Serial.println(String()+F("Arrow:xOrigin=")+result.xOrigin+F(",yOrigin=")+result.yOrigin+F(",xTarget=")+result.xTarget+F(",yTarget=")+result.yTarget+F(",ID=")+result.ID);
    }
    else{
        Serial.println("Object unknown!");
    }
}

//motor controll
void fl_forward() {
  digitalWrite(fl_a, a);
  digitalWrite(fl_b, !a);
  digitalWrite(fl_s, m_speed);
}

void fr_forward() {
  digitalWrite(fr_a, a);
  digitalWrite(fr_b, !a);
  digitalWrite(fr_s, m_speed);
}

void bl_forward() {
  digitalWrite(bl_a, a);
  digitalWrite(bl_b, !a);
  digitalWrite(bl_s, m_speed);
}

void br_forward() {
  digitalWrite(br_a, a);
  digitalWrite(br_b, !a);
  digitalWrite(br_s, m_speed);
}

void fl_backward() {
  digitalWrite(fl_a, !a);
  digitalWrite(fl_b, a);
  digitalWrite(fl_s, m_speed);
}

void fr_backward() {
  digitalWrite(fr_a, !a);
  digitalWrite(fr_b, a);
  digitalWrite(fr_s, m_speed);
}

void bl_backward() {
  digitalWrite(bl_a, !a);
  digitalWrite(bl_b, a);
  digitalWrite(bl_s, m_speed);
}

void br_backward() {
  digitalWrite(br_a, !a);
  digitalWrite(br_b, a);
  digitalWrite(br_s, m_speed);
}

//car controll
void car_forward(){
  fl_forward();
  fr_forward();
  bl_forward();
  br_forward();
}

void car_backward(){
  fl_backward();
  fr_backward();
  bl_backward();
  br_backward();
}

void car_leftward(){
  fl_backward();
  fr_forward();
  bl_forward();
  br_backward();
}

void car_rightward(){
   fl_forward();
   fr_backward();
   bl_backward();
   br_forward();
}

void car_leftturn(){
  fl_backward();
  fr_forward();
  bl_backward();
  br_forward();
}

void car_rightturn(){
  fl_forward();
  fr_backward();
  bl_forward();
  br_backward();
}

void car_stop() {
  digitalWrite(fl_a, a);
  digitalWrite(fl_b, a);
  digitalWrite(fr_a, a);
  digitalWrite(fr_b, a);
  digitalWrite(bl_a, a);
  digitalWrite(bl_b, a);
  digitalWrite(br_a, a);
  digitalWrite(br_b, a);
}
