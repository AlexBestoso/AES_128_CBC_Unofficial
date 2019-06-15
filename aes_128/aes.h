#include <stdlib.h>
#include <math.h>
#include "aesSbox.h"
#include "specialMath.h"
#include "keySched.h"
#include "shiftRow.h"
#include "mixCol.h"
/*
 * All arrays supplied to this function must
 * be null terminated
 */
void aes_128_encipher(char state[17], char key[17]){
        state[16] = '\0';
        key[16] = '\0';
        /*  16 = key size
         *  10 = round count for aes_128
         *  1  = accounts for the addRoundKey before
         *       the first round.
         *       second 1 is for null termination.       
         */
        char expandedKey[(16*(10+1))+1];
        int round = 0;

        expandKey_128(key, expandedKey);

        getRoundKey(round, key, expandedKey);
        AddRoundKey_128(state, key);
        round++;

        for(int i=0; i<9; i++){
                SubBytes(state);
           
                ShiftRows(state);
                MixColumns(state);

                getRoundKey(round, key, expandedKey);
                AddRoundKey_128(state, key);
                round++;
        }

        SubBytes(state);
        ShiftRows(state);

        getRoundKey(round, key, expandedKey);
        AddRoundKey_128(state, key);
}

void aes_128_decipher(char state[17], char key[17]){
        state[16] = '\0';
        key[16] = '\0';
        /*  16 = key size
         *  10 = round count for aes_128
         *  1  = accounts for the addRoundKey before
         *       the first round.
         *       second 1 is for null termination.
         */
        char expandedKey[(16*(10+1))+1];
        int round = 10;

        expandKey_128(key, expandedKey);

        getRoundKey(round, key, expandedKey);
        AddRoundKey_128(state, key);

        inv_ShiftRows(state);
        inv_SubBytes(state);

        round--;
        for(int i=0; i<9; i++){
                getRoundKey(round, key, expandedKey);
                AddRoundKey_128(state, key);
                inv_MixColumns(state);
                inv_ShiftRows(state);

                inv_SubBytes(state);
                round--;
        }

        getRoundKey(round, key, expandedKey);
        AddRoundKey_128(state, key);


}

