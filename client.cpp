#include <iostream>
#include <pcap.h>
#include <netinet/ip.h>
#include <vector>
#include <algorithm>
#include <map>
#include <unistd.h>

#define SIZE_ETHERNET 14

std::vector<std::string> ipvec;
std::string servadrr;
//std::string thisdevaddr;

void sendadr(std::string adr){
    int sockfd, n;
    struct sockaddr_in serv_addr;

    char buffer[256];
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
       std::cout<<"SOCK ERROR"<<std::endl;     
    

    serv_addr.sin_family = AF_INET;
    inet_pton(AF_INET, servadrr.data(), &(serv_addr.sin_addr));
    serv_addr.sin_port = htons(20001);
    
    n= sendto(sockfd,adr.data(),adr.length(),0,(struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if ((n==0) or (n==(-1))){
        std::cout<<"senderror to "<<serv_addr.sin_addr.s_addr <<std::endl;
    }

    close(sockfd);

}

void getaddr(pcap_pkthdr *header, const u_char *packet){
    struct ip* lip=(struct ip*)(packet + SIZE_ETHERNET);
        char sourceip[INET_ADDRSTRLEN];
        char destip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(lip->ip_src.s_addr), sourceip, INET_ADDRSTRLEN);
        inet_ntop(AF_INET, &(lip->ip_dst.s_addr), destip, INET_ADDRSTRLEN);
        std::string IP_srsip=sourceip;
        std::string IP_destip=destip;
        if(std::find(ipvec.begin(), ipvec.end(),IP_srsip)==std::end(ipvec)){
            ipvec.push_back(IP_srsip);
            sendadr(IP_srsip);
        }
        if(std::find(begin(ipvec), end(ipvec),IP_destip)==std::end(ipvec)){
            ipvec.push_back(IP_destip);
            sendadr(IP_destip);
        }

}

int main() {

    std::string indevv;
    std::map<std::string,std::string> mappo;
    char errbuf[PCAP_ERRBUF_SIZE+1];

    // Выбор интерфейса
    std::cout<< "Найденные интерфейсы:" << std::endl;

    pcap_if *devises;

    if (pcap_findalldevs(&devises,errbuf)!=-1){
        for(pcap_if_t *devise=devises;devise;devise=devise ->next){
            std::cout <<"Имя: "<< devise->name<<std::endl;
            if (devise->description)
                std::cout <<"Описание: "<< devise->description<<std::endl; 

            pcap_addr_t *adr;
            for(adr=devise->addresses;adr;adr=adr->next) {
                switch(adr->addr->sa_family){
                case AF_INET:
                    std::cout <<"AF_INET"<<std::endl;
                    std::cout<<"Адрес: "<<inet_ntoa(((struct sockaddr_in*)adr->addr)->sin_addr)<<std::endl;
                    mappo[devise->name] = std::string(inet_ntoa(((struct sockaddr_in*)adr->addr)->sin_addr));
                    std::cout<<"Маска: "<<inet_ntoa(((struct sockaddr_in*)adr->netmask)->sin_addr)<<std::endl;

                default:
                    std::cout<<"---"<<std::endl;
                    break; 
                }
            }
            std::cout<<std::endl<<"-------------------------------------------"<<std::endl<<std::endl;        
        }
    }

    std::cout<<"Map: "<<std::endl;
    for(const auto& elem : mappo)
    {
    std::cout << elem.first << " " << elem.second << "\n";
    }

    std::cout << "Введите название интерфейса: ";
    std::cin >> indevv;

    std::cout << "Введите адрес цензора: ";
    std::cin >> servadrr;

    std::cout << "Вы выбрали устройство "<< indevv <<"!"<<std::endl;
    for(const auto& elem : mappo)
    {
        if (elem.first==indevv){
            ipvec.push_back(elem.second);
        }
    }

    // Начало записи
    pcap_t *indescr = pcap_open_live(indevv.data() ,BUFSIZ,1,1,errbuf);

    if (indescr == NULL){
        std::cout <<"Ошибка при открытии:"<< indevv.data() << std::endl;
    }

    struct pcap_pkthdr *header;
    const u_char *packet;

    while (ipvec.size()<31){
        int bggt = pcap_next_ex(indescr,&header, &packet);    
        getaddr(header,packet);  
    }

    for (auto i:ipvec){
        std::cout<<i<<std::endl;
    }
    pcap_close(indescr);

    return 0;
}
