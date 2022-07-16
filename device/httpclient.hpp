#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>

class smarthttp{
public:
  bool DEBUG_MODE = false;
	smarthttp(){
		https.setTimeout(10000);
	}
	bool doGet(String host){
    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    client->setInsecure();
    
		if(DEBUG_MODE == true) Serial.print(String("[HTTPS] begin...\n") + String(host) + String("\n"));
		if (https.begin(*client, host)) {  // HTTPS
      https.addHeader("User-Agent", "BuildFailureDetectorESP8266");
		  if(DEBUG_MODE == true) Serial.print("[HTTPS] GET...\n");
		  // start connection and send HTTP header
		  int httpCode = https.GET();

		  // httpCode will be negative on error
		  if (httpCode > 0) {
			  // HTTP header has been send and Server response header has been handled
			  if(DEBUG_MODE == true) Serial.printf("[HTTPS] GET... code: %d\n", httpCode);

			  // file found at server
  			if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          if(DEBUG_MODE == true) {
    			  String payload = https.getString();
    			  Serial.println(payload);
          }
			  https.end();	
  			  return true;
  			}
		  } else {
			  if(DEBUG_MODE == true) Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
		  }

		  https.end();
		} else {
		  if(DEBUG_MODE == true) Serial.printf("[HTTPS] Unable to connect\n");
		}
		return false;
		
	}
	bool doPut(String host, String data){
    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    client->setInsecure();
    
		if(DEBUG_MODE == true) Serial.print(String("[HTTPS] begin...\n") + String(host) + String("\n"));
		if (https.begin(*client, host)) {  // HTTPS
      https.addHeader("User-Agent", "BuildFailureDetectorESP8266");
		  if(DEBUG_MODE == true) Serial.print("[HTTPS] GET...\n");
		  // start connection and send HTTP header
		  int httpCode = https.PUT(data);

		  // httpCode will be negative on error
		  if (httpCode > 0) {
			  // HTTP header has been send and Server response header has been handled
			  if(DEBUG_MODE == true) Serial.printf("[HTTPS] GET... code: %d\n", httpCode);

			  // file found at server
  			if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          if(DEBUG_MODE == true) {
    			  String payload = https.getString();
    			  Serial.println(payload);
          }
			  https.end();	
  			  return true;
  			}
		  } else {
			  if(DEBUG_MODE == true) Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
		  }

		  https.end();
		} else {
		  if(DEBUG_MODE == true) Serial.printf("[HTTPS] Unable to connect\n");
		}
		return false;
		
	}

	int Firebase_Set(
		const String& host, const String& auth,
		const String& path, const String& data) {
		String path_with_auth;
		this->makeFirebaseURL(path, auth, path_with_auth);
		this->doPut(host + path_with_auth, data);
	}
 bool doPost(String host, String data="", String auth_header=""){
    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    client->setInsecure();
    //Authorization: Bearer : Line auth
    if(DEBUG_MODE == true) Serial.print(String("[HTTPS] begin...\n") + String(host) + String("\n"));
    if (https.begin(*client, host)) {  // HTTPS
      https.addHeader("Content-Type", "application/x-www-form-urlencoded");
      if(auth_header != ""){
        if(DEBUG_MODE == true) Serial.println(String("Authorization: ") + String("Bearer ") + auth_header);
        https.addHeader("Authorization", String("Bearer ") + auth_header);
      }

      if(DEBUG_MODE == true) Serial.print("[HTTPS] POST...\n");
      // start connection and send HTTP header
      int httpCode = https.POST(data);

      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        if(DEBUG_MODE == true) Serial.printf("[HTTPS] POST... code: %d\n", httpCode);
  
        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          if(DEBUG_MODE == true){
            String payload = https.getString();
            Serial.println(payload);
          }
		  https.end();
          return true;
        }
      } else {
         if(DEBUG_MODE == true) Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
      }

      https.end();
    } else {
       if(DEBUG_MODE == true) Serial.printf("[HTTPS] Unable to connect\n");
    }
    return false;
    
  }
private:
	HTTPClient https;
	String makeFirebaseURL(const String& path, const String& auth, String& url) {
	  url="";
	  if (path[0] != '/') {
		url = "/";
	  }
	  url += path + ".json";
	  if (auth.length() > 0) {
		url += "?auth=" + auth;
	  }
	}
	
};

unsigned char h2int(char c)
{
    if (c >= '0' && c <='9'){
        return((unsigned char)c - '0');
    }
    if (c >= 'a' && c <='f'){
        return((unsigned char)c - 'a' + 10);
    }
    if (c >= 'A' && c <='F'){
        return((unsigned char)c - 'A' + 10);
    }
    return(0);
}

String urldecode(String str)
{
    
    String encodedString="";
    char c;
    char code0;
    char code1;
    for (int i =0; i < str.length(); i++){
        c=str.charAt(i);
      if (c == '+'){
        encodedString+=' ';  
      }else if (c == '%') {
        i++;
        code0=str.charAt(i);
        i++;
        code1=str.charAt(i);
        c = (h2int(code0) << 4) | h2int(code1);
        encodedString+=c;
      } else{
        
        encodedString+=c;  
      }
      
      yield();
    }
    
   return encodedString;
}

String urlencode(String str)
{
    String encodedString="";
    char c;
    char code0;
    char code1;
    char code2;
    for (int i =0; i < str.length(); i++){
      c=str.charAt(i);
      if (c == ' '){
        encodedString+= '+';
      } else if (isalnum(c)){
        encodedString+=c;
      } else{
        code1=(c & 0xf)+'0';
        if ((c & 0xf) >9){
            code1=(c & 0xf) - 10 + 'A';
        }
        c=(c>>4)&0xf;
        code0=c+'0';
        if (c > 9){
            code0=c - 10 + 'A';
        }
        code2='\0';
        encodedString+='%';
        encodedString+=code0;
        encodedString+=code1;
        //encodedString+=code2;
      }
      yield();
    }
    return encodedString;
    
}
