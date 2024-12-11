//시리얼모니터 '1'입력 --> 알람이 울리면
//시리얼모니터 '2'입력 --> 알람을 끄면
//서보1 = 오른쪽 눈썹 --> 9번 핀 연결
//서보2 = 왼쪽 눈썹 --> 7번 핀 연결

#include <Servo.h>

Servo myServo1; // 첫 번째 서보 객체 생성 (= 오른쪽 눈썹)
Servo myServo2; // 두 번째 서보 객체 생성 (= 왼쪽 눈썹)
int brow_R = 0; // 오른쪽 눈썹 초기화 각도
int brow_L = 180; // 왼쪽 눈썹 초기화 각도

void setup() {
  myServo1.attach(9); // 서보1 핀 번호 9
  myServo1.write(brow_R); // 오른쪽 눈썹(= 서보1) 초기 각도 설정
  myServo2.attach(7); // 서보2 핀 번호 7
  myServo2.write(brow_L); // 왼쪽 눈썹(서보2) 초기 각도 설정
  Serial.begin(9600); // 시리얼 통신 시작
  Serial.println("서보 제어 프로그램 시작");
  Serial.println("1: 각도 증가, 2: 원래 각도로 복귀");
}

void loop() {
  if (Serial.available()) {
    char input = Serial.read(); // 시리얼 모니터에서 입력값 읽기
    
    if (input == '1') { // '1'을 입력받으면 --> 알람이 울리면
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
    } else if (input == '2') { // '2'를 입력받으면 --> 알람을 끄면
      brow_R = 0;
      myServo1.write(brow_R); // 오른쪽 눈썹 각도를 0도로 설정
      Serial.println("오른쪽 눈썹 원래 각도로 복귀");
      
      brow_L = 180;
      myServo2.write(brow_L); // 왼쪽 눈썹 각도를 180도로 설정
      Serial.println("왼쪽 눈썹 원래 각도로 복귀");
    
    }
  }
}
