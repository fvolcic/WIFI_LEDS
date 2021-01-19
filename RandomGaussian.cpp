#include "RandomGaussian.h"
#include "Arduino.h" 

//Generates a random gaussian value - there are 128 unique values in this fast gaussian function
double generateRandomGaussian(){

    //Masking bits
    static const unsigned int maskA = 0xF0F0F0F0;
    static const unsigned int maskB = 0x0F0F0F0F;

    //The initial random value
    unsigned int init_val = esp_random(); 

    //operations to obtain a gaussian random value
    unsigned int valueSetA = (init_val & maskA) >> 4; 
    unsigned int valueSetB = init_val & maskB; 
    valueSetA = valueSetA + valueSetB; 
    valueSetB = valueSetA >> 16; 
    valueSetA = valueSetA + valueSetB; 
    valueSetB = valueSetA >> 8; 
    valueSetA = valueSetA + valueSetB; 
    int final = valueSetA & 0b1111111;
    return (final - 56)/12.4;   
}

//Generates a random chi-square value
double generateRandomChiSquare(int k){
    double sum = 0; 
    for(int i = 0; i < k; ++i){
        double val = generateRandomGaussian(); 
        sum+=val*val; 
    }

    return sum / k; 
}
