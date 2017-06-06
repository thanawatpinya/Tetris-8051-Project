#include <REGX51.H>
#include <math.h>
void sendserial(unsigned char gireesh);
int Convert2To10(char a[16], int y);
void display(int delay);
void move_down(unsigned char x[8][8]);
void ShiftOut(unsigned char bits);
unsigned char model[16] = {0,0,0,0,0,1,1,0,0,0,0,0,1,1,1,0};
//unsigned char m1[4][4];
//unsigned char m2[4][4];
//unsigned char m3[4][4];
//unsigned char m4[4][4];
/*unsigned char xdata model[8][8] = {
										  {0,0,0,0,0,0,0,0},
										  {0,0,0,0,0,0,0,0},
										  {0,0,0,0,0,0,1,0},
										  {0,0,0,0,0,1,1,1},
										  {0,0,0,0,0,0,0,0},
										  {0,0,0,0,0,0,0,0},
									    {0,0,0,0,0,0,0,0},
										  {0,0,0,0,0,0,0,0}
										  }; 
*/
void move_down(unsigned char x[8][8]){
	unsigned int i,j;
	for(i=0; i<8; i++){
		for(j=0; j<8; j++){
			x[i][j] = x[i][j+1];
		}
	}
}
void delay1mSec(int mSec){
	unsigned int i,j;
	for(i = 0; i<mSec; i++)
		for(j = 0; j<100; j++);
}
unsigned int i,k,temp;
void display(int delay){
	unsigned int j;
	for(j=0; j<= delay; j++){
		P1_1 = 0; // ST_CP set Low (Latch)(ST_CP)
		for(i=0; i<8; i++){
				ShiftOut(~model[i]);
				P3 = (1 << i);
		}
		P1_1 = 1; // ST_CP set High (Latch)(ST_CP)
		P1_1 = 0; // ST_CP set Low (Latch)(ST_CP)
		for(k=8; k<16;k++){
			ShiftOut(~model[k]);
				P3 = (1 << (k-8));
		}
		P1_1 = 1; // ST_CP set High (Latch)(ST_CP)
	}
}
void main(){ 
	P1 = 0x00;
	P2 = 0x00;
	P3 = 0x00; //SH_CP CLOCK
	while(1){
		display(100);
		//move_down(model);
	}
}

void sendserial(unsigned char gireesh) {
	unsigned char gk;
			P1_0 = 0; // CLOCK
			P1_1 = 0; // Sent Data
	for (gk = 0x80; gk; gk >>= 1) {
		if (gireesh & gk){
			P3_2 = 1; // Data 1
		}else P3_2 = 0; // Data 0
		P1_0 = 1; // CLOCK
		P1_0 = 0; // CLOCK
	}
		P3 = (1 << i);
		P3_1 = 1; // Sent Data Complete
}

void ShiftOut(unsigned char bits){
	unsigned char y;
	
	P1_0 = 0; // Clock set Low (Default)(SH_CP)
	//for(y = 0; y<8; y++){
		if(bits & y){
			P1_2 = 1; // Sent Data bit 1 (RD)
		}else P1_2 = 0; // Sent Data bit 0 (Rd)
		P1_0 = 1; // Clock set High 1(SH_CP)
		delay1mSec(1);
		P1_0 = 0; // Clock set Low 0(SH_CP)
	//}

	
}
int Convert2To10(char model[16], int y) {
	unsigned int j, k = 2, sum = 0;
			if(j == 0){ 
				sum += (1 * model[y]);
			} else {
				sum += (k * model[y]);
				k = k * 2;
				}
	return sum;
}