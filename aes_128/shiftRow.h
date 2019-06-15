//#include "specialMath.h"

void ShiftRows(char state[17]){
        char tmp[16];
        unsigned int tmp_[16];
        unsigned int shiftAmount;

        for(int i=0; i<16; i++){
                shiftAmount = (bestModu((0-i) , 8)) % 4;
                if(shiftAmount < 0){
                        shiftAmount = shiftAmount * -1;
                }
                tmp[bestModu(i+(shiftAmount*4), 16)] = state[i];
        }

        for(int i=0; i<16; i++){
                state[i] = tmp[i];
        }
}

void inv_ShiftRows(char state[17]){
        char tmp[16];
        int tmp_[16];
        int shiftAmount;

        for(int i=0; i<16; i++){
                shiftAmount = bestModu((0-i) , 8) % 4;
                if(shiftAmount < 0){
                        shiftAmount = shiftAmount * -1;
                }
                tmp[bestModu(i-(shiftAmount*4), 16)] = state[i];
        }

        for(int i=0; i<16; i++){
                state[i] = tmp[i];
        }
}
