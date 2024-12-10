#include <LiquidCrystal_I2C.h>

// LCD 초기화
LiquidCrystal_I2C lcd(0x27, 16, 2);

// 버튼 핀 설정
const int button1Pin = 0; // 첫 번째 버튼 (감소)
const int button2Pin = 1; // 두 번째 버튼 (조절/저장)
const int button3Pin = 2; // 세 번째 버튼 (증가)

// 변수 초기화
int currentIntake = 0; // 현재 섭취량
int targetIntake = 800; // 목표 섭취량
int tempIntake = 0; // 임시 섭취량
bool adjustMode = false; // 조절 모드 플래그

void setup() {
  // LCD 초기화
  lcd.begin();
  lcd.backlight();

  // 초기 화면 출력
  updateDisplay();

  // 버튼 핀 설정
  pinMode(button1Pin, INPUT_PULLUP);
  pinMode(button2Pin, INPUT_PULLUP);
  pinMode(button3Pin, INPUT_PULLUP);

  Serial.begin(9600);
}

void loop() {
  if (!adjustMode) {
    // 조절 모드 아님
    if (digitalRead(button2Pin) == LOW) {
      adjustMode = true;
      tempIntake = 0; // 임시 섭취량 초기화
      lcd.setCursor(0, 1);
      lcd.print("Adjust: ");
      lcd.print(tempIntake);
      lcd.print(" ml    ");
      delay(200); // 디바운싱
    }
  } else {
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
