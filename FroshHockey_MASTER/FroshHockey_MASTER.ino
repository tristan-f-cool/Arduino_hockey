//IEEE Concordia
//Summer 2016
//Frosh Hockey

/***********************************************************************
 * H: Home (Blue)
 * A: Away (Red)
 * Variable Naming: feature_attribute_team
 * Function Naming: FeatureAttributeTeam
 * //END Function Name
 * Comment: //Description - Initials
 * TODO: //TODO - task - Initials
 * Coding Practices: 4 space indent, new line for {}, Serial print debug, encapsulate into functions, keep loop() clean, use flags
 **********************************************************************/

//********************** LIBRARIES **********************************//
// ex) #include <LiquidCrystal.h>
#include <LiquidCrystal.h>
#include <DS3231.h>
#include <Adafruit_NeoPixel.h>

//********************* DEFINE *************************************//
#define PIN 6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRB + NEO_KHZ800);
DS3231  rtc(SDA, SCL);

//********************** PINS **************************************//
// ex) const int goal_LED_H = 13; //Home team goal LED - TC

// Pushbuttons
const int up_button_H   = 31;  //Increase Home team score button  - TC
const int down_button_H = 33;  //Decrease Home team score button  - TC 
const int up_button_A   = 35;  //Increase Away team score button  - TC 
const int down_button_A = 37;  //Decrease Away team score button  - TC  

// IR
const int IR_goal_H     = 6;  //Home team IR goal sensor         - TC
const int IR_goal_A     = 7;  //Away team IR goal sensor         - TC

// 7 segment + chip
const int bit2_H        = 8;  //Home 7 segment score CD4511 chip - TC
const int bit3_H        = 9;  //Home 7 segment score CD4511 chip - TC
const int bit4_H       = 10;  //Home 7 segment score CD4511 chip - TC
const int bit1_H       = 11;  //Home 7 segment score CD4511 chip - TC

const int bit2_A        = 12; //Away 7 segment score CD4511 chip - TC
const int bit3_A        = 13; //Away 7 segment score CD4511 chip - TC
const int bit4_A        = 14; //Away 7 segment score CD4511 chip - TC
const int bit1_A        = 15; //Away 7 segment score CD4511 chip - TC

// Speaker
const int speaker       = 16; //Speaker                          - TC

// LCD
LiquidCrystal lcd(22, 24, 26, 2, 3, 4); //LCD                    - ML

// Piezo
const int piezo = A0; // Faceoff piezo                           - ML         

//********************* VARIABLES **********************************//
// ex) int score_H = 0; //Home team score - TC

// Goal logic + IR
int score_H = 0;              //Home team score                        - TC 
int score_A = 0;              //Away team score                        - TC

bool goal_flag_H = false;      //Home team goal flag                   - TC
bool goal_flag_A = false;      //Away team goal flag                   - TC
bool goal_H;                   //Home team goal                        - TC
bool goal_A;                   //Away team goal                        - TC

// Game logic
int  periodNumber;            //Period                                 - TC
bool overtime_flag;           //Overtime                               - TC
bool game_start_flag;         //Game start                             - TC
bool game_end_flag;           //Game end                               - TC

// Pushbuttons
int button_debounce = 250;     //Timing between button pushes          - TC
int up_button_val_H;           //Increase Home team score button value - TC
int up_button_state_H = LOW;   //Increase Home team score button state - TC
int down_button_val_H;         //Decrease Home team score button value - TC
int down_button_state_H = LOW; //Decrease Home team score button state - TC
int up_button_val_A;           //Increase Away team score button value - TC
int up_button_state_A = LOW;   //Increase Away team score button state - TC
int down_button_val_A;         //Decrease Away team score button value - TC
int down_button_state_A = LOW; //Decrease Away team score button state - TC

// RTC
String time;                  //RTC Input time string                  - RC
int c_seconds;                //Current Seconds                        - RC
int c_minutes;                //Current Minutes                        - RC
int s_seconds;                //Start Seconds                          - RC
int s_minutes;                //Start Minutes                          - RC
int d_seconds;                //Delta Seconds                          - RC
int d_minutes;                //Delta Minutes                          - RC      
int t_seconds;                //timer Seconds                          - RC
int t_minutes;                //timer Minutes                          - RC
int period;                   //Periode time in seconds                - RC
int tempb;                    //dummy                                  - RC
char tempa;                   //dummy                                  - RC 

// LCD
unsigned long previous_millis; //                                      - ML
unsigned long current_millis;  //                                      - ML

// LED RGB
uint8_t LED_brightness;       //LED RGB brightness control             - RC
uint8_t wait;                 //LED RGB delay                          - RC


// Piezo
int threshold = 400; //                                                - ML
int debounce = 120; //                                                 - ML
int piezo_faceoff; //                                                  - ML

//******************** FUNCTION PROTOTYPES ************************//
// ex) void GoalSiren(bool goal); //Siren sounds for goal scored - TC

void HomeGoal();                                                              //TC
void AwayGoal();                                                              //TC
int ScoreButtonIncrease(int button_val, int button_state, int update_score);  //TC
int ScoreButtonDecrease(int button_val, int button_state, int update_score);  //TC
void Display7SegmentScore(int score, int bit1, int bit2, int bit3, int bit4); //TC
void GoalSiren();                                                             //TC
void PeriodSiren();                                                           //TC
void HomeGoalLEDs(uint8_t wait);                                              //RC
void AwayGoalLEDs(uint8_t wait);                                              //RC
void PeriodLEDs(uint8_t wait);                                                //RC
void StartGameLEDs(uint8_t wait);                                             //RC
void EndGameLEDs(uint8_t wait);                                               //RC
void IdleLEDs(uint8_t wait);                                                  //RC
void decode(void);                                                            //RC
void clock_setup(void);                                                       //RC
void colorWipe(uint32_t c, uint8_t wait);                                     //RC
void LCDPrint();                                                              //ML
void millisDelay(int ms);                                                     //ML
void PrintScore();                                                            //TC
void PrintGoal();                                                             //TC
void PrintClock();                                                            //RC
void PrintPeriod();                                                           //RC
void LCDPrintGoal(int team);                                                  //TC

//*------------------------------------------------------------------------------------------*//
//*------------------------------------------------------------------------------------------*//

void setup() {

  //Serial Monitor - Debug (Ctrl+Shift+m)
  Serial.begin(115200); //9600
  Serial.println("IEEE Concordia - Frosh Hockey 2016-2017");
  Serial.println("H    A");

  //IR Goal sensors
  pinMode(IR_goal_H, INPUT);
  pinMode(IR_goal_A, INPUT);

  //Push buttons to change Home & Away score - TC
  pinMode(up_button_H,   INPUT);
  pinMode(down_button_H, INPUT);
  pinMode(up_button_A,   INPUT);
  pinMode(down_button_A, INPUT);

  //7 segment display Home & Away score - TC
  pinMode(bit1_H, OUTPUT);
  pinMode(bit2_H, OUTPUT);
  pinMode(bit3_H, OUTPUT);
  pinMode(bit4_H, OUTPUT);
  pinMode(bit1_A, OUTPUT);
  pinMode(bit2_A, OUTPUT);
  pinMode(bit3_A, OUTPUT);
  pinMode(bit4_A, OUTPUT);

  // LED RGB
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  // RTC
  clock_setup();
  period=300;

  // LCD
  lcd.begin(20,4);

  // Piezo faceoff
  pinMode(piezo, INPUT);
  
}
// END setup

void loop() {

  //Debug
  PrintScore();
  PrintPeriod();

  wait = 50;

  //Piezo faceoff
  piezo_faceoff = analogRead(piezo);
  
  
  //Game timing
  //TODO - period timing
  
  if(score_H >= 9 || score_A >= 9)
  {
    score_H = score_A = 0;
    game_end_flag = true;
    Serial.println("GAME OVER");
  }
  
  //TODO - stop game when home/away reach 9 goals
  //TODO - stop game when time expires
  //TODO - overtime scenario
  
  //LED RGB
  //colorWipe(strip.Color(0, 0, 0), 0); 

  //IR Goal sensors
  goal_H = digitalRead(IR_goal_H);
  goal_A = digitalRead(IR_goal_A);
  
  //Push buttons to change Home & Away score
  up_button_val_H   = digitalRead(up_button_H);
  down_button_val_H = digitalRead(down_button_H);
  up_button_val_A   = digitalRead(up_button_A);
  down_button_val_A = digitalRead(down_button_A);
  
  if(goal_H)
  {
      Serial.println("IR-Home Goal");
      goal_flag_H = true;
      HomeGoal();
  }

  if(goal_A)
  {
      Serial.println("IR-Away Goal");
      goal_flag_A = true;
      AwayGoal(); 
  }
  
  //Button update scores
  score_H = ScoreButtonIncrease(up_button_val_H, up_button_state_H, score_H);
  score_H = ScoreButtonDecrease(down_button_val_H, down_button_state_H, score_H);
  score_A = ScoreButtonIncrease(up_button_val_A, up_button_state_A, score_A);
  score_A = ScoreButtonDecrease(down_button_val_A, down_button_state_A, score_A);

  //7 segment scores
  Display7SegmentScore(score_H, bit1_H, bit2_H, bit3_H, bit4_H);
  Display7SegmentScore(score_A, bit1_A, bit2_A, bit3_A, bit4_A);

  //LCD scores and period time
  LCDPrint();
}
//END loop

//*------------------------------------------------------------------------------------------*//
//*------------------------------------------------------------------------------------------*//

//******************* FUNCTIONS  *********************************//

//****** IR - Goals ********************************//

//Actions when Home goal is scored - TC
void HomeGoal()
{
    score_H++;
    PrintGoal();
    Display7SegmentScore(score_H, bit1_H, bit2_H, bit3_H, bit4_H);
    LCDPrint();
    HomeGoalLEDs(wait);
    GoalSiren();
    goal_flag_H = false;
}//END HomeGoal

//Actions when Away goal is scored - TC
void AwayGoal()
{
    score_A++;
    PrintGoal();
    Display7SegmentScore(score_A, bit1_A, bit2_A, bit3_A, bit4_A);
    LCDPrint();
    AwayGoalLEDs(wait);
    GoalSiren();
    goal_flag_A = false; 
}//END AwayGoal


//****** 7 Segment Display + Buttons - Goals ******************//

//Checks if Home or Away up buttons have been pushed, returns new score - TC
int ScoreButtonIncrease(int button_val, int button_state, int update_score)
{
    //Check and set button state
    if(button_val == LOW && button_state == HIGH)
    {
       button_state = LOW;
    }
    //Increment score if button is pushed
    if(button_state == LOW)
    {
        if(button_val == HIGH)
        {  
            delay(button_debounce);
            update_score++;
            Serial.println("Button - Add Goal");
            if(update_score > 9)
            {
                Serial.println("GAME OVER");
                update_score = 0;
                game_end_flag = true;
            }
            button_state = HIGH;
        }
     }
  return update_score;
}//END ScoreButtonIncrease

//Checks if Home or Away down buttons have been pushed, returns new score - TC
int ScoreButtonDecrease(int button_val, int button_state, int update_score)
{
    //Check and set button state
    if(button_val == LOW && button_state == HIGH)
    {
       button_state = LOW;
    }
    //Decrement score if button is pushed
    if(button_state == LOW)
    {
        if(button_val == HIGH)
        {  
            delay(button_debounce);
            update_score--;
            Serial.println("Button - Remove Goal");
            if(update_score < 1)
            {
                update_score = 0;
            }
            button_state = HIGH;
        }
     }
  return update_score;
}//END ScoreButtonDecrease

//7 segment display of Home & Away scores
void Display7SegmentScore(int score, int bit1, int bit2, int bit3, int bit4)
{
  switch(score)
  {
    case 0: //0000 = 0
        digitalWrite(bit1, B0);
        digitalWrite(bit2, B0);
        digitalWrite(bit3, B0);
        digitalWrite(bit4, B0);
      break;
    case 1: //0001 = 1
        digitalWrite(bit1, B1);
        digitalWrite(bit2, B0);
        digitalWrite(bit3, B0);
        digitalWrite(bit4, B0);
      break;
    case 2: //0010 = 2
        digitalWrite(bit1, B0);
        digitalWrite(bit2, B1);
        digitalWrite(bit3, B0);
        digitalWrite(bit4, B0);
      break;
    case 3: //0011 = 3
        digitalWrite(bit1, B1);
        digitalWrite(bit2, B1);
        digitalWrite(bit3, B0);
        digitalWrite(bit4, B0);
      break;
    case 4: //0100 = 4
        digitalWrite(bit1, B0);
        digitalWrite(bit2, B0);
        digitalWrite(bit3, B1);
        digitalWrite(bit4, B0);
      break;
    case 5: //0101 = 5
        digitalWrite(bit1, B1);
        digitalWrite(bit2, B0);
        digitalWrite(bit3, B1);
        digitalWrite(bit4, B0);
      break;
    case 6: //0110 = 6
        digitalWrite(bit1, B0);
        digitalWrite(bit2, B1);
        digitalWrite(bit3, B1);
        digitalWrite(bit4, B0);
      break;
    case 7: //0111 = 7
        digitalWrite(bit1, B1);
        digitalWrite(bit2, B1);
        digitalWrite(bit3, B1);
        digitalWrite(bit4, B0);
      break;
    case 8: //1000 = 8
        digitalWrite(bit1, B0);
        digitalWrite(bit2, B0);
        digitalWrite(bit3, B0);
        digitalWrite(bit4, B1);
      break;
    case 9: //1001 = 9
        digitalWrite(bit1, B1);
        digitalWrite(bit2, B0);
        digitalWrite(bit3, B0);
        digitalWrite(bit4, B1);
      break;  
    default:
        Serial.println ("**ERROR** - 7 segment display score");
      break;
  }
}//END Display7SegmentScore


//****** Speaker  - Goals/Period *************//

//Siren when goal is scored - TC
void GoalSiren()
{
  for(int j = 0; j < 4; j++)
  {
    for(int i = 300; i < 1100; i++)
    {
      tone(speaker,i,50); 
    }
    for(int i = 1100; i > 300; i--)
    {
      tone(speaker,i,50);
    }
  }
  noTone(0);
}//END GoalSiren()

//Siren when period is ended - TC
void PeriodSiren()
{
    //ex) tone(speaker, speaker_tone,tone_duration) - TC 
    tone(speaker, 500, 2000);  
}//END PeriodSiren

//********** LEDs - Goals/Period ***********************//

// Set LED colour  - RC
void colorWipe(uint32_t c, uint8_t wait) 
{
  for(uint16_t i=0; i<strip.numPixels(); i++) 
  {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}//END colorWipe

// ***** LED RGB Patterns ****//
void pattern1(uint32_t c) //*****FIRST 10 LEDS
{
  int local=30;// number of led in the pattern
  for(uint16_t i=0; i<local; i++) 
  {
      strip.setPixelColor(i, c);
      strip.show();
      //delay(20);
  }
}
void pattern2(uint32_t c) //*****SECOND 10 LEDS
{
  int local=30;// number of led in the pattern
  for(uint16_t i=10; i<local; i++) 
  {
      strip.setPixelColor(i, c);
      strip.show();
      //delay(20);
  }
}
void pattern3(uint32_t c1,uint32_t c2,uint8_t wait) //*****30 LEDS 
{ 
  int local=30;// number of led in the pattern
  for(uint16_t i=0; i<local; i++) 
  {
      strip.setPixelColor(i, c1);
      strip.show();
      delay(wait);
      strip.setPixelColor(local-i, c2);
      strip.show();
      delay(wait);
  }
}
void pattern4(uint32_t c1,uint32_t c2,uint8_t wait) //*****30 LEDS 
{ 
  int local=20;// number of led in the pattern
  for(uint16_t i=0; i<local/2; i++) 
  {
      strip.setPixelColor(i, c1);
      strip.show();
      delay(wait);
  }
  for(uint16_t i=local/2; i<local; i++) 
  {
      strip.setPixelColor(i, c2);
      strip.show();
      delay(wait);
  }
   for(uint16_t i=0; i<local/2; i++) 
  {
      strip.setPixelColor(i, c2);
      strip.show();
      delay(wait);
  }
  for(uint16_t i=local/2; i<local; i++) 
  {
      strip.setPixelColor(i, c1);
      strip.show();
      delay(wait);
  }
}

//LED RGB strips for Home goal
void HomeGoalLEDs(uint8_t wait)
{
  pattern1(strip.Color(50,0,0));
  delay(wait);
  pattern1(strip.Color(0,0,50));
  delay(wait);
  pattern1(strip.Color(50,50,50));
  delay(wait);
}//END HomeGoalLEDs

//LED RGB strips for Away goal
void AwayGoalLEDs(uint8_t wait)
{
  pattern2(strip.Color(50,50,0));
  delay(wait);
  pattern2(strip.Color(50,50,50));
  delay(wait);
}//END AwayGoalLEDs

//LED RGB strip lights at end of periods
void PeriodLEDs(uint8_t wait)
{
  //TODO - TC
}//END PeriodLEDs

//LED RGB strip lights at start of game
void StartGameLEDs(uint8_t wait)
{
  //TODO - TC
}//END StartGameLEDs

//LED RGB strip lights at end of game
void EndGameLEDs(uint8_t wait)
{
  //TODO - TC
}//END EndGameLEDs

//LED RGB strip lights at idle
void IdleLEDs(uint8_t wait)
{
  int i,j;
  for(i=0;i<10;i++)
  {
    switch (i) {
      case 0:   
      pattern4(strip.Color(0,0,0),strip.Color(0,0,0),wait);
        break;
      case 1: 
      pattern4(strip.Color(50,0,0),strip.Color(0,0,50),wait);   
        break;
      case 2:  
      pattern4(strip.Color(0,50,0),strip.Color(0,50,0),wait);
        break;
      case 3: 
      pattern4(strip.Color(0,0,50),strip.Color(50,0,0),wait); 
        break;
        case 4: 
      pattern4(strip.Color(0,50,0),strip.Color(0,50,0),wait); 
        break;
        case 5: 
      pattern4(strip.Color(50,50,0),strip.Color(0,0,50),wait); 
        break;
        case 6: 
      pattern4(strip.Color(0,50,50),strip.Color(0,50,50),wait); 
        break;
        case 7: 
      pattern4(strip.Color(50,50,0),strip.Color(50,50,0),wait); 
        break;
        case 8: 
      pattern4(strip.Color(0,0,0),strip.Color(0,0,0),wait); 
        break;
     }    
  }
}//END IdleLEDs

//****** LCD Jumbo-tron - Period + Score + Goal *****//

void LCDPrint()
{
    lcd.clear();

    //Goal scored
    if(goal_flag_H)
      LCDPrintGoal(1);

    if(goal_flag_A)
      LCDPrintGoal(0);

    //Main screen
    lcd.setCursor(0,0);
    lcd.print("HOME");
    lcd.setCursor(1,1);
    lcd.print(score_H);

    lcd.setCursor(16,0);
    lcd.print("AWAY");
    lcd.setCursor(17,1);
    lcd.print(score_A);
   
    millisDelay(500);
    
}//END LCDPrint

void LCDPrintGoal(int team)
{
  if(team == 1)
  {
        lcd.setCursor(5, 1);
        lcd.print("Blue Goal!");
        millisDelay(500);
        lcd.clear();
        lcd.print("Blue Goal!");  

        if(score_A >= 9)
        {
          lcd.setCursor(5,1);
          lcd.print("BLUE TEAMS WINS!");
        }
  }
  else if(team == 0)
  {
        lcd.setCursor(5, 1);
        lcd.print("Red Goal!");

        if(score_A >= 9)
        {
          lcd.setCursor(5,1);
          lcd.print("RED TEAMS WINS!");
        }
  }
}

void millisDelay(int ms)
{
    previous_millis = millis();
    current_millis = millis();
    Serial.println(current_millis);
    while (current_millis - previous_millis < ms)
    {
        current_millis = millis();
    }
}//END millisDelay

//****** RTC - Periods ******************************//

//Configure RTC for period timing - RC
void clock_setup()
{
   // Initialize the rtc object
   rtc.begin();
   // rtc.setTime(17,2,0); //Use this to set actual time 
   d_seconds=0;
   d_minutes=0;
  //time=rtc.getTimeStr(2);
  tempa=time.charAt(3);
  s_minutes=((int(tempa)-48)*10);
  tempa=time.charAt(4);
  s_minutes+=((int(tempa)-48)*1);
  
  tempa=time.charAt(6);
  s_seconds=((int(tempa)-48)*10);
  tempa=time.charAt(7);
  s_seconds+=((int(tempa)-48)*1);
  
  //For Debuging
  //Serial.print("Decoded Start Time: ");
  //Serial.print(s_minutes);
  //Serial.print(":");
  //Serial.println(s_seconds); 
  
}//END clock_setup

//Updates current period time - RC
void decode()
{
 //time=rtc.getTimeStr(2);// Update the current time 
  // Get Current Time
  tempa=time.charAt(3);
  c_minutes=((int(tempa)-48)*10);
  tempa=time.charAt(4);
  c_minutes+=((int(tempa)-48)*1);
  
  tempa=time.charAt(6);
  c_seconds=((int(tempa)-48)*10);
  tempa=time.charAt(7);
  c_seconds+=((int(tempa)-48)*1);
     
  // Get Time Pasted
  d_minutes=s_minutes- c_minutes;
  d_seconds=s_seconds- c_seconds;
  d_seconds+=(d_minutes*60);// This line should only be used to add 60 seconds per minute of difference
  
  tempb=(period+d_seconds);
  
  t_seconds=tempb %60;
  t_minutes=tempb/60;
  /*
  //Debuging Display currrent Time
  Serial.print("Decoded Time in INT: ");
  Serial.print(c_minutes);
  Serial.print(":");
  Serial.println(c_seconds);
  //Debuging Display currrent Time
  Serial.print("Calculated Seconds pasted: ");
  Serial.println(d_seconds);
  Serial.println(tempb);
  */
    
}//END decode


//****** Debug **************************************//

// Debug - Print scores - TC
void PrintScore()
{
  Serial.print(score_H);
  Serial.print("    ");
  Serial.print(score_A); 
  Serial.println();  
} //END PrintScore

// Debug - Print goal messages - TC
void PrintGoal()
{
    //LCDPrint();
    if(goal_flag_H)
    {
      Serial.println("HOME GOAL!");
    }
    if(goal_flag_A)
    {
      Serial.println("AWAY GOAL!");
    }
} //END PrintScore

// Debug - Print period timing
void PrintPeriod()
{
    Serial.print("Period: ");
    Serial.println(periodNumber);
    Serial.print("Time remaining: ");
    Serial.print(t_minutes);    
    Serial.print(":");
    Serial.println(t_seconds);
}//END PrintPeriod

//********************************************************************************************//
