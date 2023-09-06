#include <Arduino.h>
#include <HTTPClient.h>
#include <WiFi.h>

void wifi_connect() {
  WiFi.begin("km", "12345678");
  uint8_t cnt = 0;
  while(WiFi.status() != WL_CONNECTED && cnt++ < 10) {
    delay(300);
  }
}

String uri = "http://47.115.204.164:8080/put?";

//	获取粉丝数
uint8_t buf[4];
char tmpbuf[10] = {};
void put() {
  Serial.println("put");
  HTTPClient http;
  uint8_t cnt = sprintf(tmpbuf, "%03u%02d%d%d", *(uint16_t *)buf, buf[2], buf[3] & 0x1, (buf[3] >> 1) & 0x1);
  if(cnt > 0) {
    http.begin(uri+ String(tmpbuf, cnt));  // HTTP begin
    Serial.println(uri + String(tmpbuf, cnt));
    int httpCode = http.GET();
    if(httpCode > 0) {
      if(httpCode == HTTP_CODE_OK)  // 收到正确的内容
      {
      }
    } else {
    }
  }
  http.end();
}
uint8_t s2flag = 0;
void s2recv(void) {
  while(Serial2.available() < 4)
    ;
  Serial2.readBytes(buf, 4);
  s2flag = 1;
}
void setup() {
  Serial.begin(115200);  // open the serial port at 115200 bps;
  wifi_connect();
  Serial2.begin(115200);
  Serial2.onReceive(s2recv);
  Serial.println("start");
}
uint8_t sbuf[4];
void loop() {
  if(s2flag) {
    put();
    s2flag = 0;
  }
  delay(1000);
  Serial2.print('1');
}