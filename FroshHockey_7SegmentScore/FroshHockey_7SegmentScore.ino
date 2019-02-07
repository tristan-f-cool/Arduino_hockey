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


//********************** PINS **************************************//
// ex) const int goal_LED_H = 13; //Home team goal LED - TFC

const int up_button_H   = 2;  //Increase Home team score button - TFC
const int down_button_H = 3;  //Decrease Home team score button - TFC 
const int up_button_A   = 4;  //Increase Away team score button - TFC 
const int down_button_A = 5;  //Decrease Away team score button - TFC  

const int bit2_H = 8;  //Home 7 segment score CD4511 chip - TFC
const int bit3_H = 9;  //Home 7 segment score CD4511 chip - TFC
const int bit4_H = 10; //Home 7 segment score CD4511 chip - TFC
const int bit1_H = 11; //Home 7 segment score CD4511 chip - TFC

const int bit2_A = 12; //Away 7 segment score CD4511 chip - TFC
const int bit3_A = 13; //Away 7 segment score CD4511 chip - TFC
const int bit4_A = 14; //Away 7 segment score CD4511 chip - TFC
const int bit1_A = 15; //Away 7 segment score CD4511 chip - TFC


//********************* VARIABLES **********************************//
// ex) int score_H = 0; //Home team score - TFC

int score_H = 0; //Home team score - TFC 
int score_A = 0; //Away team score - TFC

int button_debounce = 250;     //Timing between button pushes - TFC
int up_button_val_H;           //Increase Home team score button value - TFC
int up_button_state_H = LOW;   //Increase Home team score button state - TFC
int down_button_val_H;         //Decrease Home team score button value - TFC
int down_button_state_H = LOW; //Decrease Home team score button state - TFC
int up_button_val_A;           //Increase Away team score button value - TFC
int up_button_state_A = LOW;   //Increase Away team score button state - TFC
int down_button_val_A;         //Decrease Away team score button value - TFC
int down_button_state_A = LOW; //Decrease Away team score button state - TFC

//******************** FUNCTION PROTOTYPES ************************//
// ex) void GoalSiren(bool goal); //Siren sounds for goal scored - TFC

void PrintScore(); //TFC
int ScoreButtonIncrease(int button_val, int button_state, int update_score);  //TFC
int ScoreButtonDecrease(int button_val, int button_state, int update_score);  //TFC
void Display7SegmentScore(int score, int bit1, int bit2, int bit3, int bit4); //TFC


//*------------------------------------------------------------------------------------------*//
//*------------------------------------------------------------------------------------------*//

void setup() {

  //Serial Monitor - Debug (Ctrl+Shift+m)
  Serial.begin(9600);
  Serial.println("IEEE Concordia - Frosh Hockey 2016-2017");
  Serial.println("H    A");

  //Push buttons to change Home & Away score - TFC
  pinMode(up_button_H,   INPUT);
  pinMode(down_button_H, INPUT);
  pinMode(up_button_A,   INPUT);
  pinMode(down_button_A, INPUT);

  //7 segment display Home & Away score - TFC
  pinMode(bit1_H, OUTPUT);
  pinMode(bit2_H, OUTPUT);
  pinMode(bit3_H, OUTPUT);
  pinMode(bit4_H, OUTPUT);
  pinMode(bit1_A, OUTPUT);
  pinMode(bit2_A, OUTPUT);
  pinMode(bit3_A, OUTPUT);
  pinMode(bit4_A, OUTPUT);
}
// END setup

void loop() {

  //Debug
  PrintScore();

  //Push buttons to change Home & Away score - TFC
  up_button_val_H   = digitalRead(up_button_H);
  down_button_val_H = digitalRead(down_button_H);
  up_button_val_A   = digitalRead(up_button_A);
  down_button_val_A = digitalRead(down_button_A);
  
  //Update scores with buttons - TFC
  score_H = ScoreButtonIncrease(up_button_val_H, up_button_state_H, score_H);
  score_H = ScoreButtonDecrease(down_button_val_H, down_button_state_H, score_H);
  score_A = ScoreButtonIncrease(up_button_val_A, up_button_state_A, score_A);
  score_A = ScoreButtonDecrease(down_button_val_A, down_button_state_A, score_A);

  //Display 7 segment Home & Away scores - TFC
  Display7SegmentScore(score_H, bit1_H, bit2_H, bit3_H, bit4_H);
  Display7SegmentScore(score_A, bit1_A, bit2_A, bit3_A, bit4_A);
}
//END loop

//*------------------------------------------------------------------------------------------*//
//*------------------------------------------------------------------------------------------*//

//******************* FUNCTIONS  *********************************//

//****** IR - Goals ********************************//


//****** 7 Segment Display + Buttons - Goals ******************//

//Checks if Home or Away up buttons have been pushed, returns new score - TFC
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
            Serial.println("GOAL!");
            if(update_score > 9)
            {
                Serial.println("GAME OVER");
                update_score = 0;
            }
            button_state = HIGH;
        }
     }
  return update_score;
}//END ScoreButtonIncrease

//Checks if Home or Away down buttons have been pushed, returns new score - TFC
int ScoreButtonDecrease(int button_val, int button_state, int update_score)
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
            update_score--;
            Serial.println("GOAL!");
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


//****** Speaker + Buzzer + LED - Goals *************//


//****** LCD Jumbo-tron - Period + Score + Goal *****//


//****** Debug **************************************//

// Debug - Print scores - TFC
void PrintScore()
{
  Serial.print(score_H);
  Serial.print("    ");
  Serial.print(score_A); 
  Serial.println();  
} //END PrintScore


