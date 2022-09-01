#ifndef __FIRMWARE_UPDATER_H
#define __FIRMWARE_UPDATER_H

#include <ESP8266WebServer.h>

namespace esp8266firmwareupdater {
using namespace esp8266webserver;

template <typename ServerType>
class ESP8266FirmwareUpdaterTemplate
{
  public:
    ESP8266FirmwareUpdaterTemplate(bool serial_debug=true);

    void setup(ESP8266WebServerTemplate<ServerType> *server)
    {
      setup(server, emptyString, emptyString);
    }

    void setup(ESP8266WebServerTemplate<ServerType> *server, const String& path)
    {
      setup(server, path, emptyString, emptyString);
    }

    void setup(ESP8266WebServerTemplate<ServerType> *server, const String& username, const String& password)
    {
      setup(server, "/update", username, password);
    }

    void setup(ESP8266WebServerTemplate<ServerType> *server, const String& path, const String& username, const String& password);

    void updateCredentials(const String& username, const String& password)
    {
      _username = username;
      _password = password;
    }

  protected:
    void _setUpdaterError();

  private:
    bool _serial_output;
    ESP8266WebServerTemplate<ServerType> *_server;
    String _username;
    String _password;
    bool _authenticated;
    String _updaterError;
};

};

#include "ESP8266FirmwareUpdater-impl.h"


using ESP8266FirmwareUpdater = esp8266firmwareupdater::ESP8266FirmwareUpdaterTemplate<WiFiServer>;

namespace BearSSL {
using ESP8266FirmwareUpdaterSecure = esp8266firmwareupdater::ESP8266FirmwareUpdaterTemplate<WiFiServerSecure>;
};

namespace axTLS {
using ESP8266FirmwareUpdaterSecure = esp8266firmwareupdater::ESP8266FirmwareUpdaterTemplate<WiFiServerSecure>;
};

#endif