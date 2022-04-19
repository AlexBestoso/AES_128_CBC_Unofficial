#include "./aes128.h"
#include <stdio.h>
#include <string.h>

class Aes128Cbc{
	private:
	Aes128 aes128;

	/*
	 * Used to ensure that the buffer containing the ciphertext 
	 * is a multiple of 16 (128 bits)
	 * */
	size_t calcCipherTextSize(size_t plaintextSize){
		size_t ret = 0;
		if(plaintextSize >= 16){
			// Get number of blocks, ensure it's a multiple of block size.
                	ret = (plaintextSize/16)*16;
                	// if stringSize is greater than stateSize, we're one block short.
                	if(plaintextSize > ret){
                	        ret += 16;
                	}
        	}else{
        	        ret = 16;
        	}

		return ret;
	}

	public:
	const size_t keySize = 16;
	const size_t ivSize = 16;
	const size_t stateSize = 16;

	char *result = NULL;
	size_t resultSize = 0;

	void encipher(char *plaintext, size_t plaintextSize, char iv[16], char key[16]){
		size_t messageSize = this->calcCipherTextSize(plaintextSize);
		char message[messageSize]; 
		
		/* Format plaintext so it fits in blocks. */
        	for(int i=0; i<plaintextSize; i++)
        	        message[i] = plaintext[i];

        	for(int i=plaintextSize; i<messageSize; i++)
        	        message[i] = 0x00;


		char tmp_key[16];
		unsigned int blockCount = messageSize/this->stateSize;
	
		char state_container[blockCount][16];
		for(int i=0; i<blockCount; i++){
			for(int j=0; j<this->stateSize; j++)
			state_container[i][j] = '\0';
		}
	
		signed int inc = -1;
		for(int i=0; i<messageSize; i++){
			if((i%this->stateSize) == 0){
				inc++;
			}
			state_container[inc][i%this->stateSize] = message[i];
		}

		for(int i=0; i<blockCount; i++){
			for(int j=0; j<this->keySize; j++)
				tmp_key[j] = key[j];
		
			for(int j=0; j<this->ivSize; j++)
				state_container[i][j] = state_container[i][j] ^ iv[j];
			
			this->aes128.encipher(state_container[i], tmp_key);
	
			for(int j=0; j<this->ivSize; j++)
				iv[j] = state_container[i][j];
		}	
	
		for(int i=0; i<blockCount; i++){
			for(int j=0; j<this->stateSize; j++){
				message[(i*this->stateSize)+j] = state_container[i][j];
			}
		}
		
		if(this->result != NULL){
			free(this->result);
			this->result=NULL;
		}
		this->resultSize = messageSize;
		this->result = (char *)malloc(sizeof(char)*messageSize);
		for(int i=0; i<messageSize; i++)
			this->result[i] = message[i];
	}

	void decipher(char *ciphertext, size_t ciphertextSize, char iv[16], char key[16]){
		size_t messageSize = this->calcCipherTextSize(ciphertextSize);
                char message[messageSize];

		/* Format ciphertext so it fits in blocks. */
                for(int i=0; i<ciphertextSize; i++)
                        message[i] = ciphertext[i];

                for(int i=ciphertextSize; i<messageSize; i++)
                        message[i] = 0x00;

        	char tmp_key[16];
		char tmp_IV[16];
        	unsigned int blockCount = messageSize/this->stateSize;
	
	        char msg_container[blockCount][16];
	        for(int i=0; i<blockCount; i++){
	                for(int j=0; j<this->stateSize; j++)
	                msg_container[i][j] = '\0';
	        }
	
	        signed int inc = -1;
	        for(int i=0; i<messageSize; i++){
	                if((i%this->stateSize) == 0){
	                        inc++;
	                }
	                msg_container[inc][i%this->stateSize] = message[i];
	        }
	
	        for(int i=0; i<blockCount; i++){
	                for(int j=0; j<this->keySize; j++)
	                        tmp_key[j] = key[j];
			for(int j=0; j<this->stateSize; j++){
				tmp_IV[j] = msg_container[i][j];
			}
	
			this->aes128.decipher(msg_container[i], tmp_key);
		
	                for(int j=0; j<this->ivSize; j++)
	                        msg_container[i][j] = msg_container[i][j] ^ iv[j];
	
	                for(int j=0; j<this->ivSize; j++)
	                        iv[j] = tmp_IV[j];
	        }
		
	        for(int i=0; i<blockCount; i++){
	                for(int j=0; j<this->stateSize; j++){
	                        message[(i*this->stateSize)+j] = msg_container[i][j];
	                }
	        }

                if(this->result != NULL){
                        free(this->result);
			this->result = NULL;
		}
		this->resultSize = messageSize;
                this->result = (char *)malloc(sizeof(char)*messageSize);
                for(int i=0; i<messageSize; i++)
                        this->result[i] = message[i];
	}
};

int main(int argc, char *argv[]){
	if(argc != 3){
		printf("[-] Err, program takes only two argument\n");
		printf("[%s \"message\"] [-e/-d]\n", argv[0]);
		return 1;
	}

	
	char iv[16];
	char key[16];
	strncpy(iv, "0123456789abcdef", 16);
        strncpy(key, "b87389r2@@%V4732\0", 16);
	
	Aes128Cbc aes;

	if(!strcmp(argv[2], "-e")){
		aes.encipher(argv[1], strlen(argv[1]), iv, key);
		for(int i=0; i<aes.resultSize; i++){
			printf("%c", aes.result[i]);
		}
	}else if(!strcmp(argv[2], "-d")){
		aes.decipher(argv[1], strlen(argv[1]), iv, key);
                printf("Plain Text Size : %ld\n", (long)aes.resultSize);
                printf("Result : ");
                for(int i=0; i<aes.resultSize; i++){
                        printf("%c", aes.result[i]);
                }printf("\n");
	}	


	exit(EXIT_SUCCESS);
}
