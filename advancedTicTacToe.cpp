
#include <iostream>
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

void gameBoard(int indexOne, int indexTwo){

    

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
        
    }
}