#include <iostream>
#include <string>
#include <unistd.h>
#include <termios.h>
#include "MAP.hpp"
using namespace std;

//function to make you not need to click enter
char getch() {
    char buf = 0;
    struct termios old = {0};
    if (tcgetattr(0, &old) < 0)
            perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
            perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
            perror ("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
            perror ("tcsetattr ~ICANON");
    return (buf);
}

//quicker way to call sleep
void SLEEP(double sec) {
    usleep(1000000 * sec);
}

//display anything one letter at a time
void letByLet(string sentence, double delay) {
    for(int i=0; i<sentence.length(); ++i) {
        SLEEP(delay);
        cout << sentence[i] << flush;
    }
    cout << endl;
}

//makes the title sequence
void titleSeq() {
    string dart[9];
    dart[0] = "__/\\\\\\\\\\\\\\\\\\\\\\\\________/\\\\\\\\\\\\\\\\\\_______/\\\\\\\\\\\\\\\\\\______/\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\_ ";
    dart[1] = "__\\/\\\\\\////////\\\\\\____/\\\\\\\\\\\\\\\\\\\\\\\\\\___/\\\\\\///////\\\\\\___\\///////\\\\\\/////__      ";
    dart[2] = "___\\/\\\\\\______\\//\\\\\\__/\\\\\\/////////\\\\\\_\\/\\\\\\_____\\/\\\\\\_________\\/\\\\\\_______     ";
    dart[3] = "____\\/\\\\\\_______\\/\\\\\\_\\/\\\\\\_______\\/\\\\\\_\\/\\\\\\\\\\\\\\\\\\\\\\/__________\\/\\\\\\_______    ";
    dart[4] = "_____\\/\\\\\\_______\\/\\\\\\_\\/\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\_\\/\\\\\\//////\\\\\\__________\\/\\\\\\_______   ";
    dart[5] = "______\\/\\\\\\_______\\/\\\\\\_\\/\\\\\\/////////\\\\\\_\\/\\\\\\____\\//\\\\\\_________\\/\\\\\\_______   ";
    dart[6] = "_______\\/\\\\\\_______/\\\\\\__\\/\\\\\\_______\\/\\\\\\_\\/\\\\\\_____\\//\\\\\\________\\/\\\\\\_______  ";
    dart[7] = "________\\/\\\\\\\\\\\\\\\\\\\\\\\\/___\\/\\\\\\_______\\/\\\\\\_\\/\\\\\\______\\//\\\\\\_______\\/\\\\\\_______ ";
    dart[8] = "_________\\////////////_____\\///________\\///__\\///________\\///________\\///________";
    for(int i=0; i<9; ++i) {
        for(int j=0; j<dart[i].length(); ++j) {
            SLEEP(0.0048);
            cout << dart[i][j] << flush;
        }
        cout << endl;
    }
    
}

//function to make it easy to press to continue
void pressToCont() {
    string press = "Press any key to continue: ";
    letByLet(press, 0.05);
    getch();
}

//hte opening welcome to dart sequence
void openingSeq() {
    letByLet("WELCOME TO...",0.05);
    SLEEP(1.0);
    titleSeq();
    SLEEP(0.8);
    pressToCont();
}

//the intro / tuorial
void intro() {
    string obj1 = "Hi, and welcome to DART!";
    string obj2 = "This is a simple game comprised of randomly generated maps and objectives.";
    string obj3 = "Use W, A, S, and D to move around to collect points (*) and eventually the goal ($).";
    string obj4 = "But watch out for baddies (&)! If you loose all your health you don't get the points!";
    string obj5 = "Have fun and good luck darting!";
    letByLet(obj1, 0.05);
    SLEEP(1);
    letByLet(obj2, 0.05);
    SLEEP(1);
    letByLet(obj3, 0.05);
    SLEEP(1);
    letByLet(obj4, 0.05);
    SLEEP(1);
    letByLet(obj5, 0.05);
    SLEEP(1);
    pressToCont();
}

//shows a bunch of endlines
void clearScreen() {
    for (int i=0; i<50; ++i) {
        cout << endl;
    }
}

//this funciton is what actually plays the game
int playGame() {
    MAP game;
    game.randMapGen();
    int goal = game.movePlayer();
    while(goal == 0) {
        goal = game.movePlayer();
    }
    if(goal == 1) {
        int roundPoints = game.getTotalPoints();
        return roundPoints;
    }
    else if(goal == 2) {
        //cout << "youre dead." << endl;
        return 0;
    }
    
}

//this function sees if user wants to play again
bool contPlay() {
    string wantTo = "Next level? (y/n): ";
    letByLet(wantTo, 0.05);
    char input = getch();
    while (input != 'Y' and input != 'y' and input != 'n' and input != 'N') {
        string invalid = "Invalid option, y/n: ";
        letByLet(invalid, 0.01);
        input = getch();
    }
    if(input == 'Y' or input == 'y') {
        return true;
    }
    else {
        return false;
    }
}

//shows the total points the player got
void showFinalPoints(int GTP) {
    clearScreen();
    string msg1 = "GRAND TOTAL POINTS: ";
    letByLet(msg1, 0.1);
    SLEEP(0.5);
    cout << GTP << flush;
    SLEEP(1);
}

//credits
void rollCredits() {
    string msg1 = "Thanks again for playing...";
    string msg2 = "Programing done by Scout Jarman";
    string msg3 = "Produced by Catacomb Productions";
    string msg4 = "Please play again!";
    letByLet(msg1, 0.07);
    titleSeq();
    SLEEP(1.5);
    letByLet(msg2, 0.07);
    SLEEP(1);
    letByLet(msg3, 0.07);
    SLEEP(1);
    letByLet(msg4, 0.07);
    SLEEP(1);
}


int main () {
    
    //opening sequence
    openingSeq();
    clearScreen();
    //intro/tutorial
    intro();
    clearScreen();
    //keeps track of the total points during session
    int grandTotalPoints = 0.0;
    //has player play game then gets the points and add them
    grandTotalPoints += playGame();
    //checks to see if user wants to keep playing
    bool keepPlaying = contPlay();
    while (keepPlaying == true) {
        grandTotalPoints += playGame();
        keepPlaying = contPlay();
    }
    //shows how many points the player got after they want to stop playing
    showFinalPoints(grandTotalPoints);
    SLEEP(1);
    //clears screen then rolls credits
    clearScreen();
    SLEEP(1);
    rollCredits();
    return 0;
}