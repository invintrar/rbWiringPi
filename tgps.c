#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <wiringPi.h>
#include <wiringSerial.h>

#define device "/dev/ttyS0"

int main(){
	int serial_port;
	char data,buff[100],GGA_code[3];
	unsigned char IsitGGAstring = 0;
	unsigned char GGA_index = 0;
	unsigned char is_GGA_received_completely = 0;

	if((serial_port = serialOpen(device,9600)) < 0){
		fprintf(stderr, "Unable to open serial device: %s\n",strerror(errno));
		return 1;
	}
	if(wiringPiSetup() == -1){
		fprintf(stdout, "Unable to start wiringPi: %s\n", strerror(errno));
		return 1;
	}

	while(1){
		if(serialDataAvail( serial_port)){
			data = serialGetchar(serial_port);
			if(data = '$'){
				IsitGGAstring = 0;
				GGA_index = 0;
			}
			else if(IsitGGAstring == 1){
				buff[GGA_index++] = data;
				if (data == '\r'){
					is_GGA_received_completely = 1;
				}
			else if(GGA_code[0] == 'G' && GGA_code[1] == 'G' && GGA_code[2] == 'A'){
				IsitGGAstring = 1;
				GGA_code[0] = 0;
				GGA_code[0] = 0;
				GGA_code[0] = 0;
				}
				else{
					GGA_code[0] = GGA_code[1];
					GGA_code[1] = GGA_code[2];
					GGA_code[2] = data;
				}
			}
		}// end if
		if(is_GGA_received_completely == 1){
			printf("GGA: %s ", buff);
			is_GGA_received_completely = 0;
		}//end if
		
	}// end while

	serialClose(serial_port);
	return 0;
}
