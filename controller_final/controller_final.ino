//gyro sensor
#include<Wire.h>

//nRF
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//nRF
const int MCE=12, MCSN=13;
RF24 radio(MCE, MCSN);
const byte address[6] = "00001";

//controller
const int btn = 11, LEDG=6, LEDY=5, LEDB=4;
int modeN = 1;

//gyro sensor
const int MPU_ADDR = 0x68;    // I2C통신을 위한 MPU6050의 주소
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;   // 가속도(Acceleration)와 자이로(Gyro)
double angleAcX, angleAcY;

const double RADIAN_TO_DEGREE = 180 / 3.14159;  
const double DEG_PER_SEC = 32767 / 250;    // 1초에 회전하는 각도
const double ALPHA = 1 / (1 + 0.04);

unsigned long now = 0;   // 현재 시간 저장용 변수
unsigned long past = 0;  // 이전 시간 저장용 변수
double dt = 0;           // 한 사이클 동안 걸린 시간 변수 

double averAcX, averAcY, averAcZ;
double averGyX, averGyY, averGyZ;

void setup() {
  
  //nRF
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN); //RF24_PA_MIN / RF24_PA_LOW / RF24_PA_HIGH / RF24_PA_MAX 
  radio.stopListening();
  
  initSensor();
  Serial.begin(115200);
  caliSensor();   //  초기 센서 캘리브레이션 함수 호출
  past = millis(); // past에 현재 시간 저장 

  //controller
  pinMode(btn,INPUT_PULLUP);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDY, OUTPUT);
  pinMode(LEDB, OUTPUT);
}

void loop() {
  if (digitalRead(btn)==HIGH){
    modeN+=1;
    if(modeN>2) modeN=1;
    delay(500);
  }
  
  if(modeN==1) {
    digitalWrite(LEDG,HIGH);
    digitalWrite(LEDY,LOW);
    digitalWrite(LEDB,LOW);

    const int mode = 1;
    radio.write(&mode, sizeof(mode));
  }
  else if(modeN==2) {
    digitalWrite(LEDG,LOW);
    digitalWrite(LEDY,HIGH);
    digitalWrite(LEDB,LOW);

    const int mode[32] = 2;
    radio.write(&mode, sizeof(mode));
    Serial.println(mode);

    getData(); 
    getDT();
    angleAcX = atan(AcY / sqrt(pow(AcX, 2) + pow(AcZ, 2)));
    angleAcX *= RADIAN_TO_DEGREE;
    angleAcY = atan(-AcX / sqrt(pow(AcY, 2) + pow(AcZ, 2)));
    angleAcY *= RADIAN_TO_DEGREE;
  
    Serial.print("AngleAcX:");
    Serial.print(angleAcX);
    Serial.print("\t AngleAcY:");
    Serial.println(angleAcY);

    const double x = angleAcX;
    const double y = angleAcY;
    radio.write(&x, sizeof(x));
    radio.write(&y, sizeof(y));
    delay(100);
  }

}

void initSensor() {
  Wire.begin();
  Wire.beginTransmission(MPU_ADDR);   // I2C 통신용 어드레스(주소)
  Wire.write(0x6B);    // MPU6050과 통신을 시작하기 위해서는 0x6B번지에    
  Wire.write(0);
  Wire.endTransmission(true);
}

void getData() {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);   // AcX 레지스터 위치(주소)를 지칭합니다
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 14, true);  // AcX 주소 이후의 14byte의 데이터를 요청
  AcX = Wire.read() << 8 | Wire.read(); //두 개의 나뉘어진 바이트를 하나로 이어 붙여서 각 변수에 저장
  AcY = Wire.read() << 8 | Wire.read();
  AcZ = Wire.read() << 8 | Wire.read();
  Tmp = Wire.read() << 8 | Wire.read();
  GyX = Wire.read() << 8 | Wire.read();
  GyY = Wire.read() << 8 | Wire.read();
  GyZ = Wire.read() << 8 | Wire.read();
}

// loop 한 사이클동안 걸리는 시간을 알기위한 함수
void getDT() {
  now = millis();   
  dt = (now - past) / 1000.0;  
  past = now;
}

// 센서의 초기값을 10회 정도 평균값으로 구하여 저장하는 함수
void caliSensor() {
  double sumAcX = 0 , sumAcY = 0, sumAcZ = 0;
  double sumGyX = 0 , sumGyY = 0, sumGyZ = 0;
  getData();
  for (int i=0;i<10;i++) {
    getData();
    sumAcX+=AcX;  sumAcY+=AcY;  sumAcZ+=AcZ;
    sumGyX+=GyX;  sumGyY+=GyY;  sumGyZ+=GyZ;
    delay(50);
  }
  averAcX=sumAcX/10;  averAcY=sumAcY/10;  averAcZ=sumAcY/10;
  averGyX=sumGyX/10;  averGyY=sumGyY/10;  averGyZ=sumGyZ/10;
}
