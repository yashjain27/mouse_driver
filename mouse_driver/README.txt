You are writing a driver for a new mouse. All incoming data is processed in a function named handleReport();

void handleReport(UInt8 *buffer, UInt16 length);

A mouse packet has the following structure:

B0: report format
B1: reserved
B2: dX (bits[7:0])
B3: dY (bits[7:0])
B4[1:0]: Left/Right button states
B4[3:2]: dX (bits[9:8])
B4[5:4]: dY (bits[9:8])
B4[7:6]: timestamp (ms) bits[1:0]
B5: timestamp (ms) bits[9:2]
B6: timestamp (ms) bits[17:10]

1) Write one or more functions that extract the mouse deltas, button state, and timestamp from the device.
2) The 18-bit packet timestamp reflects a device timestamp in milliseconds. It will roll over after a few minutes. The host can accommodate a 32-bit timestamp that is updated with values from the incoming packet. Write code that detects the timestamp rollover in the incoming packets, and extends it to 32 bits.


*******Further notes and explanation********

1) Timestamp testing
The following simulation was used in the main method to check if whether the rollover after 2^18 - 1 was rolled over correctly and was successfully extended to 32 bits:

   uint32_t curr  = 37449;
   for(int i = 1; i <= 35; i++){
       printf("CurrentTime: %u\n", curr*i % 262143);
       uint32_t t = timeStamp(curr*i % 262143);
       printf("Timestamp: %u\n\n", t);
   }

The following are the results to verify the "uint32_t timeStamp(uint32_t currentTime);" method was correctly coded.
Note*: A rollover print statement was added in the timeStamp method to indicate when there is an 18 bit rollover.

CurrentTime: 37449
Timestamp: 37449

CurrentTime: 74898
Timestamp: 74898

CurrentTime: 112347
Timestamp: 112347

CurrentTime: 149796
Timestamp: 149796

CurrentTime: 187245
Timestamp: 187245

CurrentTime: 224694
Timestamp: 224694

CurrentTime: 0
Rollover 1
Timestamp: 262144

CurrentTime: 37449
Timestamp: 299593

CurrentTime: 74898
Timestamp: 337042

CurrentTime: 112347
Timestamp: 374491

CurrentTime: 149796
Timestamp: 411940

CurrentTime: 187245
Timestamp: 449389

CurrentTime: 224694
Timestamp: 486838

CurrentTime: 0
Rollover 2
Timestamp: 524288

CurrentTime: 37449
Timestamp: 561737

CurrentTime: 74898
Timestamp: 599186

CurrentTime: 112347
Timestamp: 636635

CurrentTime: 149796
Timestamp: 674084

CurrentTime: 187245
Timestamp: 711533

CurrentTime: 224694
Timestamp: 748982

CurrentTime: 0
Rollover 3
Timestamp: 786432

CurrentTime: 37449
Timestamp: 823881

CurrentTime: 74898
Timestamp: 861330

CurrentTime: 112347
Timestamp: 898779

CurrentTime: 149796
Timestamp: 936228

CurrentTime: 187245
Timestamp: 973677

CurrentTime: 224694
Timestamp: 1011126

CurrentTime: 0
Rollover 4
Timestamp: 1048576

CurrentTime: 37449
Timestamp: 1086025

CurrentTime: 74898
Timestamp: 1123474

CurrentTime: 112347
Timestamp: 1160923

CurrentTime: 149796
Timestamp: 1198372

CurrentTime: 187245
Timestamp: 1235821

CurrentTime: 224694
Timestamp: 1273270

CurrentTime: 0
Rollover 5
Timestamp: 1310720


2. The following packets were used to test with the expected results:
Packet 1: 0x842F5B9AF40000 Expected dx= 756, dy= 410; Button State: Left + Right, Timestamp: 135357
Packet 2: 0xE5085ACF9C0000 Expected dx= 668, dy= 463; Button State: Left, Timestamp: 234529
Packet 3: 0x3FFFFC18290000 Expected dx= 809, dy= 792; Button State: No button, Timestamp: 327679   CurrentTime: 65535

The for-loop inside the main method marks the simulation for various different packets.

The following results were correctly displayed:

Packet 1: 0x842F5B9AF40000
B0. 0
B1. 0
B2. 244
B3. 154
B4. 91
B5. 47
B6. 132

dx=756  dy=410
Button State: Right + Left
Timestamp: 135357 ms

Packet 2: 0xE5085ACF9C0000
B0. 0
B1. 0
B2. 156
B3. 207
B4. 90
B5. 8
B6. 229

dx=668  dy=463
Button State: Left Button
Timestamp: 234529 ms

Packet 3: 0x3FFFFC18290000
B0. 0
B1. 0
B2. 41
B3. 24
B4. 252
B5. 255
B6. 63

dx=809  dy=792
Button State: No button
Timestamp: 327679 ms
