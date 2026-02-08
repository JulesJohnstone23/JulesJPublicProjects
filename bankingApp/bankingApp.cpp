#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
#include <vector>
#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>

long long gcd(long long a, long long b) {
    return (b == 0) ? a : gcd(b, a % b);
}


long long modInverse(long long a, long long m) {
    for (long long x = 1; x < m; x++) {
        if ((a * x) % m == 1) {
            return x;
        }
    }
    return -1; 
}


bool isPrime(long long num) {
    if (num <= 1) return false;
    for (long long i = 2; i <= std::sqrt(num); i++) {
        if (num % i == 0) return false;
    }
    return true;
}


long long generatePrime() {
    long long prime;
    while (true) {
        prime = rand() % 100 + 2;  
        if (isPrime(prime)) return prime;
    }
}

void generateKeys(long long &n, long long &e, long long &d) {
    long long p = generatePrime();
    long long q = generatePrime();
    n = p * q;
    long long phi = (p - 1) * (q - 1);
    
    e = 65537;  
    d = modInverse(e, phi);
}


std::vector<long long> encrypt(const std::string &message, long long e, long long n) {
    std::vector<long long> cipher;
    for (char ch : message) {
        long long m = static_cast<long long>(ch); 
        long long c = 1;
        for (long long i = 0; i < e; i++) {
            c = (c * m) % n;
        }
        cipher.push_back(c); 
    }
    return cipher;
}


std::string decrypt(const std::vector<long long> &cipher, long long d, long long n) {
    std::string message;
    for (long long c : cipher) {
        long long m = 1;
        for (long long i = 0; i < d; i++) {
            m = (m * c) % n;
        }
        message.push_back(static_cast<char>(m)); 
    }
    return message;
}

void disableEcho() {
    termios ttystate;
    tcgetattr(STDIN_FILENO, &ttystate);
    ttystate.c_lflag &= ~ECHO;          // Disable echo
    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
}

void enableEcho() {
    termios ttystate;
    tcgetattr(STDIN_FILENO, &ttystate);
    ttystate.c_lflag |= ECHO;           // Enable echo
    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
}

int main(){


    bool running = true;

    while (running){
        std::string userInput="";
        std::cout<<"Welcome to the banking app."<<'\n'<<"Please type one of the following options"<<'\n'<<"    - Login"<<'\n'<<"    - Sign up"<<std::endl;
        getline(std::cin, userInput);

        std::transform(userInput.begin(), userInput.end(),userInput.begin(),[](unsigned char c){return std::tolower(c);});

        std::cout << userInput<<std::endl;
        bool allowed = false;
        if(userInput == "login" && !allowed){
            bool usernameFound = false;
            std::vector<long long> currentCipher;
                while(!usernameFound){
                    std::string username="";
                    std::cout << "Please enter your Username (just type X to exit)"<<std::endl;
                    std::cin>>username;
                    if(username != "X" && username!="x"){
                        
                        std::ifstream reader("passwords.txt");
                        
                        if(reader.is_open()){
                            std::string line;
                            while(std::getline(reader,line)){
                                std::string currentuserName;
                                
                                int count=0;
                                for(auto ch: line){

                                    if(!usernameFound && username[count] == ch){
                                        currentuserName = currentuserName+ch;
                                        if(username==currentuserName){
                                            usernameFound=true;
                                            break;
                                        }
                                    }

                                    else if(usernameFound){
                                        currentCipher.push_back((long long)ch);
                                    }
                                    
                                    count++;

                                }
                                if(usernameFound){
                                    break;
                                }

                            }


                        }
                        reader.close();
                    }else{abort();}
                }
                
                std::string userPassword;
                std::cout<<"Please enter your password"<<std::endl;
                std::cin>>userPassword;
                long long predictedD, predictedN;
                std::cout<<"Please enter the first number(order does not matter)"<<std::endl;
                std::cin>>predictedD;
                std::cout<<"Please enter the second number"<<std::endl;
                std::cin>>predictedN;
                std::string predictedPassword = decrypt(currentCipher, predictedD, predictedN);
                if(predictedPassword!=userPassword){
                    predictedPassword = decrypt(currentCipher, predictedN, predictedD);
                    if(predictedPassword!=userPassword){
                        userInput="login";
                    }
                    else{
                        allowed=true;
                    }
                }
                else{allowed=true;}

        }
            
        
        else if(userInput=="sign up"){
            srand(time(NULL));
            std::string username = "";
            std::cout << "\n\nWelcome to the sign up Page\nWhat is your username?"<<std::endl;
            std::cin >> username;
            std::transform(username.begin(), username.end(),username.begin(),[](unsigned char c){return std::tolower(c);});
            std::string password="";

            bool allowed = false;
            while (!allowed){
                std::cout << "Please enter a password(atleast 6 characters and atleast one special character and one number and no spaces)"<<std::endl;
                disableEcho();
                std::cin>>password;
                enableEcho();
                std::cout<<password.size()<<std::endl;
                if(password.size()>=6 &&(password.find("!")!=std::string::npos || password.find("@")!=std::string::npos || password.find("#")!=std::string::npos || password.find("$") !=std::string::npos|| password.find("%")!=std::string::npos|| password.find("^")!=std::string::npos || password.find("&") !=std::string::npos|| password.find("*") !=std::string::npos|| password.find("(") !=std::string::npos|| password.find(")") !=std::string::npos|| password.find(",") !=std::string::npos|| password.find(".")!=std::string::npos || password.find("/") !=std::string::npos|| password.find("~")!=std::string::npos ) && (password.find("1") !=std::string::npos|| password.find("2") !=std::string::npos|| password.find("3") !=std::string::npos|| password.find("4") !=std::string::npos|| password.find("5") !=std::string::npos|| password.find("6") !=std::string::npos|| password.find("7") !=std::string::npos|| password.find("8")!=std::string::npos || password.find("9")!=std::string::npos)){
                    allowed= true;
                }
                else{
                    std::cout<<"Password does not meet specifications, please try again"<<std::endl;
                }
            }
            long long n, e, d;
            generateKeys(n, e, d);
            
            std::vector<long long> cipher = encrypt(password, e, n);
            
            std::ofstream writer("passwords.txt");
            writer<<username;
            for (long long c : cipher) {
                writer<<c;
            }
            writer.close();
            std::cout<<"The following numbers are for you to remeber: "<< e<<" " << n<<std::endl;

            bool finalized = false;

            while (!finalized){
                std::cout<<"Have you written down and or stored the numbers someplace securly? (Y/N)"<<std::endl;
                std::cin >> userInput;
                std::transform(userInput.begin(), userInput.end(),userInput.begin(),[](unsigned char c){return std::tolower(c);});

                if(userInput=="yes" || userInput=="y"){
                    std::cout<<"Great! You will now go through the process of logging in"<<std::endl;
                    finalized = true;
                }   
            }
        } 

        if(allowed){

        }

    }
    return 0;

}