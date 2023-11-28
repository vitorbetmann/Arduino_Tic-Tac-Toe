//missing features
//to be able to turn off or on by pressing the power button
//to ask if the player wants to play again
//to play trying to win, prevent winning, and the turns 2-3 plays
//to be able to pick colour using the same buttons as to select a position
//the led's gotta blink to indicate which position the player's "cursor" is at
//ai's gotta be more friendly, by taking some time to play rather than playing immediately
//well played or bummer messages when the game ends
//maybe have the player pick a game for 1 or 2 and also have messages for when p1 wins or p2 wins
//perhaps ask players name

#include <LiquidCrystal.h>
#include <IRremote.hpp>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(6, 7, 8, 9, 10, 11);
int receiver = 12;  // Signal Pin of IR receiver to Arduino Digital Pin 11
char symb1 = 'X', symb2 = 'O';
static int posx = 0, posy = 0;

//variables
int turn = 1;
bool draw = false, assigned = false;
int aiPlay[2];
long playerNum;
char blank = ' ';

// board array
char board[3][3] = { { ' ', ' ', ' ' }, { ' ', ' ', ' ' }, { ' ', ' ', ' ' } };

/*-----( Declare objects )-----*/
IRrecv irrecv(receiver);  // create instance of 'irrecv'
//vairable uses to store the last decodedRawData
uint32_t last_decodedRawData = 0;
/*-----( Function )-----*/
void translateIR()  // takes action based on IR code received
{
  // Check if it is a repeat IR code
  if (irrecv.decodedIRData.flags) {
    //set the current decodedRawData to the last decodedRawData
    irrecv.decodedIRData.decodedRawData = last_decodedRawData;
    Serial.println("REPEAT!");
  } else {
    //output the IR code on the serial monitor
    Serial.print("IR code:0x");
    Serial.println(irrecv.decodedIRData.decodedRawData, HEX);
  }
  //map the IR code to the remote key
  switch (irrecv.decodedIRData.decodedRawData) {
    case 0xBA45FF00: Serial.println("POWER"); break;
    case 0xB946FF00: posy -= 1; break;        // vol+
    case 0xBB44FF00: posx -= 1; break;        //fast back
    case 0xBF40FF00: assigned = true; break;  // play/pause button
    case 0xBC43FF00: posx += 1; break;        //fast forward
    case 0xF807FF00: symb1 = 'O'; break;      //donw arrow
    case 0xEA15FF00: posy += 1; break;        // vol-
    case 0xF609FF00: symb1 = 'X'; break;      //up arrow
    default:
      Serial.println(" other button   ");
  }  // End Case
  //store the last decodedRawData
  last_decodedRawData = irrecv.decodedIRData.decodedRawData;
  delay(500);  // Do not get immediate repeat
}  //END translateIR

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  irrecv.enableIRIn();  // Start the receiver

  lcd.begin(16, 2);
  lcd.print("Welcome to");
  lcd.setCursor(0, 1);
  lcd.print("TIC-TAC-TOE!");
  delay(3000);
  lcd.clear();

  while (true) {
    // pick symbol
    lcd.setCursor(0, 0);
    lcd.print("Press UP for RED");
    lcd.setCursor(0, 1);
    lcd.print("Or DOWN for BLUE");

    if (irrecv.decode())  // have we received an IR signal?
    {
      translateIR();
      irrecv.resume();  // receive the next value
      break;
    }
  }
  lcd.clear();

  Serial.println(symb1);

  // determines first player
  randomSeed(analogRead(A0));
  playerNum = random(1, 3);
  Serial.println(playerNum);

  if (playerNum == 2) {
    if (symb1 == 'X') symb1 = 'O';
    else if (symb1 == 'O') symb1 = 'X';
    lcd.setCursor(0, 0);
    lcd.print("You play SECOND");
    lcd.setCursor(0, 1);
    lcd.print("Good Luck!");
    delay(3000);
    lcd.clear();

  } else {
    lcd.setCursor(0, 0);
    lcd.print("You play FIRST");
    lcd.setCursor(0, 1);
    lcd.print("Good Luck!");
    delay(3000);
    lcd.clear();
  }
}

void loop() {  // put your main code here, to run repeatedly:

  //print board
  Serial.print(" ");
  Serial.print(board[0][0]);
  Serial.print(" | ");
  Serial.print(board[0][1]);
  Serial.print(" | ");
  Serial.print(board[0][2]);
  Serial.println(" ");

  Serial.println("---|---|---");

  Serial.print(" ");
  Serial.print(board[1][0]);
  Serial.print(" | ");
  Serial.print(board[1][1]);
  Serial.print(" | ");
  Serial.print(board[1][2]);

  Serial.println(" ");
  Serial.println("---|---|---");

  Serial.print(" ");
  Serial.print(board[2][0]);
  Serial.print(" | ");
  Serial.print(board[2][1]);
  Serial.print(" | ");
  Serial.print(board[2][2]);
  Serial.println(" ");

  // victory conditions
  // horizontal
  if (board[0][0] != ' ' && board[0][0] == board[0][1] && board[0][1] == board[0][2]) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(symb1);
    lcd.print("  Won the game!");
    lcd.setCursor(0, 1);
    lcd.print("See you around");
    delay(100000);  ///////////////////////////fix those
  }
  if ((board[1][0] == 'X' || board[1][0] == 'O') && board[1][0] == board[1][1] && board[1][1] == board[1][2]) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(symb1);
    lcd.print("  Won the game!");
    lcd.setCursor(0, 1);
    lcd.print("See you around");
    delay(100000);  ///////////////////////////fix those
  }
  if (board[2][1] != ' ' && board[2][0] == board[2][1] && board[2][1] == board[2][2]) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(symb1);
    lcd.print("  Won the game!");
    lcd.setCursor(0, 1);
    lcd.print("See you around");
    delay(100000);  ///////////////////////////fix those
  }
  // vertical
  if (board[0][0] != ' ' && board[0][0] == board[1][0] && board[1][0] == board[2][0]) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(symb1);
    lcd.print("  Won the game!");
    lcd.setCursor(0, 1);
    lcd.print("See you around");
    delay(100000);  ///////////////////////////fix those
  }
  if (board[0][1] != ' ' && board[0][1] == board[1][1] && board[1][1] == board[2][1]) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(symb1);
    lcd.print("  Won the game!");
    lcd.setCursor(0, 1);
    lcd.print("See you around");
    delay(100000);  ///////////////////////////fix those
  }
  if ((board[0][2] == 'X' || board[0][2] == 'O') && board[0][2] == board[1][2] && board[1][2] == board[2][2]) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(symb1);
    lcd.print("  Won the game!");
    lcd.setCursor(0, 1);
    lcd.print("See you around");
    delay(100000);  ///////////////////////////fix those
  }
  // diagonal
  if (board[0][0] != ' ' && board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(symb1);
    lcd.print("  Won the game!");
    lcd.setCursor(0, 1);
    lcd.print("See you around");
    delay(100000);  ///////////////////////////fix those
  }
  if (board[2][0] != ' ' && board[2][0] == board[1][1] && board[1][1] == board[0][2]) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(symb1);
    lcd.print("  Won the game!");
    lcd.setCursor(0, 1);
    lcd.print("See you around");
    delay(100000);  ///////////////////////////fix those
  };

  // draw condition
  if (turn == 10) {
    draw = true;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Ops, it's a draw");
    lcd.setCursor(0, 1);
    lcd.print("See you around");
    delay(100000);  ///////////////////////////fix those
  }


  if (turn != 1) {
    if (symb1 == 'X') {
      symb2 = symb1;
      symb1 = 'O';
    } else {
      symb2 = symb1;
      symb1 = 'X';
    }
  }

  Serial.println();
  Serial.print("It's ");
  Serial.print(symb1);
  Serial.println("'s turn.");
  Serial.println();

  // if player turn
  if ((playerNum == 1 && turn % 2 != 0) || (playerNum == 2 && turn % 2 == 0)) {

    assigned = false;
    // check for valid play
    while (!assigned) {

      if (irrecv.decode())  // have we received an IR signal?
      {
        translateIR();
        irrecv.resume();  // receive the next value
        if (posx > 2) posx = 0;
        if (posx < 0) posx = 2;
        if (posy > 2) posy = 0;
        if (posy < 0) posy = 2;
        Serial.println(posy);
        Serial.println(posx);
      }

      if (assigned == true && board[posy][posx] != ' ') {
        assigned = false;
        Serial.println("\nSorry, that's not a valid play");

      } else if (assigned == true) {
        board[posy][posx] = symb1;
      }
    }
  }

  // if ai turn
  else {
    bool found = false;

    while (!found) {
      //variables

      // try to win horizontal
      if (board[0][0] == board[0][1] && board[0][0] == symb1 && board[0][2] == ' ') {
        board[0][2] = symb1;
        break;
      } else if (board[0][0] == board[0][2] && board[0][0] == symb1 && board[0][1] == ' ') {
        board[0][1] = symb1;
        break;
      } else if (board[0][1] == board[0][2] && board[0][1] == symb1 && board[0][0] == ' ') {
        board[0][0] = symb1;
        break;
      } else if (board[1][0] == board[1][1] && board[1][0] == symb1 && board[1][2] == ' ') {
        board[1][2] = symb1;
        break;
      } else if (board[1][0] == board[1][2] && board[1][0] == symb1 && board[1][1] == ' ') {
        board[1][1] = symb1;
        break;
      } else if (board[1][1] == board[1][2] && board[1][1] == symb1 && board[1][0] == ' ') {
        board[1][0] = symb1;
        break;
      } else if (board[2][0] == board[2][1] && board[2][0] == symb1 && board[2][2] == ' ') {
        board[2][2] = symb1;
        break;
      } else if (board[2][0] == board[2][2] && board[2][0] == symb1 && board[2][1] == ' ') {
        board[2][1] = symb1;
        break;
      } else if (board[2][1] == board[2][2] && board[2][1] == symb1 && board[2][0] == ' ') {
        board[2][0] = symb1;
        break;
      }

      // try to win vertical
      else if (board[0][0] == board[1][0] && board[0][0] == symb1 && board[2][0] == ' ') {
        board[2][0] = symb1;
        break;
      } else if (board[0][0] == board[2][0] && board[0][0] == symb1 && board[1][0] == ' ') {
        board[1][0] = symb1;
        break;
      } else if (board[1][0] == board[2][0] && board[1][0] == symb1 && board[0][0] == ' ') {
        board[0][0] = symb1;
        break;
      } else if (board[0][1] == board[1][0] && board[0][0] == symb1 && board[2][0] == ' ') {
        board[2][1] = symb1;
        break;
      } else if (board[0][1] == board[2][0] && board[0][0] == symb1 && board[1][0] == ' ') {
        board[1][1] = symb1;
        break;
      } else if (board[1][1] == board[2][0] && board[1][0] == symb1 && board[0][0] == ' ') {
        board[0][1] = symb1;
        break;
      } else if (board[0][2] == board[1][0] && board[0][0] == symb1 && board[2][0] == ' ') {
        board[2][2] = symb1;
        break;
      } else if (board[0][2] == board[2][0] && board[0][0] == symb1 && board[1][0] == ' ') {
        board[1][2] = symb1;
        break;
      } else if (board[1][2] == board[2][0] && board[1][0] == symb1 && board[0][0] == ' ') {
        board[0][2] = symb1;
        break;
      }

      // try to win diagonal
      // downward
      else if (board[0][0] == board[1][1] && board[0][0] == symb1 && board[2][2] == ' ') {
        board[2][2] = symb1;
        break;
      } else if (board[0][0] == board[2][2] && board[0][0] == symb1 && board[1][1] == ' ') {
        board[1][1] = symb1;
        break;
      } else if (board[1][1] == board[2][2] && board[1][1] == symb1 && board[0][0] == ' ') {
        board[0][0] = symb1;
        break;
      }

      // upward
      else if (board[0][2] == board[1][1] && board[0][2] == symb1 && board[2][0] == ' ') {
        board[2][0] = symb1;
        break;
      } else if (board[0][2] == board[2][0] && board[0][2] == symb1 && board[1][1] == ' ') {
        board[1][1] = symb1;
        break;
      } else if (board[1][1] == board[2][0] && board[1][1] == symb1 && board[0][2] == ' ') {
        board[0][2] = symb1;
        break;
      }

      // prevent player win horizontal
      else if (board[0][0] == board[0][1] && board[0][0] == symb2 && board[0][2] == ' ') {
        board[0][2] = symb1;
        break;
      } else if (board[0][0] == board[0][2] && board[0][0] == symb2 && board[0][1] == ' ') {
        board[0][1] = symb1;
        break;
      } else if (board[0][1] == board[0][2] && board[0][1] == symb2 && board[0][0] == ' ') {
        board[0][0] = symb1;
        break;
      } else if (board[1][0] == board[1][1] && board[1][0] == symb2 && board[1][2] == ' ') {
        board[1][2] = symb1;
        break;
      } else if (board[1][0] == board[1][2] && board[1][0] == symb2 && board[1][1] == ' ') {
        board[1][1] = symb1;
        break;
      } else if (board[1][1] == board[1][2] && board[1][1] == symb2 && board[1][0] == ' ') {
        board[1][0] = symb1;
        break;
      } else if (board[2][0] == board[2][1] && board[2][0] == symb2 && board[2][2] == ' ') {
        board[2][2] = symb1;
        break;
      } else if (board[2][0] == board[2][2] && board[2][0] == symb2 && board[2][1] == ' ') {
        board[2][1] = symb1;
        break;
      } else if (board[2][1] == board[2][2] && board[2][1] == symb1 && board[2][0] == ' ') {
        board[2][0] = symb1;
        break;
      }

      // prevent player win vertical
      else if (board[0][0] == board[1][0] && board[0][0] == symb2 && board[2][0] == ' ') {
        board[2][0] = symb1;
        break;
      } else if (board[0][0] == board[2][0] && board[0][0] == symb2 && board[1][0] == ' ') {
        board[1][0] = symb1;
        break;
      } else if (board[1][0] == board[2][0] && board[1][0] == symb2 && board[0][0] == ' ') {
        board[0][0] = symb1;
        break;
      } else if (board[0][1] == board[1][0] && board[0][0] == symb2 && board[2][0] == ' ') {
        board[2][1] = symb1;
        break;
      } else if (board[0][1] == board[2][0] && board[0][0] == symb2 && board[1][0] == ' ') {
        board[1][1] = symb1;
        break;
      } else if (board[1][1] == board[2][0] && board[1][0] == symb2 && board[0][0] == ' ') {
        board[0][1] = symb1;
        break;
      } else if (board[0][2] == board[1][0] && board[0][0] == symb2 && board[2][0] == ' ') {
        board[2][2] = symb1;
        break;
      } else if (board[0][2] == board[2][0] && board[0][0] == symb2 && board[1][0] == ' ') {
        board[1][2] = symb1;
        break;
      } else if (board[1][2] == board[2][0] && board[1][0] == symb2 && board[0][0] == ' ') {
        board[0][2] = symb1;
        break;
      }

      // prevent player win diagonal
      // upwards
      else if (board[0][0] == board[1][1] && board[0][0] == symb2 && board[2][2] == ' ') {
        board[2][2] = symb1;
        break;
      } else if (board[0][0] == board[2][2] && board[0][0] == symb2 && board[1][1] == ' ') {
        board[1][1] = symb1;
        break;
      } else if (board[1][1] == board[2][2] && board[1][1] == symb2 && board[0][0] == ' ') {
        board[0][0] = symb1;
        break;
      }

      // downwards
      else if (board[0][2] == board[1][1] && board[0][2] == symb2 && board[2][0] == ' ') {
        board[2][0] = symb1;
        break;
      } else if (board[0][2] == board[2][0] && board[0][2] == symb2 && board[1][1] == ' ') {
        board[1][1] = symb1;
        break;
      } else if (board[1][1] == board[2][0] && board[1][1] == symb2 && board[0][2] == ' ') {
        board[0][2] = symb1;
        break;
      }

      //other plays
      else {
        for (int i = 0; i < sizeof(board) && !found; i++) {
          for (int j = 0; j < sizeof(board[i]) && !found; j++) {
            if (board[i][j] != ' ')
              continue;
            else {
              board[i][j] = symb1;
              found = true;
              break;
            }
          }
        }
      }
      if (found = true) break;
    }
  }
  turn++;
}