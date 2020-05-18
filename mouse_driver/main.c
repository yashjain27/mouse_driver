//
//  main.c
//  mouse_driver
//
//  Created by Yash Jain on 5/15/20.
//  Built using Xcode 11


#include <stdio.h>

//Global variables
uint32_t prevTime = 0;
uint16_t rollover = 0;

void handleReport(uint8_t *buffer, uint16_t length);
long packetGenerator(int index);
char* stateOfButton(uint8_t state);
uint32_t timeStamp(uint32_t currentTime);

//Main Program -- Simulation
int main(int argc, const char * argv[]) {
    //Variables
    uint64_t packet;
    uint8_t* buffer;     //Pointer to memory where mouse packet is stored
    uint16_t length = 6;   //Num. of times to iterate until the entire packet is read
    
    //Simulate packets
    for(int i = 0; i < 3; i++){
        packet = packetGenerator(i);
        buffer = (uint8_t*)&packet;
        printf("Packet %d: 0x%llX\n", i+1, packet);
        handleReport(buffer, length);
    }
    
    return 0;
}

/// Driver function for the mouse to extract incoming packets
/// @param buffer buffer that points to the the mouse packet
/// @param length number of times to iterate over the packet starting from buffer
void handleReport(uint8_t *buffer, uint16_t length){
    //Print each Byte in the packet
    for(int i = 0; i <= length; i++){
        printf("B%d. %u\n", i, *(buffer+i));
    }
    
    //Mouse coordinates (10 bits each)
    unsigned short dx = ((*(buffer + 4) << 6) & 0x300) | *(buffer + 2);
    unsigned short dy = ((*(buffer + 4) << 4) & 0x300) | *(buffer + 3);
    
    //Button state: 00 - No button, 01 - Right button, 10 - Left Button, 11 - Right + Left
    uint8_t state = ((*(buffer + 4)) & 0x3);
    char *buttonState = stateOfButton(state);
        
    //Timestamp
    uint32_t currentTime = (*(buffer+6) << 10) | (*(buffer+5) << 2) | ((*(buffer + 4) >> 6) & 0x3); //B6(17:10) + B5(9:2) + B4(1:0)
    uint32_t timestamp = timeStamp(currentTime);
        
    //Output
    printf("\ndx=%u  dy=%u\n", dx, dy);
    printf("Button State: %s\n", buttonState);
    printf("Timestamp: %u ms\n\n", timestamp);
}


/// Method that returns the timestamp of the mouse in ms over 32 bits
/// @param currentTime - the current time in ms from the packet that was extracted
/// @return the timestamp in ms over 32 bits as a uint32_t
uint32_t timeStamp(uint32_t currentTime){
    uint32_t returnTime;
    
    //Check if the currentTime is greater than or equal to previousTime. If false, increment rollover
    if(currentTime >= prevTime){
        returnTime = (rollover << 18) | currentTime;
    }else{
        rollover++;
        returnTime = (rollover << 18) | currentTime;
    }
    
    //Update the prevTime to currentTime for future comparisons
    prevTime = currentTime;
    
    return returnTime;
}


/// Return button state of the mouse
/// @param state - the state of the button represented by 2 bits
/// @return a char pointer which points to a string literal representing the button states as a char*
char* stateOfButton(uint8_t state){
    char* returnState;
    
    //Values: 00 - No button, 01 - Right button, 10 - Left Button, 11 - Right + Left
    switch (state) {
        case 0:
            returnState = "No button";
            break;
       case 1:
            returnState = "Right button";
            break;
       case 2:
            returnState = "Left Button";
            break;
       case 3:
            returnState = "Right + Left";
            break;
        default:
            returnState = "Invalid button state was passed. Make sure the unsigned short state is only 2 bits.";
            break;
    }
    return returnState;
}

/// Random packet generator
/// @return a 56 bit packet as a long
long packetGenerator(int index){
    //0x842F5B9AF40000 Expected dx= 756, dy= 410; Button State: Left + Right, Timestamp: 135357
    //0xE5085ACF9C0000 Expected dx= 668, dy= 463; Button State: Left, Timestamp: 234529
    //0x3FFFFC18290000 Expected dx= 809, dy= 792; Button State: None, Timestamp: 327679   CurrentTime: 65535
    long packet[3] = {0x842F5B9AF40000, 0xE5085ACF9C0000, 0x3FFFFC18290000};
    return packet[index];
}
