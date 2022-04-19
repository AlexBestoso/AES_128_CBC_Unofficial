/*
 *	This code currently only supports the ECB mode of operation.
 *	More modes will be added in the future.
 * */

#define ECB 0
#define KEY_SIZE 16
#define ROUND_COUNT 10
#define STATE_SIZE 16

#include <math.h>
#include <string.h>

class Aes128{
	public:
		void encipher(char state[STATE_SIZE], char key[KEY_SIZE]){
        		char expandedKey[KEY_SIZE * (ROUND_COUNT+1)];
        		int round = 0;

        		this->expandKey_128(key, expandedKey);

        		this->AddRoundKey_128(round, key, expandedKey, state);
        		round++;

        		for(int i=0; i<ROUND_COUNT - 1; i++){
                		this->SubBytes(state);

                		this->ShiftRows(state);
                		this->MixColumns(state);

                		this->AddRoundKey_128(round, key, expandedKey, state);
                		round++;
        		}

        		this->SubBytes(state);
        		this->ShiftRows(state);
	
        		this->AddRoundKey_128(round, key, expandedKey, state);
		}

		void decipher(char state[STATE_SIZE], char key[KEY_SIZE]){
        		char expandedKey[KEY_SIZE * (ROUND_COUNT+1)];
        		int round = ROUND_COUNT;
	
        		this->expandKey_128(key, expandedKey);

        		this->AddRoundKey_128(round, key, expandedKey, state);

        		this->inv_ShiftRows(state);
        		this->inv_SubBytes(state);

        		round--;
        		for(int i=0; i<ROUND_COUNT-1; i++){
        		        this->AddRoundKey_128(round, key, expandedKey, state);
        		        this->inv_MixColumns(state);
        		        this->inv_ShiftRows(state);

        		        this->inv_SubBytes(state);
        		        round--;
        		}

        		this->AddRoundKey_128(round, key, expandedKey, state);
		}

	private:
		int mode = ECB;
		const unsigned int sBox[16*16] =
                        {
        0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
        0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
        0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
        0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
        0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
        0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
        0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
        0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
        0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
        0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
        0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
        0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
        0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
        0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
        0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
        0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
                        };

		const unsigned int inv_sBox[16*16] =
                        {
        0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
        0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
        0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
        0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
        0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
        0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
        0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
        0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
        0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
        0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
        0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
        0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
        0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
        0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
        0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
        0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
                        };

		/*
		 * Standard ROT permutation. Essentailly caesar with a left shift of 1.
		 * */
		void ROT_Word(char word[4]){
		        char tmp[4];
		        for(int i=0; i<4; i++){
		                tmp[this->mod((i-1), 4)] = word[i];
		        }
		        for(int i=0; i<4; i++){
		                word[i] = tmp[i];
		        }
		}

		/*
		 * Round needs to be subtracted by one when it's passed into the OG function
		 * This function gets a value that adds greater security to the key expansion process.
		 */
		char getRoundCoeff(int round){
		        int test = pow(2, round);
		        char ret;
			/*
			 * If the value is greater than GF(2⁸), reduce the value until it's 8 bits in size.
			 * */
		        if(test > 256){
				// 0x11b is an irreducable polynomial value specified by rijndael.
		                test = this->galoisMod(test, 0x11b);
		                ret = (char)test;
		                return ret;
		        }
		        ret = (char)test;
		        return ret;
		}

		/*
		 * A custom permutiation for key expansion defined by rijndael.
		 * */
		void OG(int round, char word[4]){
		        char RC;
		        ROT_Word(word);
		        for(int i=0; i<4; i++){
				unsigned int x = ((unsigned int)word[i] & 0xf0) >> 0x04;
                                unsigned int y = (unsigned int)word[i] & 0x0f;

                                word[i] = (char)this->sBox[(x*0x10) + y];
		        }
		        RC = this->getRoundCoeff(round);
		        word[0] = word[0] ^ RC;
		}

		/*
		 * The process to key expansion is similar to Cipher Block Chaining. Where you use the
		 * first chunk of data as padding for the next following chunk of data.
		 * */
		void expandKey_128(char key[KEY_SIZE], char expandedKey[KEY_SIZE * (ROUND_COUNT+1)]){
		        char word[4];
		        int round = 0;

			/*
			 * Place Key into expanded Key Buffer. Call this round one.
			 * */
			memset(expandedKey, 0x00, KEY_SIZE * (ROUND_COUNT+1));
		        for(int i=0; i<KEY_SIZE; i++){
		                expandedKey[i] = key[i];
		        }
		        /*
			 * nullify the key array to reduce the amount of
			 * sensitive bytes in memory.
			 * */
			memset(key, 0x00, KEY_SIZE);
		        round++;

			/*
			 * Now Actually Expand The Key!
			 * */
		        for(int i=0; i<ROUND_COUNT; i++){
				char _key[KEY_SIZE];

				/* Take the a KEY_SIZE chunk of data from the expanded key.
				 * The math equation states, take the first (round-1) KEY_SIZE block of bytes
				 * And populate the key buffer with them.*/
		                for(int j=0; j<KEY_SIZE; j++){
		                        _key[j] = expandedKey[((round-1)*KEY_SIZE)+j];
		                }

				/*
				 * take the last 4 bytes out of our key buffer
				 * */
		                for(int j=KEY_SIZE-4; j<KEY_SIZE; j++){
		                        word[j%4] = _key[j];
		                }

				/*
				 * Do OG Magical Operations.
				 * */
		                this->OG(round-1, word);

				/*
				 * Take the modified word variable and XOR each value with the
				 * first 4 bytes of the key buffer. Storing the resulting value in the next block
				 * of the expanded key. If the earlier loops took the first 16 bytes of the expanded key array,
				 * this loop places the first 4 bytes into of the next 16 bytes.
				 * */
		                for(int j=0; j<4; j++){
		                        expandedKey[(round*KEY_SIZE)+j] = key[j] ^ word[j];
		                }
	
				/*
				 * We now xor the remainder of the key with the expansion key. Where the iterator value
				 * of the expanded key is -4, the size of the word variable, from the current position.
				 * */
		                for(int j=4; j<16; j++){
		                        expandedKey[(round*KEY_SIZE)+j] = expandedKey[((round)*KEY_SIZE)+(j-4)] ^ key[j];
		                }
		                round++;
		        }
		
		}

		void getRoundKey(int round, char key[KEY_SIZE], char expandedKey[KEY_SIZE * (ROUND_COUNT + 1)]){
		        for(int i=0; i<KEY_SIZE; i++){
		                key[i] = expandedKey[(round*KEY_SIZE)+i];
		        }
		}
		
		void AddRoundKey_128(int round, char key[KEY_SIZE], char expandedKey[KEY_SIZE * (ROUND_COUNT + 1)], char state[STATE_SIZE]){
			/*
			 * Get the Key
			 * */
			for(int i=0; i<KEY_SIZE; i++)
		                key[i] = expandedKey[(round*KEY_SIZE)+i];
			/*
			 * Add the key
			 * */
		        for(int i=0; i<STATE_SIZE && i < KEY_SIZE; i++)
		                state[i] = state[i] ^ key[i];
		}
		void SubBytes(char state[STATE_SIZE]){
        		for(int i=0; i<STATE_SIZE; i++){
        		        unsigned int x = ((unsigned int)state[i] & 0xf0) >> 0x04;
        			unsigned int y = (unsigned int)state[i] & 0x0f;

        			state[i] = (char)this->sBox[(x*0x10) + y];
			}
		}

		void inv_SubBytes(char state[STATE_SIZE]){
 	      		for(int i=0; i<STATE_SIZE; i++){
				unsigned int x = ((unsigned int)state[i] & 0xf0) >> 0x04;
        			unsigned int y = (unsigned int)state[i] & 0x0f;

 	             		state[i] = (char)this->inv_sBox[(x*0x10) + y];
			}
		}

		void MixColumns(char state[STATE_SIZE]){
			/*
			 * This matrix is defined specifically by rijndael.
			 * */
		        const unsigned int mc_constant[STATE_SIZE] =
		        {
		                0x02, 0x03, 0x01, 0x01,
		                0x01, 0x02, 0x03, 0x01,
		                0x01, 0x01, 0x02, 0x03,
		                0x03, 0x01, 0x01, 0x02
		        };

			unsigned int _state[STATE_SIZE];

			for(int i=0; i<STATE_SIZE; i++){
				_state[i] = (state[i] & 0xf0)+(state[i] & 0x0f);
			}

		        for(int i=0; i<4; i++){
        		        for(int k=0; k<4; k++){
        		                unsigned int ret = 0x0;
        		                for(int l=0; l<4; l++){
        		                        ret = ret ^ this->galoisMultiply(mc_constant[(k*4)+l], _state[(i*4)+l]);
        		                }
        		                state[(i*4)+k] = ret;
        		        }
        		}
		}

		void inv_MixColumns(char state[STATE_SIZE]){
		        const unsigned int mc_constant[STATE_SIZE] =
		        {
		                0x0e, 0x0b, 0x0d, 0x09,
		                0x09, 0x0e, 0x0b, 0x0d,
		                0x0d, 0x09, 0x0e, 0x0b,
		                0x0b, 0x0d, 0x09, 0x0e
		        };
	
			unsigned int _state[STATE_SIZE];
			for(int i=0; i<STATE_SIZE; i++){
				_state[i] = (state[i] & 0xf0)+(state[i] & 0x0f);
			}

		        for(int i=0; i<4; i++){
		                for(int k=0; k<4; k++){
		                        unsigned int ret = 0x0;
		                        for(int l=0; l<4; l++){
		                                ret = ret ^ this->galoisMultiply(mc_constant[(k*4)+l], _state[(i*4)+l]);
		                        }
		                        state[(i*4)+k] = ret;
		                }
		        }
		}

		/*
		 * Imgine the state as a 4 by 4 matrix
		 * */

		void ShiftRows(char state[STATE_SIZE]){
		        char _state[STATE_SIZE];
		        unsigned int shiftAmount;

		        for(int i=0; i<STATE_SIZE; i++){
		                shiftAmount = mod(0-i, 4);
		                _state[this->mod(i+(shiftAmount*4), STATE_SIZE)] = state[i];
		        }

		        for(int i=0; i<STATE_SIZE; i++)
		                state[i] = _state[i];
		}

		void inv_ShiftRows(char state[STATE_SIZE]){
		        char _state[STATE_SIZE];
		        int shiftAmount;

		        for(int i=0; i<STATE_SIZE; i++){
		                shiftAmount = mod(0-i, 4);
		                _state[this->mod(i-(shiftAmount*4), STATE_SIZE)] = state[i];
		        }

		        for(int i=0; i<STATE_SIZE; i++)
		                state[i] = _state[i];
		}

		int countBinaryDigits(int value){
		        int ret = 0;
		        while(value > 0){
		                value = value / 2;
		                ret++;
		        }
		        return ret;
		}

		int buildBinaryNumber(int value, unsigned int *binaryNumber, int binarySize){
		        // Shift Base value is 0x01
		        int ret = 0;
		        memset(binaryNumber, 0x00, binarySize);
		        for(int i=0; i<binarySize; i++){ 
		                binaryNumber[i] = (value & (0x01<<i));
		                if((value & (0x01<<i)) != 0){
		                        ret++;
		                }
		        }
		        return ret;
		}

		int buildBinaryPolynomial(int value, unsigned int *polynomial, size_t size){
		        int binaryDigits = this->countBinaryDigits(value);
		        unsigned int binaryNumber[binaryDigits];
		        int polySize = this->buildBinaryNumber(value, binaryNumber, binaryDigits);

		        int j = 0;
		        for(int i=0; i<binaryDigits && j < size; i++){
		                if(binaryNumber[i] != 0){
		                        if(j < polySize)
		                                polynomial[j] = i;
		                        j++;
		                }
		        }
	
		        return polySize;
		}

		int mod(int value, int field){
			if((value + field) == 0)
		                return 0;

		        int valueIsNegative = (value < 0) ? 1 : 0;
		        int greaterThanField = ((-1)*value > (-1)*field) ? 1 : 0;
		        int fieldIsNegative = (field < 0)  ? 1 : 0;

		        if(!valueIsNegative && !fieldIsNegative){
		                return value % field;
		        }else if(!valueIsNegative && fieldIsNegative){
		                field = (-1)*field;
		                return (value % field) * (-1);
		        }else if(greaterThanField && !fieldIsNegative){
		                return field - (((-1)*value) % field);
		        }else if(!greaterThanField && !fieldIsNegative){
		                return field - (-1)*value;
		        }else if(greaterThanField && fieldIsNegative){
		                field = (-1)*field;
		                return (field - (((-1)*value) % field)) * (-1);
		        }else{
		                field = (-1)*field;
		                return (field - (-1)*value) * (-1);
		        }
		}

		int galoisMod(int value, int field){
			if(value < 0)
	        	        value = (value & 0xf0)+(value & 0x0f);
	
		        if(value <= 256 || value == 0)
		                return value;
	
		        int galoisField = field << (this->countBinaryDigits(value) - this->countBinaryDigits(field));
		        while(value > 256){
                		value = value ^ galoisField;
		                if(value <= 256)
                		        break;
		                galoisField = field << (this->countBinaryDigits(value) - this->countBinaryDigits(field));
		        }

		        if(value < 0)
		                value = (value & 0xf0)+(value & 0x0f);

		        return value;
		}

		char galoisMultiply(const unsigned int x, char y){
			unsigned int ret = 0x00;
		        const unsigned int shifter = 0x1;
		        unsigned int _y = y;
	
		        if(y < 0){
		                _y = (y & 0xf0)+(y & 0x0f);
		        }
	
		        unsigned int polynomial[100];
		        size_t polySize = this->buildBinaryPolynomial(x, polynomial, 100);

		        /* where p = 0, and s is an array of polynomial subscripts
		         * p = p[i] XOR (y LEFT_SHIFT s[i])
		         * */
		        for(int i=0; i<polySize && i<100; i++)
		                ret = ret ^ (_y << polynomial[i]);

		        return ret;
		}
};
