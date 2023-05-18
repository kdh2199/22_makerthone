# W.T.F - (Wagon That Follows human)

## 2022 makeathon project

### summary

* 인공지능 카메라 모듈 (HuskeyLens)
  * 물체 분류 기술 (Machine Learnigng)
  * 학습한 물체 추적 (object tracking)
* 아두이노 컨드롤러
  * 3축 자이로센서 (MPU-6050)
  * nRF 무선 통신 (nRF24L01)
* 기능 및 목적
  * 인공지능 카메라를 활용하여 특정한 사물, 사람을 따라가도록 할 수 있는 다용도 로봇
  * 인공지능 카메라, 무선 컨트롤러, 스마트폰 등 다양한 방식으로 자유롭게 컨트롤
  * 사람들이 직접 갈 수 없는 지역에서 다양한 작업 / 운송
  * 일상생활에서도 다양한 목적으로 활용 가능


----------

### about project

* H/W
  * main body
    * Arduino (Arduino Mega 2560)
    * AI camera module (HuskyLens)
    * nRF module (nRF24L01)
    * bluetooth module (HC-05)
    * motor driver (L298N) * 2
    * motor * 4
    * etc. (LED, Battery, wheels, other sensors...)
  * nRF controller
    * gyto sensor (MPU-6050)
    * nRF module (nRF24L01)
    * etc. (LED, Battery...)

![image](https://github.com/kdh2199/22_makerthone/assets/68812665/51539ee9-15f0-46ec-8ef5-0239f28d6120)
![image](https://github.com/kdh2199/22_makerthone/assets/68812665/f35f52ae-64e2-4890-b1b8-c2bd0a824cc6)

 
 *S/W
  * Arduino IDE
    * Argorithm
      * Mode1 - 인공지능과 알고리즘으로 자동으로 작동

         AI 카메라 모듈(HuskeyLens)로 학습시킨 물체의 카메라 화면에서 x좌표, y좌표, 크기 값을 실시간으로 입력
         
         화면의 너비(x좌표 범위)를 3등분
         
          - 물체의 x좌표가 왼쪽에 있으면 왼쪽으로 회전
         
          - 물체의 x좌표가 오른쪽에 있으면 오른쪽으로 회전
         
          - 물체의 x좌표가 중앙에 있으면 회전 없이 직진
         
         물체의 크기값에 따라 모터의 속도 배수 조절
         
       * Mode2 - 컨트롤러를 통해 수동으로 작동
          
          컨트롤러에서 nRF 통신으로 3축중 2개의 축(x축, y축)의 기울기 값을 실시간으로 입력
          
          - x축의 기울기가 일정값 이상으로 커지면 무선 통신을 통해 본체를 오른쪽으로 이동
          - x축의 기울기가 일정값 이하로 내려가면 무선 통신을 통해 본체를 왼쪽으로 이동

          
          - y축의 기울기가 일정값 이상으로 커지면 무선 통신을 통해 본체를 앞으로 이동
          - y축의 기울기가 일정값 이하로 내려가면 무선 통신을 통해 본체를 뒤로 이동
