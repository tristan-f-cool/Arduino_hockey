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

const int IR_goal_H = 1;  //Home team IR goal sensor - TFC
const int IR_goal_A = 2;  //Away team IR goal sensor - TFC

const int speaker   = 12; //Speaker                  - TFC

const int LED_R_H   = 3;  //Home goal LED Red  (PWM) - TFC
const int LED_G_H   = 5;  //Home goal LED Green(PWM) - TFC
const int LED_B_H   = 6;  //Home goal LED Blue (PWM) - TFC
const int LED_R_A   = 9;  //Away goal LED Red  (PWM) - TFC
const int LED_G_A   = 10; //Away goal LED Green(PWM) - TFC
const int LED_B_A   = 11; //Away goal LED Blue (PWM) - TFC


//********************* VARIABLES **********************************//
// ex) int score_H = 0; //Home team score - TFC

bool goal_flag_H = false; //Home team goal flag - TFC
bool goal_flag_A = false; //Away team goal flag - TFC
int  goal_H;              //Home team goal      - TFC
int  goal_A;              //Away team goal      - TFC


//******************** FUNCTION PROTOTYPES ************************//
// ex) void GoalSiren(); //Siren sounds when a goal is scored - TFC

void PrintGoal();                                 //Debug                     - TFC
void HomeGoal();                                  //Home goal has been scored - TFC
void AwayGoal();                                  //Away goal has been scored - TFC
void GoalSiren();                                 //Goal siren                - TFC
void PeriodSiren();                               //End of period siren       - TFC
void GoalLEDs(int LED_R, int LED_G, int LED_B);   //LED strip every goal      - TFC
void PeriodLEDs();                                //LED strip end of period   - TFC
void StartGameLEDs();                             //LED strip start of game   - TFC
void EndGameLEDs();                               //LED strip end of game     - TFC

//*------------------------------------------------------------------------------------------*//
//*------------------------------------------------------------------------------------------*//

void setup() {

  //Serial Monitor - Debug (Ctrl+Shift+m)
  Serial.begin(9600);
  Serial.println("IEEE Concordia - Frosh Hockey 2016-2017");

  //IR Goal sensors
  pinMode(IR_goal_H, INPUT);
  pinMode(IR_goal_A, INPUT);

  //Goal LED RGB strip
  pinMode(LED_R_H, OUTPUT);
  pinMode(LED_G_H, OUTPUT);
  pinMode(LED_B_H, OUTPUT);
  pinMode(LED_R_A, OUTPUT);
  pinMode(LED_G_A, OUTPUT);
  pinMode(LED_B_A, OUTPUT);

}//END setup

void loop() {

  //IR Goal sensors
  goal_H = digitalRead(IR_goal_H);
  goal_A = digitalRead(IR_goal_A);

  if(goal_H)
  {
      HomeGoal();
  }

  if(goal_A)
  {
      AwayGoal(); 
  }

}//END loop

//*------------------------------------------------------------------------------------------*//
//*------------------------------------------------------------------------------------------*//

//******************* FUNCTIONS  *********************************//

//****** IR - Goals ********************************//

//Actions when Home goal is scored - TFC
void HomeGoal()
{
    goal_flag_H = true;
    PrintScore();
    GoalLEDs(LED_R_H, LED_G_H, LED_B_H);
    GoalSiren();
    //TODO - TFC
    goal_flag_H = false;
}//END HomeGoal

//Actions when Away goal is scored - TFC
void AwayGoal()
{
    goal_flag_A = true;
    PrintScore();
    GoalLEDs(LED_R_A, LED_G_A, LED_B_A);
    GoalSiren();
    //TODO - TFC
    goal_flag_A = false; 
}//END AwayGoal


//****** 7 Segment Display - Goals ******************//


//****** Speaker + LED - Goals/Period *************//

//Siren when goal is scored - TFC
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
}//END GoalSiren()

//Siren when period is ended - TFC
void PeriodSiren()
{
    //ex) tone(speaker, speaker_tone,tone_duration) - TFC 
    tone(speaker, 500, 2000);  
}//END PeriodSiren

//LED RGB strip lights up half of the arena when a team scores - TFC
void GoalLEDs(int LED_R, int LED_G, int LED_B)
{
    //TODO - TFC
}//END GoalLEDs

//LED RGB strip lights at end of periods
void PeriodLEDs()
{
  //TODO - TFC
}//END PeriodLEDs

//LED RGB strip lights at start of game
void StartGameLEDs()
{
  //TODO - TFC
}//END StartGameLEDs

//LED RGB strip lights at end of game
void EndGameLEDs()
{
  //TODO - TFC
}//END EndGameLEDs


//****** LCD Jumbo-tron - Period + Score + Goal *****//


//****** Debug **************************************//

// Debug - Print goal messages - TFC
void PrintGoal()
{
    if(goal_flag_H)
    {
      Serial.println("HOME GOAL!");
    }
    if(goal_flag_A)
    {
      Serial.println("AWAY GOAL!");
    }
} //END PrintScore
