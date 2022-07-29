#ifndef __HTTPSERVICE__
#define __HTTPSERVICE__

class httpService { 
  
public:
  httpService(const char* ssid, const char* password, const char* service);
  void send(int t, int l);
  void on();
  
private:
  void connectToWifi(char* ssid, char* password);
  void sendData(String address, float temp,float light, String place);
  const char*  ssid;
  const char*  password;
  const char*  serviceURI;  
};

#endif
