#include <PPMReader.h>
#include <Servo.h>

byte interruptPin = 3;
byte channelAmount = 7;
PPMReader ppm(interruptPin, channelAmount);
float angle = 1100;
int val = 833;

const int numReadings = 10;
int readings[numReadings];
int readIndex = 0;
int median = 833;
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

int servo_max = 1722;
int servo_min = 833;

void setup() {
    Serial.begin(9600);

    for (int thisReading = 0; thisReading < numReadings; thisReading++){
      readings[thisReading] = 0;
    }

    servo1.attach(5);
    servo2.attach(6);
    servo3.attach(9);
    servo4.attach(10);
}

void loop() {
    for (byte channel = 1; channel <= channelAmount; ++channel) {
        unsigned value = ppm.latestValidChannelValue(channel, 0);
        if (channel == 7) {
          angle = value;
          val = map(angle, 1100, 1900, servo_min, servo_max);
          
          readings[readIndex] = val;
          readIndex = (readIndex + 1) % numReadings;

          // 중앙값 계산
          int median = getMedian(readings, numReadings);

          if (median > servo_max) median = servo_max;
          else if (median < servo_min) median = servo_min;

          Serial.print(median);
        } else {
          Serial.print(value);
        }
        if(channel < channelAmount) Serial.print('\t');
    }
    Serial.println();
    
    servo1.writeMicroseconds(median);
    servo2.writeMicroseconds(median);
    servo3.writeMicroseconds(median);
    servo4.writeMicroseconds(median);
    delay(20);
}

// 중앙값을 계산하는 함수
int getMedian(int *array, int size) {
    int sortedArray[size];
    memcpy(sortedArray, array, sizeof(int) * size);
    sortArray(sortedArray, size);
    return sortedArray[size / 2];
}

// 배열을 정렬하는 함수 (단순 버블 정렬 사용)
void sortArray(int *array, int size) {
    for(int i = 0; i < size - 1; i++) {
        for(int j = 0; j < size - 1 - i; j++) {
            if(array[j] > array[j + 1]) {
                int temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }
}
