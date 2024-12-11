#include <ThreeWire.h>
#include <RtcDS1302.h>
#include <LiquidCrystal_I2C.h>

ThreeWire myWire(4, 5, 3); // IO, SCLK, CE 디지털 연결 번호
RtcDS1302<ThreeWire> Rtc(myWire);
LiquidCrystal_I2C lcd(0x27, 16, 2); // LCD 주소, 크기 (16x2)

int buzzerPin = 8;       // 부저 핀
int btnPin = 2;          // 버튼 핀

bool isBuzzerOn = true; // 부저 상태 (기본값: 꺼짐)

void setup() {
    Serial.begin(9600);
    lcd.begin();
    lcd.backlight();
    Rtc.Begin();
    pinMode(buzzerPin, OUTPUT);
    pinMode(btnPin, INPUT_PULLUP); // 버튼 핀을 풀업 저항으로 설정

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
            isBuzzerOn = true; // 부저 켜기
             
            Serial.println("Button Pressed: Buzzer ON");
        } else {
            isBuzzerOn = false; // 부저 끄기
            noTone(buzzerPin); // 부저 소리 끄기
            Serial.println("Button Released: Buzzer OFF");
        }
    }

    lastButtonState = btnState; // 버튼 상태 업데이트

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
            playMelody();
        }
    } else {
        noTone(buzzerPin); // 알람 시간이 아니면 부저 끄기
    }

    delay(1000); // 1초 대기
}

void playMelody() {
    // 첫 번째 소절
  tone(buzzerPin, 330, 300); // 미
  delay(300);
  tone(buzzerPin, 330, 300); // 미
  delay(300);
  tone(buzzerPin, 330, 600); // 미
  delay(600);
  tone(buzzerPin, 330, 300); // 미
  delay(300);
  tone(buzzerPin, 330, 300); // 미
  delay(300);
  tone(buzzerPin, 330, 600); // 미
  delay(600);
  tone(buzzerPin, 330, 300); // 미
  delay(300);
  tone(buzzerPin, 392, 300); // 솔
  delay(300);
  tone(buzzerPin, 262, 300); // 도
  delay(300);
  tone(buzzerPin, 294, 300); // 레
  delay(300);
  tone(buzzerPin, 330, 600); // 미
  delay(600);

  // 두 번째 소절
  tone(buzzerPin, 349, 300); // 파
  delay(300);
  tone(buzzerPin, 349, 300); // 파
  delay(300);
  tone(buzzerPin, 349, 300); // 파
  delay(300);
  tone(buzzerPin, 349, 300); // 파
  delay(300);
  tone(buzzerPin, 349, 300); // 파
  delay(300);
  tone(buzzerPin, 330, 300); // 미
  delay(300);
  tone(buzzerPin, 330, 300); // 미
  delay(300);
  tone(buzzerPin, 330, 300); // 미
  delay(300);
  tone(buzzerPin, 330, 300); // 미
  delay(300);
  tone(buzzerPin, 294, 300); // 레
  delay(300);
  tone(buzzerPin, 294, 300); // 레
  delay(300);
  tone(buzzerPin, 330, 300); // 미
  delay(300);
  tone(buzzerPin, 294, 300); // 레
  delay(600);
  tone(buzzerPin, 392, 600); // 솔
  delay(600);

  // 소절 간 대기
  delay(1000);
}

void printDateTime(const RtcDateTime& dt) {
    char datestring[20];
    snprintf_P(datestring, sizeof(datestring), PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
               dt.Month(), dt.Day(), dt.Year(), dt.Hour(), dt.Minute(), dt.Second());
    Serial.print(datestring);
}
