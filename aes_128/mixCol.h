//#include "specialMath.h"
void MixColumns(char state[17]){
        const unsigned int mc_constant[16] =
        {
                0x02, 0x03, 0x01, 0x01,
                0x01, 0x02, 0x03, 0x01,
                0x01, 0x01, 0x02, 0x03,
                0x03, 0x01, 0x01, 0x02
        };

	unsigned int tmp_state[16];
        unsigned int tmp;
        unsigned int word[4];
        unsigned int word_tmp[4];

	for(int i=0; i<16; i++){
		tmp_state[i] = (state[i] & 0xf0)+(state[i] & 0x0f);
	}

        for(int i=0; i<4; i++){
                for(int j=0; j<4; j++){
                        word[j] = tmp_state[(i*4)+j];
                }
                
                for(int k=0; k<4; k++){
                        tmp = 0x0;
                        for(int l=0; l<4; l++){
                                word_tmp[l] = galoisMultiply(mc_constant[(k*4)+l], word[l]);
                                tmp = tmp ^ word_tmp[l];
                        }
                        state[(i*4)+k] = tmp;
                }
        }
}

void inv_MixColumns(char state[17]){
        const unsigned int mc_constant[16] =
        {
                0x0e, 0x0b, 0x0d, 0x09,
                0x09, 0x0e, 0x0b, 0x0d,
                0x0d, 0x09, 0x0e, 0x0b,
                0x0b, 0x0d, 0x09, 0x0e
        };
	unsigned int tmp_state[16];
        unsigned int tmp;
        unsigned int word[4];
        unsigned int word_tmp[4];

	for(int i=0; i<16; i++){
		tmp_state[i] = (state[i] & 0xf0)+(state[i] & 0x0f);
	}

        for(int i=0; i<4; i++){
                for(int j=0; j<4; j++){
                        word[j] = tmp_state[(i*4)+j];
                }

                for(int k=0; k<4; k++){
                        tmp = 0x0;
                        for(int l=0; l<4; l++){
                                word_tmp[l] = galoisMultiply(mc_constant[(k*4)+l], word[l]);
                                tmp = tmp ^ word_tmp[l];
                        }
                        state[(i*4)+k] = tmp;
                }
        }
}
