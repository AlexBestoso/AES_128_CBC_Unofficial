#include "aes_128/aes.h"
#include <stdio.h>

void aes_128_cbc_encipher(char *state, size_t size, char IV[17], char key[17]){
	char tmp_key[17];
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
	
		for(int j=0; j<16; j++)
			msg_container[i][j] = msg_container[i][j] ^ IV[j];
		
		aes_128_encipher(msg_container[i], tmp_key);

		for(int j=0; j<16; j++)
			IV[j] = msg_container[i][j];
	}	
	
	for(int i=0; i<blockCount; i++){
		for(int j=0; j<16; j++){
			state[(i*16)+j] = msg_container[i][j];
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
		printf("\n");
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

int main(int argc, char *argv[]){
	if(argc != 2){
		printf("[-] Err, program takes only one argument\n");
		printf("[%s \"message\"]\n", argv[0]);
		return 1;
	}
	
	char IV[17] = "0123456789abcdef\0";
	char IV_2[17] = "0123456789abcdef\0";

	char key[17] = "b87389r2@@%V4732\0";
	size_t size=0, size_II=0;
	char *state;

	while(argv[1][size] != '\0'){
		size++;
	}
	state = malloc(size);
	for(int i=0; i<size; i++){
		*(state + i) = argv[1][i];
	}

	if(size >= 16){
		size_II = (size/16)*16;
		if(size > size_II){
			size_II += 16;
		}
	}else{
		size_II = (16-size)+size;
	}

	state = realloc(state, size_II);
	if(state == NULL){
		printf("Reallocation failed\n");
		return 1;
	}
	for(int i=size; i<size_II; i++){
		*(state+i) = '\0';
	}
	
	printf("Your message:\n");
	for(int i=0; i<size_II; i++){
		printf("%c", state[i]);
	}

	aes_128_cbc_encipher(state, size_II, IV, key);

	printf("\n\nENCIPHERED: \n");
	for(int i=0; i<size_II; i++){
                printf("%c", state[i]);
        }

	aes_128_cbc_decipher(state, size_II, IV_2, key);

	printf("DECIPHERED: \n");
        for(int i=0; i<size_II; i++){
                printf("%c", state[i]);
        }

	printf("\n");

	if(state != NULL){
		free(state);
	}
	return 0;
}
