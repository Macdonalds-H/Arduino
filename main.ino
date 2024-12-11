#include <ThreeWire.h>
#include <RtcDS1302.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

ThreeWire myWire(4, 5, 3); // IO, SCLK, CE 디지털 연결 번호
RtcDS1302<ThreeWire> Rtc(myWire);
LiquidCrystal_I2C lcd(0x27, 16, 2); // LCD 주소, 크기 (16x2)

int buzzerPin = 8;       // 부저 핀
int btnPin = 2;          // 버튼 핀
int resetPin = 0; // 알람 리셋 핀
int count = 0; // 알람 카운트
bool isBuzzerOn = true; // 부저 상태 (기본값: 꺼짐)

Servo myServo1; // 첫 번째 서보 객체 생성 (= 오른쪽 눈썹)
Servo myServo2; // 두 번째 서보 객체 생성 (= 왼쪽 눈썹)
int brow_R = 0; // 오른쪽 눈썹 초기화 각도
int brow_L = 180; // 왼쪽 눈썹 초기화 각도

void setup() {
    Serial.begin(9600);
    lcd.begin();
    lcd.backlight();
    Rtc.Begin();
    pinMode(buzzerPin, OUTPUT);
    pinMode(btnPin, INPUT_PULLUP); // 버튼 핀을 풀업 저항으로 설정
    pinMode(resetPin, INPUT_PULLUP); // 알람 리셋 핀을 풀업 저항으로 설정
    // RTC 모듈 초기화 (2024년 12월 9일 오전 12시 2분)
    RtcDateTime compiled(2024, 12, 9, 12, 2, 58);
    Rtc.SetDateTime(compiled);

    // 초기화 메시지 출력
    RtcDateTime now = Rtc.GetDateTime();
    printDateTime(now);
    Serial.println();
    lcd.clear();
    lcd.print("RTC Initialized");
    delay(2000);

    myServo1.attach(9); // 서보1 핀 번호 9
    myServo1.write(brow_R); // 오른쪽 눈썹(= 서보1) 초기 각도 설정
    myServo2.attach(7); // 서보2 핀 번호 7
    myServo2.write(brow_L); // 왼쪽 눈썹(서보2) 초기 각도 설정
    Serial.begin(9600); // 시리얼 통신 시작
    Serial.println("서보 제어 프로그램 시작");
    Serial.println("1: 각도 증가, 2: 원래 각도로 복귀");
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
      if (now.Year() == 2024 && now.Month() == 12 && now.Day() == 9 && now.Hour() == 12 && now.Minute() == 3) {
          lcd.clear();
          lcd.print("2024-12-9");
          lcd.setCursor(0, 1);
          lcd.print("Wake Up!");

          // 부저 상태가 켜져 있으면 멜로디 재생
          if (isBuzzerOn) {
              Serial.println("시작");
              playMelody();
              Serial.println("끝남");
              if(count == 3){
                 eyebrow(count);
              }
          }
      } else {
          noTone(buzzerPin); // 알람 시간이 아니면 부저 끄기
      }
    }else{
      Serial.print("카운트 종료");
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
