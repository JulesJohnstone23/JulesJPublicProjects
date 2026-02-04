#include <iostream>
#include <cmath>
#include <vector>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <algorithm>

std::vector<std::vector<char>> gameBoard;
char symbol;
int size;

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
    int count=0;
    for(int i =0; i<gameBoard.size(); i++){
        for(int j =0; j<gameBoard.size(); j++){
            count++;
            if ((gameBoard[0][0] == 'N' || gameBoard[0][0] == 'n') && gameBoard[i][j] !='X' && gameBoard[i][j]!='O'){
                std::cout<<std::setw(2)<<count<<" ";
            }
            else{
                std::cout<<gameBoard[i][j]<<" ";
            }
        }
        std::cout<<"\n";
        
    }
    
}

void winnerAndLooser(char player){
    if (player=='u'){
        printGameBoard(gameBoard);
        std::cout<< "Congradulations player, you have won tic tac toe!"<<std::endl;
        abort();
    }
    else{
        printGameBoard(gameBoard);
        std::cout<< "Sorry you lost to the computer"<<std::endl;
        abort();
    }
}

int diagonalResults(int currentCol, int currentRow, char player){
    int result=0;

    if(currentCol<0 || currentRow<0||currentCol>=gameBoard.size()||currentRow>=gameBoard.size()){
        return result;
    }

    if(gameBoard[currentCol][currentRow]=='X' && player=='X'){
        if((currentCol == gameBoard.size()-1 && currentRow==gameBoard.size()-1) || (currentCol == 0 && currentRow==gameBoard.size()-1)){
            return 1;
        }
        
    }

    else if(gameBoard[currentCol][currentRow]=='O' && player=='O'){

        if((currentCol == gameBoard.size()-1 && currentRow==gameBoard.size()-1) || (currentCol == 0 && currentRow==gameBoard.size()-1)){
            return 2;

        }

    }
    if(gameBoard[currentCol][currentRow]!=symbol){

        for(int i =0; i<2; i++){
            if(i==0) result = diagonalResults(currentCol+1, currentRow+1, player);
            else result = diagonalResults(currentCol-1, currentRow+1, player);
               
            if (result!=0) break;
        }
    }

    return result;


}

int horizontalResults(int currentCol, int currentRow, char player){
    int result=0;

    if(currentCol<0 || currentRow<0||currentCol>=gameBoard.size()||currentRow>=gameBoard.size()){
        return result;
    }

    if(gameBoard[currentCol][currentRow]=='X' && player=='X'){
        if(currentRow==gameBoard.size()-1){
            return 1;
        }
        
    }

    else if(gameBoard[currentCol][currentRow]=='O' && player=='O'){

        if(currentRow==gameBoard.size()-1){
            return 2;

        }

    }
    if(gameBoard[currentCol][currentRow]!=symbol){
        result = horizontalResults(currentCol, currentRow+1, symbol);               
        
    }

    return result;


}

int verticalResults(int currentCol, int currentRow, char player){
    int result=0;

    if(currentCol<0 || currentRow<0||currentCol>=gameBoard.size()||currentRow>=gameBoard.size()){
        return result;
    }

    if(gameBoard[currentCol][currentRow]=='X' && player=='X'){
        if(currentCol == gameBoard.size()-1){
            return 1;
        }
        
    }

    else if(gameBoard[currentCol][currentRow]=='O' && player=='O'){

        if(currentCol == gameBoard.size()-1){
            return 2;

        }

    }
    if(gameBoard[currentCol][currentRow]!=symbol){
        result = diagonalResults(currentCol+1, currentRow, symbol);
    }

    return result;

    
}


void gameResults(){
    
    for(int i=0; i<2;i++){
        if(i==0){

            for(int j=0;j<gameBoard.size();j++){

                if(horizontalResults(j,0,'X')==1 || verticalResults(0,j,'X')==1){
                    winnerAndLooser('u');
                }
            }
            if(diagonalResults(0,0,'X')==1 || diagonalResults(gameBoard.size()-1,0,'X') == 1){
                winnerAndLooser('u');
            }
            
        }
        else{
            for(int j=0;j<gameBoard.size();j++){
                
                if(horizontalResults(j,0,'O')==1 || verticalResults(0,j,'O')==1){
                    winnerAndLooser('c');
                }
            }
            if(diagonalResults(0,0,'O')==1 || diagonalResults(gameBoard.size()-1,0,'O') == 1){
                winnerAndLooser('c');
            }
        }
        
    }

}


std::vector<std::vector<char>> alteredGameBoard(int size, int num,char symbol, std::vector<std::vector<char>> gameBoard){

    num--;
    int rowNum = num%size;
        
    int colNum = ((double)(num))/size;    
    gameResults();

    bool found = false;

    while(!found){

        if(gameBoard[colNum][rowNum] != symbol){
            printGameBoard(gameBoard);
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
    
    bool filled=true;
    for(int col=0; col<gameBoard.size(); col++){

        for(int row=0; row<gameBoard.size(); row++){
            if(gameBoard[col][row]==symbol){

                filled=false;
                break;
            }
        }
    }

    if(filled){
        printGameBoard(gameBoard);
        std::cout<<"There is no more space, the game is a draw"<<std::endl;
        abort();
    }

    gameResults();

    int upperBound = ((int)pow(gameBoard.size(),2));

    srand(time(0));
    int computerGuess = rand() % upperBound;


    int computerRowNum = (computerGuess)%gameBoard.size(); 
    int computerColNum = ((double)(computerGuess))/gameBoard.size();    

    found = false;
    while(!found){

        if(gameBoard[computerColNum][computerRowNum] != symbol || computerGuess>(int)pow(gameBoard.size(),2)){
            srand(time(0));
            computerGuess = rand() % upperBound;

            computerRowNum = computerGuess%gameBoard.size();

            computerColNum = ((double)(computerGuess))/gameBoard.size();    

        }
        else if(gameBoard[computerColNum][computerRowNum]==symbol && computerGuess<=(int)pow(gameBoard.size(),2)){
            gameBoard[computerColNum][computerRowNum] = 'O';
            found=true;
        }
    }

    return gameBoard;
}



int main(){

    std::cout<<"Welcome to tic tac toe!"<<std::endl;
    std::cout << "How many rows and columns do you want for gameboard?"<<std::endl;
    std::cin >> size;
    std::cout<<"What symbol would you like to use for gameboard? (N or n for numbers)"<<std::endl;
    std::cin>>symbol;
    gameBoard = returnGameBoard(size,symbol);
    bool running = true;

    while (running){

        bool filled=true;
        for(int col=0; col<gameBoard.size(); col++){

            for(int row=0; row<gameBoard.size(); row++){
                if(gameBoard[col][row]==symbol){

                    filled=false;
                    break;
                }
                    

            }
        }


        if(filled){
            std::cout<<"There is no more space, the game is a draw"<<std::endl;
            abort();
        }

        printGameBoard(gameBoard);
        std::cout<< "You are X, computer is O" <<std::endl;
        
        int userChoice;
        std::cout << "Pick a number (1-" << pow(size,2)<<")"<<std::endl;
        std::cin >> userChoice;
        
        gameBoard= alteredGameBoard(size,userChoice,symbol,gameBoard);
        
        gameResults();

        


    }
}