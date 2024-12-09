#include <LiquidCrystal_I2C.h>

// LCD 초기화
LiquidCrystal_I2C lcd(0x27, 16, 2);

// 버튼 핀 설정
const int button1Pin = 0; // 첫 번째 버튼 (감소)
const int button2Pin = 1; // 두 번째 버튼 (저장)
const int button3Pin = 2; // 세 번째 버튼 (증가)

// 숫자 변수
int number = 100;
int savedNumber = 0;

void setup() {
  // LCD 초기화
  lcd.begin();
  lcd.backlight();
  lcd.print("Number: ");
  lcd.setCursor(0, 1);
  lcd.print(number);

  // 버튼 핀 입력으로 설정
  pinMode(button1Pin, INPUT_PULLUP);
  pinMode(button2Pin, INPUT_PULLUP);
  pinMode(button3Pin, INPUT_PULLUP);

  Serial.begin(9600);
}

void loop() {
  // 첫 번째 버튼을 눌렀을 때 숫자 감소
  if (digitalRead(button1Pin) == LOW) {
    number--;
    updateLCD();
    delay(200); // 디바운싱
  }

  // 세 번째 버튼을 눌렀을 때 숫자 증가
  if (digitalRead(button3Pin) == LOW) {
    number++;
    updateLCD();
    delay(200); // 디바운싱
  }

  // 두 번째 버튼을 눌렀을 때 숫자 저장
  if (digitalRead(button2Pin) == LOW) {
    savedNumber = number;
    lcd.setCursor(0, 0);
    lcd.print("Saved: ");
    lcd.print(savedNumber);
    delay(500); // 디바운싱
    updateLCD(); // 다시 초기 상태로
  }
}

// LCD 업데이트 함수
void updateLCD() {
  lcd.setCursor(0, 1);
  lcd.print("                "); // 줄 지우기
  lcd.setCursor(0, 1);
  lcd.print(number);
}
