#include <pcap.h>
#include <stdio.h>

//a really bad frame
// struct probeRequest
// {
//   u_char radioTapHeader [56];
//   u_char probeRequestInfo [24];
//   u_char other [73];
// };

int main(int argc, char const *argv[])
{
  char* dev = argv[1];
  printf("Device: %s \n",dev);

  pcap_t* handle;
  char errbuf[PCAP_ERRBUF_SIZE];
  struct bpf_program fp;
  char filter_exp[] = ""; //filter 802.11 wifi packets
  bpf_u_int32 mask; //netmask of sniffing device
  bpf_u_int32 net; //IP of sniffing device
  struct pcap_pkthdr header;
  const u_char* packet;




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
  const char* mac;

  while(1){
    packet = pcap_next(handle,&header);
    printf("Captured packet length:%d \n",header.len);
    //printf("Captured data: %s",packet);
    if(packet[56] == 0x40){
      printf("Received probe request \n");
      //Trasmitter MAC address are bytes 66-71

      printf("Sender MAC Address:");
      // printf("%s",mac);
      int i;
      for(i = 0; i < 6; i ++){
        // if((i % 2) == 0){
        //   printf("%d",packet[66+i/2]%0xF);
        // }
        // else{
        //   printf("%d",packet[66+i/2]-(packet[66+i/2]%0xF));
        // }
        printf("%d",packet[66+i]);
        //printf("%d",(packet[66+i]));
        printf("\n"); 
      }
    }
  }

  pcap_close(handle);



  return 0;
}

