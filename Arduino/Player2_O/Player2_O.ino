#include <LiquidCrystal.h>
#include <Keypad.h>

#define rs 13
#define en 12
#define d4 8
#define d5 9
#define d6 10
#define d7 11

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int turn = 1;
bool turnPlayer=true;
const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {A12, A11, A10, A9}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {A13, A14, A15}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
int ledArray[18] = {22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39};

char grid[9]={
  '-','-','-',
  '-','-','-',
  '-','-','-'
};

char keyss;
char receivedData;
void setup() {
  
  Serial.begin(9600);
  lcd.begin(16, 4);
  lcd.clear();
 for(int i = 0 ; i < 18 ; i++){
   pinMode(ledArray[i], OUTPUT);
 }
  printDisplay();
  
}

void loop() {
  if(turnPlayer){
     keyss = keypad.getKey();  
     if (keyss){
        if (keyss > 48 && keyss <= 57){
            int index = keyss - 48 -1 ;
            if(grid[index] == '-'){
               grid[index] = 'o';    
               printDisplay();
               turnPlayer = false;
               char setWinner =  winner();
               Serial.write(keyss); 
               turnONLED(keyss);
               if(setWinner =='x'){
                  Serial.write('x');
                  lcd.setCursor(3 , 0);
                  lcd.print("Player1 X won");
                  LEDBlinking();
                  delay(300);
                  LEDBlinking();
               }else if(setWinner =='o'){
                     Serial.write('o');
                     lcd.setCursor(3 , 0);
                     lcd.print("Player2 O won");
                     LEDBlinking();
                     delay(300);
                     LEDBlinking();
               }else if(setWinner == '='){
                     Serial.write('=');
                     lcd.setCursor(3 , 0);
                     lcd.print("NO  Winner");
               }      
            }   
         }else if(keyss == '0')
               resetGame();  
     }
}
  if(Serial.available()){
        receivedData = Serial.read();
        turnONLED(receivedData);
         if(receivedData == 'x'){             
            lcd.setCursor(3 , 0);
            lcd.print("Player1 X Won");
            LEDBlinking();
            delay(300);
            LEDBlinking();
            turnPlayer = false;
        }else if(receivedData == 'o'){                    
           lcd.setCursor(3 , 0);
           lcd.print("Player2 O Won");
           LEDBlinking();
           delay(300);
           LEDBlinking();
           turnPlayer = false;
        }else if(receivedData == '='){
           lcd.setCursor(3 , 0);
           lcd.print("NO Winner");
           turnPlayer = false;
        }else{
           turnPlayer = true;
       
           int index = receivedData - 48 - 1 ;
           
           if(grid[index]=='-'){
              grid[index]= 'x';
              
              printDisplay();
           }
                                   
        }
      
      }
}



void printDisplay()
{
  lcd.clear();
  lcd.setCursor(0 , 0);
  lcd.print("O] ");
  lcd.setCursor(7 , 1);
  lcd.write(grid[0]);
  lcd.setCursor(8 , 1);
  lcd.write(grid[1]);
  lcd.setCursor(9 , 1);
  lcd.write(grid[2]);
  lcd.setCursor(7 , 2);
  lcd.write(grid[3]);
  lcd.setCursor(8 , 2);
  lcd.write(grid[4]);
  lcd.setCursor(9 , 2);
  lcd.write(grid[5]);
  lcd.setCursor(7 , 3);
  lcd.write(grid[6]);
  lcd.setCursor(8 , 3);
  lcd.write(grid[7]);
  lcd.setCursor(9 , 3);
  lcd.write(grid[8]);
  
  }
  
void turnONLED(char inpData)
{
  int index = inpData - 48 - 1;

  if (turn % 2 == 0)
  {
    digitalWrite(ledArray[index], HIGH);
    digitalWrite(ledArray[index+9], LOW);
  }
  else
  {
    digitalWrite(ledArray[index], LOW);
    digitalWrite(ledArray[index+9], HIGH);
  }
  switchPlayers(); 
}

void switchPlayers(){
  turn++;
}

void LEDBlinking()
{
  if (turn % 2 == 0)
  { 
    for (int i = 0; i < 9 ; i++)
    {
      digitalWrite(ledArray[i], HIGH);
      digitalWrite(ledArray[i + 9], LOW);
    }
  }
  else
  {
    for (int i = 0; i < 9 ; i++)
    {
      digitalWrite(ledArray[i], LOW);
      digitalWrite(ledArray[i + 9], HIGH);
    }
  }
  delay(500);
}

char  winner(){
  char winner='0';
  boolean chk = false;
  
  if(grid[0] == grid[1] && grid[1] == grid[2] && (grid[0] != '-')){
      chk = true;
      winner = grid[0];
    }else if(grid[3] == grid[4] && grid[4] == grid[5] && (grid[3] != '-')){
      chk = true;
      winner = grid[3];
    }else if(grid[6] == grid[7] && grid[7] == grid[8] && (grid[6] != '-')){
      chk = true;
      winner = grid[6];
    }else if(grid[0] == grid[3] && grid[3] == grid[6] && (grid[0] != '-')){
      chk = true;
      winner = grid[0];
    }else if(grid[1] == grid[4] && grid[4] == grid[7] && (grid[1] != '-')){
      chk = true;
      winner = grid[1];
    }else if(grid[2] == grid[5] && grid[5] == grid[8] && (grid[2] != '-')){
      chk = true;
      winner = grid[2];
    }else if(grid[0] == grid[4] && grid[4] == grid[8] && (grid[0] != '-')){
      chk = true;
      winner = grid[0];
    }else if(grid[2] == grid[4] && grid[4] == grid[6] && (grid[2] != '-')){
      chk = true;
      winner = grid[2];
    }

    int counter = 0;
    for (int i = 0; i < 9 ; i++)
        if (grid[i] != '-')
             counter++;

    if (counter == 9 && chk == false)
        winner = '=';
   
  return winner;
}

 void resetGame()
{
  turn = 1;
  turnPlayer=true;
  for (int i = 0 ; i < 9 ; i++)
  {
    grid[i] = '-';
  }
    for (int i = 0 ; i < 18 ; i++)
  {
    digitalWrite(ledArray[i], LOW);
  }
   printDisplay();
}
