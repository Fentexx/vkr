#include <iostream>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <vector>
#include <algorithm>
#include <fstream>

int main(int argc, char *argv[])
{
    int sockfd, len;
    char buff[1024] = {0};
    bool flag = true;
    struct sockaddr_in serv_addr;
    std::vector<std::string> censoripvec;
    std::ofstream myfile;
    myfile.open ("adresses.txt");

    sockfd =  socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        std::cout<<"SOCK ERROR"<<std::endl;    
        return 1;   
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;  
    serv_addr.sin_addr.s_addr = INADDR_ANY;  
    serv_addr.sin_port = htons(20001);

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        std::cout<<"BIND ERROR"<<std::endl;
        return 0;        
    }

    while (censoripvec.size()<9000){
        ssize_t readStatus = recvfrom(sockfd, buff, 1024, 0, (struct sockaddr*)&serv_addr, (socklen_t*)&len);
        buff[readStatus] = '\0';   
        std::string buffip(buff);
        myfile<<buffip<<std::endl;
        if(std::find(censoripvec.begin(), censoripvec.end(),buffip)==std::end(censoripvec)){
            censoripvec.push_back(buffip);
            std::cout<<"NA: "<<buffip<<std::endl;
        }
        buff[0]=0;   
    }

    for (auto i:censoripvec){
        std::cout<<i<<std::endl;
    }
     
    close(sockfd);
    myfile.close();
    return 0; 
}
