#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Preferences.h>

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


Preferences preferences;


int leds[] = {13, 18, 19, 17};   // 0:Yellow, 1:Red, 2:Green, 3:Blue
int btns[] = {27, 26, 25, 16};   // Coressponding buttons
int buzzer = 14;              

// Game State Machine
enum GameState {
  MENU,
  MEM_START, REACT_START, CODE_START, CATCH_START, 
  LIGHTS_START, CIRCUIT_START, TWENTYONE_START,
  TRADER_START, DINO_START, WHACK_START, LUNAR_START, 
  SNAKE_START, MAZE_START, ASTEROID_START, SKY_START, CAVE_START
};
GameState currentState = MENU;

// Menu Pagination
int menuPage = 0; // 0 = Pg 1, 1 = Pg 2, 2 = Pg 3, 3 = Pg 4, 4 = Pg 5

// Global Game Variables
int gameSequence[50];            
int level = 2;                   

void setup() {
  Serial.begin(115200);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { for(;;); }

  for(int i=0; i<4; i++) {
    pinMode(leds[i], OUTPUT);
    pinMode(btns[i], INPUT_PULLUP);
  }
  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);
  randomSeed(analogRead(0));
}

void loop() {
  switch (currentState) {
    case MENU: {
      showMenu();
      int btnPressed = waitForButton();
      
      if (btnPressed == 3) { // Blue button scrolls pages
        menuPage++;
        if (menuPage > 5) menuPage = 0;
        delay(200); 
      }
      else if (menuPage == 0) {
        if (btnPressed == 0) currentState = MEM_START;
        else if (btnPressed == 1) currentState = REACT_START;
        else if (btnPressed == 2) currentState = CODE_START;
      }
      else if (menuPage == 1) {
        if (btnPressed == 0) currentState = CATCH_START;
        else if (btnPressed == 1) currentState = LIGHTS_START;
        else if (btnPressed == 2) currentState = CIRCUIT_START;
      }
      else if (menuPage == 2) {
        if (btnPressed == 0) currentState = TWENTYONE_START;
        else if (btnPressed == 1) currentState = TRADER_START;
        else if (btnPressed == 2) currentState = DINO_START;
      }
      else if (menuPage == 3) {
        if (btnPressed == 0) currentState = WHACK_START;
        else if (btnPressed == 1) currentState = LUNAR_START; 
        else if (btnPressed == 2) currentState = SNAKE_START;
      }
      else if (menuPage == 4) {
        if (btnPressed == 0) currentState = MAZE_START;
        else if (btnPressed == 1) currentState = ASTEROID_START;
        else if (btnPressed == 2) currentState = SKY_START;
      }
      else if (menuPage == 5) {
        if (btnPressed == 0) currentState = CAVE_START;
      }
      break;
    }

    case MEM_START:
      showGameIntro("MEMORY GAME", "YELLOW");
      if (waitForButton() == 0) playMemoryGame();
      currentState = MENU; break;

    case REACT_START:
      showGameIntro("REACTION GAME", "RED");
      if (waitForButton() == 1) playReactionGame();
      currentState = MENU; break;

    case CODE_START:
      showGameIntro("CODE BREAKER", "GREEN");
      if (waitForButton() == 2) playCodeBreaker();
      currentState = MENU; break;

    case CATCH_START:
      showGameIntro("CATCH THE DOT", "YELLOW"); 
      if (waitForButton() == 0) playCatchGame();
      currentState = MENU; break;

    case LIGHTS_START:
      showGameIntro("LIGHTS OUT", "RED");
      if (waitForButton() == 1) playLightsOut();
      currentState = MENU; break;

    case CIRCUIT_START:
      showGameIntro("CIRCUIT CONNECT", "GREEN");
      if (waitForButton() == 2) playCircuitConnector();
      currentState = MENU; break;

    case TWENTYONE_START:
      showGameIntro("GAME OF 21", "YELLOW");
      if (waitForButton() == 0) playGameOf21();
      currentState = MENU; break;
      
    case TRADER_START:
      showGameIntro("DAY TRADER", "RED");
      if (waitForButton() == 1) playDayTrader();
      currentState = MENU; break;

    case DINO_START:
      showGameIntro("DINO RUN", "GREEN");
      if (waitForButton() == 2) playDinoRun();
      currentState = MENU; break;
      
    case WHACK_START:
      showGameIntro("WHACK A MOLE", "YELLOW");
      if (waitForButton() == 0) playWhackAMole();
      currentState = MENU; break;

    case LUNAR_START:
      showGameIntro("LUNAR LANDER", "RED");
      if (waitForButton() == 1) playLunarLander();
      currentState = MENU; break;
    
    case SNAKE_START:
      showGameIntro("CLASSIC SNAKE", "GREEN");
      if (waitForButton() == 2) playSnake();
      currentState = MENU; break;

    case MAZE_START:
      showGameIntro("SHADOW MAZE", "YELLOW");
      if (waitForButton() == 0) playShadowMaze();
      currentState = MENU; break;
    
    case ASTEROID_START:
      showGameIntro("ASTEROID DEF", "RED");
      if (waitForButton() == 1) playAsteroidDefender();
      currentState = MENU; break;

    case SKY_START:
      showGameIntro("SKY DEFENDER", "GREEN");
      if (waitForButton() == 2) playSkyDefender();
      currentState = MENU; break;
    
    case CAVE_START:
      showGameIntro("CAVE FLYER", "YELLOW");
      if (waitForButton() == 0) playCaveFlyer();
      currentState = MENU; break;
  }
}

// --- UI DISPLAY FUNCTIONS ---

void showMenu() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  
  display.setCursor(0, 0);
  display.print("SELECT GAME  (Pg "); display.print(menuPage + 1); display.println("/6)");
  display.drawLine(0, 10, 128, 10, WHITE);
  
  if (menuPage == 0) {
    display.setCursor(0, 16); display.println("YEL: Memory");
    display.setCursor(0, 28); display.println("RED: Reaction");
    display.setCursor(0, 40); display.println("GRN: Code Breaker");
  } else if (menuPage == 1) {
    display.setCursor(0, 16); display.println("YEL: Catch the Dot");
    display.setCursor(0, 28); display.println("RED: Lights Out");
    display.setCursor(0, 40); display.println("GRN: Circuit Connect");
  } else if (menuPage == 2) {
    display.setCursor(0, 16); display.println("YEL: Game of 21");
    display.setCursor(0, 28); display.println("RED: Day Trader");
    display.setCursor(0, 40); display.println("GRN: Dino Run");
  } else if (menuPage == 3) {
    display.setCursor(0, 16); display.println("YEL: Whack-a-Mole");
    display.setCursor(0, 28); display.println("RED: Lunar Lander");
    display.setCursor(0, 40); display.println("GRN: Snake");
  } else if (menuPage == 4) {
    display.setCursor(0, 16); display.println("YEL: Shadow Maze");
    display.setCursor(0, 28); display.println("RED: Asteroid Def"); 
    display.setCursor(0, 40); display.println("GRN: Sky Defender");
  } else if (menuPage == 5) {
    display.setCursor(0, 16); display.println("YEL: Cave Flyer");
    display.setCursor(0, 28); display.println("RED: ---");
    display.setCursor(0, 40); display.println("GRN: ---");
  }
  
  display.setCursor(0, 52); display.println("BLU: Next Page ->");
  display.display();
}

void showGameIntro(String title, String buttonColor) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 10); display.println(title);
  display.setCursor(0, 35);
  display.print("Press "); display.print(buttonColor); display.println(" again");
  display.println("to Start!");
  display.display();
}

void countdown() {
  for (int i = 3; i > 0; i--) {
    display.clearDisplay();
    display.setTextSize(4); display.setCursor(50, 20); display.println(i);
    display.display();
    digitalWrite(buzzer, HIGH); delay(100); digitalWrite(buzzer, LOW); delay(900); 
  }
  display.clearDisplay(); display.setCursor(30, 20); display.println("GO!");
  display.display(); delay(500);
}

// --- HARDWARE HELPER FUNCTIONS ---

void flashLED(int index) {
  digitalWrite(leds[index], HIGH);
  digitalWrite(buzzer, HIGH); delay(200);
  digitalWrite(leds[index], LOW);
  digitalWrite(buzzer, LOW);
}

int waitForButton() {
  while (true) {
    for (int i = 0; i < 4; i++) {
      if (digitalRead(btns[i]) == LOW) {
        delay(50); // Debounce
        while(digitalRead(btns[i]) == LOW); // Wait for release
        return i;
      }
    }
  }
}

void LEDMultiflash() {
  for (int count = 0; count < 7; count++) { 
    for (int i = 0; i < 4; i++) digitalWrite(leds[i], HIGH);
    delay(150);
    for (int i = 0; i < 4; i++) digitalWrite(leds[i], LOW);
    delay(150);
  }
}

// --- GAME 1: MEMORY (SIMON) ---
void playMemoryGame() {
  preferences.begin("scores", false);
  int best = preferences.getInt("mem", 0);

  level = 2; 
  bool isAlive = true;
  countdown();
  while (isAlive) {
    display.clearDisplay(); display.setCursor(20, 25); display.setTextSize(2);
    display.print("LEVEL: "); display.println(level); display.display();
    delay(1000);

    for (int i = 0; i < level; i++) gameSequence[i] = random(0, 4);
    for (int i = 0; i < level; i++) { flashLED(gameSequence[i]); delay(200); }
    for (int i = 0; i < level; i++) {
      int expected = gameSequence[i];
      int pressed = waitForButton();
      if (pressed != expected) { isAlive = false; break; }
      flashLED(pressed); 
    }
    if (isAlive) { level++; delay(500); }
  }
  
  int finalScore = level - 1;
  bool newRecord = false;
  if (finalScore > best) {
    best = finalScore;
    preferences.putInt("mem", best);
    newRecord = true;
  }
  preferences.end();

  display.clearDisplay(); display.setTextSize(2); 
  if (newRecord) {
    display.setCursor(5, 10); display.println("NEW RECORD");
  } else {
    display.setCursor(10, 10); display.println("LOSER");
  }
  display.setTextSize(1); 
  display.setCursor(0, 35); display.print("Score: "); display.println(finalScore);
  display.setCursor(0, 48); display.print("Best:  "); display.println(best);
  display.display();

  digitalWrite(buzzer, HIGH); LEDMultiflash(); digitalWrite(buzzer, LOW); delay(3000); 
}

// --- GAME 2: REACTION ---
void playReactionGame() {
  preferences.begin("scores", false);
  int best = preferences.getInt("react", 99999);

  countdown();
  unsigned long totalTime = 0;
  for(int trial = 1; trial <= 3; trial++) {
    display.clearDisplay(); display.setTextSize(2); display.setCursor(10, 20);
    display.print("TRIAL "); display.println(trial); display.setTextSize(1);
    display.setCursor(0, 45); display.println("Wait for it..."); display.display();

    delay(random(1500, 5000));
    int target = random(0, 4);
    digitalWrite(leds[target], HIGH);
    
    unsigned long startTime = millis();
    int pressed = -1;
    while(pressed != target) { pressed = waitForButton(); }
    
    unsigned long reactionTime = millis() - startTime;
    digitalWrite(leds[target], LOW);
    totalTime += reactionTime;

    display.clearDisplay(); display.setTextSize(2); display.setCursor(10, 20);
    display.print(reactionTime); display.println(" ms"); display.display(); delay(2000);
  }

  int avg = totalTime / 3;
  bool newRecord = false;
  if (avg < best) {
    best = avg;
    preferences.putInt("react", best);
    newRecord = true;
  }
  preferences.end();

  display.clearDisplay(); display.setTextSize(2); 
  if (newRecord) {
    display.setCursor(5, 10); display.println("NEW RECORD");
  } else {
    display.setCursor(20, 10); display.println("AVERAGE");
  }
  display.setTextSize(1); 
  display.setCursor(0, 35); display.print("Avg:  "); display.print(avg); display.println("ms");
  display.setCursor(0, 48); display.print("Best: "); display.print(best); display.println("ms");
  display.display(); 
  
  if(newRecord) LEDMultiflash();
  delay(5000);
}

// --- GAME 3: CODE BREAKER ---
void playCodeBreaker() {
  countdown();
  int secret[4];
  for(int i=0; i<4; i++) secret[i] = random(0, 4); 
  
  int attempts = 0; bool cracked = false;
  
  while(attempts < 10 && !cracked) {
    display.clearDisplay(); display.setTextSize(1); display.setCursor(0,0);
    display.print("Attempt "); display.print(attempts + 1); display.println("/10");
    display.drawLine(0, 10, 128, 10, WHITE); display.setCursor(0, 20);
    display.println("Input 4 colors:"); display.display();
    
    int guess[4]; display.setCursor(0, 35); display.setTextSize(2);
    for(int i=0; i<4; i++) {
      guess[i] = waitForButton(); flashLED(guess[i]);
      display.print("* "); display.display();
    }
    
    int exactMatch = 0; int colorMatch = 0;  
    bool secretUsed[4] = {false, false, false, false};
    bool guessUsed[4]  = {false, false, false, false};
    
    for(int i=0; i<4; i++) {
      if(guess[i] == secret[i]) { exactMatch++; secretUsed[i] = true; guessUsed[i] = true; }
    }
    for(int i=0; i<4; i++) {
      if(!guessUsed[i]) {
        for(int j=0; j<4; j++) {
          if(!secretUsed[j] && guess[i] == secret[j]) { colorMatch++; secretUsed[j] = true; break; }
        }
      }
    }
    
    display.clearDisplay(); display.setTextSize(1); display.setCursor(0, 0); 
    display.println("CLUES:"); display.drawLine(0, 10, 128, 10, WHITE);
    display.setCursor(0, 20); display.print("Perfect:     "); display.println(exactMatch);
    display.setCursor(0, 35); display.print("Wrong Spot:  "); display.println(colorMatch);
    display.display();
    
    if(exactMatch == 4) { cracked = true; break; }
    attempts++; delay(3500); 
  }
  display.clearDisplay(); display.setTextSize(2); display.setCursor(0, 15);
  if(cracked) { display.println("CRACKED!"); LEDMultiflash(); } 
  else { display.println("FAILED..."); }
  display.display(); delay(4000);
}

// --- GAME 4: CATCH THE DOT ---
void playCatchGame() {
  preferences.begin("scores", false);
  int best = preferences.getInt("catch", 0);

  countdown();
  unsigned long speed = 250; 
  int score = 0; bool isAlive = true;
  int currentPos = 0; int direction = 1; 
  String colorNames[] = {"YELLOW", "RED", "GREEN", "BLUE"};

  while(isAlive) {
    int target = random(0, 4); bool caught = false;
    
    display.clearDisplay(); display.setTextSize(2); display.setCursor(0, 0); 
    display.print("Sc: "); display.println(score); display.setTextSize(1);
    display.setCursor(0, 25); display.println("TARGET COLOR:"); display.setTextSize(2);
    display.setCursor(0, 40); display.println(colorNames[target]); display.display();
    
    while(!caught && isAlive) {
      digitalWrite(leds[currentPos], HIGH);
      unsigned long startMs = millis(); int pressedBtn = -1;
      
      while(millis() - startMs < speed) {
        for(int i=0; i<4; i++) {
          if(digitalRead(btns[i]) == LOW) { pressedBtn = i; delay(50); break; }
        }
        if(pressedBtn != -1) break; 
      }
      digitalWrite(leds[currentPos], LOW);
      
      if(pressedBtn != -1) {
         if(pressedBtn == target && currentPos == target) {
            digitalWrite(buzzer, HIGH); delay(100); digitalWrite(buzzer, LOW);
            score++; speed = speed - (speed / 10); caught = true;
            while(digitalRead(btns[pressedBtn]) == LOW);
         } else { isAlive = false; }
      }
      
      if(!caught && isAlive) {
        currentPos += direction;
        if(currentPos >= 3) { currentPos = 3; direction = -1; } 
        else if(currentPos <= 0) { currentPos = 0; direction = 1; }
      }
    }
  }

  bool newRecord = false;
  if (score > best) {
    best = score;
    preferences.putInt("catch", best);
    newRecord = true;
  }
  preferences.end();

  display.clearDisplay(); display.setTextSize(2); 
  if (newRecord) {
    display.setCursor(5, 10); display.println("NEW RECORD");
  } else {
    display.setCursor(10, 10); display.println("LOSER");
  }
  display.setTextSize(1); 
  display.setCursor(0, 35); display.print("Score: "); display.println(score);
  display.setCursor(0, 48); display.print("Best:  "); display.println(best);
  display.display();

  digitalWrite(buzzer, HIGH); LEDMultiflash(); digitalWrite(buzzer, LOW); delay(4000);
}

// --- GAME 5: LIGHTS OUT ---
void playLightsOut() {
  preferences.begin("scores", false);
  int best = preferences.getInt("lights", 9999);

  countdown();
  bool board[4] = {false, false, false, false};
  
  for(int i=0; i<5; i++) {
     int r = random(0, 4);
     board[r] = !board[r];
     if(r > 0) board[r-1] = !board[r-1];
     if(r < 3) board[r+1] = !board[r+1];
  }
  if(!board[0] && !board[1] && !board[2] && !board[3]) board[0] = true;

  bool playing = true;
  int moves = 0;
  
  while(playing) {
     for(int i=0; i<4; i++) digitalWrite(leds[i], board[i] ? HIGH : LOW);
     
     display.clearDisplay();
     display.setTextSize(2); display.setCursor(0,0); display.println("LIGHTS OUT");
     display.setTextSize(1); display.setCursor(0, 25); 
     display.println("Turn all LEDs OFF");
     display.setCursor(0, 45); display.print("Moves: "); display.println(moves);
     display.display();
     
     int btn = waitForButton();
     moves++;
     digitalWrite(buzzer, HIGH); delay(50); digitalWrite(buzzer, LOW);
     
     board[btn] = !board[btn];
     if(btn > 0) board[btn-1] = !board[btn-1];
     if(btn < 3) board[btn+1] = !board[btn+1];
     
     if(!board[0] && !board[1] && !board[2] && !board[3]) playing = false;
  }
  
  for(int i=0; i<4; i++) digitalWrite(leds[i], LOW);

  bool newRecord = false;
  if (moves < best) {
    best = moves;
    preferences.putInt("lights", best);
    newRecord = true;
  }
  preferences.end();

  display.clearDisplay(); display.setTextSize(2); 
  if (newRecord) {
    display.setCursor(5, 10); display.println("NEW RECORD");
  } else {
    display.setCursor(20, 10); display.println("SOLVED!");
  }
  display.setTextSize(1); 
  display.setCursor(0, 35); display.print("Moves: "); display.println(moves);
  display.setCursor(0, 48); display.print("Best:  "); display.println(best);
  display.display();
  
  LEDMultiflash(); delay(4000);
}

// --- GAME 6: CIRCUIT CONNECTOR ---
void playCircuitConnector() {
  countdown();
  bool isAlive = true;

  for (int level = 1; level <= 3; level++) {
    if (!isAlive) break;

    int tileTypes[4]; 
    int solution[4];  
    unsigned long timeLimit = 20000; 

    if (level == 1) {
      int t[] = {1, 0, 1, 0}; memcpy(tileTypes, t, sizeof(t));
      int s[] = {3, 1, 1, 0}; memcpy(solution, s, sizeof(s));
    } else if (level == 2) {
      int t[] = {0, 1, 0, 1}; memcpy(tileTypes, t, sizeof(t));
      int s[] = {0, 3, 1, 1}; memcpy(solution, s, sizeof(s)); 
      timeLimit = 15000;
    } else {
      int t[] = {1, 1, 1, 1}; memcpy(tileTypes, t, sizeof(t));
      int s[] = {3, 2, 1, 0}; memcpy(solution, s, sizeof(s));
      timeLimit = 12000;
    }

    int tiles[4];
    for (int i = 0; i < 4; i++) {
      tiles[i] = random(0, 4);
      if (tiles[i] == solution[i]) tiles[i] = (tiles[i] + 1) % 4; 
    }

    unsigned long startTime = millis();
    bool levelWon = false;

    while (millis() - startTime < timeLimit && !levelWon) {
      int timeLeft = (timeLimit - (millis() - startTime)) / 1000;

      int btn = -1;
      for (int i = 0; i < 4; i++) {
        if (digitalRead(btns[i]) == LOW) {
          btn = i;
          delay(50); 
          while (digitalRead(btns[i]) == LOW); 
          break;
        }
      }

      if (btn != -1) {
        tiles[btn] = (tiles[btn] + 1) % 4;
        digitalWrite(buzzer, HIGH); delay(30); digitalWrite(buzzer, LOW);
      }

      display.clearDisplay();
      display.setTextSize(1);
      display.setCursor(0, 0); display.print("Lvl: "); display.print(level);
      display.setCursor(70, 0); display.print("Time: "); display.print(timeLeft);

      display.drawRect(5, 16, 12, 32, WHITE);
      display.setCursor(8, 28); display.print("+");
      display.drawLine(17, 24, 40, 24, WHITE); 
      display.drawLine(17, 48, 40, 48, WHITE); 

      display.drawCircle(115, 36, 10, WHITE);
      display.drawRect(105, 30, 5, 12, WHITE);
      display.drawLine(80, 24, 105, 24, WHITE); 
      display.drawLine(80, 48, 105, 48, WHITE); 

      display.drawLine(56, 24, 64, 24, WHITE); 
      display.drawLine(56, 48, 64, 48, WHITE); 
      display.drawLine(48, 32, 48, 40, WHITE); 
      display.drawLine(72, 32, 72, 40, WHITE); 

      int tx[] = {40, 64, 40, 64};
      int ty[] = {16, 16, 40, 40};
      
      for (int i = 0; i < 4; i++) {
        display.drawRect(tx[i], ty[i], 16, 16, WHITE); 
        int cx = tx[i] + 8; 
        int cy = ty[i] + 8; 

        int type = tileTypes[i];
        int rot = tiles[i];

        if (type == 0) { 
          if (rot == 0 || rot == 2) display.drawLine(tx[i], cy, tx[i] + 16, cy, WHITE);
          else display.drawLine(cx, ty[i], cx, ty[i] + 16, WHITE);
        } else { 
          if (rot == 0) { display.drawLine(cx, cy, tx[i], cy, WHITE); display.drawLine(cx, cy, cx, ty[i], WHITE); }       
          else if (rot == 1) { display.drawLine(cx, cy, cx, ty[i], WHITE); display.drawLine(cx, cy, tx[i] + 16, cy, WHITE); } 
          else if (rot == 2) { display.drawLine(cx, cy, tx[i] + 16, cy, WHITE); display.drawLine(cx, cy, cx, ty[i] + 16, WHITE); } 
          else if (rot == 3) { display.drawLine(cx, cy, cx, ty[i] + 16, WHITE); display.drawLine(cx, cy, tx[i], cy, WHITE); } 
        }
      }
      display.display();

      bool winCheck = true;
      for (int i = 0; i < 4; i++) {
        if (tileTypes[i] == 0) {
          if (tiles[i] % 2 != solution[i] % 2) winCheck = false; 
        } else {
          if (tiles[i] != solution[i]) winCheck = false;
        }
      }

      if (winCheck) {
        levelWon = true;
        display.fillCircle(115, 36, 10, WHITE);
        display.display();
        
        digitalWrite(buzzer, HIGH);
        for(int j=0; j<4; j++) digitalWrite(leds[j], HIGH);
        delay(500);
        digitalWrite(buzzer, LOW);
        for(int j=0; j<4; j++) digitalWrite(leds[j], LOW);
        delay(1000);
      }
    } 

    if (!levelWon) isAlive = false; 
  } 

  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(10, 10);
  
  if (isAlive) {
    display.println("YOU WIN!");
    LEDMultiflash();
  } else {
    display.println("OUT OF TIME");
    digitalWrite(buzzer, HIGH); LEDMultiflash(); digitalWrite(buzzer, LOW);
  }
  
  display.display();
  delay(3000);
}

// --- GAME 7: GAME OF 21 ---
void playGameOf21() {
  countdown();
  int pile = 21;
  bool playerTurn = true; 
  bool playerWon = false;

  while (pile > 0) {
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0, 0); display.print("PILE: "); display.println(pile);
    
    if (playerTurn) {
      display.setTextSize(1);
      display.setCursor(0, 25); display.println("Your Turn!");
      display.setCursor(0, 40); display.println("YEL:1  RED:2  GRN:3");
      display.display();

      int take = 0;
      while (take == 0) {
        int btn = waitForButton();
        if (btn == 0) take = 1;
        else if (btn == 1) take = 2;
        else if (btn == 2) take = 3;
      }

      if (take > pile) take = pile; 
      pile -= take;
      
      flashLED(take - 1); 
      
      if (pile <= 0) {
        playerWon = false; 
        break;
      }
      playerTurn = false;
      delay(500); 
      
    } else {
      display.setTextSize(1);
      display.setCursor(0, 25); display.println("ESP32 is thinking...");
      display.display();
      delay(1500); 

      int aiTake = 1;
      int idealMove = (pile - 1) % 4; 
      
      if (idealMove == 0 || random(0, 100) < 30) { 
        aiTake = random(1, 4);
      } else {
        aiTake = idealMove;
      }
      
      if (aiTake > pile) aiTake = pile;
      pile -= aiTake;

      display.clearDisplay();
      display.setTextSize(2);
      display.setCursor(0, 0); display.print("PILE: "); display.println(pile + aiTake); 
      display.setTextSize(1);
      display.setCursor(0, 25); display.print("ESP32 took "); display.println(aiTake);
      display.display();
      
      for(int i=0; i < aiTake; i++) {
         digitalWrite(buzzer, HIGH); delay(100); digitalWrite(buzzer, LOW); delay(100);
      }
      delay(2000); 
      
      if (pile <= 0) {
        playerWon = true; 
        break;
      }
      playerTurn = true;
    }
  }

  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(10, 10);
  
  if (playerWon) {
    display.println("YOU WIN!");
    LEDMultiflash();
  } else {
    display.println("ESP32 WINS");
    digitalWrite(buzzer, HIGH); delay(1500); digitalWrite(buzzer, LOW);
  }
  
  display.display();
  delay(4000);
}

// --- GAME 8: DAY TRADER ---
void playDayTrader() {
  preferences.begin("scores", false);
  int best = preferences.getInt("trade", 100); // Start at 100

  countdown();
  unsigned long startTime = millis();
  float cash = 100.0;
  float shares = 0.0;
  float currentPrice = 30.0; 
  
  uint8_t history[128];
  for(int i=0; i<128; i++) history[i] = 30;

  while(millis() - startTime < 30000) { 
    int timeLeft = 30 - ((millis() - startTime) / 1000);
    
    currentPrice += random(-4, 5);
    if(currentPrice > 45) currentPrice = 45; 
    if(currentPrice < 5) currentPrice = 5;
    
    for(int i=0; i<127; i++) history[i] = history[i+1];
    history[127] = currentPrice;

    if (digitalRead(btns[0]) == LOW && cash > 0) { 
       shares = cash / currentPrice;
       cash = 0;
       digitalWrite(buzzer, HIGH); delay(30); digitalWrite(buzzer, LOW);
       while(digitalRead(btns[0]) == LOW); 
    }
    if (digitalRead(btns[2]) == LOW && shares > 0) { 
       cash = shares * currentPrice;
       shares = 0;
       digitalWrite(buzzer, HIGH); delay(30); digitalWrite(buzzer, LOW);
       while(digitalRead(btns[2]) == LOW);
    }

    display.clearDisplay();
    
    for(int i=0; i<127; i++) {
       display.drawLine(i, 64 - history[i], i+1, 64 - history[i+1], WHITE);
    }
    
    display.fillRect(0, 0, 128, 16, BLACK); 
    display.setTextSize(1); display.setCursor(0, 0);
    
    display.print("XEQT: $"); display.print((int)currentPrice);
    display.setCursor(95, 0); display.print(timeLeft); display.print("s");
    
    display.setCursor(0, 8);
    if (shares > 0) {
      display.print("HOLD (Val: $"); display.print((int)(shares * currentPrice)); display.print(")");
    } else {
      display.print("CASH: $"); display.print((int)cash);
    }
    
    display.display();
    delay(50); 
  }
  
  if (shares > 0) cash = shares * currentPrice;
  int finalCash = (int)cash;

  bool newRecord = false;
  if (finalCash > best) {
    best = finalCash;
    preferences.putInt("trade", best);
    newRecord = true;
  }
  preferences.end();
  
  display.clearDisplay();
  display.setTextSize(2); 
  if (newRecord) {
    display.setCursor(5, 10); display.println("NEW RECORD");
  } else {
    display.setCursor(10, 10); display.println("TIME'S UP!");
  }
  display.setTextSize(1); 
  display.setCursor(0, 35); display.print("Bal:  $"); display.println(finalCash);
  display.setCursor(0, 48); display.print("Best: $"); display.println(best);
  display.display();
  
  if(newRecord) { LEDMultiflash(); }
  else { digitalWrite(buzzer, HIGH); delay(2000); digitalWrite(buzzer, LOW); }
  delay(4000);
}

// --- GAME 9: DINO RUN ---
void playDinoRun() {
  preferences.begin("scores", false);
  int best = preferences.getInt("dino", 0);

  countdown();
  bool isAlive = true;
  float dinoY = 44; 
  float velocity = 0;
  float gravity = 1.0;
  bool isDucking = false;
  
  float obsX = 128;
  int obsType = 0; 
  float gameSpeed = 3.5; 
  int score = 0;
  
  while(isAlive) {
    isDucking = false;
    if (digitalRead(btns[1]) == LOW) { 
       isDucking = true;
       if (dinoY < 44) velocity += 2.0; 
    }
    if (digitalRead(btns[0]) == LOW && dinoY >= 44 && !isDucking) { 
       velocity = -7.5; 
       digitalWrite(buzzer, HIGH); delay(20); digitalWrite(buzzer, LOW);
    }
    
    velocity += gravity;
    dinoY += velocity;
    if (dinoY > 44) { 
       dinoY = 44;
       velocity = 0;
    }
    
    obsX -= gameSpeed;
    if (obsX < -15) {
       obsX = 128;
       obsType = random(0, 2); 
       score++;
       gameSpeed += 0.4; 
    }
    
    int dX = 10;
    int dY = (int)dinoY;
    int dW = isDucking ? 20 : 16; 
    int dH = isDucking ? 8 : 16;  
    if (isDucking) dY += 8; 
    
    int oX = (int)obsX;
    int oY = (obsType == 0) ? 44 : 38; 
    int oW = (obsType == 0) ? 8 : 12;
    int oH = (obsType == 0) ? 16 : 8;
    
    if (dX < oX + oW && dX + dW > oX && dY < oY + oH && dY + dH > oY) {
       isAlive = false; 
    }
    
    display.clearDisplay();
    display.drawLine(0, 62, 128, 62, WHITE); 
    
    if (isDucking) {
      display.fillRect(dX, dY, 14, 8, WHITE); 
      display.fillRect(dX+14, dY+2, 6, 6, WHITE); 
      display.drawLine(dX-2, dY+4, dX, dY+6, WHITE); 
      display.drawLine(dX+4, dY+8, dX+4, dY+10, WHITE); 
      display.drawLine(dX+10, dY+8, dX+10, dY+10, WHITE); 
    } else {
      display.fillRect(dX+2, dY+6, 10, 10, WHITE); 
      display.fillRect(dX+8, dY, 8, 8, WHITE); 
      display.drawLine(dX, dY+10, dX+2, dY+14, WHITE); 
      if ((millis() / 100) % 2 == 0) {
        display.drawLine(dX+4, dY+16, dX+4, dY+18, WHITE); 
        display.drawLine(dX+10, dY+14, dX+12, dY+14, WHITE); 
      } else {
        display.drawLine(dX+4, dY+14, dX+6, dY+14, WHITE); 
        display.drawLine(dX+10, dY+16, dX+10, dY+18, WHITE); 
      }
    }

    if (obsType == 0) {
      display.fillRect(oX+2, oY, 4, 16, WHITE); 
      display.fillRect(oX, oY+4, 2, 6, WHITE);  
      display.fillRect(oX+6, oY+2, 2, 6, WHITE); 
    } else {
      display.fillRect(oX, oY+2, 10, 4, WHITE); 
      display.fillRect(oX-2, oY+3, 2, 2, WHITE); 
      if ((millis() / 150) % 2 == 0) {
         display.drawLine(oX+4, oY+2, oX+8, oY-2, WHITE); 
      } else {
         display.drawLine(oX+4, oY+6, oX+8, oY+10, WHITE); 
      }
    }
    
    display.setTextSize(1); display.setCursor(0, 0);
    display.print("Sc: "); display.print(score);
    
    display.display();
    delay(30); 
  }
  
  bool newRecord = false;
  if (score > best) {
    best = score;
    preferences.putInt("dino", best);
    newRecord = true;
  }
  preferences.end();

  display.clearDisplay(); display.setTextSize(2); 
  if (newRecord) {
    display.setCursor(5, 10); display.println("NEW RECORD");
  } else {
    display.setCursor(10, 10); display.println("CRASHED");
  }
  display.setTextSize(1); 
  display.setCursor(0, 35); display.print("Score: "); display.println(score);
  display.setCursor(0, 48); display.print("Best:  "); display.println(best);
  display.display();
  
  digitalWrite(buzzer, HIGH); LEDMultiflash(); digitalWrite(buzzer, LOW);
  delay(3000);
}

// --- GAME 10: WHACK-A-MOLE ---
void playWhackAMole() {
  preferences.begin("scores", false);
  int best = preferences.getInt("whack", 0);

  countdown();
  int score = 0;
  int lives = 3;
  unsigned long popupTime = 1500; 
  
  while (lives > 0) {
    int target = random(0, 4);
    digitalWrite(leds[target], HIGH);
    
    display.clearDisplay();
    display.setTextSize(2); display.setCursor(0,0); 
    display.print("Sc: "); display.print(score);
    display.setTextSize(1); display.setCursor(80,0); 
    display.print("Lvs: "); display.print(lives);
    
    display.setTextSize(2);
    display.setCursor(30, 35); display.println("WHACK!");
    display.display();
    
    unsigned long startTime = millis();
    int pressed = -1;
    
    while (millis() - startTime < popupTime) {
       for (int i=0; i<4; i++) {
          if (digitalRead(btns[i]) == LOW) {
             pressed = i;
             delay(50); // debounce
             while(digitalRead(btns[i]) == LOW);
             break;
          }
       }
       if (pressed != -1) break;
    }
    
    digitalWrite(leds[target], LOW);
    
    if (pressed == target) {
       score++;
       digitalWrite(buzzer, HIGH); delay(50); digitalWrite(buzzer, LOW);
       popupTime = max(300UL, popupTime - 50); // Gets faster, min 300ms
    } else {
       lives--;
       digitalWrite(buzzer, HIGH); delay(300); digitalWrite(buzzer, LOW);
    }
    
    delay(random(200, 800)); // Random delay before the next mole pops up
  }
  
  bool newRecord = false;
  if (score > best) {
    best = score;
    preferences.putInt("whack", best);
    newRecord = true;
  }
  preferences.end();

  display.clearDisplay(); display.setTextSize(2); 
  if (newRecord) {
    display.setCursor(5, 10); display.println("NEW RECORD");
  } else {
    display.setCursor(10, 10); display.println("GAME OVER");
  }
  display.setTextSize(1); 
  display.setCursor(0, 35); display.print("Score: "); display.println(score);
  display.setCursor(0, 48); display.print("Best:  "); display.println(best);
  display.display();
  
  digitalWrite(buzzer, HIGH); LEDMultiflash(); digitalWrite(buzzer, LOW);
  delay(3000);
}

// --- GAME 11: LUNAR LANDER ---
void playLunarLander() {
  countdown();
  
  // Ship Physics Variables
  float x = 64, y = 10;
  float vx = 0, vy = 0;
  float angle = 0;       
  float fuel = 1000.0;
  float gravity = 0.05;  
  float thrustPower = 0.0;
  
  // Landing Pad
  int padX = random(10, 90);
  int padW = 25;
  
  bool isAlive = true;
  bool landed = false;
  
  while (isAlive && !landed) {
    int joyY = analogRead(35); 
    int joyX = analogRead(34); 
    
    // 1. Analog Joystick Input
    if (joyX < 1000) angle -= 0.1; 
    if (joyX > 3000) angle += 0.1; 
    
    thrustPower = 0;
    if (joyY < 1800 && fuel > 0) {
       thrustPower = map(joyY, 1800, 0, 0, 15) / 100.0; 
       fuel -= (thrustPower * 10); 
    }
    
    // 2. The Capacitor Dump (Now with Debounce & Flash!)
    if (digitalRead(btns[1]) == LOW && fuel > 200) {
       digitalWrite(leds[1], HIGH); // Flash the Red LED to show it fired
       vy -= 1.0; 
       fuel -= 200; 
       digitalWrite(buzzer, HIGH); delay(50); digitalWrite(buzzer, LOW);
       digitalWrite(leds[1], LOW);
       
       // Stop the runaway drain by waiting for the button to be released
       while(digitalRead(btns[1]) == LOW); 
    }
    
    // 3. 2D Vector Physics Engine
    vy += gravity; 
    vx += sin(angle) * thrustPower; 
    vy -= cos(angle) * thrustPower; 
    
    x += vx;
    y += vy;
    
    if (x < 0) x = 128;
    if (x > 128) x = 0;
    
    // 4. LED Power Grid (Now Single-LED style for a cleaner look)
    for(int i=0; i<4; i++) digitalWrite(leds[i], LOW); // Clear them first
    
    if (fuel > 750) digitalWrite(leds[3], HIGH);      // Blue
    else if (fuel > 500) digitalWrite(leds[2], HIGH); // Green
    else if (fuel > 250) digitalWrite(leds[1], HIGH); // Red Warning!
    else if (fuel > 0) digitalWrite(leds[0], HIGH);   // Yellow Fumes!
    
    // 5. Collision & Landing Detection
    if (y >= 60) {
       if (x > padX && x < padX + padW) {
          if (abs(vy) < 1.0 && abs(angle) < 0.3) {
             landed = true;
          } else {
             isAlive = false; 
          }
       } else {
          isAlive = false; 
       }
    }
    
    // 6. Graphics & Rendering
    display.clearDisplay();
    display.fillRect(padX, 61, padW, 3, WHITE);
    
    int nx = x + sin(angle) * 6;
    int ny = y - cos(angle) * 6;
    int lx = x - cos(angle) * 4 - sin(angle) * 4;
    int ly = y - sin(angle) * 4 + cos(angle) * 4;
    int rx = x + cos(angle) * 4 - sin(angle) * 4;
    int ry = y + sin(angle) * 4 + cos(angle) * 4;
    
    display.drawTriangle(nx, ny, lx, ly, rx, ry, WHITE);
    
    if (thrustPower > 0) {
       int fx = x - sin(angle) * 8;
       int fy = y + cos(angle) * 8;
       display.drawLine((x+lx)/2, (y+ly)/2, fx, fy, WHITE);
    }
    
    display.setTextSize(1); display.setCursor(0,0);
    display.print("Spd: "); display.print(abs(vy));
    
    display.display();
    delay(30); 
  }
  
  // Game Over Sequence
  for(int i=0; i<4; i++) digitalWrite(leds[i], LOW); 
  
  display.clearDisplay();
  display.setTextSize(2);
  if (landed) {
     display.setCursor(20, 20); display.println("LANDED!");
     LEDMultiflash();
  } else {
     display.setCursor(15, 20); display.println("CRASHED!");
     digitalWrite(leds[1], HIGH); // Turn on the Red LED to indicate failure
     digitalWrite(buzzer, HIGH); delay(1000); digitalWrite(buzzer, LOW);
     digitalWrite(leds[1], LOW);
  }
  display.display();
  delay(3000);
}

// --- GAME 12: CLASSIC SNAKE (Anti-Suicide Patch) ---
void playSnake() {
  preferences.begin("scores", false);
  int best = preferences.getInt("snake", 3);

  countdown();
  
  int snakeX[50]; 
  int snakeY[50]; 
  int snakeLength = 3;
  
  int direction = 1;          // 0=Up, 1=Right, 2=Down, 3=Left
  int lastMovedDirection = 1; // Tracks the physical direction of the last step
  bool inputLocked = false;   // Prevents double-inputs before the next frame

  for(int i = 0; i < snakeLength; i++) {
    snakeX[i] = 10 - i;
    snakeY[i] = 8;
  }

  int foodX = random(0, 32);
  int foodY = random(0, 16);

  bool isAlive = true;
  unsigned long lastMoveTime = 0;
  int speed = 200; 

  while(isAlive) {
    int joyX = analogRead(34);
    int joyY = analogRead(35);

    // 1. The Input Lock & Deadzone Patch
    // We widened the deadzone (800 and 3200) to ignore accidental diagonal shifts.
    // We check against 'lastMovedDirection' instead of 'direction'.
    if (!inputLocked) {
       if (joyY < 800 && lastMovedDirection != 2) { direction = 0; inputLocked = true; } 
       else if (joyY > 3200 && lastMovedDirection != 0) { direction = 2; inputLocked = true; } 
       else if (joyX < 800 && lastMovedDirection != 1) { direction = 3; inputLocked = true; } 
       else if (joyX > 3200 && lastMovedDirection != 3) { direction = 1; inputLocked = true; } 
    }

    int currentFrameDelay = speed; 
    
    if (digitalRead(btns[3]) == LOW) { 
       currentFrameDelay = speed / 3;  
       digitalWrite(leds[3], HIGH);    
    } else {
       digitalWrite(leds[3], LOW);     
    }

    // 2. Frame Rate Timer
    if (millis() - lastMoveTime > currentFrameDelay) {
      lastMoveTime = millis();
      
      // Unlock the inputs so the player can turn again
      inputLocked = false;
      lastMovedDirection = direction;

      for(int i = snakeLength - 1; i > 0; i--) {
        snakeX[i] = snakeX[i-1];
        snakeY[i] = snakeY[i-1];
      }

      if (direction == 0) snakeY[0]--;
      else if (direction == 1) snakeX[0]++;
      else if (direction == 2) snakeY[0]++;
      else if (direction == 3) snakeX[0]--;

      if (snakeX[0] < 0 || snakeX[0] >= 32 || snakeY[0] < 0 || snakeY[0] >= 16) {
        isAlive = false;
      }

      for (int i = 1; i < snakeLength; i++) {
        if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i]) {
          isAlive = false;
        }
      }

      if (snakeX[0] == foodX && snakeY[0] == foodY) {
        digitalWrite(buzzer, HIGH); 
        digitalWrite(leds[random(0,4)], HIGH);
        delay(20); 
        digitalWrite(buzzer, LOW);
        for(int i=0; i<4; i++) digitalWrite(leds[i], LOW);

        if (snakeLength < 50) snakeLength++;
        
        foodX = random(0, 32);
        foodY = random(0, 16);
        
        if (speed > 40) speed -= 3; 
      }

      display.clearDisplay();
      display.fillRect(foodX * 4, foodY * 4, 4, 4, WHITE);
      
      for(int i = 0; i < snakeLength; i++) {
        display.fillRect(snakeX[i] * 4, snakeY[i] * 4, 4, 4, WHITE);
      }
      
      display.display();
    }
  }

  bool newRecord = false;
  if (snakeLength > best) {
    best = snakeLength;
    preferences.putInt("snake", best);
    newRecord = true;
  }
  preferences.end();

  display.clearDisplay(); display.setTextSize(2); 
  if (newRecord) {
    display.setCursor(5, 10); display.println("NEW RECORD");
  } else {
    display.setCursor(10, 10); display.println("GAME OVER");
  }
  display.setTextSize(1); 
  display.setCursor(0, 35); display.print("Length: "); display.println(snakeLength);
  display.setCursor(0, 48); display.print("Best:   "); display.println(best);
  display.display();
  
  digitalWrite(buzzer, HIGH); LEDMultiflash(); digitalWrite(buzzer, LOW);
  delay(3000);
}

// --- GAME 13: SHADOW MAZE (Procedural Edition) ---
void playShadowMaze() {
  countdown();

  // 1. Show Loading Screen
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(25, 25); display.println("Generating");
  display.setCursor(25, 40); display.println("Maze...");
  display.display();

  // 2. Initialize a solid block of walls
  char level[16][32];
  for(int r = 0; r < 16; r++) {
    for(int c = 0; c < 32; c++) {
      level[r][c] = '1'; 
    }
  }

  // 3. Procedural Generation: Depth-First Search Algorithm
  int stackX[150]; // Our backtrack memory stack
  int stackY[150];
  int head = 0;

  int cx = 1, cy = 1;
  level[cy][cx] = '0'; // Carve the starting cell
  stackX[head] = cx;
  stackY[head] = cy;
  head++;

  while(head > 0) {
    cx = stackX[head-1];
    cy = stackY[head-1];

    int nx[4], ny[4];
    int ncount = 0;

    // Look for walls we can punch through (checking 2 spaces away)
    if (cy >= 3 && level[cy-2][cx] == '1') { nx[ncount]=cx; ny[ncount]=cy-2; ncount++; }
    if (cy <= 12 && level[cy+2][cx] == '1') { nx[ncount]=cx; ny[ncount]=cy+2; ncount++; }
    if (cx >= 3 && level[cy][cx-2] == '1') { nx[ncount]=cx-2; ny[ncount]=cy; ncount++; }
    if (cx <= 28 && level[cy][cx+2] == '1') { nx[ncount]=cx+2; ny[ncount]=cy; ncount++; }

    if (ncount > 0) {
      // Pick a random valid direction
      int r = random(0, ncount);
      int pickX = nx[r];
      int pickY = ny[r];

      // Knock down the wall between our current spot and the new spot
      level[(cy+pickY)/2][(cx+pickX)/2] = '0';
      level[pickY][pickX] = '0'; // Carve the new spot

      // Save our progress to the stack
      stackX[head] = pickX;
      stackY[head] = pickY;
      head++;
    } else {
      // Dead end reached! Backtrack.
      head--;
    }
  }

  // 4. Set the Player Start and Exit locations
  int pX = 1, pY = 1;          
  int exitX = 29, exitY = 13; // Placed at the bottom right corner
  level[exitY][exitX] = '2';  // Mark the exit
  
  int scansLeft = 3;
  bool isAlive = true;
  bool won = false;
  unsigned long scanEndTime = 0;
  unsigned long lastMoveTime = 0;

  while (isAlive && !won) {
    int joyX = analogRead(34);
    int joyY = analogRead(35);

    // 5. Movement Logic
    if (millis() - lastMoveTime > 150) {
      int newX = pX;
      int newY = pY;

      if (joyY < 1000) newY--;
      else if (joyY > 3000) newY++;
      else if (joyX < 1000) newX--;
      else if (joyX > 3000) newX++;

      if (level[newY][newX] != '1') {
        if (newX != pX || newY != pY) { 
           pX = newX;
           pY = newY;
           lastMoveTime = millis();
           digitalWrite(buzzer, HIGH); delay(5); digitalWrite(buzzer, LOW); 
        }
      }
    }

    if (pX == exitX && pY == exitY) won = true;

    // 6. Hardware Radar
    int dist = abs(pX - exitX) + abs(pY - exitY);
    for(int i=0; i<4; i++) digitalWrite(leds[i], LOW);

    if (dist < 8) { 
      digitalWrite(leds[0], HIGH); digitalWrite(leds[1], HIGH); 
      digitalWrite(leds[2], HIGH); digitalWrite(leds[3], HIGH); 
    }
    else if (dist < 18) { digitalWrite(leds[1], HIGH); digitalWrite(leds[2], HIGH); digitalWrite(leds[3], HIGH); }
    else if (dist < 28) { digitalWrite(leds[2], HIGH); digitalWrite(leds[3], HIGH); }
    else { digitalWrite(leds[3], HIGH); }

    // 7. The Sonar Ping (Yellow Button)
    if (digitalRead(btns[0]) == LOW && scansLeft > 0 && millis() > scanEndTime) {
      scansLeft--;
      scanEndTime = millis() + 1500; 
      digitalWrite(buzzer, HIGH); delay(100); digitalWrite(buzzer, LOW);
    }

    // 8. Graphics & Rendering
    display.clearDisplay();
    bool showFullMap = (millis() < scanEndTime);

    for (int row = 0; row < 16; row++) {
      for (int col = 0; col < 32; col++) {
        if (showFullMap || (abs(col - pX) <= 2 && abs(row - pY) <= 2)) {
           if (level[row][col] == '1') {
              display.fillRect(col * 4, row * 4, 4, 4, WHITE); 
           }
           else if (level[row][col] == '2') {
              display.drawRect(col * 4, row * 4, 4, 4, WHITE); 
              display.drawPixel(col * 4 + 1, row * 4 + 1, WHITE);
           }
        }
      }
    }

    // --- CHARACTER RESIZING UPDATE ---
    // The wall grid is 4x4. We draw a 2x2 player, offset by +1 pixel to center it!
    display.fillRect((pX * 4) + 1, (pY * 4) + 1, 2, 2, WHITE);

    if (!showFullMap) {
      display.fillRect(0, 0, 50, 10, BLACK);
      display.setCursor(0, 0); display.setTextSize(1);
      display.print("Scans: "); display.print(scansLeft);
    }

    display.display();
  }

  // Game Over Screen
  for(int i=0; i<4; i++) digitalWrite(leds[i], LOW);
  display.clearDisplay();
  display.setTextSize(2); display.setCursor(15, 20);
  
  if (won) {
    display.println("ESCAPED!");
    LEDMultiflash();
  }
  display.display();
  delay(3000);
}

// --- GAME 14: ASTEROID DEFENDER ---
void playAsteroidDefender() {
  preferences.begin("scores", false);
  int best = preferences.getInt("ast", 0);

  countdown();

  // 1. Player Variables
  float pX = 64, pY = 32;       // Start in the center
  float aimX = 0, aimY = -1;    // Default aim is UP
  int score = 0;
  bool isAlive = true;
  
  // Shield Battery (Max 400)
  float shieldPower = 400.0;
  bool shieldActive = false;

  // 2. Projectile Arrays (Max 5 lasers on screen)
  float laserX[5], laserY[5], laserVX[5], laserVY[5];
  bool laserActive[5] = {false, false, false, false, false};
  unsigned long lastFireTime = 0;

  // 3. Asteroid Arrays (Max 6 asteroids)
  float astX[6], astY[6], astVX[6], astVY[6];
  bool astActive[6] = {false, false, false, false, false, false};
  int astSize[6];

  unsigned long lastFrame = millis();

  while(isAlive) {
    // Lock frame rate to ~30 FPS for smooth physics
    if (millis() - lastFrame < 30) continue; 
    lastFrame = millis();

    // --- INPUT HANDLING ---
    int joyX = analogRead(34);
    int joyY = analogRead(35);
    
    // Calculate movement vectors (-1.0 to 1.0)
    float moveX = 0, moveY = 0;
    if (joyX < 1000) moveX = -2.0;
    else if (joyX > 3000) moveX = 2.0;
    
    if (joyY < 1000) moveY = -2.0;
    else if (joyY > 3000) moveY = 2.0;

    // Update Aim Direction if moving
    if (moveX != 0 || moveY != 0) {
      aimX = moveX / 2.0; 
      aimY = moveY / 2.0;
    }

    pX += moveX;
    pY += moveY;

    // Screen Wrap for Player
    if (pX < 0) pX = 128; if (pX > 128) pX = 0;
    if (pY < 0) pY = 64;  if (pY > 64) pY = 0;

    // Shield Logic (Blue Button - btns[3])
    shieldActive = false;
    if (digitalRead(btns[1]) == LOW && shieldPower > 0) { // FIXED: btns[3] for Blue
      shieldActive = true;
      shieldPower -= 5.0; // Drain shield
    } else if (shieldPower < 400) {
      shieldPower += 1.0; // Recharge shield slowly
    }

    // Firing Logic (Red Button - btns[1])
    if (digitalRead(btns[0]) == LOW && millis() - lastFireTime > 250) { // FIXED: btns[1] for Red
      // Find an inactive laser slot
      for(int i=0; i<5; i++) {
        if (!laserActive[i]) {
          laserActive[i] = true;
          laserX[i] = pX;
          laserY[i] = pY;
          // Shoot in the direction we are aiming
          laserVX[i] = aimX * 4.0;
          laserVY[i] = aimY * 4.0;
          lastFireTime = millis();
          digitalWrite(buzzer, HIGH); delay(10); digitalWrite(buzzer, LOW);
          break;
        }
      }
    }

    // --- LED SHIELD BATTERY ---
    for(int i=0; i<4; i++) digitalWrite(leds[i], LOW);
    if (shieldPower > 300) { digitalWrite(leds[0], HIGH); digitalWrite(leds[1], HIGH); digitalWrite(leds[2], HIGH); digitalWrite(leds[3], HIGH); }
    else if (shieldPower > 200) { digitalWrite(leds[1], HIGH); digitalWrite(leds[2], HIGH); digitalWrite(leds[3], HIGH); }
    else if (shieldPower > 100) { digitalWrite(leds[2], HIGH); digitalWrite(leds[3], HIGH); }
    else if (shieldPower > 0) { digitalWrite(leds[3], HIGH); }

    // --- PHYSICS & COLLISIONS ---
    
    // 1. Move Lasers
    for(int i=0; i<5; i++) {
      if (laserActive[i]) {
        laserX[i] += laserVX[i];
        laserY[i] += laserVY[i];
        if (laserX[i] < 0 || laserX[i] > 128 || laserY[i] < 0 || laserY[i] > 64) {
          laserActive[i] = false; // Despawn off-screen lasers
        }
      }
    }

    // 2. Spawn and Move Asteroids
    for(int i=0; i<6; i++) {
      if (!astActive[i]) {
        // Randomly spawn new asteroids (2% chance per frame per slot)
        if (random(0, 100) < 2) {
          astActive[i] = true;
          astSize[i] = random(4, 8);
          
          // Spawn on edges
          if (random(0,2) == 0) { astX[i] = (random(0,2)==0) ? 0 : 128; astY[i] = random(0, 64); } 
          else { astX[i] = random(0, 128); astY[i] = (random(0,2)==0) ? 0 : 64; }
          
          // Aim roughly towards the center
          astVX[i] = (64 - astX[i]) / random(30, 80);
          astVY[i] = (32 - astY[i]) / random(30, 80);
        }
      } else {
        astX[i] += astVX[i];
        astY[i] += astVY[i];
        
        if (astX[i] < -10 || astX[i] > 138 || astY[i] < -10 || astY[i] > 74) astActive[i] = false;

        // Player Collision
        if (abs(pX - astX[i]) < astSize[i] && abs(pY - astY[i]) < astSize[i]) {
          if (shieldActive) {
            astActive[i] = false; // Shield destroys asteroid safely
            digitalWrite(leds[3], LOW); delay(20); // Visual hit
          } else {
            isAlive = false; // Unshielded hit!
          }
        }

        // Laser Collision
        for(int j=0; j<5; j++) {
          if (laserActive[j] && astActive[i]) {
            if (abs(laserX[j] - astX[i]) < astSize[i] && abs(laserY[j] - astY[i]) < astSize[i]) {
              astActive[i] = false;
              laserActive[j] = false;
              score += 10;
              break;
            }
          }
        }
      }
    }

    // --- GRAPHICS ---
    display.clearDisplay();

    // Draw Player
    if (shieldActive) {
      display.drawCircle(pX, pY, 6, WHITE); // Shield Bubble
    }
    display.fillRect(pX - 2, pY - 2, 4, 4, WHITE); // Ship core
    // Draw Aim Barrel
    display.drawLine(pX, pY, pX + (aimX * 6), pY + (aimY * 6), WHITE);

    // Draw Lasers
    for(int i=0; i<5; i++) {
      if (laserActive[i]) display.drawPixel(laserX[i], laserY[i], WHITE);
    }

    // Draw Asteroids
    for(int i=0; i<6; i++) {
      if (astActive[i]) display.drawRect(astX[i] - astSize[i]/2, astY[i] - astSize[i]/2, astSize[i], astSize[i], WHITE);
    }

    // Draw Score
    display.setTextSize(1); display.setCursor(0,0); display.print(score);

    display.display();
  }

  bool newRecord = false;
  if (score > best) {
    best = score;
    preferences.putInt("ast", best);
    newRecord = true;
  }
  preferences.end();

  // --- GAME OVER ---
  for(int i=0; i<4; i++) digitalWrite(leds[i], LOW);
  display.clearDisplay(); display.setTextSize(2); 
  if (newRecord) {
    display.setCursor(5, 10); display.println("NEW RECORD");
  } else {
    display.setCursor(10, 10); display.println("DESTROYED");
  }
  display.setTextSize(1); 
  display.setCursor(0, 35); display.print("Score: "); display.println(score);
  display.setCursor(0, 48); display.print("Best:  "); display.println(best);
  display.display();
  
  digitalWrite(buzzer, HIGH); LEDMultiflash(); digitalWrite(buzzer, LOW);
  delay(3000);
}
// --- GAME 15: SKY DEFENDER (Balanced Patch) ---
void playSkyDefender() {
  preferences.begin("scores", false);
  int best = preferences.getInt("sky", 0);

  countdown();

  int score = 0;
  bool isAlive = true;
  
  // Crosshair variables
  float crossX = 64, crossY = 32;

  // City Health Variables
  bool cityAlive[4] = {true, true, true, true};
  int cityX[4] = {16, 48, 80, 112}; // Physical X locations

  // Friendly Missiles
  float fX[4], fY[4], fVX[4], fVY[4], fTargetX[4], fTargetY[4];
  bool fActive[4] = {false, false, false, false};
  
  // Explosions
  float expX[4], expY[4], expRadius[4];
  bool expActive[4] = {false, false, false, false};

  // Enemy Missiles
  float eX[5], eY[5], eVX[5], eVY[5];
  bool eActive[5] = {false, false, false, false, false};
  
  // PATCHED: Enemy base speed reduced by 50%
  float enemySpeed = 0.25; 

  unsigned long lastFrame = millis();
  unsigned long lastFireTime[4] = {0, 0, 0, 0};

  while(isAlive) {
    if (millis() - lastFrame < 30) continue; 
    lastFrame = millis();

    // 1. Joystick Crosshair Movement
    int joyX = analogRead(34);
    int joyY = analogRead(35);
    
    if (joyX < 1000) crossX -= 2.5; else if (joyX > 3000) crossX += 2.5;
    if (joyY < 1000) crossY -= 2.5; else if (joyY > 3000) crossY += 2.5;
    
    if (crossX < 0) crossX = 0; if (crossX > 128) crossX = 128;
    if (crossY < 0) crossY = 0; if (crossY > 50) crossY = 50; 

    // 2. Firing Interceptors (PATCHED: Now with a 1500ms Reload Penalty!)
    for (int i = 0; i < 4; i++) {
      if (digitalRead(btns[i]) == LOW && cityAlive[i] && !fActive[i] && millis() - lastFireTime[i] > 1500) {
        fActive[i] = true;
        fX[i] = cityX[i];
        fY[i] = 60; 
        fTargetX[i] = crossX;
        fTargetY[i] = crossY;
        
        float dx = fTargetX[i] - fX[i];
        float dy = fTargetY[i] - fY[i];
        float dist = sqrt((dx*dx) + (dy*dy));
        
        fVX[i] = (dx / dist) * 2.5; 
        fVY[i] = (dy / dist) * 2.5;
        
        lastFireTime[i] = millis();
        digitalWrite(buzzer, HIGH); delay(10); digitalWrite(buzzer, LOW); 
      }
    }

    // 3. Move Friendly Missiles
    for(int i = 0; i < 4; i++) {
      if (fActive[i]) {
        fX[i] += fVX[i];
        fY[i] += fVY[i];
        
        if (fY[i] <= fTargetY[i]) {
           fActive[i] = false;
           expActive[i] = true;
           expX[i] = fTargetX[i];
           expY[i] = fTargetY[i];
           expRadius[i] = 1.0; 
        }
      }
    }

    // 4. Expand Explosions
    for(int i = 0; i < 4; i++) {
      if (expActive[i]) {
        expRadius[i] += 0.5; 
        if (expRadius[i] > 12.0) expActive[i] = false; 
      }
    }

    // 5. Spawn & Move Enemy Missiles 
    for(int i = 0; i < 5; i++) {
      if (!eActive[i]) {
        if (random(0, 100) < 2 + (score / 40)) {
           int targetCity = random(0, 4);
           int attempts = 0;
           while (!cityAlive[targetCity] && attempts < 10) { targetCity = random(0, 4); attempts++; }
           
           if (cityAlive[targetCity]) {
              eActive[i] = true;
              eX[i] = random(0, 128); 
              eY[i] = 0;
              
              float dx = cityX[targetCity] - eX[i];
              float dy = 60 - eY[i];
              float dist = sqrt((dx*dx) + (dy*dy));
              
              // PATCHED: Difficulty curve stretches out further so they don't get impossibly fast
              float currentSpeed = enemySpeed + ((float)score / 600.0); 
              eVX[i] = (dx / dist) * currentSpeed;
              eVY[i] = (dy / dist) * currentSpeed;
           }
        }
      } else {
        eX[i] += eVX[i];
        eY[i] += eVY[i];
        
        if (eY[i] >= 60) {
           eActive[i] = false;
           for(int c = 0; c < 4; c++) {
              if (cityAlive[c] && abs(eX[i] - cityX[c]) < 16) {
                 cityAlive[c] = false; 
                 display.invertDisplay(true); delay(30); display.invertDisplay(false); 
                 digitalWrite(buzzer, HIGH); delay(200); digitalWrite(buzzer, LOW);
              }
           }
        }
        
        for(int j = 0; j < 4; j++) {
           if (expActive[j] && eActive[i]) {
              float dx = eX[i] - expX[j];
              float dy = eY[i] - expY[j];
              if (sqrt((dx*dx) + (dy*dy)) < expRadius[j]) {
                 eActive[i] = false; 
                 score += 5;
              }
           }
        }
      }
    }

    // 6. Update Hardware LEDs & Check Game Over
    int livingCities = 0;
    for (int i = 0; i < 4; i++) {
      if (cityAlive[i]) {
         digitalWrite(leds[i], HIGH);
         livingCities++;
      } else {
         digitalWrite(leds[i], LOW);
      }
    }
    if (livingCities == 0) isAlive = false; 

    // 7. Graphics & Rendering
    display.clearDisplay();

    display.drawLine(crossX - 4, crossY, crossX + 4, crossY, WHITE);
    display.drawLine(crossX, crossY - 4, crossX, crossY + 4, WHITE);
    display.drawPixel(crossX, crossY, BLACK);

    // Draw Cities (PATCHED: UI Indicator matches 1500ms delay)
    for (int i = 0; i < 4; i++) {
       if (cityAlive[i]) {
          display.fillRect(cityX[i] - 6, 56, 12, 8, WHITE); // Main block
          
          if (millis() - lastFireTime[i] > 1500) {
             display.fillRect(cityX[i] - 2, 52, 4, 4, WHITE); // Tower UP (Ready)
          } else {
             display.drawLine(cityX[i] - 2, 56, cityX[i] + 2, 56, WHITE); // Tower DOWN (Reloading)
          }
       } else {
          display.drawRect(cityX[i] - 6, 60, 12, 4, WHITE); // Ruins
       }
    }

    for (int i = 0; i < 4; i++) {
       if (fActive[i]) {
          display.drawLine(cityX[i], 60, fX[i], fY[i], WHITE); 
          display.drawPixel(fX[i], fY[i], WHITE);
       }
    }

    for (int i = 0; i < 4; i++) {
       if (expActive[i]) {
          if ((millis() / 50) % 2 == 0) display.drawCircle(expX[i], expY[i], expRadius[i], WHITE);
          else display.drawCircle(expX[i], expY[i], expRadius[i] - 1, WHITE);
       }
    }

    for (int i = 0; i < 5; i++) {
       if (eActive[i]) {
          display.drawLine(eX[i], eY[i], eX[i] - (eVX[i]*4), eY[i] - (eVY[i]*4), WHITE); 
          display.fillRect(eX[i]-1, eY[i]-1, 2, 2, WHITE); 
       }
    }

    display.fillRect(0, 0, 40, 10, BLACK);
    display.setTextSize(1); display.setCursor(0,0); display.print(score);

    display.display();
  }

  bool newRecord = false;
  if (score > best) {
    best = score;
    preferences.putInt("sky", best);
    newRecord = true;
  }
  preferences.end();

  for(int i=0; i<4; i++) digitalWrite(leds[i], LOW);
  display.clearDisplay(); display.setTextSize(2); 
  if (newRecord) {
    display.setCursor(5, 10); display.println("NEW RECORD");
  } else {
    display.setCursor(15, 10); display.println("DEFEATED");
  }
  display.setTextSize(1); 
  display.setCursor(0, 35); display.print("Score: "); display.println(score);
  display.setCursor(0, 48); display.print("Best:  "); display.println(best);
  display.display();
  
  digitalWrite(buzzer, HIGH); delay(1000); digitalWrite(buzzer, LOW);
  if(newRecord) LEDMultiflash();
  delay(3000);
}

// --- GAME 16: CAVE FLYER (ZERO-G EDITION) ---
void playCaveFlyer() {
  preferences.begin("scores", false);
  int bestFuel = preferences.getInt("cave", 0);

  countdown();

  float shipX = 32; // Ship is horizontally locked on the screen
  float y = 32;
  float vy = 0;
  float vx = 0;
  float angle = 1.57; // Start facing RIGHT (90 degrees / Pi/2 radians)
  float fuel = 1000.0;
  float gravity = 0.04; // Reduced gravity for a floatier feel
  
  float distance = 0;
  float targetDistance = random(500, 701); // Random track length 500-700
  
  float obsX[3];
  float obsGapY[3], obsGapH[3];
  for(int i=0; i<3; i++) {
    obsX[i] = 128 + (i * 80);
    obsGapY[i] = random(20, 44);
    obsGapH[i] = random(25, 35);
  }
  
  bool padSpawned = false;
  float padX = -100;
  float padY = random(40, 58);
  int padW = 30;
  
  bool isAlive = true;
  bool won = false;
  unsigned long lastFrame = millis();

  while(isAlive && !won) {
     if (millis() - lastFrame < 30) continue;
     lastFrame = millis();

     int joyX = analogRead(34);
     bool thrustMain = false;
     bool thrustRightAir = false; // Right thruster (pushes ship left)
     
     // 1. 360-Degree Rotation (Joystick X-Axis)
     // Pushing left/right freely rotates the nose of the ship
     if (joyX < 1000) angle -= 0.15;
     if (joyX > 3000) angle += 0.15;
     
     // 2. Main Engine (Yellow Button)
     if (digitalRead(btns[0]) == LOW && fuel > 0) {
        // Pushes the ship in whatever direction the nose is pointing
        vx += sin(angle) * 0.10; // Reduced thrust power
        vy -= cos(angle) * 0.10;
        fuel -= 1.0;
        thrustMain = true;
     }

     // 3. The Emergency Retro-Thruster (Red Button)
     // Fires a jet out the RIGHT side of the ship, pushing the ship to its local LEFT
     if (digitalRead(btns[1]) == LOW && fuel > 0) {
        vx -= cos(angle) * 0.06; // Vector math for lateral left movement
        vy -= sin(angle) * 0.06;
        fuel -= 0.8;
        thrustRightAir = true;
     }
     
     // 4. Newtonian Physics Engine
     vy += gravity; // Constant downward pull
     
     // Drag (Very light, meaning you drift for a long time)
     vx *= 0.98;
     vy *= 0.98;
     
     y += vy;
     distance += vx; 
     
     if(thrustMain || thrustRightAir) {
        digitalWrite(buzzer, HIGH); delayMicroseconds(200); digitalWrite(buzzer, LOW);
     }
     
     // 5. Floor/Ceiling Collision
     if (y < 0 || y > 64) isAlive = false;
     
     // 6. Cave Obstacle Logic
     for(int i=0; i<3; i++) {
        obsX[i] -= vx; // World moves backward based on your speed
        
        // Ship hit-box
        if (obsX[i] < shipX + 4 && obsX[i] + 12 > shipX - 4) {
           if (y - 4 < obsGapY[i] - obsGapH[i]/2 || y + 4 > obsGapY[i] + obsGapH[i]/2) {
              isAlive = false; // Smacked a rock
           }
        }
        
        if (obsX[i] < -20) {
           if (!padSpawned && distance > targetDistance) {
              padSpawned = true;
              padX = 128 + 20;
           } 
           if (!padSpawned) {
              float maxObs = 0;
              for(int j=0; j<3; j++) { if(obsX[j] > maxObs) maxObs = obsX[j]; }
              obsX[i] = maxObs + random(70, 100);
              obsGapY[i] = random(20, 44);
              obsGapH[i] = random(25, 35);
           } else {
              obsX[i] = -500; 
           }
        }
     }
     
     // 7. Landing Pad Logic
     if (padSpawned) {
        padX -= vx;
        if (shipX > padX && shipX < padX + padW) {
           if (y + 4 >= padY) {
              // Strict landing check: Must be upright AND slow!
              // (angle % 2*PI must be close to 0 to be upright, but math gets tricky if you do flips. 
              // We'll use cos(angle) > 0.8 to ensure the nose is generally pointing UP)
              if (vy < 1.0 && abs(vx) < 0.7 && cos(angle) > 0.8) {
                 won = true;
                 y = padY - 4;
                 angle = 0; // Snap upright
                 vx = 0;
              } else {
                 isAlive = false; // Landed upside down, or too fast!
              }
           }
        } else if (padX + padW < shipX && y > 40) {
           isAlive = false; // Missed the pad
        }
     }
     
     // 8. LED Fuel Gauge
     for(int i=0; i<4; i++) digitalWrite(leds[i], LOW);
     if(fuel > 750) digitalWrite(leds[3], HIGH);
     else if(fuel > 500) digitalWrite(leds[2], HIGH);
     else if(fuel > 250) digitalWrite(leds[1], HIGH);
     else if(fuel > 0) digitalWrite(leds[0], HIGH);
     
     // 9. Graphics & Rendering
     display.clearDisplay();
     
     // Calculate the ship's 3 points based on rotation angle
     int nx = shipX + sin(angle) * 6; // Nose
     int ny = y - cos(angle) * 6;
     int lx = shipX - cos(angle) * 4 - sin(angle) * 4; // Left wing
     int ly = y - sin(angle) * 4 + cos(angle) * 4;
     int rx = shipX + cos(angle) * 4 - sin(angle) * 4; // Right wing
     int ry = y + sin(angle) * 4 + cos(angle) * 4;
     
     // Draw the Ship
     display.drawTriangle(nx, ny, lx, ly, rx, ry, WHITE);

     // Draw Main Thruster (Yellow Button)
     if(thrustMain) {
        display.drawLine(
          shipX - sin(angle)*4, y + cos(angle)*4, 
          shipX - sin(angle)*10, y + cos(angle)*10, WHITE);
     }
     
     // Draw Right Lateral Thruster Gust (Red Button)
     // Draws air shooting out of the right wing to push the ship left
     if(thrustRightAir) {
        display.drawLine(rx, ry, rx + cos(angle)*6, ry + sin(angle)*6, WHITE);
        display.drawLine(rx - sin(angle)*2, ry + cos(angle)*2, rx + cos(angle)*5 - sin(angle)*2, ry + sin(angle)*5 + cos(angle)*2, WHITE);
     }
     
     // Draw Cave Walls
     for(int i=0; i<3; i++) {
        if(obsX[i] > -20) {
           display.fillRect(obsX[i], 0, 12, obsGapY[i] - obsGapH[i]/2, WHITE);
           display.fillRect(obsX[i], obsGapY[i] + obsGapH[i]/2, 12, 64, WHITE);
        }
     }
     
     // Draw Landing Pad
     if(padSpawned) display.fillRect(padX, padY, padW, 4, WHITE);
     
     // Draw HUD
     display.setTextSize(1); display.setCursor(0,0);
     display.print("Dist: "); display.print((int)distance);
     display.display();
  }

  // --- End Game Logic ---
  int finalFuel = (int)fuel;
  bool newRec = false;
  if(won && finalFuel > bestFuel) {
    bestFuel = finalFuel;
    preferences.putInt("cave", bestFuel);
    newRec = true;
  }
  preferences.end();

  for(int i=0; i<4; i++) digitalWrite(leds[i], LOW);
  display.clearDisplay(); display.setTextSize(2);
  display.setCursor(15, 10);
  if(won) display.println(newRec ? "NEW RECORD" : "LANDED!");
  else display.println("CRASHED!");
  display.setTextSize(1);
  display.setCursor(0, 40); display.print("Fuel: "); display.println(finalFuel);
  display.setCursor(0, 48); display.print("Best: "); display.println(bestFuel);
  display.display();
  
  if(won) LEDMultiflash();
  else { digitalWrite(buzzer, HIGH); delay(1000); digitalWrite(buzzer, LOW); }
  delay(3000);
}