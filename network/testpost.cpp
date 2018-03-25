#include <pcap.h>
#include <stdio.h>
#include <iostream>
#include <unordered_map>
#include <string.h>
#include <ctime>
#include <iomanip>
#include <map>
#include <curl/curl.h>  
#include <cstdlib>
#include <fstream>


#define server "10.120.56.48:3000"
#define FILE_NAME "/../nodes.txt"
#define EVENT_NAME "HACKIOT"

int htmlPost(){
  CURL *curl;
  CURLcode res;
 
  char postthis[] = "hello";

  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, server);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postthis);
 
    /* if we don't provide POSTFIELDSIZE, libcurl will strlen() by
       itself */ 
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)sizeof(postthis));
 
    /* Perform the request, res will get the return code */ 
    res = curl_easy_perform(curl);
    /* Check for errors */ 
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
      curl_easy_strerror(res));
      return(1);
 
    /* always cleanup */ 
    curl_easy_cleanup(curl);
  }
  return(0);
}

int main(int argc, char const *argv[])
{
	htmlPost();
	return 0;
}