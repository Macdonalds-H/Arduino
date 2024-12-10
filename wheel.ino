#include <Arduino.h>

// 핀 정의
#define ENA 6   // 왼쪽 바퀴 속도 제어 (PWM 핀)
#define IN1 7   // 왼쪽 바퀴 제어 핀 1
#define IN2 8   // 왼쪽 바퀴 제어 핀 2

#define ENB 11  // 오른쪽 바퀴 속도 제어 (PWM 핀)
#define IN3 9   // 오른쪽 바퀴 제어 핀 1
#define IN4 10  // 오른쪽 바퀴 제어 핀 2

void setup() {
  // 핀 모드 설정
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // 랜덤 시드 설정 (아날로그 핀의 노이즈로 시드 생성)
  randomSeed(analogRead(0));
}

void loop() {
  int direction = random(1, 5); // 1 ~ 4까지의 랜덤 값 생성

  switch (direction) {
    case 1:
      moveForward();
      break;
    case 2:
      moveBackward();
      break;
    case 3:
      turnLeft();
      break;
    case 4:
      turnRight();
      break;
  }

  delay(12000);
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
