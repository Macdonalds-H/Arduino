#include <ThreeWire.h>
#include <RtcDS1302.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

#define ENA 6   // 왼쪽 바퀴 속도 제어 (PWM 핀)
#define IN1 7   // 왼쪽 바퀴 제어 핀 1
#define IN2 8   // 왼쪽 바퀴 제어 핀 2

#define ENB 11  // 오른쪽 바퀴 속도 제어 (PWM 핀)
#define IN3 9   // 오른쪽 바퀴 제어 핀 1
#define IN4 10  // 오른쪽 바퀴 제어 핀 2

#define button1Pin A0 // 첫 번째 버튼 (감소)
#define button2Pin A1 // 두 번째 버튼 (조절/저장)
#define button3Pin 1 // 세 번째 버튼 (증가)

ThreeWire myWire(4, 5, 3); // IO, SCLK, CE 디지털 연결 번호
RtcDS1302<ThreeWire> Rtc(myWire);
LiquidCrystal_I2C lcd(0x27, 16, 2); // LCD 주소, 크기 (16x2)

int buzzerPin = 13;       // 부저 핀 8 -> 13
int btnPin = 2;          // 버튼 핀
int count = 0; // 알람 카운트
bool isBuzzerOn = true; // 부저 상태 (기본값: 꺼짐)

Servo myServo1; // 첫 번째 서보 객체 생성 (= 오른쪽 눈썹)
Servo myServo2; // 두 번째 서보 객체 생성 (= 왼쪽 눈썹)
int brow_R = 0; // 오른쪽 눈썹 초기화 각도
int brow_L = 180; // 왼쪽 눈썹 초기화 각도

// 변수 초기화
int currentIntake = 0; // 현재 섭취량
int targetIntake = 1500; // 목표 섭취량
int tempIntake = 0; // 임시 섭취량
bool adjustMode = false; // 조절 모드 플래그

void setup() {
    Serial.begin(9600);
    lcd.begin();
    lcd.backlight();
    Rtc.Begin();
    pinMode(buzzerPin, OUTPUT);
    pinMode(btnPin, INPUT_PULLUP); // 버튼 핀을 풀업 저항으로 설정
    // RTC 모듈 초기화 (2024년 12월 9일 오전 12시 2분)
    RtcDateTime compiled(2024, 12, 12, 12, 2, 50);
    Rtc.SetDateTime(compiled);

    // 초기화 메시지 출력
    RtcDateTime now = Rtc.GetDateTime();
    printDateTime(now);
    Serial.println();
    lcd.clear();
    lcd.print("RTC Initialized");
    delay(2000);
    myServo1.attach(12); // 서보1 핀 번호 9 - > 12
    myServo1.write(brow_R); // 오른쪽 눈썹(= 서보1) 초기 각도 설정
    myServo2.attach(7); // 서보2 핀 번호 7 -> 2
    myServo2.write(brow_L); // 왼쪽 눈썹(서보2) 초기 각도 설정
    Serial.begin(9600); // 시리얼 통신 시작
    Serial.println("서보 제어 프로그램 시작");
    Serial.println("1: 각도 증가, 2: 원래 각도로 복귀");

    // 핀 모드 설정
    pinMode(ENA, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);

    pinMode(ENB, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);

    pinMode(button1Pin, INPUT_PULLUP);
    pinMode(button2Pin, INPUT_PULLUP);
    pinMode(button3Pin, INPUT_PULLUP);
}

void loop() {
    static int lastButtonState = HIGH; // 이전 버튼 상태 저장
    static bool buttonPressed = false; // 버튼이 눌렸는지 여부

    int btnState = digitalRead(btnPin); // 버튼 상태 읽기

    // 버튼 눌림 확인 (디바운싱 처리 추가)
    if (btnState == LOW && lastButtonState == HIGH) {
        delay(50); // 디바운싱을 위한 잠시 대기
        buttonPressed = !buttonPressed; // 상태 토글
        if (buttonPressed) {
            isBuzzerOn = true; // 부저 끄기
            noTone(buzzerPin); // 부저 소리 끄기
            Serial.println("Button Pressed: Buzzer ON");
        } else {
            isBuzzerOn = false; // 부저 켜기
            Serial.println("Button Released: Buzzer OFF");
        }
    }

    lastButtonState = btnState; // 버튼 상태 업데이트
    if (count < 3) {
      Serial.print(count);
      Serial.println("번 시작");
      // 현재 시간 가져오기
      RtcDateTime now = Rtc.GetDateTime();
      printDateTime(now);
      Serial.println();

      // 특정 시간에 LCD 메시지 출력 및 멜로디 재생
      if (now.Year() == 2024 && now.Month() == 12 && now.Day() == 12 && now.Hour() == 12 && now.Minute() == 3) {
          lcd.clear();
          lcd.print("2024-12-12");
          lcd.setCursor(0, 1);
          lcd.print("Wake Up!");

          // 부저 상태가 켜져 있으면 멜로디 재생
          if (isBuzzerOn) {
              if(count == 0){
                 Serial.print(brow_R);
                 Serial.print(brow_L);
                 Serial.println(" 눈썹각도");
              }
              Serial.println("시작");
              playMelody();
              Serial.println("끝남");
              stopMovement();
              if(count == 3){
                 // eyebrow(count);
              }
          }
      } else {
          noTone(buzzerPin); // 알람 시간이 아니면 부저 끄기
      }
    }else{
      Serial.print("카운트 종료");
      updateIntake();
    }
    delay(3000); // 1초 대기
}

void playMelody() {
  int melody[] =    { 330, 330, 330, 330, 330, 330, 330, 392, 262, 294, 330, 349, 349, 349, 349, 349, 330, 330, 330, 330, 294, 294, 330, 294, 392 };
  int durations[] = { 300, 300, 600, 300, 300, 600, 300, 300, 300, 300, 600, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 600};
  int delays[] =    { 300, 300, 600, 300, 300, 600, 300, 300, 300, 300, 600, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 600, 600 };
   for (int i = 0; i < 25; i++) {
    // 버튼 상태를 계속 확인
    if (i == 1){
      eyebrow(count);
      moveForward();
    }
    if (i == 7){
       moveBackward();
    }
    if (i == 13){
      turnLeft();
    }
    if(i==16){
      moveForward();
    }
    if(i==21){
      turnRight();
    }

    int btnState = digitalRead(btnPin); // 버튼 상태 읽기
    if (btnState == HIGH) {
      Serial.println("Button pressed, stopping melody.");
      // 버튼으로 꺼짐 -> 상태변화 주기 --
      count = 3;
      eyebrow(count);
      return; // 버튼이 눌리면 함수 종료
    }

    tone(buzzerPin, melody[i], durations[i]); // 현재 음 재생
    delay(delays[i]);
  }
  count = count + 1;
  // 소절 간 대기
  delay(1000);
}

void printDateTime(const RtcDateTime& dt) {
    char datestring[20];
    snprintf_P(datestring, sizeof(datestring), PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
               dt.Month(), dt.Day(), dt.Year(), dt.Hour(), dt.Minute(), dt.Second());
    Serial.print(datestring);
}

void eyebrow(int alarmcount){
  if (alarmcount == 1 || alarmcount == 2) { // '1'을 입력받으면 --> 알람이 울리면
    if (brow_R < 50) { // 오른쪽 눈썹 최대 각도 50도
      brow_R += 25;
      if (brow_R > 50) {
        brow_R = 50; // 50도를 초과하지 않도록 제한
      }
      myServo1.write(brow_R); // 오른쪽 눈썹 각도 설정
      Serial.print("서보1 현재 각도: ");
      Serial.println(brow_R);
    } 
    
    if (brow_L > 130) { // 왼쪽 눈썹 최소 각도 130도
      brow_L -= 25;
      if (brow_L < 130) {
        brow_L = 130; // 각도가 130도 미만이 되지 않도록 제한
      }
      myServo2.write(brow_L); // 왼쪽 눈썹 각도 설정
      Serial.print("서보2 현재 각도: ");
      Serial.println(brow_L);
    }
  } 
  if (alarmcount > 2) { // '2'를 입력받으면 --> 알람을 끄면
    brow_R = 0;
    myServo1.write(brow_R); // 오른쪽 눈썹 각도를 0도로 설정
    Serial.println("오른쪽 눈썹 원래 각도로 복귀");
    
    brow_L = 180;
    myServo2.write(brow_L); // 왼쪽 눈썹 각도를 180도로 설정
    Serial.println("왼쪽 눈썹 원래 각도로 복귀");
  }
}

// 모든 모터 정지 함수
void stopMovement() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

// 전진
void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 255); // 왼쪽 바퀴 속도
  analogWrite(ENB, 255); // 오른쪽 바퀴 속도
}

// 후진
void moveBackward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, 255); // 왼쪽 바퀴 속도
  analogWrite(ENB, 255); // 오른쪽 바퀴 속도
}

// 좌회전
void turnLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);   // 왼쪽 바퀴 정지
  analogWrite(ENB, 255); // 오른쪽 바퀴 속도
}

// 우회전
void turnRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 255); // 왼쪽 바퀴 속도
  analogWrite(ENB, 0);   // 오른쪽 바퀴 정지
}
// 현재 상태 화면 업데이트
void updateDisplay() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Intake: ");
  lcd.print(currentIntake);
  lcd.print(" ml");

  lcd.setCursor(0, 1);
  lcd.print("Target: ");
  lcd.print(targetIntake);
  lcd.print(" ml");
}

// 조절 모드 화면 업데이트
void updateAdjustDisplay() {
  lcd.setCursor(0, 1);
  lcd.print("Adjust: ");
  lcd.print(tempIntake);
  lcd.print(" ml    ");
}

void updateIntake(){
  updateDisplay();
  while(1){
  
  // 조절 모드
    if (digitalRead(button1Pin) == LOW) {
      // 첫 번째 버튼으로 10ml 감소
      tempIntake = max(0, tempIntake - 10);
      updateAdjustDisplay();
      delay(200);
    }

    if (digitalRead(button3Pin) == LOW) {
      // 세 번째 버튼으로 10ml 증가
      tempIntake += 10;
      updateAdjustDisplay();
      delay(200);
    }

    if (digitalRead(button2Pin) == LOW) {
      // 가운데 버튼을 눌러 저장
      currentIntake = min(targetIntake, currentIntake + tempIntake);
      adjustMode = false;
      updateDisplay();
      delay(200); // 디바운싱
    }
  }
  
}
