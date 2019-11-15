#include "aes_128/aes.h"
#include <stdio.h>
#include <string.h>

void aes_128_cbc_encipher(char *state, size_t size, char IV[17], char key[17]){
	char tmp_key[17];
	unsigned int blockCount = size/16;
	
	char state_container[blockCount][17];
	for(int i=0; i<blockCount; i++){
		for(int j=0; j<17; j++)
		state_container[i][j] = '\0';
	}
	
	signed int inc = -1;
	for(int i=0; i<size; i++){
		if((i%16) == 0){
			inc++;
		}
		state_container[inc][i%16] = state[i];
	}

	for(int i=0; i<blockCount; i++){
		for(int j=0; j<17; j++)
			tmp_key[j] = key[j];
	
		for(int j=0; j<16; j++)
			state_container[i][j] = state_container[i][j] ^ IV[j];
		
		aes_128_encipher(state_container[i], tmp_key);

		for(int j=0; j<16; j++)
			IV[j] = state_container[i][j];
	}	
	
	for(int i=0; i<blockCount; i++){
		for(int j=0; j<16; j++){
			state[(i*16)+j] = state_container[i][j];
		}
	}
}

void aes_128_cbc_decipher(char *state, size_t size, char IV[17], char key[17]){
        char tmp_key[17];
	char tmp_IV[17];
        unsigned int blockCount = size/16;

        char msg_container[blockCount][17];
        for(int i=0; i<blockCount; i++){
                for(int j=0; j<17; j++)
                msg_container[i][j] = '\0';
        }

        signed int inc = -1;
        for(int i=0; i<size; i++){
                if((i%16) == 0){
                        inc++;
                }
                msg_container[inc][i%16] = state[i];
        }

        for(int i=0; i<blockCount; i++){
                for(int j=0; j<17; j++)
                        tmp_key[j] = key[j];
		for(int j=0; j<16; j++){
			tmp_IV[j] = msg_container[i][j];
		}

		aes_128_decipher(msg_container[i], tmp_key);
	
                for(int j=0; j<16; j++)
                        msg_container[i][j] = msg_container[i][j] ^ IV[j];

                for(int j=0; j<16; j++)
                        IV[j] = tmp_IV[j];
        }
	
        for(int i=0; i<blockCount; i++){
                for(int j=0; j<16; j++){
                        state[(i*16)+j] = msg_container[i][j];
                }
        }
}

#define STATE_MAX 900000
struct AES_128_Struct{
        char state[STATE_MAX];
        size_t state_size;
        char key[17];
        char IV[17];
	int error;
};

#define AES_ENCRYPT 0
#define AES_DECRYPT 1

struct AES_128_Struct AES_128_CBC(int mode, char *data, struct AES_128_Struct aes_128_struct){
	size_t messageSize=0, stateSize=0;
	messageSize = strlen(data);
        /*
         * If size is greater than 16, then our state
         * needs extra blocks.
         */
        if(messageSize >= 16){
                /*
                 * get amount of 128 bit chunks.
                 * multiply that amount by 16(128 bits)
                 */
                stateSize = (messageSize/16)*16;
                // if stringSize is greater than stateSize, we're one block short.
                if(messageSize > stateSize){
                        stateSize += 16;
                }
        /*
         * Else the size is 16 bytes
         */
        }else{
                stateSize = 16;
        }
	/*
         * Fill state with your secret message
         */
        char state[stateSize];
        for(int i=0; i<messageSize; i++){
                state[i] = data[i];
        }
        /*
         * Fill the rest of the block with null bytes.
         */
        for(int i=messageSize; i<stateSize; i++){
                state[i] = 0x00;
        }

	aes_128_struct.error = 0;
	if(stateSize >= STATE_MAX){
		aes_128_struct.error = 1;
		return aes_128_struct;
	}
	memset(aes_128_struct.state, '\0', STATE_MAX);
	strncpy(aes_128_struct.state, state, stateSize);	
        aes_128_struct.state_size = stateSize;

        /*
         * Enicipher function
         */
        if(mode == 0){
                aes_128_cbc_encipher(aes_128_struct.state, aes_128_struct.state_size, aes_128_struct.IV, aes_128_struct.key);
		return aes_128_struct;
        }	
        /*
         * Decipher function
         */
        if(mode == 1){
                aes_128_cbc_decipher(aes_128_struct.state, aes_128_struct.state_size, aes_128_struct.IV, aes_128_struct.key);
		return aes_128_struct;
        }
}

int main(int argc, char *argv[]){
	if(argc != 3){
		printf("[-] Err, program takes only two argument\n");
		printf("[%s \"message\"] [-e/-d]\n", argv[0]);
		return 1;
	}

	int mode = 0;
	if(!strcmp(argv[2], "-e")){
		mode = AES_ENCRYPT;
	}else if(!strcmp(argv[2], "-d")){
		mode = AES_DECRYPT;
	}	
	
	struct AES_128_Struct aes;
	strncpy(aes.IV, "0123456789abcdef\0", 17);
        strncpy(aes.key, "b87389r2@@%V4732\0", 17);

	aes = AES_128_CBC(mode, argv[1], aes);
	if(aes.error){
		printf("Encryption Failed\n");
		exit(EXIT_FAILURE);
	}

	for(int i=0; i<aes.state_size; i++){
		printf("%c", aes.state[i]);
       	}
	exit(EXIT_SUCCESS);
}
