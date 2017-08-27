#include <Adafruit_NeoPixel.h>
 
#define PIN            6 //シリアル通信GPIOピン番号
//#define MAXPIXELS      30 //LED素子の最大数
#define MAXPIXELS     60
 
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(MAXPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int type = 0; // 心拍のタイプ 1:通常, 2:ドキドキ, 3:ばくばく
int MAXBRIGHTNESS = 255;
int flags[MAXPIXELS]; //足すか足さないか
int values[MAXPIXELS]; //各LEDの明るさ
int increment[MAXPIXELS]; //各フレームで足す数値

int plus[3] = {15, 30, 120}; // かがやきMaxまでのはやさ
int minus[3] = {-15, -30, -120}; // かがやき0までの速さ
int interval[3] = {40, 30, 20}; //次のドクンまでの間隔

bool changeFlg = false; // 次のタイミングでタイプを変えるかどうか
int changeType = 0; // 次のタイミングでこのタイプに変更

// --- < nagahora > --- //
double Duration = 0;
double Distance = 0;
int trigPin = 3;
int echoPin = 2;
int prevTime = 0;
// --- < /nagahora > --- //


//
//
void setup() {
  Serial.begin(19200);

  // LED初期化
  for (int i = MAXPIXELS - 1; i + 1; i--) {
    flags[i] = 0;
    values[i] = 0;
    increment[i] = plus[type];
  }
  
  pixels.begin();
  pixels.setBrightness(MAXBRIGHTNESS);


  // --- nagahora 超音波センサ初期化
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
  
}

void loop() {

//  char data = '0';

  // まずはoFからの信号を見る
  if (Serial.available() > 0) {
    char data = Serial.read();
    if (data == '1') {
      
      // 信号があればフェーズ1へ
      // data = '1';
      changeType = 1;
      changeFlg = true;
      Serial.println("hellllllllll");
    }
  }

  // --- < nagahora > --- //
  // 
  // 0.5秒間隔で距離センサーの値を取得
//  int duration = millis() - prevTime;
//  if (duration > 500) {
//
//      digitalWrite(trigPin, LOW);
//      delayMicroseconds(2);
//      digitalWrite( trigPin, HIGH ); //超音波を出力
//      delayMicroseconds( 10 );
//      digitalWrite( trigPin, LOW );
//      Duration = pulseIn( echoPin, HIGH ); //センサからの入力
//      if (Duration > 0) {
//        Duration = Duration/2; //往復距離を半分にする
//         Distance = Duration*340*100/1000000; // 音速を340m/sに設定
//      }
//      
//      changeType = 1;
//      changeFlg = true;
//
//      Serial.println(Distance);
//      
//      // 更新時間を保持
//      prevTime = millis();
//  }
  // --- < /nagahora > --- //

//  if (Distance < 50) {
//    changeType = 2;
//    changeFlg = true;
//  } else if (changeType != 1) {
//    changeType = 0;
//    changeFlg = true;
//  }

  //
  // LED 
  for (int i = 0; i < MAXPIXELS; i++) {
    values[i] += increment[i];
    
    if (values[i] > MAXBRIGHTNESS) {
      values[i] = MAXBRIGHTNESS;
      increment[i] = minus[type];
    } else if (values[i] <= 0) {
      values[i] = 0;
      increment[i] = plus[type];
    }

    // 色設定
    if (type == 0) {
      pixels.setPixelColor(i, pixels.Color(0, 0, values[i]));
    } else if (type == 1) {
      pixels.setPixelColor(i, pixels.Color(values[i], values[i], 0));
    } else {
      pixels.setPixelColor(i, pixels.Color(values[i], 0, 0));
    }

    //pixels.setPixelColor(i, pixels.Color(values[i], 0, 0));
    pixels.show();

    // 全部消えてからタイプを変える
    if(flags[MAXPIXELS-1] == interval[type] && changeFlg) {
      Serial.println("hello3");
      Serial.println(changeType);
      type = changeType;
      changeFlg = false;
    }
  }
}
