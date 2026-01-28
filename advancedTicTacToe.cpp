
#include <iostream>
#include <cmath>
#include <vector>

std::vector<std::vector<char>> returnGameBoard(int num, char symbol){
    std::vector<std::vector<char>> board;
    for(int row=0; row<num; row++){
        std::vector<char> currentRow;

        for(int col =0; col<num; col++){
            currentRow.push_back(symbol);
        }
        board.push_back(currentRow);
    }
    return board;

}

void printGameBoard(std::vector<std::vector<char>> gameBoard){
    for(int i =0; i<gameBoard.size(); i++){
        for(int j =0; j<gameBoard.size(); j++){
            std::cout<<gameBoard[i][j]<<" ";
        }
        std::cout<<"\n";
        
    }
    
}

std::vector<std::vector<char>> alteredGameBoard(int size, int num,char symbol, std::vector<std::vector<char>> gameBoard){

    num--;
    int rowNum = num%size;
        
    int colNum = ((double)(num))/size;    

    bool found = false;
    while(!found){

        if(gameBoard[colNum][rowNum] != symbol){
            int newNumber;
            std::cout<<"Please try again that spot is taken"<<std::endl;
            std::cin>> newNumber;
            rowNum = newNumber%size;
            colNum = ((double)(newNumber))/size;   
        }
        else{
            gameBoard[colNum][rowNum] = 'X';
            found=true;
        }
    }
    
    int computerGuess = (rand() % ((int)pow(size,2)));

    int computerRowNum = computerGuess%size;
    int computerColNum = ((computerGuess))/size;    
    std::cout<< computerRowNum << computerColNum;
    found = false;
    while(!found){

        if(gameBoard[computerColNum][computerRowNum] != symbol){
            computerGuess = (rand() % (int)pow(num,2));

            computerRowNum = computerGuess%size;
            computerColNum = ((computerGuess))/size;    
        }
        else{
            gameBoard[computerColNum][computerRowNum] = 'O';
            found=true;
        }
    }

    return gameBoard;
}

int main(){

    int num;
    std::cout<<"Welcome to tic tac toe!"<<std::endl;
    std::cout << "How many rows and columns do you want for gameboard?"<<std::endl;
    std::cin >> num;
    char symbol;
    std::cout<<"What symbol would you like to use for gameboard?"<<std::endl;
    std::cin>>symbol;
    std::vector<std::vector<char>> gameBoard = returnGameBoard(num,symbol);
    
    bool running = true;

    while (running){
        printGameBoard(gameBoard);
        std::cout<< "You are X, computer is O" <<std::endl;
        
        int userChoice;
        std::cout << "Pick a number (1-" << pow(num,2)<<")"<<std::endl;
        std::cin >> userChoice;
        
        gameBoard= alteredGameBoard(num,userChoice,symbol,gameBoard);

    }
}