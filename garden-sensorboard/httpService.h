#ifndef __HTTPSERVICE__
#define __HTTPSERVICE__



class HTTPService: public HTTPService { 
  
public:
  HTTPService(char* ssid, char* password,char* service);
  void send();
  void on();
  
private:
  void connectToWifi();
  void sendData();
  const char* ssid;
  const char* password;
  const char *serviceURI;  
};

#endif