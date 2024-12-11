//시리얼모니터 '1'입력 --> 알람이 울리면
//시리얼모니터 '2'입력 --> 알람을 끄면
//서보1 = 오른쪽 눈썹 --> 9번 핀 연결
//서보2 = 왼쪽 눈썹 --> 7번 핀 연결

#include <Servo.h>

Servo myServo1; // 첫 번째 서보 객체 생성
Servo myServo2; // 두 번째 서보 객체 생성
int angle1 = 0; // 첫 번째 서보의 현재 각도 초기화
int angle2 = 180; // 두 번째 서보의 현재 각도 초기화 (최대 각도에서 시작)

void setup() {
  myServo1.attach(9); // 첫 번째 서보 핀 설정 (핀 번호 9 사용)
  myServo1.write(angle1); // 첫 번째 서보 초기 각도를 0도로 설정
  myServo2.attach(7); // 두 번째 서보 핀 설정 (핀 번호 7 사용)
  myServo2.write(angle2); // 두 번째 서보 초기 각도를 90도로 설정
  Serial.begin(9600); // 시리얼 통신 시작
  Serial.println("서보 제어 프로그램 시작!");
  Serial.println("1: 각도 증가, 2: 원래 각도로 복귀");
}

void loop() {
  if (Serial.available()) {
    char input = Serial.read(); // 시리얼 모니터에서 입력값 읽기
    
    if (input == '1') { // '1'을 입력받으면
      if (angle1 < 50) { // 첫 번째 서보의 최대 각도는 50도
        angle1 += 25;
        if (angle1 > 50) {
          angle1 = 50; // 첫 번째 서보 각도가 50도를 초과하지 않도록 제한
        }
        myServo1.write(angle1); // 첫 번째 서보 각도 설정
        Serial.print("서보1 현재 각도: ");
        Serial.println(angle1);
      } 
      
      if (angle2 > 130) { // 두 번째 서보의 최소 각도는 130도
        angle2 -= 25;
        if (angle2 < 130) {
          angle2 = 130; // 두 번째 서보 각도가 130도 미만이 되지 않도록 제한
        }
        myServo2.write(angle2); // 두 번째 서보 각도 설정
        Serial.print("서보2 현재 각도: ");
        Serial.println(angle2);
      }
    } else if (input == '2') { // '2'를 입력받으면
      angle1 = 0;
      myServo1.write(angle1); // 첫 번째 서보 각도를 0도로 설정
      Serial.println("첫 번째 서보가 원래 각도로 복귀");
      
      angle2 = 180;
      myServo2.write(angle2); // 두 번째 서보 각도를 180도로 설정
      Serial.println("두 번째 서보가 원래 각도로 복귀");
    
    }
  }
}
