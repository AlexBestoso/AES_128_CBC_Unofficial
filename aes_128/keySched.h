void ROT_Word(char word[4]){
        char tmp[4];
        for(int i=0; i<4; i++){
                tmp[((i+8)-1)%4] = word[i];
        }
        for(int i=0; i<4; i++){
                word[i] = tmp[i];
        }
}

/*
 * Round needs to be subtracted by one when it's passed into the OG function
 */
char getRoundCoeff(int round){
        int test = pow(2, round);
        char ret;
        if(test > 256){/* then it Needs modular reduction */
                test = rijnMod(test);
                ret = (char)test;
                return ret;
        }
        ret = (char)test;
        return ret;
}

void OG(int round, char word[4]){
        char RC;
        ROT_Word(word);
        for(int i=0; i<4; i++){
                word[i] = sBox(word[i]);
        }
        RC = getRoundCoeff(round);
        word[0] = word[0] ^ RC;
}

void expandKey_128(char key[17], char expandedKey[(16*(10+1))+1]){
        char word[4];
        int round = 0;

        /*
         * Null terminate the array with these
         * two lines.
         */
        int expandedKeyNullLoc = (16*(10+1));
        expandedKey[expandedKeyNullLoc] = '\0';

        for(int i=0; i<16; i++){
                expandedKey[i] = key[i];
        }
        round++;

        for(int i=0; i<10; i++){
                for(int j=0; j<16; j++){
                        key[j] = expandedKey[((round-1)*16)+j];
                }
                for(int j=12; j<16; j++){
                        word[j%4] = key[j];
                }

                OG(round-1, word);

                for(int j=0; j<4; j++){
                        expandedKey[(round*16)+j] = key[j] ^ word[j];
                }
                for(int j=4; j<16; j++){
                        expandedKey[(round*16)+j] = expandedKey[((round)*16)+(j-4)] ^ key[j];
                }

                round++;
        }

        /* nullify the key array */
        for(int i=0; i<16; i++){
                key[i] = '\0';
        }
}

void getRoundKey(int round, char key[17], char expandedKey[(16*(10+1))+1]){
        for(int i=0; i<16; i++){
                key[i] = expandedKey[(round*16)+i];
        }
}

void AddRoundKey_128(char state[17], char key[17]){
        for(int i=0; i<16; i++){
                state[i] = state[i] ^ key[i];
        }
}
