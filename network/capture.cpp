#include <pcap.h>
#include <stdio.h>
#include <iostream>
#include <unordered_map>
#include <string>
#include <ctime>
#include <iomanip>
#include <map>

//a really bad frame
// struct probeRequest
// {
//   u_char radioTapHeader [56];
//   u_char probeRequestInfo [24];
//   u_char other [73];
// };

struct MacRecord
{
  std::string address;
};

struct tm y2k = {0};

bool operator < (const MacRecord& lhs, const MacRecord& rhs){
  return(lhs.address<rhs.address);
}

std::ostream& operator <<(std::ostream& os, const MacRecord& record){
  os << std::hex;
  for(char c: record.address){
    os << std::setfill('0') << std::setw(2) << (unsigned int)(unsigned char)c << ":";
  }
  os << std::dec << std::endl;
}

void retireExpiredAddresses(std::map<struct MacRecord,double>& macs){
  auto now = std::time(NULL);
  double s = difftime(now,mktime(&y2k));
  for(auto it = macs.begin(), end = macs.end(); it != end; ++it){
    if(it->second < s){
      auto dup = it++;
      macs.erase(dup);
    }
  }
}

void printAddressesInSystem(std::map<struct MacRecord,double>& macs){
  auto now = std::time(NULL);
  double nowS = difftime(now,mktime(&y2k));

  for(auto it = macs.begin(), end = macs.end(); it != end; ++it){
    std::cout << "Address: " << (it->first) << std::endl;
    double timeLeft = it->second - nowS;
    std::cout << "Expiration in " <<  timeLeft << std::endl;
  }
}

int getNodeCount(std::map<struct MacRecord,double>& macs){
  return(macs.size());
}


int main(int argc, char *argv[])
{
  char* dev = argv[1];
  printf("Device: %s \n",dev);

  pcap_t* handle;
  char errbuf[PCAP_ERRBUF_SIZE];
  struct bpf_program fp;
  char filter_exp[] = ""; 
  bpf_u_int32 mask; //netmask of sniffing device
  bpf_u_int32 net; //IP of sniffing device
  struct pcap_pkthdr header;
  const u_char* packet;

  std::map <struct MacRecord,double> macs;

  handle = pcap_open_live(dev,BUFSIZ,0,1000,errbuf);

  if(pcap_lookupnet(dev,&net,&mask,errbuf) == -1){
    printf("Can't get netmask \n");
    net = 0; 
    mask = 0;
  }

  if(handle == NULL){
    printf("Device could not be opened. \n");
    return(2);
  }

  if(pcap_compile(handle,&fp,filter_exp,0,net) == -1){
    printf("Could not compile filter expression\n");
    return(2);
  }

  if(pcap_setfilter(handle,&fp) == -1){
    printf("Could not set filter \n");
    return(2);
  }

  const struct probeRequest* frame;

  time_t now;
  double expirationSeconds;
  struct MacRecord current;
  current.address.reserve(10);
  while(1){
    current.address.clear();
    packet = pcap_next(handle,&header);
    //printf("Captured packet length:%d \n",header.len);

    if(packet[56] == 0x40){ // check if packet is a IEEE 802.11 probe request packet
      //printf("Received probe request \n");
      //Trasmitter MAC address are bytes 66-71

      // std::cout << std::hex;
      for(int i = 0; i < 6; i ++){
      current.address.push_back((uint)packet[66+i]);

      //   std::cout << std::setfill('0') << std::setw(2) << (int)packet[66+i];
      //   if(i < 5){
      //     std::cout << ':';
      //   }
      }
      // std::cout << std::endl;

      now = time(NULL);
      expirationSeconds = difftime(now,mktime(&y2k)) + 1800;
      if(macs.find(current) != macs.end()){
        //std::cout << "Found duplicate" << std::endl;
      }
      else{
        macs[current] = expirationSeconds;
      }
    }

    retireExpiredAddresses(macs);
    //printAddressesInSystem(macs);
    //std::cout << "Node Count:" << getNodeCount(macs) << std::endl;
  }

  pcap_close(handle);
  macs.clear();



  return 0;
}

