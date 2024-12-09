#include <ThreeWire.h>
#include <RtcDS1302.h>
#include <LiquidCrystal_I2C.h>

ThreeWire myWire(4, 5, 3); // IO, SCLK, CE 디지털 연결 번호
RtcDS1302<ThreeWire> Rtc(myWire);
LiquidCrystal_I2C lcd(0x27, 16, 2); // LCD 주소, 크기 (16x2)

int buzzerPin = 8; // 부저 핀

void setup() {    
    Serial.begin(9600);
    lcd.begin();
    lcd.backlight();
    Rtc.Begin();
    pinMode(buzzerPin, OUTPUT);

    // RTC 모듈의 날짜와 시간 초기화 (2024년 12월 9일 오전 12시 2분)
    RtcDateTime compiled(2024, 12, 9, 12, 2, 0);
    Rtc.SetDateTime(compiled);

    // RTC 시간을 읽어서 시리얼 모니터에 출력
    RtcDateTime now = Rtc.GetDateTime();
    printDateTime(now);  // 현재 시간 시리얼 모니터에 출력
    Serial.println();
}

void loop() {
    RtcDateTime now = Rtc.GetDateTime(); // 현재 시간 가져오기
    printDateTime(now);  // 시리얼 모니터에 현재 시간 출력
    Serial.println();

    // 2024년 12월 9일 오전 12시 3분에 LCD에 메시지 출력
    if (now.Year() == 2024 && now.Month() == 12 && now.Day() == 9 && now.Hour() == 12 && now.Minute() == 3) {
        lcd.clear(); // 화면을 지운 후
        lcd.print("2024-12-9");
        lcd.setCursor(0, 1); // 두 번째 줄로 이동
        lcd.print("Wake up");

        // "징글벨" 소리 울리기
        playJingleBells();
    }

    delay(1000);  // 1초 대기
}

// "징글벨" 부저 소리 함수
void playJingleBells() {
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
    delay(300);
    tone(buzzerPin, 392, 600); // 솔
    delay(600);

    // 소절 간 대기
    delay(1000);
}

// 시계 모듈 년도, 달, 시간, 분, 초 받아오는 함수
#define countof(a) (sizeof(a) / sizeof(a[0]))

void printDateTime(const RtcDateTime& dt) {
    char datestring[20];
    snprintf_P(datestring, 
            countof(datestring),
            PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
            dt.Month(),
            dt.Day(),
            dt.Year(),
            dt.Hour(),
            dt.Minute(),
            dt.Second());
    Serial.print(datestring); // 시리얼 모니터에 출력
}
