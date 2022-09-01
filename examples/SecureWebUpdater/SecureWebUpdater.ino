/*
  To upload through terminal you can use: curl -u admin:admin -F "image=@firmware.bin" esp8266-webupdate.com/firmware
*/

// SPIFFS系统文件库
#include <FS.h>
// WiFi库
#include <ESP8266WiFi.h>
// 客户端库
#include <WiFiClient.h>
// 服务端库
#include <DNSServer.h>
// 网页服务器库
#include <ESP8266WebServer.h>
// 网页OTA更新库
#include <ESP8266FirmwareUpdater.h>

// 设置固定AP信息
IPAddress _ip = IPAddress(192, 168, 4, 25);  // IP地址
IPAddress _gw = IPAddress(192, 168, 4, 1);  // 网关
IPAddress _sn = IPAddress(255, 255, 255, 0);  // 子网掩码
// DNS服务器端口
const byte DNS_PORT = 53;
// 访问域名
const char* host = "esp8266-webupdate.com";
// 上传路径
const char* update_path = "/firmware";
// 网页认证用户名
const char* update_username = "admin";
// 网页认证密码
const char* update_password = "admin";

DNSServer dnsServer;
ESP8266WebServer httpServer(80);
ESP8266FirmwareUpdater httpUpdater;

/**
* @desc 串口输出 SPIFFS 下的文件目录结构
* @param 无参数
* @return 无
*/
void listFiles(void) {
  Serial.println();
  Serial.println("SPIFFS files found:");
  // Root directory
  fs::Dir dir = SPIFFS.openDir("/");
  while (dir.next()) {
    String fileName = dir.fileName();
    size_t fileSize = dir.fileSize();
    Serial.printf("FS File:%s,size:%d\n",fileName.c_str(),fileSize);
    yield();
  }
}

void setup(void) {

  Serial.begin(115200);
  Serial.println("Device Version: v1.0.0-Alpha");
  Serial.println();
  Serial.println("Booting Sketch...");
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(_ip, _gw, _sn);
  WiFi.softAP("ESP8266 OTA");

  dnsServer.setTTL(300);
  dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);
  bool status = dnsServer.start(DNS_PORT, host, _ip);
  if (status) {
    Serial.println("start dnsserver success.");
  } else {
    Serial.println("start dnsserver failed.");
  }

  httpUpdater.setup(&httpServer, update_path, update_username, update_password);
  httpServer.begin();

  Serial.printf("HTTPUpdateServer ready! Open http://%s%s in your browser and login with username '%s' and password '%s'\n", host, update_path, update_username, update_password);
  // 初始化 SPIFFS
  if (!SPIFFS.begin()) {
    Serial.println("SPIFFS 初始化失败!");
    // 留在这里等待
    while (1) yield();
  }
  Serial.println("\r\nSPIFFS 初始化完成。");
  // 串口输出 SPIFFS 下的文件目录结构
  listFiles();
}

void loop(void) {
  httpServer.handleClient();
  dnsServer.processNextRequest();
}