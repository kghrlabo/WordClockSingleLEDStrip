//    WORD CLOCK - for ESP32

#define NUM_LEDS     109 // 10x10 + 9 = 109  LEDs
#define ALLLED_BRIGHTNESS 80 // Adjust to less than 2000mA when all LEDs are on

/* 10x10 matrix word clock (Flat Layout)
*
*      0   1   2   3   4   5   6   7   8   9

*      |10|    |32|    |54|    |76|    |98|
* 00   9  11  31  33  53  55  75  77  97  99
* 10   8  12  30  34  52  56  74  78  96  100
* 20   7  13  29  35  51  57  73  79  95  101
* 30   6  14  28  36  50  58  72  80  94  102
* 40   5  15  27  37  49  59  71  81  93  103
* 50   4  16  26  38  48  60  70  82  92  104
* 60   3  17  25  39  47  61  69  83  91  105
* 70   2  18  24  40  46  62  68  84  90  106
* 80   1  19  23  41  45  63  67  85  89  107
* 90   0  20  22  42  44  64  66  86  88  108
*     |||  |21|    |43|    |65|    |87|
*     |||
*     |||
*     ESP32
*/

uint8_t LED[] = {
  9, 11, 31, 33, 53, 55, 75, 77, 97, 99,
  8, 12, 30, 34, 52, 56, 74, 78, 96, 100,
  7, 13, 29, 35, 51, 57, 73, 79, 95, 101,
  6, 14, 28, 36, 50, 58, 72, 80, 94, 102,
  5, 15, 27, 37, 49, 59, 71, 81, 93, 103,
  4, 16, 26, 38, 48, 60, 70, 82, 92, 104,
  3, 17, 25, 39, 47, 61, 69, 83, 91, 105,
  2, 18, 24, 40, 46, 62, 68, 84, 90, 106,
  1, 19, 23, 41, 45, 63, 67, 85, 89, 107,
  0, 20, 22, 42, 44, 64, 66, 86, 88, 108,
  
  10,21,32,43,54,65,76,87,98
};


/* 10x10 matrix word clock
 *
 *     0 1 2 3 4 5 6 7 8 9
 * 00  I T x I S . * * * *
 * 10  T W E N T Y W I F I
 * 20  H A L F I V E T E N
 * 30  Q U A R T E R R T O
 * 40  P A S T x S E V E N
 * 50  E I G H T W E L V E
 * 60  T W O N E F I V E x
 * 70  F O U R T E N I N E
 * 80  T H R E E L E V E N
 * 90  S I X D O C L O C K
  */

uint8_t word_ITIS[]=    {0,1,3,4};
uint8_t word_AM[]=      {6,7}; 
uint8_t word_PM[]=      {8,9}; 
uint8_t word_OCLOCK[]=  {94,95,96,97,98,99};

uint8_t word_TO[]=      {38,39};
uint8_t word_PAST[]=    {40,41,42,43};
uint8_t word_AQUARTER[]={21,30,31,32,33,34,35,36};
uint8_t word_HALF[]=    {20,21,22,23};

uint8_t word_ONE[]=     {62,63,64};
uint8_t word_TWO[]=     {60,61,62};
uint8_t word_THREE[]=   {80,81,82,83,84};
uint8_t word_FOUR[]=    {70,71,72,73};
uint8_t word_FIVE1[]=   {23,24,25,26};
uint8_t word_FIVE2[]=   {65,66,67,68};
uint8_t word_SIX[]=     {90,91,92};
uint8_t word_SEVEN[]=   {45,46,47,48,49};
uint8_t word_EIGHT[]=   {50,51,52,53,54};
uint8_t word_NINE[]=    {76,77,78,79};
uint8_t word_TEN1[]=    {27,28,29};
uint8_t word_TEN2[]=    {74,75,76};
uint8_t word_ELEVEN[]=  {84,85,86,87,88,89};
uint8_t word_TWELVE[]=  {54,55,56,57,58,59};
uint8_t word_TWENTY[]=  {10,11,12,13,14,15};

uint8_t word_MIN1[]=    {6};
uint8_t word_MIN2[]=    {7};
uint8_t word_MIN3[]=    {8};
uint8_t word_MIN4[]=    {9};
uint8_t word_SECONDS[]= {5};

uint8_t word_WIFI[]=    {16,17,18,19};
uint8_t word_ERR[]=     {35,36,37};

uint8_t word_WORD[]=    {61,71,82,93};
uint8_t word_CLOCK[]=   {95,96,97,98,99};
