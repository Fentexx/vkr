#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main() {

	while (true){
		system("torbrowser-launcher");
		sleep(15);
		system("pkill firefox.real");	
		sleep(1);
		system("echo 1836 | sudo pkill tor");		
		sleep(2);	
		system("rm ~/.local/share/torbrowser/tbb/x86_64/tor-browser_en-US/Browser/TorBrowser/Data/Tor/state");
		sleep(2);	
	}
	return 0;
}
