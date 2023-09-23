#include <M5Atom.h>
//#include <HTTPClient.h>
#include <WiFiClientSecure.h>

uint8_t seq = 0; // remember number of boots in RTC Memory

//WiFiClient client;
WiFiClientSecure client;
// const char* ssid = "7C8FDE08AD15-2G";  // Wi-FiのSSID
// const char* password = "7kbdfrebr96dr4";  // Wi-Fiのパスワード
const char* ssid = "i6sg";  // Wi-FiのSSID
const char* password = "88888888";  // Wi-Fiのパスワード

const char* server = "host02.norahack.tk";  // サーバー名
//const char* server = "www.yahoo.co.jp";  // サーバー名
//const char* path = "/count-a";  // パス
const char* path = "/count-b";  // パス

//HTTPClient http;

void setup() {
    M5.begin(true, false, true);
    M5.dis.setBrightness(10);  // >= 3
    M5.dis.drawpix(0, 0x000000);  // 初期LED: 黒
    Serial.begin(115200);
    Serial.print("\r\nWi-Fi scan start.\r\n");
    WiFi.begin(ssid, password);  //  Wi-Fi APに接続
}

void loop() {
    int lpcnt=0 ;
    int lpcnt2=0 ;

    while (WiFi.status() != WL_CONNECTED) {  //  Wi-Fi AP接続待ち
      delay(500);
      lpcnt += 1 ;                           //
      if (lpcnt > 6) {                       // 6回目(3秒) で切断/再接続
        WiFi.disconnect(true,true);          //
        WiFi.begin(ssid, password);    //
        lpcnt = 0 ;                          //
        lpcnt2 += 1 ;                        // 再接続の回数をカウント
      }                                      //
      if (lpcnt2 > 3) {                      // 3回 接続できなければ、
        ESP.restart() ;                      // ソフトウェアリセット
      }                  
      Serial.print(".");
    }

    M5.dis.drawpix(0, 0x00f000);  // Wi-Fi接続LED: 赤
    Serial.print("\nWiFi connected. \nIP address: ");
    Serial.println(WiFi.localIP());

    // ONフラグとカウンタ値をAmbientに送信する
    seq++;  // カウンタを更新
    Serial.print("Seq: ");
    Serial.println(seq);

    Serial.println("\nStarting connection to server...");
    client.setInsecure();
    if (!client.connect(server, 443))
      Serial.println("Connection failed!");
    else {
      Serial.println("Connected to server!");
      client.println("GET " + String(path) + "?seq=" + seq + " HTTP/1.1");
      client.println("Host: " + String(server)); 
      client.println("Connection: close");
      client.println();
      while (client.connected()) {
        String line = client.readStringUntil('\n');
        if (line == "\r") {
          Serial.println("headers received");
          break;
        }
      }
      while (client.available()) {
        char c = client.read();
        Serial.write(c);
      }
      client.stop();
    }

    Serial.print("HTTPS: data sent. \n");

    M5.dis.drawpix(0, 0xf00000);  // 初期LED: 緑
    //WiFi.disconnect(true,true);
    //Serial.print("WiFi disconnected. \n");

    delay(1000);  // 1秒毎に更新
    delay(random(100));  // ランダム時間
}
