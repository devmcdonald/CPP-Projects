
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>

using namespace std;

void printBoard(const char board[20][20]){
    for(int x = 0; x<20; x++){
        for(int y = 0; y<20; y++)
            cout << board[x][y] << " ";
        cout << endl;
    }
}

void fillEmptyBoard(char board[20][20]){
    for(int x = 0; x<20; x++){
        for(int y = 0; y<20; y++)
            board[x][y] = ('-');
    }
}

int rng(int maxNum){
    int num = (rand()%maxNum);
    return num;
    //returns range from 0-(maxnum-1)
}

class Organism{
private:
    int numSteps;
    int row;
    int col;
public:
    Organism ();
    Organism (int newSteps, int newRow, int newCol) : numSteps(newSteps), row(newRow), col(newCol){}
    virtual void move(int direction){}
    int getSteps () const{return numSteps;}
    void increaseSteps(){numSteps++;}
    void setSteps(int newSteps){numSteps = newSteps;}
    void newPosition (int newRow, int newCol) {row = newRow; col = newCol;}
    int getRow() const {return row;}
    int getCol() const {return col;}
};

class Doodlebug : public Organism{
private:
    int lastMeal;
public:
    Doodlebug(): Organism(), lastMeal(0){}
    Doodlebug(int newSteps, int newRow, int newCol, int newMeal) : Organism(newSteps, newRow, newCol), lastMeal(newMeal){}
    virtual void move(int direction);
    Doodlebug breed(int direction);
    void resetMeal(){lastMeal = 0;}
    void incMeal() {lastMeal++;}
    int getMeal() const {return lastMeal;}
    void die(vector<Doodlebug> &vec, int index);

};

class Ant : public Organism{
public:
    Ant() : Organism(){}
    Ant(int newSteps, int newRow, int newCol) : Organism(newSteps, newRow, newCol){}
    virtual void move(int direction); //chosen direction
    Ant breed(int direction); //chosen direction
    void die(vector<Ant> &vec, int index);
};


Organism::Organism(){
    numSteps = 0;
    row = rng(20);
    col = rng(20);
}


void Ant::move(int direction){ // only call move if 100% a valid option
    //0 = N, 1 = E, 2 = S, 3 = W
    if (direction == 0){
        this->newPosition(getRow() - 1, getCol());
    }
    else if (direction == 1){
        this->newPosition(getRow(), getCol() + 1);
    }
    else if(direction == 2){
        this ->newPosition(getRow() + 1, getCol());
    }
    else{
        this->newPosition(getRow(), getCol() - 1);
    }
}

Ant Ant::breed(int direction){
    //0 = N, 1 = E, 2 = S, 3 = W
    if (direction == 0){
        return Ant(0, this->getRow() - 1, this->getCol());
    }
    else if (direction == 1){
        return Ant(0, this->getRow(), this->getCol() + 1);
    }
    else if(direction == 2){
        return Ant(0, this->getRow() + 1, this->getCol());
    }
    else{
        return Ant(0, this->getRow(), this->getCol() - 1);
    }
}

void Doodlebug::move(int direction){ // only call move if 100% a valid option
    //0 = N, 1 = E, 2 = S, 3 = W
    if (direction == 0){
        this->newPosition(getRow() - 1, getCol());
    }
    else if (direction == 1){
        this->newPosition(getRow(), getCol() + 1);
    }
    else if(direction == 2){
        this ->newPosition(getRow() + 1, getCol());
    }
    else{
        this->newPosition(getRow(), getCol() - 1);
    }
}


Doodlebug Doodlebug::breed(int direction){
    //0 = N, 1 = E, 2 = S, 3 = W
    if (direction == 0){
        return Doodlebug(0, this->getRow() - 1, this->getCol(), 0);
    }
    else if (direction == 1){
        return Doodlebug(0, this->getRow(), this->getCol() + 1, 0);
    }
    else if(direction == 2){
        return Doodlebug(0, this->getRow() + 1, this->getCol(), 0);
    }
    else{
        return Doodlebug(0, this->getRow(), this->getCol() - 1, 0);
    }
}
int numValidMoves(const char currBoard[20][20], const Doodlebug &bug){
    int moves = 0;
    if((currBoard[bug.getRow() + 1][bug.getCol()] == '-' || currBoard[bug.getRow() + 1][bug.getCol()] == 'o') && bug.getRow() + 1 < 20)
        moves++;
    if((currBoard[bug.getRow() - 1][bug.getCol()] == '-' || currBoard[bug.getRow() - 1][bug.getCol()] == 'o') && bug.getRow() - 1 >=0)
        moves++;
    if((currBoard[bug.getRow()][bug.getCol() + 1] == '-' || currBoard[bug.getRow()][bug.getCol() + 1] == 'o') && bug.getCol() + 1 < 20)
        moves++;
    if((currBoard[bug.getRow()][bug.getCol() - 1] == '-' || currBoard[bug.getRow()][bug.getCol() - 1] == 'o') && bug.getCol() - 1 >=0)
        moves++;
    
    return moves;
}

int numValidAntMoves(const char currBoard[20][20], const Ant &ant){
    int moves = 0;
    if((currBoard[ant.getRow() + 1][ant.getCol()] == '-') && ant.getRow() + 1 < 20)
        moves++;
    if((currBoard[ant.getRow() - 1][ant.getCol()] == '-') && ant.getRow() - 1 >=0)
        moves++;
    if((currBoard[ant.getRow()][ant.getCol() + 1] == '-') && ant.getCol() + 1 < 20)
        moves++;
    if((currBoard[ant.getRow()][ant.getCol() - 1] == '-') && ant.getCol() - 1 >=0)
        moves++;
    
    return moves;
}

int numValidBreeds(const char currBoard[20][20], int col, int row){
    int moves = 0;
    if(currBoard[row + 1][col] == '-' && row+1 < 20)
        moves++;
    if(currBoard[row - 1][col] == '-' && row-1 >=0)
        moves++;
    if(currBoard[row][col + 1] == '-' && col+1 <20)
        moves++;
    if(currBoard[row][col - 1] == '-' && col-1 >=0)
        moves++;
    
    return moves;
}
bool checkForAnts(const char currBoard[20][20], const Doodlebug &bug){
    if(currBoard[bug.getRow() + 1][bug.getCol()] == 'o' && bug.getRow() + 1 < 20)
        return true;
    if(currBoard[bug.getRow() - 1][bug.getCol()] == 'o' && bug.getRow() - 1 >= 0)
        return true;
    if(currBoard[bug.getRow()][bug.getCol() + 1] == 'o' && bug.getCol() + 1 < 20)
        return true;
    if(currBoard[bug.getRow()][bug.getCol() - 1] == 'o' && bug.getCol() - 1 >= 0)
        return true;

    return false;
}

bool checkDir(const char currBoard[20][20], const Doodlebug &bug, const char &searchChar, const int& dir){
    if(dir == 0){
        if(currBoard[bug.getRow() - 1][bug.getCol()] == searchChar && bug.getRow() - 1 >= 0)
            return true;
    }
    else if(dir == 2){
        if(currBoard[bug.getRow() + 1][bug.getCol()] == searchChar && bug.getRow() + 1 < 20)
            return true;
    }
    else if(dir == 1){
        if(currBoard[bug.getRow()][bug.getCol() + 1] == searchChar && bug.getCol() + 1 < 20)
            return true;
    }
    else{
        if(currBoard[bug.getRow()][bug.getCol() - 1] == searchChar && bug.getCol() - 1 >= 0)
            return true;
    }
    
    return false;
}

bool checkDir(const char currBoard[20][20], const Ant &ant, const char &searchChar, const int& dir){
    if(dir == 0){
        if(currBoard[ant.getRow() - 1][ant.getCol()] == searchChar && ant.getRow() - 1 >= 0)
            return true;
    }
    else if(dir == 2){
        if(currBoard[ant.getRow() + 1][ant.getCol()] == searchChar && ant.getRow() + 1 < 20)
            return true;
    }
    else if(dir == 1){
        if(currBoard[ant.getRow()][ant.getCol() + 1] == searchChar && ant.getCol() + 1 < 20)
            return true;
    }
    else{
        if(currBoard[ant.getRow()][ant.getCol() - 1] == searchChar && ant.getCol() - 1 >= 0)
            return true;
    }
    
    return false;
}

void Doodlebug::die(vector<Doodlebug> &vec, int index){
    int initialSize = vec.size();
    vec.push_back(vec[index]);
    for(int i = index; i<initialSize; i++){
        vec[i] = vec[i+1];
    }
    vec.resize(initialSize - 1);
}

void Ant::die(vector<Ant> &vec, int index){
    int initialSize = vec.size();
    vec.push_back(vec[index]);
    for(int i = index; i<initialSize; i++){
        vec[i] = vec[i+1];
    }
    vec.resize(initialSize - 1);
}

int main(){

    srand(time(0));

    char board[20][20];
    fillEmptyBoard(board);
    

    //vectors of current doodlebugs and ants
    vector <Doodlebug> currentBugs;
    for(int i = 0; i<5; i++){
        Doodlebug nextBug; 
        //check if alloted square is empty
        while(board[nextBug.getRow()][nextBug.getCol()] != '-'){
            nextBug.newPosition(rng(20), rng(20));
        }
        board[nextBug.getRow()][nextBug.getCol()] = 'X';
        currentBugs.push_back(nextBug);
    }

    vector <Ant> currentAnts;
    for(int i=0; i<100; i++){
        Ant nextAnt;
        while(board[nextAnt.getRow()][nextAnt.getCol()] != '-'){
            nextAnt.newPosition(rng(20), rng(20));
        }
        board[nextAnt.getRow()][nextAnt.getCol()] = 'o';
        currentAnts.push_back(nextAnt);
    }

    int turn = 0;
    string userInput;
    cout << "Welcome to Doodlebug! Here is the world at turn = " << turn << endl;
    printBoard(board);
    cout << endl;
    cout << "Number of DoodleBugs: " << currentBugs.size() << endl;
    cout << "Number of Ants: " << currentAnts.size() << endl;
    cout << "Press ENTER for next move. Press any key to end simulation." << endl;
    getline(cin, userInput);
    while(userInput == ""){
        turn++;
        cout << "World at turn = " << turn << endl;
        //Doodlebug turn
        for(int i=0; i<currentBugs.size(); i++){
            //check for ants around
            if(checkForAnts(board, currentBugs[i])){
                //At least one ant neighbor
                int dir = rng(4);
                while(!checkDir(board, currentBugs[i], 'o', dir)){
                    dir = rng(4);
                }
                board[currentBugs[i].getRow()][currentBugs[i].getCol()] = '-';
                currentBugs[i].move(dir);
                board[currentBugs[i].getRow()][currentBugs[i].getCol()] = 'X';
                currentBugs[i].resetMeal();
                currentBugs[i].increaseSteps();
                //kill ant
                for(int j=0; j<currentAnts.size(); j++){
                    if(currentAnts[j].getRow() == currentBugs[i].getRow() && currentAnts[j].getCol() == currentBugs[i].getCol()){
                        currentAnts[j].die(currentAnts, j);
                        break;
                    }
                }
            }
            //if no ants, check 4 directions for valid move
            else{
                int moves = numValidMoves(board, currentBugs[i]);
                if(moves == 0){
                    currentBugs[i].increaseSteps();
                    currentBugs[i].incMeal();
                }
                else{ //at least 1 valid move to empty space, but need to randomize
                    int dir = rng(4);
                    while(!checkDir(board, currentBugs[i], '-', dir)){
                        dir = rng(4);
                    }
                    board[currentBugs[i].getRow()][currentBugs[i].getCol()] = '-';
                    currentBugs[i].move(dir);
                    board[currentBugs[i].getRow()][currentBugs[i].getCol()] = 'X';
                    currentBugs[i].increaseSteps();
                    currentBugs[i].incMeal();
                }
                
            }
        }
        //breed if valid
        for(int i=0; i<currentBugs.size(); i++){
            if(currentBugs[i].getSteps() >= 8){
                int moves = numValidBreeds(board, currentBugs[i].getCol(), currentBugs[i].getRow());
                if(moves>0){
                    int dir = rng(4);
                    while(!checkDir(board, currentBugs[i], '-', dir)){
                        dir = rng(4);
                    } 
                    Doodlebug newBug = currentBugs[i].breed(dir);
                    currentBugs.push_back(newBug);
                    board[newBug.getRow()][newBug.getCol()] = 'X';
                } 
                currentBugs[i].setSteps(0);
            }

        }
        
        //starve if valid 
        for(int i=0; i<currentBugs.size(); i++){
            if(currentBugs[i].getMeal() >= 3){
                board[currentBugs[i].getRow()][currentBugs[i].getCol()] = '-';
                currentBugs[i].die(currentBugs, i);
            }
        }

        
        //Ant turn
        for(int i=0; i<currentAnts.size(); i++){
            //move if possible
            int moves = numValidAntMoves(board, currentAnts[i]);
                if(moves == 0){
                    currentAnts[i].increaseSteps();
                }
                else{ //at least 1 valid move to empty space, but need to randomize
                    int dir = rng(4);
                    while(!checkDir(board, currentAnts[i], '-', dir)){
                        dir = rng(4);
                    }
                    board[currentAnts[i].getRow()][currentAnts[i].getCol()] = '-';
                    currentAnts[i].move(dir);
                    board[currentAnts[i].getRow()][currentAnts[i].getCol()] = 'o';
                    currentAnts[i].increaseSteps();
                }


            //breed
            if(currentAnts[i].getSteps() >= 3){
                int potentialBreedingMoves = numValidAntMoves(board, currentAnts[i]);
                if(potentialBreedingMoves > 0){
                    int direction = rng(4);
                    while(!checkDir(board, currentAnts[i], '-', direction)){
                        direction = rng(4);
                    }
                    Ant newAnt = currentAnts[i].breed(direction);
                    currentAnts.push_back(newAnt);
                    board[newAnt.getRow()][newAnt.getCol()] = 'o';
                }
                currentAnts[i].setSteps(0);
            }
        }



        printBoard(board);
        cout << "Number of DoodleBugs: " << currentBugs.size() << endl;
        cout << "Number of Ants: " << currentAnts.size() << endl;
        cout << "Press ENTER for next move. Press any key to end simulation." << endl;
        getline(cin, userInput);
    }

    cout << "Exiting simulation, goodbye!" << endl;

    return 0;
}
