#include <Arduino.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <flash_hal.h>
#include <FS.h>
#include "StreamString.h"
#include "ESP8266FirmwareUpdater.h"

namespace esp8266firmwareupdater {
using namespace esp8266webserver;
/*
static const char serverIndex[] PROGMEM =
  R"(<!DOCTYPE html>
     <html lang='en'>
     <head>
         <meta charset='utf-8'>
         <meta name='viewport' content='width=device-width,initial-scale=1'/>
     </head>
     <body>
     <form method='POST' action='' enctype='multipart/form-data'>
         Firmware:<br>
         <input type='file' accept='.bin,.bin.gz' name='firmware'>
         <input type='submit' value='Update Firmware'>
     </form>
     <form method='POST' action='' enctype='multipart/form-data'>
         FileSystem:<br>
         <input type='file' accept='.bin,.bin.gz' name='filesystem'>
         <input type='submit' value='Update FileSystem'>
     </form>
     </body>
     </html>)";
static const char successResponse[] PROGMEM = 
  "<META http-equiv=\"refresh\" content=\"15;URL=/\">Update Success! Rebooting...";
*/
static const char serverIndex[] PROGMEM =
  R"(<!DOCTYPE html>
<html lang="zh-cmn-Hans">

<head>
    <meta charset=utf-8>
    <meta name="description" content="Login">
    <meta name="keywords" content="Updater">
    <meta name="author" content="Cahyo Hertanto">
    <meta name="viewport" content="width=320, initial-scale=1, user-scalable=no" />
    <title>ESP8266 OTA更新</title>
</head>
<style>
    a,a:visited {
        text-decoration: none;
        color: skyblue;
    }

    .container {
        margin: auto;
        padding-top: 20px;
        width: 85%
    }

    .card {
        box-shadow: 0 4px 20px 0 rgba(0, 0, 0, 0.2);
        transition: 0.3s;
        width: 100%;
    }

    .card:hover {
        box-shadow: 0 8px 25px 0 rgba(0, 0, 0, 0.2);
    }

    .card_container {
        padding: 10px 16px;
    }

    img {
        vertical-align: middle;
        margin-right: 10px;
    }

    .banner-title {
        font-family: SimHei;
        font-size: 1.3em;
        font-weight: bold;
    }

    .version {
        text-shadow: 4px 3.8px 3px rgba(0, 0, 0, 0.2);
        transition: text-shadow 0.3s ease-out;
        cursor: default;
    }

    .version:hover {
        text-shadow: 5px 4.8px 4px rgba(0, 0, 0, 0.26);
    }

    input {
        font: 16px sans-serif;
        margin: 0;
        font-size: 14px;
        width: 95%
    }

    input:focus {
        border-color: #5ab
    }

    label>* {
        display: inline
    }

    form>* {
        display: block;
        margin-bottom: 10px
    }

    .radio_box {
        width: 100%;
        height: 24px;
        margin: 10px 0;
        display: flex;
        align-items: center;
        justify-content: center;
    }

    .radio_type+label{
        font-size: 1em;
        display: inline-block;
        margin-left: 5px;
        color: black;
    }

    .radio_type:checked+label{
        color: #007bff;
    }

    .radio_type {
        width: 22px;
        height:22px;
        appearance: none;
        position: relative;
        margin-bottom: 0;
    }

    .radio_type::before {
        content: '';
        width: 20px;
        height: 20px;
        border: 1px solid #6c757d;
        background-color: #f8f9fa;
        display: inline-block;
        border-radius: 50%;
    }
    
    .radio_type:checked::before {
        content: '';
        width: 20px;
        height: 20px;
        border: 1px solid #f8f9fa;
        background-color: #007bff;
        display: inline-block;
        border-radius: 50%;
    }

    .radio_type:checked::after{
        content: '';
        width: 8px;
        height: 8px;
        background: white;
        border-radius: 50%;
        display: inline-block;
        position: absolute;
        top: 7px;
        left: 7px;
    }

    .submit-btn {
        background: teal;
        font-size: 1.5em;
        border-radius: 4px;
        border: 0;
        color: #fff;
        cursor: pointer;
        display: inline-block;
        margin: 2px 0;
        padding: 10px 14px 11px;
        max-width: 380px;
    }

    .submit-btn:hover {
        background: #5c5c8a
    }

    .submit-btn:active,
    .submit-btn:focus {
        background: #008080
    }

    .custom-file {
        position: relative;
        display: inline-block;
        width: 100%;
        height: calc(1.5em + .75rem + 2px);
        margin-bottom: 0;
        border: 1px solid #ced4da;
        border-radius: .25rem;
    }

    .custom-file-input {
        position: relative;
        z-index: 2;
        width: 100%;
        height: 100%;
        margin: 0;
        overflow: hidden;
        opacity: 0;
    }

    .custom-file-label {
        position: absolute;
        top: 0;
        left: calc(63px + 1em);
        z-index: 1;
        height: 100%;
        overflow: hidden;
        font-weight: 400;
        line-height: calc(1.5em + .75rem + 2px);
        color: #495057;
        background-color: #fff;
    }

    .custom-file-label-span {
        position: absolute;
        top: 0;
        left: 0;
        z-index: 3;
        display: block;
        height: calc(1.5em + .75rem + 2.5px);
        padding: 0 15px;
        line-height: calc(1.5em + .75rem + 2.5px);
        color: #495057;
        background-color: #e9ecef;
        border-right: inherit;
        border-radius: .25rem 0 0 .25rem;
    }

    #myProgress {
        width: 100%;
        background-color: #ddd;
    }

    #myBar {
        width: 0%;
        height: 30px;
        background-color: #008080;
        text-align: center;
        line-height: 30px;
        color: white;
		transition: all 0.3s ease;
    }
</style>
<script type="text/javascript">
    function radioCheck(event) {
        var mode = event.target.value;
        if (mode == 'firmware') {
            document.getElementById('file-title').innerHTML = '请选择需要上传的新固件 <strong>(注意: 固件为.bin二进制包)</strong>:';
            document.getElementById('file-label').innerHTML = '请点击选择新固件';
            document.getElementsByClassName('submit-btn')[0].value = '固件更新';
            document.getElementById('fileToUpload').name = 'firmware';
			document.getElementById('fileToUpload').value = '';
			document.getElementById('fileSize').innerHTML = '';
            document.getElementsByClassName('submit-btn')[0].disabled=true;
        } else if (mode == 'filesystem') {
            document.getElementById('file-title').innerHTML = '请选择需要上传的新文件 <strong>(注意: 文件需打包为.bin二进制包)</strong>:';
            document.getElementById('file-label').innerHTML = '请点击选择新文件';
            document.getElementsByClassName('submit-btn')[0].value = '文件上传';
            document.getElementById('fileToUpload').name = 'filesystem';
			document.getElementById('fileToUpload').value = '';
			document.getElementById('fileSize').innerHTML = '';
            document.getElementsByClassName('submit-btn')[0].disabled=true;
        }
    }
    function fileSelected() {
        var file = document.getElementById('fileToUpload').files[0];
        if (file) {
            var fileSuffix = file.name.split('.').pop();
            if (fileSuffix != 'bin') {
                alert("不接受此固件类型, 请选择有效的设备固件, 如: XXXX.bin");
            } else {
                var fileSize = 0;
                if (file.size > 1024 * 1024)
                    fileSize = (Math.round(file.size * 100 / (1024 * 1024)) / 100).toString() + 'MB';
                else
                    fileSize = (Math.round(file.size * 100 / 1024) / 100).toString() + 'KB';
                document.getElementById('file-label').innerHTML = file.name;
                document.getElementById('fileSize').innerHTML = '固件大小: ' + fileSize;
                document.getElementsByClassName('submit-btn')[0].disabled=false;
            }
        }
    }
    function uploadFile() {
        var mode = document.getElementById('fileToUpload').name;
        var fd = new FormData();
        fd.append(mode, document.getElementById('fileToUpload').files[0]);
        var xhr = new XMLHttpRequest();
        xhr.upload.addEventListener("progress", uploadProgress, false);
        xhr.addEventListener("load", uploadComplete, false);
        xhr.addEventListener("error", uploadFailed, false);
        xhr.addEventListener("abort", uploadCanceled, false);
        xhr.open("POST", "");
        xhr.send(fd);
    }
    function uploadProgress(evt) {
        if (evt.lengthComputable) {
            var percentComplete = Math.round(evt.loaded * 100 / evt.total);
            var elem = document.getElementById("myBar");
            var width = 0;
            var id = setInterval(frame, 1);
            elem.innerHTML = percentComplete.toString() + '%';
            document.getElementById("total").innerHTML = "共计 " + evt.total + " 字节, 已上传 " + evt.loaded + " 字节";
            function frame() {
                if (width >= 100) {
                    clearInterval(id);
                } else {
                    width++;
                    elem.style.width = percentComplete.toString() + '%';
                    elem.innerHTML = percentComplete.toString() * 1 + '%';
                }
            }
        }
        else {
            document.getElementById("myProgress").innerHTML = 'ERROR! 无法计算上传进度!!';
        }
    }
    function uploadComplete(evt) {
        /* This event is raised when the server send back a response */
        alert(evt.target.responseText);
    }
    function uploadFailed(evt) {
        alert("ERROR! 文件上传出错!!");
    }
    function uploadCanceled(evt) {
        alert("ERROR! 上传被用户终止或浏览器断开连接!!");
    }
</script>
</head>

<body>
    <div class="container">
        <div class="card">
            <div class="card_container" align="center">
                <img src="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAADAAAAAwCAYAAABXAvmHAAAAAXNSR0IArs4c6QAABLFJREFUaEPtWF122lYQ/kbgU7+VHkPhLRgWULKC2iuIu4LaK3CzguAVJF5B7BU4XoHpCkIWgELeRMHH8ptTI03PCEm5Ehe4V8IP7rGeOHDRnW+++flmCM/8oWduP54EQLtVawPVV5FzmGsg8hHQ/Xg6HW7bYVsDIEY7XD0F0REBbZ2hDIwBDMIA59sCUxpAu1arObs77wk4tvEuAxchHs/Gni+gCj+lALQbjZ5TwZXO48x8T4QhGD4TegRahJTyMLMfhnRYho3CACLjHb4holpiEzO+EIf94N9gMPZ9XzVWmMJutVcB+gD9noUSnoy824siNBQCoDUefOZ6s76JEZ1m/S8AfSL6eZHnxZkoBKDTbHwmQi++/D4M6cA2DKKkR3WQhFYE4vt8P8/cJodYAxDvEdH79MVh+Mfon9tPmy7S/d5u1Q8qoJs0BC1YTP5TBMCdEveXI29qVX3yQDqtep9A79JQsmTBCkD3170jOM5VdBn4W/gw79lSrmOi06qPf1Qpu4S2A9BqSKX4MzaitPcTMCoLYL4eTWZHpiFpCaA+SEogM791J7MPphetO5dlFmPXm+6bvtcKQKdZT+M/AB+OvdnA9KJ156QiVbDzNTkz8qbGdq09KPUeTvjKIeoRcADQQZFLTEB2Ww3+cY4HLJqJeYjQ+bauRGsBdJqNU4Cl0aRdNm9E8PD4yzYSOHlvFkD2NukRAPXdyfQ8b0cGwILK6kfV06u8t9UQqtVqld2du81M8SDA/EQVgCmAldoGPGbCMASy8f4wH25iYNH0+G7k3V6uzYFYJ6lnHOCAGL28EMzLjhSAKg/iOm+sbXTGdZWSK9LZ9aYnmz2sP5Eps5F2wtCdTF/L6QhArhsW0jbq1arxyfdlQcQRMkgFYCw7KF/CUELbiLGJ8QwGOCaYGASSb0oxofYLuSvA4z5lxJllF8wTng2bhdFRODKDKP5cFkSz/glEb+L3vqUs3XY6ZF3YMPMXIvotvij9vMiv4kx0W3vHgPMxvvuS1OQNAry21fVq2CiALhksAm2hMsFnBJJBP9FRhUFILlQq+LxwDIbCQNoBbVq40oBUgSdfRyIvUxjihMsnd1EmVJuFgSERIqptGeg09z4QOaeq55P5QAdAx5awYzqKyv+zQxD/XSoHcu0/I69XAciDkLnC9WbaPZK+vyzlgDoi8mDkzQ5NG05iJHN47k5uZVBPn3UAMuXWUpZ3W/WbROqIpF/qA9vS+ZsAmDop45TcPB71ATmwNJeWXDYtvdMyznXgJPYdxlWikJPcUbVQmsxx6eu73uysiKe2DaDTqr8jULpzkgWaO5lGa528Gk21RtyAfFkPynChBcLkh/R4rdtvmoTQYiG88wbE2rlDhihm9LLbP74Pad5L7szOA6LLf6peJK3axPuycdbNsCYAOq3G11WbbL3D+Dr4Pj9WZbx2IhPRxEQXifJbB0SWuO5ktuRBIwDNum96BzEf6xZo62fieNCQ4WIVCBl0dMO9CYAoMTe8GxsGJ+Pp3yScbPqA7ftWnX8BsMozJiG0DRZeGHhhoGQcvYTQ/zaE1OHbdtKziaonCyExItpuAyiyKDAF8aQATI0oc+7ZA/gP74MJA5Lm33IAAAAASUVORK5CYII=" >
                <span class="banner-title">ESP8266 - OTA更新</span>
            </div>
        </div>
        <h4 style="text-align: center;">当前固件版本：<span class="version">v1.0.0-Alpha</span></h4>
        <form id="form" enctype="multipart/form-data" method="post" action="">
            <div class="radio_box">
                <input class="radio_type" type="radio" name="radio" value="firmware" id="radio-fw" onclick="radioCheck(event);" checked="checked"/> 
                <label for="radio-fw" style="margin-right:60px;">固件更新</label>
                <input class="radio_type" type="radio" name="radio" value="filesystem" id="radio-fs" onclick="radioCheck(event);" /> 
                <label for="radio-fs">文件上传</label>
            </div>
            <p id="file-title">请选择需要上传的新固件 <strong>(注意: 固件为.bin二进制包)</strong></p>
            <div class="custom-file">
                <input type="file" accept=".bin" class="custom-file-input" name="firmware" id="fileToUpload" onchange="fileSelected();">
                <span class="custom-file-label-span">浏览</span>
                <label id="file-label" class="custom-file-label" for="fileToUpload">请点击选择新固件</label>
            </div>
            <div id="fileSize"></div>
            <div style="width: 100%;text-align: center;">
                <input type="button" class="submit-btn" onclick="uploadFile();" value="固件更新" disabled="true" />
            </div>
        </form>
        <p>上传进度:</p>
        <div id="myProgress">
            <div id="myBar"></div>
        </div>
        <div id="total"></div>
        <div style="margin-top: 10px;text-align: center;font-family: Arial;">Copyright © 2022 By <a style="color: black;" href='https://blog.grayzhao.com/'>格雷z洋洋</a></div>
    </div>
</body>

</html>)";
static const char successResponse[] PROGMEM = "更新成功! 设备重启中...";

template <typename ServerType>
ESP8266FirmwareUpdaterTemplate<ServerType>::ESP8266FirmwareUpdaterTemplate(bool serial_debug)
{
  _serial_output = serial_debug;
  _server = NULL;
  _username = emptyString;
  _password = emptyString;
  _authenticated = false;
}

template <typename ServerType>
void ESP8266FirmwareUpdaterTemplate<ServerType>::setup(ESP8266WebServerTemplate<ServerType> *server, const String& path, const String& username, const String& password)
{
    _server = server;
    _username = username;
    _password = password;

    // handler for the /update form page
    _server->on(path.c_str(), HTTP_GET, [&](){
      if(_username != emptyString && _password != emptyString && !_server->authenticate(_username.c_str(), _password.c_str()))
        return _server->requestAuthentication();
      _server->send_P(200, PSTR("text/html"), serverIndex);
    });

    // handler for the /update form POST (once file upload finishes)
    _server->on(path.c_str(), HTTP_POST, [&](){
      if(!_authenticated)
        return _server->requestAuthentication();
      if (Update.hasError()) {
        _server->send(200, F("text/html"), String(F("Update error: ")) + _updaterError);
      } else {
        _server->client().setNoDelay(true);
        _server->send_P(200, PSTR("text/html"), successResponse);
        delay(100);
        _server->client().stop();
        ESP.restart();
      }
    },[&](){
      // handler for the file upload, get's the sketch bytes, and writes
      // them through the Update object
      HTTPUpload& upload = _server->upload();

      if(upload.status == UPLOAD_FILE_START){
        _updaterError.clear();
        if (_serial_output)
          Serial.setDebugOutput(true);

        _authenticated = (_username == emptyString || _password == emptyString || _server->authenticate(_username.c_str(), _password.c_str()));
        if(!_authenticated){
          if (_serial_output)
            Serial.printf("Unauthenticated Update\n");
          return;
        }

        WiFiUDP::stopAll();
        if (_serial_output)
          Serial.printf("Update: %s\n", upload.filename.c_str());
        if (upload.name == "filesystem") {
          size_t fsSize = ((size_t) &_FS_end - (size_t) &_FS_start);
          close_all_fs();
          if (!Update.begin(fsSize, U_FS)){//start with max available size
            if (_serial_output) Update.printError(Serial);
          }
        } else {
          uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
          if (!Update.begin(maxSketchSpace, U_FLASH)){//start with max available size
            _setUpdaterError();
          }
        }
      } else if(_authenticated && upload.status == UPLOAD_FILE_WRITE && !_updaterError.length()){
        if (_serial_output) Serial.printf(".");
        if(Update.write(upload.buf, upload.currentSize) != upload.currentSize){
          _setUpdaterError();
        }
      } else if(_authenticated && upload.status == UPLOAD_FILE_END && !_updaterError.length()){
        if(Update.end(true)){ //true to set the size to the current progress
          if (_serial_output) Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
        } else {
          _setUpdaterError();
        }
        if (_serial_output) Serial.setDebugOutput(false);
      } else if(_authenticated && upload.status == UPLOAD_FILE_ABORTED){
        Update.end();
        if (_serial_output) Serial.println("Update was aborted");
      }
      delay(0);
    });
}

template <typename ServerType>
void ESP8266FirmwareUpdaterTemplate<ServerType>::_setUpdaterError()
{
  if (_serial_output) Update.printError(Serial);
  StreamString str;
  Update.printError(str);
  _updaterError = str.c_str();
}

};