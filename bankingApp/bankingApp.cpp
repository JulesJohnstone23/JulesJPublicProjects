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


class Bank{

    private:
        std::string username;   
        double bankBalance;
        double interestRate;
        std::vector<std::string> savedContacts;

    
    public:

        Bank(std::string username = "Test", double bankBalance=10000, double interestRate=0.0435, std::vector<std::string> savedContacts ={}){
            this->username=username;
            this->bankBalance= bankBalance;
            this->interestRate = interestRate;
            this->savedContacts=savedContacts;
        }


};




void loggedIn(){

    

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

                while(!usernameFound || !finished){
                    std::string username="";
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

                    std::cout<<"You are now logged in"<<std::endl;
                    loggedIn();


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