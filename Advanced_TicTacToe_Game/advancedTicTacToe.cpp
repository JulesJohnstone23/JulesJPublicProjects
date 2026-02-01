
#include <iostream>
#include <cmath>
#include <vector>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <unistd.h>

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

int recursiveResults(std::vector<std::vector<char>> gameBoard, int currentCol, int currentRow, char symbol){


    if(gameBoard[currentCol][currentRow]=='X' && symbol=='X'){
        if(currentCol == gameBoard.size()-1 && currentRow==gameBoard.size()-1){
            return 1;

        }
        else{
            int diagonal = recursiveResults(gameBoard, currentCol+1, currentRow+1, symbol);
            return diagonal;

        }
        
    }

    else if(gameBoard[currentCol][currentRow]=='O' && symbol=='O'){

         if(currentCol == gameBoard.size()-1 && currentRow==gameBoard.size()-1){
            return 2;

        }
        else{
            int diagonal = recursiveResults(gameBoard,currentCol+1, currentRow+1,symbol);
            return diagonal;
        }

    }

    return 0;


}


int gameResults(std::vector<std::vector<char>> gameBoard){
    int result=0;
    if((gameBoard[0][0]=='X' && gameBoard[gameBoard.size()-1][gameBoard.size()-1] == 'X' ) || (gameBoard[0][0]=='O' && gameBoard[gameBoard.size()-1][gameBoard.size()-1] == 'O' )){
        result = recursiveResults(gameBoard, 0, 0, gameBoard[0][0]);

    }
    return result;  
    
}


std::vector<std::vector<char>> alteredGameBoard(int size, int num,char symbol, std::vector<std::vector<char>> gameBoard){

    num--;
    int rowNum = num%size;
        
    int colNum = ((double)(num))/size;    
    int results = gameResults(gameBoard);

    if(results==1){
        printGameBoard(gameBoard);
        std::cout<< "Congradulations player, you have won tic tac toe!"<<std::endl;
        abort();
    }

    if (results==2){
        printGameBoard(gameBoard);
        std::cout<<"Sorry you lost Tic Tac Toe"<<std::endl;
        abort();
    }

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

    int num;
    std::cout<<"Welcome to tic tac toe!"<<std::endl;
    std::cout << "How many rows and columns do you want for gameboard?"<<std::endl;
    std::cin >> num;
    char symbol;
    std::cout<<"What symbol would you like to use for gameboard? (N or n for numbers)"<<std::endl;
    std::cin>>symbol;
    std::vector<std::vector<char>> gameBoard = returnGameBoard(num,symbol);
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
        std::cout << "Pick a number (1-" << pow(num,2)<<")"<<std::endl;
        std::cin >> userChoice;
        
        gameBoard= alteredGameBoard(num,userChoice,symbol,gameBoard);
        
        int results = gameResults(gameBoard);

        if(results==1){
            printGameBoard(gameBoard);
            std::cout<< "Congradulations player, you have won tic tac toe!"<<std::endl;
            running = false;
        }

        if (results==2){
            printGameBoard(gameBoard);
            std::cout<<"Sorry you lost Tic Tac Toe"<<std::endl;
            running = false;
        }


    }
}