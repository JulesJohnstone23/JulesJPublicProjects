#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>

bool isOSLinux = true;

#ifdef __WIN32
    #include <windows.h>

    void clear(){
        system("cls");
    }
    void disableEcho() {
        HANDLE hConsole = GetStdHandle(STD_INPUT_HANDLE);
        DWORD mode;

        GetConsoleMode(hConsole, &mode);
        
        SetConsoleMode(hConsole, mode & ~ENABLE_ECHO_INPUT);
    }

    void enableEcho() {
        HANDLE hConsole = GetStdHandle(STD_INPUT_HANDLE);
        DWORD mode;

        GetConsoleMode(hConsole, &mode);
        
        SetConsoleMode(hConsole, mode | ENABLE_ECHO_INPUT);
    }




#else  
    #include <termios.h>
    void clear(){
        system("clear");
    }

    void disableEcho() {
        termios ttystate;
        tcgetattr(STDIN_FILENO, &ttystate);
        ttystate.c_lflag &= ~ECHO;          
        tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
    }

    void enableEcho() {
        termios ttystate;
        tcgetattr(STDIN_FILENO, &ttystate);
        ttystate.c_lflag |= ECHO;          
        tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
    }

#endif


        //if check all contacts have it so user can sort alphabetically, or sort by their balance
/*
    have program run as a simulation, as in time, eg term deposit actually compounds over time, have it so that time is either determined by computer time
    and have this saved in a txt file.

*/

enum Outputs{checkbalance = 0, deposit=1, checktermdeposit=2,maketermdeposit=3, makeapayment=4, checkallcontacts=5,addnewcontact=6};

class Bank{

    private:
        std::string username;   
        double bankBalance=100.0;
        double interestRate=0.0;
        std::vector<std::string> savedContacts={};
        std::string password;
        double termDepositBalance=0.0;

    
    public:

        Bank(std::string username = "Test", std::string password = "password"){
            this->username=username;
            this->bankBalance= bankBalance;
            this->interestRate = interestRate;
            this->savedContacts=savedContacts;
        }

        void checkBalance(){
            std::cout<<"You're balance is: $"<<bankBalance<<std::endl;
        }

        void checkTermDeposit(){

            if(termDepositBalance==0.0){
                std::cout<<"You do not have a term deposit"<<std::endl;
            }
            else{
                std::cout<<"You're current term deposit value is"<< termDepositBalance<<std::endl;
            }

        }

        


};


Outputs convert(std::string userInput){

    if(userInput=="checkbalance"){
        return checkbalance;
    }
    else if(userInput== "deposit"){
        return deposit;
    }
    else if(userInput=="checktermdeposit"){
        return checktermdeposit;
    }
    else if(userInput=="maketermdeposit"){
        return maketermdeposit;
    }
    else if(userInput=="makeapayment"){
        return makeapayment;
    }
    else if(userInput=="checkallcontacts"){
        return checkallcontacts;
    }
    else{
        return addnewcontact;
    }
    

}

void loggedIn(std::string username, std::string password){

    
    Bank user(username, password);

    bool running = true;
    while (running){
        std::string userInput="";
        std::cout<<"You are now logged in"<<'\n'<<"What would you like to do?"<<'\n'<<"    - Check Balance"<<'\n'<<"    - "<<'\n'<<"    - Deposit"<<'\n'<<"    - Check Term deposit"<<'\n'<<"    - Make term deposit"<<'\n'<<"    - Make a payment"<<'\n'<<"    - Check all contacts"<<'\n'<<"    - Add new Contact"<<std::endl;
        std::cin >> userInput;
        std::transform(userInput.begin(), userInput.end(),userInput.begin(),[](unsigned char c){return std::tolower(c);});
        userInput.erase(std::remove(userInput.begin(), userInput.end(), ' '), userInput.end());        
        Outputs userChoice = convert(userInput);

        switch(userChoice){


            case checkbalance:
                user.checkBalance();
                break;
            case deposit:
                break;
            case checktermdeposit:
                break;
            case maketermdeposit:
                break;
            case makeapayment:
                break;
            case checkallcontacts:
                break;
            case addnewcontact:
                break;
            
        }

        //srand(time(0));
        //double randomNum = (double)rand()/0.05;



    }
}


int main(){

    clear();
    bool running = true;

    while (running){
        std::string userInput="";
        std::cout<<"Welcome to the banking app."<<'\n'<<"Please type one of the following options"<<'\n'<<"    - Login"<<'\n'<<"    - Sign up"<<std::endl;
        getline(std::cin, userInput);

        std::transform(userInput.begin(), userInput.end(),userInput.begin(),[](unsigned char c){return std::tolower(c);});

        if(userInput == "login"){
            bool usernameFound = false;
            bool finished= false;
            std::string password;
            std::string username="";

                while(!usernameFound || !finished){
        
                    std::cout << "Please enter your Username (just type X to exit)"<<std::endl;
                    std::cin>>username;
                    if(username != "X" && username!="x"){
                        
                        std::ifstream reader("passwords.txt");
                        
                        if(reader.is_open()){
                            std::string line;

                            while(std::getline(reader,line)){
                                
                                std::string currentuserName="";
                                
                                int count=0;
                                for(int i =0; i<line.size();i++){

                                    char c = line[i];
                                    std::string ch;
                                    ch.push_back(c);
                                    
                                    if(i==0){count=0; currentuserName=""; password="";}

                                    if(username[count] == ch[0] && !usernameFound){
                                        currentuserName=currentuserName+ch[0];
                                        if(username==currentuserName){
                                            usernameFound=true;
                                        }
                                        count++;

                                    }
                                    else if(usernameFound){
                                        password = password + c;
                                    }

                                }

                                if(usernameFound){
                                    reader.close();
                                    finished=true;
                                    
                                }
                            

                            }
                            finished =true;

                        }
                        reader.close();
                    }else{
                        std::cout<<"Something went wrong please try again"<<std::endl;
                        abort();}
                }
                
        

                std::string userPassword;
                std::cout<<"Please enter your password"<<std::endl;
                

                disableEcho();
                std::cin>>userPassword;
                enableEcho();
                
                if(userPassword==password){

                    loggedIn(username,userPassword);


                }
                else{
                    std::cout<<"PLease try again"<<std::endl;
                    usernameFound=false;
                    finished=false;
                }
        }
        
        else if(userInput=="sign up"){
            srand(time(NULL));
            std::string username = "";
            std::cout << "\n\nWelcome to the sign up Page\nWhat is your username?"<<std::endl;
            std::cin >> username;
            std::string password="";

            bool allowed = false;
            while (!allowed){
                std::cout << "Please enter a password(atleast 6 characters and atleast one special character and one number and no spaces)"<<std::endl;
                disableEcho();
                std::cin>>password;
                enableEcho();
                if(password.size()>=6 &&(password.find("!")!=std::string::npos || password.find("@")!=std::string::npos || password.find("#")!=std::string::npos || password.find("$") !=std::string::npos|| password.find("%")!=std::string::npos|| password.find("^")!=std::string::npos || password.find("&") !=std::string::npos|| password.find("*") !=std::string::npos|| password.find("(") !=std::string::npos|| password.find(")") !=std::string::npos|| password.find(",") !=std::string::npos|| password.find(".")!=std::string::npos || password.find("/") !=std::string::npos|| password.find("~")!=std::string::npos ) && (password.find("1") !=std::string::npos|| password.find("2") !=std::string::npos|| password.find("3") !=std::string::npos|| password.find("4") !=std::string::npos|| password.find("5") !=std::string::npos|| password.find("6") !=std::string::npos|| password.find("7") !=std::string::npos|| password.find("8")!=std::string::npos || password.find("9")!=std::string::npos)){
                    allowed= true;
                }
                else{
                    std::cout<<"Password does not meet specifications, please try again"<<std::endl;
                }
            }

            std::ofstream writer;
            writer.open("passwords.txt",std::ios::app);

            if(!writer){
                std::cout<<"error opening file"<<std::endl;
                return 1;
            }
            writer<<username + password + '\n';
            writer.close();

            std::cout<<"You're account has been created, you may now attempt to login"<<std::endl;


        } 

    }   


    return 0;

}