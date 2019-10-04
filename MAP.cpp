#include "MAP.hpp"
#include <iostream>
#include <cmath>
#include <unistd.h>
#include <termios.h>
using namespace std;


MAP::MAP() {
    //variables that set player start at 1,1
    PlayerX = 1;
    PlayerY = 1;
    playerPoints = 0;
    playerHealth = 3;
    //generates a simple map, just boarder and floors
    for(int i=0; i<20; ++i) {
        for(int j=0; j<50; ++j) {
            if(i == 0 or i == 19 or j == 0 or j == 49) {
                Map[i][j] = Boundary;
            }
            else {
                Map[i][j] = Floor;
            }
        }
    }
    //sets the players position at 1,1
    Map[PlayerY][PlayerX] = Player;
    Map[4][7] = Baddie;
}

char MAP::getch() {
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

void MAP::showMap() {
    //space before displaying map 
    for(int k=0; k<18; ++k) {
        cout << endl;
    }
    cout << "POINTS: " << playerPoints << endl;
    cout << "HEALTH: " << playerHealth << endl;
    //prints every part of the map
    for(int i=0; i<20; ++i) {
        for(int j=0; j<50; ++j) {
            cout << Map[i][j];
        }
        cout << endl;
    }
    
}

int MAP::movePlayer() {
    //starts by moving baddies
    moveBaddies();
    //altBaddieMove();
    //starts by showing the map so the player know where to move
    showMap();
    //variables to keep track of the players starting position
    int origPlayerX = PlayerX;
    int origPlayerY = PlayerY;
    char temp = Map[PlayerY][PlayerX];
    //sets the current position of player to floor
    Map[PlayerY][PlayerX] = Floor;
    //gets users direction and validates
    //cout << "wasd = up left down right: ";
    char playerInput = getch();
    while(playerInput != 'w' and playerInput != 'a' and playerInput != 's' and playerInput != 'd' ) {
        //cout << "Invalid: ";
        playerInput = getch();
    }
    //does the math depending on what the player inputs
    switch (playerInput) {
        case 'w': PlayerY -=1; break;
        case 's': PlayerY +=1; break;
        case 'a': PlayerX -=1; break;
        case 'd': PlayerX +=1; break;
        default:PlayerY=PlayerY; PlayerX=PlayerX; return 0;
    }
    //checks players health
    if(playerHealth == 0) {
        cout << "YOURE DEAD!!" << endl;
        return 2;
    }
    //checks to see if where the player wants to move is floor, if so then it allows the movement
    else if(Map[PlayerY][PlayerX] == Floor ) {
        Map[PlayerY][PlayerX] = Player;
        return 0;
    }
    //checks to see if the player reaches the goal, and makes the part end
    else if(Map[PlayerY][PlayerX] == Goal) {
        cout << "Congradulations you got to the goal!!" << endl;
        Map[PlayerY][PlayerX] = Player;
        return 1;
    }
    //if player gets a point to increase total
    else if(Map[PlayerY][PlayerX] == Point) {
        //cout << "You got a point GG" << endl;
        Map[PlayerY][PlayerX] = Player;
        playerPoints += 1;
        return 0;
    }
    //checks if player the player hits a baddie
    else if(Map[PlayerY][PlayerX] == Baddie) {
        playerHealth -= 1;
        PlayerX = origPlayerX;
        PlayerY = origPlayerY;
        Map[PlayerY][PlayerX] = Player;
        return 0;
        
    }
    //if player tries to move into forbiden space, returns player to previous spot
    else {
        //cout << "Thats a wall! probably..." << endl;
        PlayerX = origPlayerX;
        PlayerY = origPlayerY;
        Map[PlayerY][PlayerX] = Player;
        return 0;
    }
}


void MAP::genMap() {
    //generates a very simple map with walls
    for(int i=0; i<20; ++i) {
        for(int j=0; j<50; ++j) {
            double iRoot = sqrt(i);
            int iRootMod2 = (int)iRoot % 2;
            if(i == 0 or i == 19 or j == 0 or j == 49) {
                Map[i][j] = Boundary;
            }
            else if(i%2 == 0 and Map[i][j] != Boundary) {
                if(j>=2 and iRootMod2 == 0) {
                    Map[i][j] = Wall;
                }
                else if(j<48 and iRootMod2 != 0) {
                    Map[i][j] = Wall;
                }
                else {
                    Map[i][j] = Floor;
                }
            }
        }
    }
    //generates goal cooridnates
    srand(time(NULL));
    int goalX = rand()%50;
    int goalY = rand()%20;
    //checks if the goal will work
    while(Map[goalY][goalX] != Floor) {
        int goalX = rand()%50;
        int goalY = rand()%20;
    }
    Map[goalY][goalX] = Goal;
}

void MAP::randMapGen() {
    srand(time(NULL));
    //generates the boundary and randomly places walls
    for(int i=0; i<20; ++i) {
        for(int j=0; j<50; ++j) {
            if(j == 0 or i == 19 or i == 0 or j == 49) {
                Map[i][j] = Boundary;
            }
            else {
                if(rand()%2 == 0) {
                    Map[i][j] = Wall;
                }
                else {
                    Map[i][j] = Floor;
                }
            }
        }
    }
    int wallCounter[20];
    //a counter to count howmany walls there are in each row (y)
    for(int k=0; k<20; ++k) {
        wallCounter[k] = 0;
    }
    //counts how many walls in each row
    for(int i=0; i<20; ++i) {
        for(int j=0; j<50; ++j) {
            if(Map[i][j] == Wall) {
                wallCounter[i]+=1;
            }
        }
        //if wall amount small, will leave alone
        if(wallCounter[i] <=10) {
        }
        //will replace a certain amount of walls with floor
        else {
            for(int k=0; k<wallCounter[i]-4; ++k) {
                int randPick = rand()%50;
                while(Map[i][randPick] != Wall) {
                    randPick = rand()%50;
            }
                Map[i][randPick] = Floor;
            }
        }
    } 
    //places an acceptable goal
    genGoal();
    //sets area around player to floor
    for(int i=1; i<4; ++i) {
        for(int j=1; j<4; ++j) {
            Map[i][j] = Floor;
        }
    }
    //sets area around goal to floor
    for(int i=GoalY-3; i<GoalY+3; ++i) {
        for(int j=GoalX-3; j<GoalX+3; ++j) {
            if(Map[i][j] != Boundary) {
                Map[i][j] = Floor;
            }
        }
    }
    //generates places for points
    genPoints();
    //generates baddies
    genBaddies();
    //sets player in starting position
    Map[1][1] = Player;
    //puts the goal back to position 
}

void MAP::genPoints() {
    srand(time(NULL));
    //rand gen x and y for point
    for(int i=0; i<7; ++i) {
        int randX = rand()%50;
        int randY = rand()%20;
        while(Map[randY][randX] != Floor) {
            randX = rand()%50;
            randY = rand()%20;
        }
        Map[randY][randX] = Point;
    }
}

//generates baddies
void MAP::genBaddies() {
    srand(time(NULL));
    for(int i=0; i<numBaddies; ++i) {
        int randX = rand()%50;
        int randY = rand()%20;
        while(Map[randY][randX] != Floor) {
            randX = rand()%50;
            randY = rand()%20;
        }
        Map[randY][randX] = Baddie;
    }
}

//generates the goal
void MAP::genGoal() {
    srand(time(NULL));
    int GoalX = rand()%50;
    int GoalY = rand()%20;
    while(Map[GoalY][GoalX] != Floor) {
        GoalX = rand()%50;
        GoalY = rand()%20;
    }
    Map[GoalY][GoalX] = Goal;
}

//moves the baddies
void MAP::moveBaddies() {
    srand(time(NULL));
    for(int i=0; i<20; ++i) {
        for(int j=0; j<50; ++j) {
            if(Map[i][j] == Baddie) {
                int random = rand()%4;
                switch (random) {
                    case 0: {
                        if(Map[i+1][j] == Floor) {
                            Map[i+1][j] = Baddie;
                            Map[i][j] = Floor;
                        }
                        break;
                    }
                    case 1: {
                        if(Map[i-1][j] == Floor) {
                            Map[i-1][j] = Baddie;
                            Map[i][j] = Floor;
                        }
                        break;
                    }
                    case 2: {
                        if(Map[i][j+1] == Floor) {
                            Map[i][j+1] = Baddie;
                            Map[i][j] = Floor;
                        }
                        break;
                    }
                    case 3: {
                        if(Map[i][j-1] == Floor) {
                            Map[i][j-1] = Baddie;
                            Map[i][j] = Floor;
                        }
                        break;
                    }
                    default: {
                        Map[i][j] = Baddie;
                        break;
                    }
                }
            }
        }
    }
}

//move baddies to go towards player
void MAP::altBaddieMove() {
    int baddiesX[numBaddies];
    int baddiesY[numBaddies];
    int baddieCounter = 0;
    srand(time(NULL));
    for(int i=0; i<20; ++i) {
        for(int j=0; j<50; ++j) {
            if(Map[i][j] == Baddie) {
                int difX = PlayerX - j;
                int difY = PlayerY - i;
                int differences;
                //i+1 j+1
                if (difX > 0 and difY > 0) {
                    differences = 10;
                }
                else if(difX == 0 and difY > 0) {
                    differences = 11;
                }
                else if(difX > 0 and difY == 0) {
                    differences = 12;
                }
                //i+1 j-1
                else if(difX > 0 and difY < 0) {
                    differences = 20;
                }
                else if(difX == 0 and difY < 0) {
                    differences = 21;
                }
                else if(difX > 0 and difY == 0) {
                    differences = 22;
                }
                //i-1 j+1
                else if(difX < 0 and difY > 0) {
                    differences = 30;
                }
                else if(difX == 0 and difY > 0) {
                    differences = 31;
                }
                else if(difX < 0 and difY == 0) {
                    differences = 32;
                }
                //i-1 j-1
                else if(difX < 0 and difY < 0) {
                    differences = 40;
                }
                else if(difX == 0 and difY < 0) {
                    differences = 41;
                }
                else if(difX < 0 and difY == 0) {
                    differences = 42;
                }
                //does that logic
                switch (differences) {
                    case 10: {
                        Map[i][j] = Floor;
                        if(Map[i+1][j] == Floor) {
                            baddiesY[baddieCounter] = i+1;
                            baddiesX[baddieCounter] = j;
                            baddieCounter++;
                        }
                        else if(Map[i][j+1] == Floor) {
                            baddiesY[baddieCounter] = i;
                            baddiesX[baddieCounter] = j+1;
                            baddieCounter++;
                        }
                        else{
                            if(rand()%2 == 0) {
                                if(Map[i-1][j] == Floor) {
                                    baddiesY[baddieCounter] = i-1;
                                    baddiesX[baddieCounter] = j;
                                    baddieCounter++;
                                }
                                else if(Map[i][j-1] == Floor) {
                                    baddiesY[baddieCounter] = i;
                                    baddiesX[baddieCounter] = j-1;
                                    baddieCounter++;
                                }
                                else {
                                    baddiesY[baddieCounter] = i;
                                    baddiesX[baddieCounter] = j;
                                    baddieCounter++;
                                }
                            }
                            else {
                                if(Map[i][j-1] == Floor) {
                                    baddiesY[baddieCounter] = i;
                                    baddiesX[baddieCounter] = j-1;
                                    baddieCounter++;
                                }
                                else if(Map[i-1][j] == Floor) {
                                    baddiesY[baddieCounter] = i-1;
                                    baddiesX[baddieCounter] = j;
                                    baddieCounter++;
                                }
                                else {
                                    baddiesY[baddieCounter] = i;
                                    baddiesX[baddieCounter] = j;
                                    baddieCounter++;
                                }
                            }
                        }
                        break;
                    }
                    case 11: {
                        Map[i][j] = Floor;
                        if(Map[i][j+1] == Floor) {
                            baddiesY[baddieCounter] = i;
                            baddiesX[baddieCounter] = j+1;
                            baddieCounter++;
                        }
                        else {
                            if(rand()%2 == 0) {
                                if(Map[i+1][j] == Floor) {
                                    baddiesY[baddieCounter] = i+1;
                                    baddiesX[baddieCounter] = j;
                                    baddieCounter++;
                                }
                                else if(Map[i-1][j] == Floor) {
                                    baddiesY[baddieCounter] = i-1;
                                    baddiesX[baddieCounter] = j;
                                    baddieCounter++;
                                }
                                else if(Map[i][j-1] == Floor) {
                                    baddiesY[baddieCounter] = i;
                                    baddiesX[baddieCounter] = j-1;
                                    baddieCounter++;
                                }
                                else {
                                    baddiesY[baddieCounter] = i;
                                    baddiesX[baddieCounter] = j;
                                    baddieCounter++;
                                }
                            }
                            else {
                                if(Map[i-1][j] == Floor) {
                                    baddiesY[baddieCounter] = i-1;
                                    baddiesX[baddieCounter] = j;
                                    baddieCounter++;
                                }
                                else if(Map[i+1][j] == Floor) {
                                    baddiesY[baddieCounter] = i+1;
                                    baddiesX[baddieCounter] = j;
                                    baddieCounter++;
                                }
                                else if(Map[i][j-1] == Floor) {
                                    baddiesY[baddieCounter] = i;
                                    baddiesX[baddieCounter] = j-1;
                                    baddieCounter++;
                                }
                            }
                        }
                        break;
                    }
                    case 12: {
                        Map[i][j] = Floor;
                        if(Map[i+1][j] == Floor) {
                            baddiesY[baddieCounter] = i+1;
                            baddiesX[baddieCounter] = j;
                            baddieCounter++;
                        }
                        else {
                            if(rand()%2 == 0) {
                                if(Map[i][j+1] == Floor) {
                                    baddiesY[baddieCounter] = i;
                                    baddiesX[baddieCounter] = j+1;
                                    baddieCounter++;
                                }
                                else if(Map[i][j-1] == Floor) {
                                    baddiesY[baddieCounter] = i;
                                    baddiesX[baddieCounter] = j-1;
                                    baddieCounter++;
                                }
                                else if(Map[i-1][j] == Floor) {
                                    baddiesY[baddieCounter] = i-1;
                                    baddiesX[baddieCounter] = j;
                                    baddieCounter++;
                                }
                                else {
                                    baddiesY[baddieCounter] = i;
                                    baddiesX[baddieCounter] = j;
                                    baddieCounter++;
                                }
                            }
                            else {
                                if(Map[i][j-1] == Floor) {
                                    baddiesY[baddieCounter] = i;
                                    baddiesX[baddieCounter] = j-1;
                                    baddieCounter++;
                                }
                                else if(Map[i][j+1] == Floor) {
                                    baddiesY[baddieCounter] = i;
                                    baddiesX[baddieCounter] = j+1;
                                    baddieCounter++;
                                }
                                else if(Map[i-1][j] == Floor) {
                                    baddiesY[baddieCounter] = i-1;
                                    baddiesX[baddieCounter] = j;
                                    baddieCounter++;
                                }
                                else {
                                    baddiesY[baddieCounter] = i;
                                    baddiesX[baddieCounter] = j;
                                    baddieCounter++;
                                }
                            }
                        }
                        break;
                    }
                    case 20: {
                        Map[i][j] = Floor;
                        if(Map[i+1][j] == Floor) {
                            baddiesY[baddieCounter] = i+1;
                            baddiesX[baddieCounter] = j;
                            baddieCounter++;
                        }
                        else if(Map[i][j-1] == Floor) {
                            baddiesY[baddieCounter] = i;
                            baddiesX[baddieCounter] = j-1;
                            baddieCounter++;
                        }
                        else{
                            if(rand()%2 == 0) {
                                if(Map[i-1][j] == Floor) {
                                    baddiesY[baddieCounter] = i-1;
                                    baddiesX[baddieCounter] = j;
                                    baddieCounter++;
                                }
                                else if(Map[i][j+1] == Floor) {
                                    baddiesY[baddieCounter] = i;
                                    baddiesX[baddieCounter] = j+1;
                                    baddieCounter++;
                                }
                                else {
                                    baddiesY[baddieCounter] = i;
                                    baddiesX[baddieCounter] = j;
                                    baddieCounter++;
                                }
                            }
                            else {
                                if(Map[i][j+1] == Floor) {
                                    baddiesY[baddieCounter] = i;
                                    baddiesX[baddieCounter] = j+1;
                                    baddieCounter++;
                                }
                                else if(Map[i-1][j] == Floor) {
                                    baddiesY[baddieCounter] = i-1;
                                    baddiesX[baddieCounter] = j;
                                    baddieCounter++;
                                }
                                else {
                                    baddiesY[baddieCounter] = i;
                                    baddiesX[baddieCounter] = j;
                                    baddieCounter++;
                                }
                            }
                        }
                        break;
                    }
                    case 21: {
                        Map[i][j] = Floor;
                        if(Map[i][j-1] == Floor) {
                            baddiesY[baddieCounter] = i;
                            baddiesX[baddieCounter] = j-1;
                            baddieCounter++;
                        }
                        else {
                            if(rand()%2 == 0) {
                                if(Map[i+1][j] == Floor) {
                                    baddiesY[baddieCounter] = i+1;
                                    baddiesX[baddieCounter] = j;
                                    baddieCounter++;
                                }
                                else if(Map[i-1][j] == Floor) {
                                    baddiesY[baddieCounter] = i-1;
                                    baddiesX[baddieCounter] = j;
                                    baddieCounter++;
                                }
                                else if(Map[i][j+1] == Floor) {
                                    baddiesY[baddieCounter] = i;
                                    baddiesX[baddieCounter] = j+1;
                                    baddieCounter++;
                                }
                                else {
                                    baddiesY[baddieCounter] = i;
                                    baddiesX[baddieCounter] = j;
                                    baddieCounter++;
                                }
                            }
                            else {
                                if(Map[i-1][j] == Floor) {
                                    baddiesY[baddieCounter] = i-1;
                                    baddiesX[baddieCounter] = j;
                                    baddieCounter++;
                                }
                                else if(Map[i+1][j] == Floor) {
                                    baddiesY[baddieCounter] = i+1;
                                    baddiesX[baddieCounter] = j;
                                    baddieCounter++;
                                }
                                else if(Map[i][j+1] == Floor) {
                                    baddiesY[baddieCounter] = i;
                                    baddiesX[baddieCounter] = j+1;
                                    baddieCounter++;
                                }
                                else {
                                    baddiesY[baddieCounter] = i;
                                    baddiesX[baddieCounter] = j;
                                    baddieCounter++;
                                }
                            }
                        }
                        break;
                    }
                    case 22: {
                        Map[i][j] = Floor;
                        if(Map[i+1][j] == Floor) {
                            baddiesY[baddieCounter] = i+1;
                            baddiesX[baddieCounter] = j;
                            baddieCounter++;
                        }
                        else {
                            if(rand()%2 == 0) {
                                if(Map[i][j-1] == Floor) {
                                    baddiesY[baddieCounter] = i;
                                    baddiesX[baddieCounter] = j-1;
                                    baddieCounter++;
                                }
                                else if(Map[i][j+1] == Floor) {
                                    baddiesY[baddieCounter] = i;
                                    baddiesX[baddieCounter] = j+1;
                                    baddieCounter++;
                                }
                                else if(Map[i-1][j] == Floor) {
                                    baddiesY[baddieCounter] = i-1;
                                    baddiesX[baddieCounter] = j;
                                    baddieCounter++;
                                }
                                else {
                                    baddiesY[baddieCounter] = i;
                                    baddiesX[baddieCounter] = j;
                                    baddieCounter++;
                                }
                            }
                            else {
                                if(Map[i][j+1] == Floor) {
                                    baddiesY[baddieCounter] = i;
                                    baddiesX[baddieCounter] = j+1;
                                    baddieCounter++;
                                }
                                else if(Map[i][j-1] == Floor) {
                                    baddiesY[baddieCounter] = i;
                                    baddiesX[baddieCounter] = j-1;
                                    baddieCounter++;
                                }
                                else if(Map[i-1][j] == Floor) {
                                    baddiesY[baddieCounter] = i-1;
                                    baddiesX[baddieCounter] = j;
                                    baddieCounter++;
                                }
                                else {
                                    baddiesY[baddieCounter] = i;
                                    baddiesX[baddieCounter] = j;
                                    baddieCounter++;
                                }
                            }
                        }
                        break;
                    }
                    case 30: {
                        Map[i][j] = Floor;
                        if(Map[i-1][j] == Floor) {
                            baddiesY[baddieCounter] = i-1;
                            baddiesX[baddieCounter] = j;
                            baddieCounter++;
                        }
                        else if(Map[i][j+1] == Floor) {
                            baddiesY[baddieCounter] = i;
                            baddiesX[baddieCounter] = j+1;
                            baddieCounter++;
                        }
                        else{
                            if(rand()%2 == 0) {
                                if(Map[i+1][j] == Floor) {
                                    baddiesY[baddieCounter] = i+1;
                                    baddiesX[baddieCounter] = j;
                                    baddieCounter++;
                                }
                                else if(Map[i][j-1] == Floor) {
                                    baddiesY[baddieCounter] = i;
                                    baddiesX[baddieCounter] = j-1;
                                    baddieCounter++;
                                }
                                else {
                                    baddiesY[baddieCounter] = i;
                                    baddiesX[baddieCounter] = j;
                                    baddieCounter++;
                                }
                            }
                            else {
                                if(Map[i][j-1] == Floor) {
                                    baddiesY[baddieCounter] = i;
                                    baddiesX[baddieCounter] = j-1;
                                    baddieCounter++;
                                }
                                else if(Map[i+1][j] == Floor) {
                                    baddiesY[baddieCounter] = i+1;
                                    baddiesX[baddieCounter] = j;
                                    baddieCounter++;
                                }
                                else {
                                    baddiesY[baddieCounter] = i;
                                    baddiesX[baddieCounter] = j;
                                    baddieCounter++;
                                }
                            }
                        }
                        break;
                    }
                    case 31: {
                        Map[i][j] = Floor;
                        if(Map[i][j+1] == Floor) {
                            baddiesY[baddieCounter] = i;
                            baddiesX[baddieCounter] = j+1;
                            baddieCounter++;
                        }
                        else {
                            if(rand()%2 == 0) {
                                if(Map[i-1][j] == Floor) {
                                    baddiesY[baddieCounter] = i-1;
                                    baddiesX[baddieCounter] = j;
                                    baddieCounter++;
                                }
                                else if(Map[i+1][j] == Floor) {
                                    baddiesY[baddieCounter] = i+1;
                                    baddiesX[baddieCounter] = j;
                                    baddieCounter++;
                                }
                                else if(Map[i][j-1] == Floor) {
                                    baddiesY[baddieCounter] = i;
                                    baddiesX[baddieCounter] = j-1;
                                    baddieCounter++;
                                }
                                else {
                                    baddiesY[baddieCounter] = i;
                                    baddiesX[baddieCounter] = j;
                                    baddieCounter++;
                                }
                            }
                            else {
                                if(Map[i+1][j] == Floor) {
                                    baddiesY[baddieCounter] = i+1;
                                    baddiesX[baddieCounter] = j;
                                    baddieCounter++;
                                }
                                else if(Map[i-1][j] == Floor) {
                                    baddiesY[baddieCounter] = i-1;
                                    baddiesX[baddieCounter] = j;
                                    baddieCounter++;
                                }
                                else if(Map[i][j-1] == Floor) {
                                    baddiesY[baddieCounter] = i;
                                    baddiesX[baddieCounter] = j-1;
                                    baddieCounter++;
                                }
                                else {
                                    baddiesY[baddieCounter] = i;
                                    baddiesX[baddieCounter] = j;
                                    baddieCounter++;
                                }
                            }
                        }
                        break;
                    }
                    case 32: {
                        Map[i][j] = Floor;
                        if(Map[i-1][j] == Floor) {
                            baddiesY[baddieCounter] = i-1;
                            baddiesX[baddieCounter] = j;
                            baddieCounter++;
                        }
                        else {
                            if(rand()%2 == 0) {
                                if(Map[i][j+1] == Floor) {
                                    baddiesY[baddieCounter] = i;
                                    baddiesX[baddieCounter] = j+1;
                                    baddieCounter++;
                                }
                                else if(Map[i][j-1] == Floor) {
                                    baddiesY[baddieCounter] = i;
                                    baddiesX[baddieCounter] = j-1;
                                    baddieCounter++;
                                }
                                else if(Map[i-1][j] == Floor) {
                                    baddiesY[baddieCounter] = i-1;
                                    baddiesX[baddieCounter] = j;
                                    baddieCounter++;
                                }
                                else {
                                    baddiesY[baddieCounter] = i;
                                    baddiesX[baddieCounter] = j;
                                    baddieCounter++;
                                }
                            }
                            else {
                                if(Map[i][j-1] == Floor) {
                                    baddiesY[baddieCounter] = i;
                                    baddiesX[baddieCounter] = j-1;
                                    baddieCounter++;
                                }
                                else if(Map[i][j+1] == Floor) {
                                    baddiesY[baddieCounter] = i;
                                    baddiesX[baddieCounter] = j+1;
                                    baddieCounter++;
                                }
                                else if(Map[i-1][j] == Floor) {
                                    baddiesY[baddieCounter] = i-1;
                                    baddiesX[baddieCounter] = j;
                                    baddieCounter++;
                                }
                                else {
                                    baddiesY[baddieCounter] = i;
                                    baddiesX[baddieCounter] = j;
                                    baddieCounter++;
                                }
                            }
                        }
                        break;
                    }
                    case 40: {
                        Map[i][j] = Floor;
                        if(Map[i-1][j] == Floor) {
                            baddiesY[baddieCounter] = i-1;
                            baddiesX[baddieCounter] = j;
                            baddieCounter++;
                        }
                        else if(Map[i][j-1] == Floor) {
                            baddiesY[baddieCounter] = i;
                            baddiesX[baddieCounter] = j-1;
                            baddieCounter++;
                        }
                        else{
                            if(rand()%2 == 0) {
                                if(Map[i+1][j] == Floor) {
                                    baddiesY[baddieCounter] = i+1;
                                    baddiesX[baddieCounter] = j;
                                    baddieCounter++;
                                }
                                else if(Map[i][j+1] == Floor) {
                                    baddiesY[baddieCounter] = i;
                                    baddiesX[baddieCounter] = j+1;
                                    baddieCounter++;
                                }
                                else {
                                    baddiesY[baddieCounter] = i;
                                    baddiesX[baddieCounter] = j;
                                    baddieCounter++;
                                }
                            }
                            else {
                                if(Map[i][j+1] == Floor) {
                                    baddiesY[baddieCounter] = i;
                                    baddiesX[baddieCounter] = j+1;
                                    baddieCounter++;
                                }
                                else if(Map[i+1][j] == Floor) {
                                    baddiesY[baddieCounter] = i+1;
                                    baddiesX[baddieCounter] = j;
                                    baddieCounter++;
                                }
                                else {
                                    baddiesY[baddieCounter] = i;
                                    baddiesX[baddieCounter] = j;
                                    baddieCounter++;
                                }
                            }
                        }
                        break;
                    }
                    case 41: {
                        Map[i][j] = Floor;
                        if(Map[i][j-1] == Floor) {
                            baddiesY[baddieCounter] = i;
                            baddiesX[baddieCounter] = j-1;
                            baddieCounter++;
                        }
                        else {
                            if(rand()%2 == 0) {
                                if(Map[i-1][j] == Floor) {
                                    baddiesY[baddieCounter] = i-1;
                                    baddiesX[baddieCounter] = j;
                                    baddieCounter++;
                                }
                                else if(Map[i+1][j] == Floor) {
                                    baddiesY[baddieCounter] = i+1;
                                    baddiesX[baddieCounter] = j;
                                    baddieCounter++;
                                }
                                else if(Map[i][j+1] == Floor) {
                                    baddiesY[baddieCounter] = i;
                                    baddiesX[baddieCounter] = j+1;
                                    baddieCounter++;
                                }
                                else {
                                    baddiesY[baddieCounter] = i;
                                    baddiesX[baddieCounter] = j;
                                    baddieCounter++;
                                }
                            }
                            else {
                                if(Map[i+1][j] == Floor) {
                                    baddiesY[baddieCounter] = i+1;
                                    baddiesX[baddieCounter] = j;
                                    baddieCounter++;
                                }
                                else if(Map[i-1][j] == Floor) {
                                    baddiesY[baddieCounter] = i-1;
                                    baddiesX[baddieCounter] = j;
                                    baddieCounter++;
                                }
                                else if(Map[i][j+1] == Floor) {
                                    baddiesY[baddieCounter] = i;
                                    baddiesX[baddieCounter] = j+1;
                                    baddieCounter++;
                                }
                                else {
                                    baddiesY[baddieCounter] = i;
                                    baddiesX[baddieCounter] = j;
                                    baddieCounter++;
                                }
                            }
                        }
                        break;
                    }
                    case 42: {
                        Map[i][j] = Floor;
                        if(Map[i-1][j] == Floor) {
                            baddiesY[baddieCounter] = i-1;
                            baddiesX[baddieCounter] = j;
                            baddieCounter++;
                        }
                        else {
                            if(rand()%2 == 0) {
                                if(Map[i][j-1] == Floor) {
                                    baddiesY[baddieCounter] = i;
                                    baddiesX[baddieCounter] = j-1;
                                    baddieCounter++;
                                }
                                else if(Map[i][j+1] == Floor) {
                                    baddiesY[baddieCounter] = i;
                                    baddiesX[baddieCounter] = j+1;
                                    baddieCounter++;
                                }
                                else if(Map[i+1][j] == Floor) {
                                    baddiesY[baddieCounter] = i+1;
                                    baddiesX[baddieCounter] = j;
                                    baddieCounter++;
                                }
                                else {
                                    baddiesY[baddieCounter] = i;
                                    baddiesX[baddieCounter] = j;
                                    baddieCounter++;
                                }
                            }
                            else {
                                if(Map[i][j+1] == Floor) {
                                    baddiesY[baddieCounter] = i;
                                    baddiesX[baddieCounter] = j+1;
                                    baddieCounter++;
                                }
                                else if(Map[i][j-1] == Floor) {
                                    baddiesY[baddieCounter] = i;
                                    baddiesX[baddieCounter] = j-1;
                                    baddieCounter++;
                                }
                                else if(Map[i+1][j] == Floor) {
                                    baddiesY[baddieCounter] = i+1;
                                    baddiesX[baddieCounter] = j;
                                    baddieCounter++;
                                }
                                else {
                                    baddiesY[baddieCounter] = i;
                                    baddiesX[baddieCounter] = j;
                                    baddieCounter++;
                                }
                            }
                        }
                        break;
                    }
                    default: {
                        baddiesY[baddieCounter] = i;
                        baddiesX[baddieCounter] = j;
                        baddieCounter++;
                    }
                }
            }
        }
    }
    //puts back the baddies
    for(int k=0; k<numBaddies; ++k) {
        Map[baddiesY[k]][baddiesX[k]] = Baddie;
    }
}

//gives the total points collected
int MAP::getTotalPoints() {
    return playerPoints;
}

//destructor
MAP::~MAP() {
    cout << endl;
}

