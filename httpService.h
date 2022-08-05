#ifndef __HTTPSERVICE__
#define __HTTPSERVICE__

class httpService { 
  
public:
  httpService(char* ssid, char* password, char* service);
  void send(int t, int l);
  void on();
  
private:
  void connectToWifi(char* ssid, char* password);
  int sendData(String address, float temp,float light, String place);
  char*  ssid;
  char*  password;
  char*  serviceURI;  
};

#endif
