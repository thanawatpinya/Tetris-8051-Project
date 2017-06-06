#include <REGX52.H>
#include <stdlib.h>

void shiftOut(char sdata, char Gdata, char seg);
void DataOut(unsigned int sdata, unsigned int Gdata, unsigned int seg);
void Move_Down(unsigned int x[8]);
void Move_Left();
void Move_Right();
void Random();
void Clear_Model(void);
int Check_Location();
void Move_Rotate();
void Move_One_Down();
void Check_Block();
void check_Game_Over();
void check_Line();
void Check_Button();
void Check_floor();
void Save();

unsigned int model[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
unsigned int savemodel[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
unsigned int countpoint = 0x00;

int j,omb,count,width; // value Global
int Boxtype;
// ====================== Main ===============================
void main() {
	int i; // y1 and y2 for check left,right button push
	P3 = 0; // All P3 = 0
  P3_1 = 1; // Latch = 1 (Defualt)
	P1_0 = 1; // P1_0 is Left button
	P1_1 = 1; // P1_1 is Right button
	P1_2 = 1;
	P1_3 = 1;	// P1_3 is DownOne button
	count = 0;
	
	model[0] = 0x000C;
	model[1] = 0x091E;
	model[2] = 0xA93E;
	model[3] = 0x467C;
	model[4] = 0xF07C;
	model[5] = 0x063E;
	model[6] = 0x091E;
	model[7] = 0x0F0C;
	for(i=0;i<500;i++){
			while(j < 8){
				DataOut(~(model[j]|savemodel[j]), ~savemodel[j], countpoint);
				j++;
			}
			j=0;
		}
	Clear_Model();
	
  while(1) {
		Random();
    for(i=0;i<50;i++){
			while(j < 8){
				DataOut(~(model[j]|savemodel[j]), ~savemodel[j], countpoint);
				Check_Button();
				j++;
			}
			j=0;
		}
		// -------------- Check Stay On the Floor ------------------
		Check_floor();
		// ----------------------------------------------------------
		count++;
		Check_Block();
		Move_Down(model);
		check_Line();
		check_Game_Over();
    //delay1mSec(400);
  }
}

void Clear_Model(void){
	int i;
	for(i=0;i<8;i++){
		model[i] = 0x0000;
	}
}

// ========================== Control ===========================
int BO,BL,BR,BRo;
void Check_Button(){
	if(P1_0 == 1){
		BL = 1;
	}
	if(P1_0 == 0 && BL == 1){
		BL = 0;
		Move_Left();
	}
	if(P1_1 == 1){
		BR = 1;
	}
	if(P1_1 == 0 && BR ==1){
		BR = 0;
		Move_Right();
	}
	if(P1_2 == 1){
		BRo = 1;
	}
	if(P1_2 == 0 && BRo == 1){
		BRo = 0;
		Move_Rotate();
	}
	if(P1_3 == 1){
		BO = 1;
	}
	if(P1_3 == 0 && BO == 1){
		BO = 0;
		Move_One_Down();
	}
}

void Move_Down(unsigned int x[8]){ // -------- Move Down ------
	int i;
	for(i=0;i<8;i++){
		x[i] = x[i]<<1; // 0000 0000 0000 0100
	}
}

void Move_Left(){		// ---------------- Move Left -------------
	int i,PB0=0;
	unsigned int k;
				if(model[7] == 0x0000){ 
					for(i=6;i>=0;i--){
						k = model[i];
						model[i] = model[i+1];
						model[i+1] = k;
					}
				}
}
void Move_Right(){	// ---------------- Move Right ---------------
	int i,k;	
		if(model[0] == 0x0000){
			for(i=0;i<7;i++){
				k = model[i];
				model[i] = model[i+1];
				model[i+1] = k;
			}
		}
}
void Move_Rotate(){	// ----------------- Rotate ------------------
		switch(Boxtype)
			{
				case 0: // rotate Z
					if(model[0] != 0x0000){
					model[Check_Location()] = 0x04<<count;
					model[Check_Location()+2] = 0x02<<count;
					} else {
					model[Check_Location()-1] = 0x02<<count;
					model[Check_Location()+2] = 0x01<<count;
					}
					omb = 2;
					Boxtype = 4;
					width = 3;
				break;
				case 1: // rotate ----
					if(count == 13 || count == 14 || count == 15){
					model[Check_Location()] = 0x00;
					model[Check_Location()] = 0x0F<<12;
					model[Check_Location()+1] = 0x00;
					model[Check_Location()+2] = 0x00;
					} else {
					model[Check_Location()] = 0x00;
					model[Check_Location()] = 0x0F<<count;
					model[Check_Location()+1] = 0x00;
					model[Check_Location()+2] = 0x00;
					}
					omb = 4;
					Boxtype = 5;
					width = 1;
				break;
				case 2: // rotate L 
					if(model[0] != 0x0000){
					model[Check_Location()] = 0x02<<count;
					model[Check_Location()+1] = 0x02<<count;
					model[Check_Location()+2] = 0x03<<count;
					}
					model[Check_Location()-1] = 0x02<<count;
					model[Check_Location()+1] = 0x02<<count;
					model[Check_Location()+2] = 0x03<<count;
					omb = 2;
					Boxtype = 6;
					width = 3;
				break;
				case 3: // ratate |-
					model[Check_Location()] = 0x02<<count;
					model[Check_Location()+1] = 0x07<<count;
					model[Check_Location()+2] = 0x00;
					omb = 3;
					Boxtype = 9;
					width = 2;
				break;
				// ------------------ for defualt --------------------
				case 4: // rotate Z
					if(count == 14){
					model[Check_Location()] = 0x00;
					model[Check_Location()] = 0x03<<13;
					model[Check_Location()+1] = 0x06<<13;
					} else{
					model[Check_Location()] = 0x00;
					model[Check_Location()] = 0x03<<count;
					model[Check_Location()+1] = 0x06<<count;
					}
					omb = 3;
					Boxtype = 0;
					width = 2;
				break;
				case 5: // rotate ----
					if(count == 13 || count == 14 || count == 15){
						break;
					}
					if(model[5] != 0 || model[6] != 0 || model[7] != 0){
					model[4] = 0x01<<count;
					model[5] = 0x01<<count;
					model[6] = 0x01<<count;
					model[7] = 0x01<<count;
					} else if(model[0] != 0){
					model[0] = 0x01<<count;
					model[1] = 0x01<<count;
					model[2] = 0x01<<count;
					model[3] = 0x01<<count;
					} else{
					model[Check_Location()-1] = 0x01<<count;
					model[Check_Location()+1] = 0x01<<count;
					model[Check_Location()+2] = 0x01<<count;
					model[Check_Location()+3] = 0x01<<count;
					}
					omb = 1;
					Boxtype = 1;
					width = 4;
				break;
				case 6: // rotate L
					if(count == 14 || count == 15){
				  model[Check_Location()] = 0x01<<13;
					model[Check_Location()+1] = 0x07<<13;
					model[Check_Location()+2] = 0x00;
					} else{
					model[Check_Location()] = 0x01<<count;
					model[Check_Location()+1] = 0x07<<count;
					model[Check_Location()+2] = 0x00;
					}
					omb = 3;
					Boxtype = 7;
					width = 2;
				break;
				case 7: // rotate L
					if(model[7] != 0x0000){
					model[Check_Location()-1] = 0x03<<count;
					model[Check_Location()+1] = 0x01<<count;
					model[Check_Location()+2] = 0x01<<count;
					}
					model[Check_Location()] = 0x03<<count;
					model[Check_Location()+1] = 0x01<<count;
					model[Check_Location()+2] = 0x01<<count;
					omb = 2;
					Boxtype = 8;
					width = 2;
				break;
				case 8: // rotate L
					if(count == 14){
					model[Check_Location()] = 0x00;
					model[Check_Location()] = 0x07<<13;
					model[Check_Location()+1] = 0x04<<13;
					} else{
					model[Check_Location()] = 0x00;
					model[Check_Location()] = 0x07<<count;
					model[Check_Location()+1] = 0x04<<count;
					}
					omb = 3;
					Boxtype = 2;
					width = 2;
				break;
				case 9: // rotate -.-
					if(model[7] != 0x0000){
					model[Check_Location()-1] = 0x02<<count;
					model[Check_Location()+1] = 0x06<<count;
					model[Check_Location()+2] = 0x02<<count;	
					} else {
					model[Check_Location()+1] = 0x06<<count;
					model[Check_Location()+2] = 0x02<<count;
					}
					omb = 3;
					Boxtype = 10;
					width = 2;
				break;
				case 10: // ratate -|
					model[Check_Location()] = 0x00;
					model[Check_Location()] = 0x07<<count;
					model[Check_Location()+1] = 0x02<<count;
					omb = 3;
					Boxtype = 11;
					width = 2;
				break;
				case 11: // ratate -'-
					if(model[0] != 0x0000){
					model[Check_Location()] = 0x02<<count;
					model[Check_Location()+1] = 0x03<<count;
					model[Check_Location()+2] = 0x02<<count;
					} else{ 
					model[Check_Location()-1] = 0x02<<count;
					model[Check_Location()+1] = 0x03<<count;
					model[Check_Location()+2] = 0x02<<count;
					}
					omb = 2;
					Boxtype = 3;
					width = 2;
				break;
				case 12: // rotate Z reverse
					if(model[7] != 0x0000){
					model[Check_Location()-1] = 0x01<<count;
					model[Check_Location()+1] = 0x03<<count;
					model[Check_Location()+2] = 0x02<<count;
					} else {
					model[Check_Location()] = 0x01<<count;
					model[Check_Location()+2] = 0x02<<count;
					}
					omb = 2;
					Boxtype = 13;
					width = 3;
				break;
				case 13: // rotate Z reverse
					if(count == 14){
					model[Check_Location()] = 0x06<<13;
					model[Check_Location()+1] = 0x03<<13;
					model[Check_Location()+2] = 0x00;
					} else{
					model[Check_Location()] = 0x06<<count;
					model[Check_Location()+1] = 0x03<<count;
					model[Check_Location()+2] = 0x00;
					}
					omb = 3;
					Boxtype = 12;
					width = 2;
				break;
				case 14: // rotate L reverse
					if(model[0] != 0x0000){
					model[Check_Location()] = 0x01<<count;
					model[Check_Location()+1] = 0x01<<count;
					model[Check_Location()+2] = 0x03<<count;
					} else{
					model[Check_Location()-1] = 0x01<<count;
					model[Check_Location()+1] = 0x01<<count;
					model[Check_Location()+2] = 0x03<<count;
					}
					omb = 2;
					Boxtype = 15;
					width = 3;
				break;
				case 15: // rotate L reverse
					if(count == 14 || count == 15){
				  model[Check_Location()] = 0x07<<13;
					model[Check_Location()+1] = 0x01<<13;
					model[Check_Location()+2] = 0x00;
					} else{
					model[Check_Location()] = 0x07<<count;
					model[Check_Location()+1] = 0x01<<count;
					model[Check_Location()+2] = 0x00;
					}
					omb = 3;
					Boxtype = 16;
					width = 2;
				break;
				case 16: // rotate L reverse
					if(model[7] != 0x0000){
					model[Check_Location()-1] = 0x03<<count;
					model[Check_Location()+1] = 0x02<<count;
					model[Check_Location()+2] = 0x02<<count;
					}
					model[Check_Location()] = 0x03<<count;
					model[Check_Location()+1] = 0x02<<count;
					model[Check_Location()+2] = 0x02<<count;
					omb = 2;
					Boxtype = 17;
					width = 2;
				break;
				case 17: // rotate L reverse
					if(count == 14){
					model[Check_Location()] = 0x00;
					model[Check_Location()] = 0x04<<13;
					model[Check_Location()+1] = 0x07<<13;
					} else{
					model[Check_Location()] = 0x00;
					model[Check_Location()] = 0x04<<count;
					model[Check_Location()+1] = 0x07<<count;
					}
					omb = 3;
					Boxtype = 14;
					width = 2;
				break;
			}
}

void Move_One_Down(){ // ---------- One push for down all --------
	int i,j;
	for(i=16-count;i>=0;i--){
		Check_floor();
		Check_Block();
		Move_Down(model);
		if(i==(1+omb)){
			for(j=0;j<8;j++){
				savemodel[j] = savemodel[j] | model[j];
			}
			Clear_Model();
			break;
		}
	}
}

// ======================= Check Block =========================
void Check_Block(){
	int i,k;
	for(i=0;i<8;i++){
		for(k=0;k<8;k++){
			if((model[k]<<1)&savemodel[k]){ 
				for(j=0;j<8;j++){
						savemodel[j] = savemodel[j] | model[j];
				 }
				Clear_Model();
			}
		}
	}
}
void Check_floor(){
	if(count == 15 && omb == 1){Save();	Clear_Model();}
		if(count == 14 && omb == 2){	Save(); Clear_Model();}
		if(count == 13 && omb == 3 || count == 14 && omb == 3 || count == 15 && omb == 3){	Save();	Clear_Model();}
		if(count == 12 && omb == 4 || count == 13 && omb == 4 || count == 14 && omb == 4 || count == 15 && omb == 4){	Save();	Clear_Model();}
}
void Save(){
	int i;
		 for(i=0;i<8;i++){
					savemodel[i] = savemodel[i] | model[i];
			}
}
// ========================= Random ==========================
void Random(){
		if(model[0]==0x0000&&model[1]==0x0000&&model[2]==0x0000&&model[3]==0x0000&&
			model[4]==0x0000&&model[5]==0x0000&&model[6]==0x0000&&model[7]==0x0000){
			switch(rand()%7)
			{
				case 0: // Z
					model[3] = 0x0003;
					model[4] = 0x0006;
					omb = 3;
					Boxtype = 0;
					width = 2;
					break;
				case 1: // []
					model[3] = 0x0003;
					model[4] = 0x0003;
					omb = 2;
					Boxtype = -1;
					width = 2;
				  break;
				case 2: // ----
					model[2] = 0x0001;
					model[3] = 0x0001;
					model[4] = 0x0001;
					model[5] = 0x0001; 
					omb = 1;
					Boxtype = 1;
					width = 4; 
					break;
				case 3: // L
					model[3] = 0x0007;
					model[4] = 0x0004;
					omb = 3;
					Boxtype = 2;
					width = 2;
				  break;
				case 4: // -'-
					model[3] = 0x0002;
					model[4] = 0x0003;
					model[5] = 0x0002;
					omb = 2;
					Boxtype = 3;
					width = 2;
					break;
				case 5: // Z Reverse
					model[3] = 0x0006;
					model[4] = 0x0003;
					omb = 3;
					Boxtype = 12;
					width = 3;
					break;
				case 6: // L Reverse
					model[3] = 0x0004;
					model[4] = 0x0007;
					omb = 3;
					Boxtype = 14;
					width = 2;
				  break;
			}
			count = 0;
		}
}

// ======================== Output ===========================
int ibit=0;
void shiftOut(char Rdata, char Gdata, char seg) {
  for(ibit=7;ibit>=0;ibit--) {
    P3_2 = (Rdata>>ibit)&0x01;
		P3_3 = (Gdata>>ibit)&0x01;
		P3_4 = (seg>>ibit)&0x01;
    P3_0=1; // clock
    P3_0=0; // clock
  }
  P3_2=0;
}
void DataOut(unsigned int Rdata, unsigned int Gdata,unsigned int seg) {
  P3_1=0; // Latch = 0 (Start to send bit)
  shiftOut(Rdata>>8, Gdata>>8, seg>>8); // (send high bit first) 
  shiftOut(Rdata&0xFF, Gdata&0xFF, seg&0xFF); // (send low bit second)
	P2=(1<<j); // Refresh P2 for Outout
  P3_1=1; // Latch = 1 (End send bit)
}

// ===================== etc. Function =======================
int Check_Location(){
	int i;
	for(i=0;i<8;i++){
		if(model[i] != 0x0000){
			return i;
			break;
		}
	}
	return -1;
}

void check_Game_Over(){
	int i,j;
	for(i=0;i<8;i++){
		if((savemodel[i]&0x0001)==1){	
			Clear_Model();
			countpoint = 0x00;
			for(j=0;j<8;j++){
				savemodel[j] = 0x0000;
			}
		}
	}
}
void check_Line(){
	int j,k,b,p;
	unsigned int i,Mem = 0x0000;
	k=0;
	for(i=0x0001;i;i<<=1){
		for(j=0;j<8;j++){
			if(savemodel[j]&i){
				k++;
			}
			if(k == 8){
				for(b=0;b<8;b++){
					savemodel[b] = savemodel[b] & ~i;
				}
				if(i == 0x8000){			// Floor number 1
					Move_Down(savemodel);	
				}
				if(i == 0x4000){			// Floor number 2
					for(p=0;p<8;p++){
						Mem = savemodel[p] & 0x7FFF;
						savemodel[p] = savemodel[p] & 0x8000;
						Mem = Mem << 1;
						savemodel[p] = savemodel[p] | Mem;
					}
				}
				if(i == 0x2000){			// Floor number 3
					for(p=0;p<8;p++){
						Mem = savemodel[p] & 0x3FFF;
						savemodel[p] = savemodel[p] & 0xC000;
						Mem = Mem << 1;
						savemodel[p] = savemodel[p] | Mem;
					}
				}
				if(i == 0x1000){			// Floor number 4
					for(p=0;p<8;p++){
						Mem = savemodel[p] & 0x1FFF;
						savemodel[p] = savemodel[p] & 0xE000;
						Mem = Mem << 1;
						savemodel[p] = savemodel[p] | Mem;
					}
				}
				if(i == 0x0800){			// Floor number 5
					for(p=0;p<8;p++){
						Mem = savemodel[p] & 0x0FFF;
						savemodel[p] = savemodel[p] & 0xF000;
						Mem = Mem << 1;
						savemodel[p] = savemodel[p] | Mem;
					}
				}
				if(i == 0x0400){			// Floor number 6
					for(p=0;p<8;p++){
						Mem = savemodel[p] & 0x07FF;
						savemodel[p] = savemodel[p] & 0xF800;
						Mem = Mem << 1;
						savemodel[p] = savemodel[p] | Mem;
					}
				}
				if(i == 0x0200){			// Floor number 7
					for(p=0;p<8;p++){
						Mem = savemodel[p] & 0x03FF;
						savemodel[p] = savemodel[p] & 0xFC00;
						Mem = Mem << 1;
						savemodel[p] = savemodel[p] | Mem;
					}
				}
				if(i == 0x0100){			// Floor number 8
					for(p=0;p<8;p++){
						Mem = savemodel[p] & 0x01FF;
						savemodel[p] = savemodel[p] & 0xFF00;
						Mem = Mem << 1;
						savemodel[p] = savemodel[p] | Mem;
					}
				}
				if(i == 0x0080){			// Floor number 9
					for(p=0;p<8;p++){
						Mem = savemodel[p] & 0x007F;
						savemodel[p] = savemodel[p] & 0xFF80;
						Mem = Mem << 1;
						savemodel[p] = savemodel[p] | Mem;
					}
				}
				if(i == 0x0040){			// Floor number 10
					for(p=0;p<8;p++){
						Mem = savemodel[p] & 0x003F;
						savemodel[p] = savemodel[p] & 0xFFC0;
						Mem = Mem << 1;
						savemodel[p] = savemodel[p] | Mem;
					}
				}
				if(i == 0x0020){			// Floor number 11
					for(p=0;p<8;p++){
						Mem = savemodel[p] & 0x001F;
						savemodel[p] = savemodel[p] & 0xFFE0;
						Mem = Mem << 1;
						savemodel[p] = savemodel[p] | Mem;
					}
				}
				if(i == 0x0010){			// Floor number 12
					for(p=0;p<8;p++){
						Mem = savemodel[p] & 0x000F;
						savemodel[p] = savemodel[p] & 0xFFF0;
						Mem = Mem << 1;
						savemodel[p] = savemodel[p] | Mem;
					}
				}
				if(i == 0x0008){			// Floor number 13
					for(p=0;p<8;p++){
						Mem = savemodel[p] & 0x0007;
						savemodel[p] = savemodel[p] & 0xFFF8;
						Mem = Mem << 1;
						savemodel[p] = savemodel[p] | Mem;
					}
				}
				if(i == 0x0004){			// Floor number 14
					for(p=0;p<8;p++){
						Mem = savemodel[p] & 0x0003;
						savemodel[p] = savemodel[p] & 0xFFFC;
						Mem = Mem << 1;
						savemodel[p] = savemodel[p] | Mem;
					}
				}
				if(i == 0x0002){			// Floor number 15		And Floor number 16 GameOver
					for(p=0;p<8;p++){
						Mem = savemodel[p] & 0x0002;
						savemodel[p] = savemodel[p] & 0xFFFE;
						Mem = Mem << 1;
						savemodel[p] = savemodel[p] | Mem;
					}
				}
				countpoint = countpoint + 1;
				if(countpoint == 0x0A || countpoint == 0x1A || countpoint == 0x2A || countpoint == 0x3A
					|| countpoint == 0x4A || countpoint == 0x5A || countpoint == 0x6A || countpoint == 0x7A
					|| countpoint == 0x8A){
					countpoint = countpoint + 6;
				}
					break;
			}
		}
		k=0;
	}
}

// ===========================================================