//
// Created by Chan Yen Yee on 07/06/2022.
//

#include <iostream>
#include <bit>
#include <bitset>
#include "key.h"


int substitution(uint64_t x) {
    int sBox[16] = {12, 5, 6, 11, 9, 0, 10, 13, 3, 14, 15, 8, 4, 7, 1, 2};

    uint64_t total = 0;

    for (int i = 0; i < 4; i++) {
        int last_digit = x & 0x000F;
        x = (x >> 4);
        uint64_t sub_x = sBox[ last_digit ];

        // sub_x << 4*(i) meaning add the proper digits for the sub_x before adding into total
        total += (sub_x << 4 * (i));
    }

    return total;
}

int permutation(uint64_t x) {
    int pBox[16] = {7, 13, 1, 8, 11, 14, 2, 5, 4, 10, 15, 0, 3, 6, 9, 12};

    uint64_t total = 0;

    for (int i = 0; i < 16; i++) { // 16 bit right halves
        uint64_t remainder = x % 2;
        int shift_bit = pBox[ i ]; // when you are at last bit, meaning your only choice is moving forward--> shift left
        total += remainder << shift_bit; // therefore the shift bit is just how many bits you should shift left
        x = x >> 1; // shift the next bits
    }
    return total;
}

int main() {

int round=10;
    uint32_t plaintext = 0x4356cded;
    uint32_t ciphertext = 0;
    uint8_t master_key[20]={
            0b0011, 0b1001, 0b1011, 0b0100,
            0b1110, 0b1100, 0b1011,0b0110,
            0b0001, 0b1001, 0b0110, 0b1001,
            0b1010, 0b1000 ,0b1101 ,0b1001,
            0b0011, 0b1101 ,0b0010, 0b1110,
    };

    uint16_t l16 = ((plaintext >> 16) & 0xffff);
    uint16_t r16 = plaintext & 0xffff; // to get the original number--> x AND F= x cuz F==1111 in bin
    uint16_t round_key[round];
    uint16_t *temp;

    temp= key_scheduling(master_key, round);

    for (int i=0; i<round; i++){
        round_key[i]=0;
        round_key[i]= *(temp + i);
    }

    /*
     * keyed_r16= output of right halves input xor with round key
     * sub_r16= output of right halves input after sbox
     * per_r16= output of right halves input after pbox
     * round_r16= variable to preserve original value of r16
     * */

    for (int r = 0; r < round; r++) {
        uint16_t keyed_r16 = r16 xor round_key[r]; // r16_x1
        uint16_t sub_r16 = substitution(keyed_r16);
        uint16_t per_r16 = permutation(sub_r16);

        // switch position
        uint16_t round_r16 = r16;
        r16 = per_r16 xor l16;
        l16 = round_r16;



    }

    ciphertext= (l16<<16)+ r16;

    std::cout<< "output: "<< std::hex<<ciphertext<< "\n";


}