#ifndef MAP_H
#define MAP_H

class MAP {
private:
    char Map[20][50];
    const char Player = '@';
    const char Boundary = '!';
    const char Floor = '_';
    const char Wall = '|';
    const char Goal = '$';
    const char Point = '*';
    const char Baddie = '&';
    int playerPoints;
    int playerHealth;
    int PlayerX;
    int PlayerY;
    int GoalX;
    int GoalY;
    int numBaddies = 3;
public:
    MAP(); //default constructor just generates a simple boarder and floor map
    ~MAP();
    void genMap(); //makes a simple map
    void randMapGen(); // makes a randomized map
    void showMap(); // shows map with space before
    int movePlayer(); //moves the player and checks for goal
    char getch(); //gets character with out enter
    void genPoints(); //rand gen points
    int getTotalPoints();//counts how many points player gets
    void genBaddies(); // makes baddies
    void genGoal(); // makes the goal
    void moveBaddies(); //randomly moves baddies
    void altBaddieMove(); //chases player
    
};
#endif