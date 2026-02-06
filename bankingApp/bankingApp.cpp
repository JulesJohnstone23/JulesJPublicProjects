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

        if(userInput == "login"){
            std::string username="";
            std::cout << "Please enter your Username (just type X to exit)"<<std::endl;
            std::cin>>username;
            if(username != "X" && username!="x"){
                
            }
        }
        else if(userInput=="sign up"){
            srand(time(NULL));
            std::string username = "";
            std::cout << "\n\nWelcome to the sign up Page\nWhat is your username?"<<std::endl;
            std::cin >> username;
            std::string password="";

            std::cout << "Please enter a password"<<std::endl;
            disableEcho();
            std::cin>>password;
            enableEcho();

            long long n, e, d;
            generateKeys(n, e, d);

            std::cout << "Public Key (n, e): (" << n << ", " << e << ")" << std::endl;

            std::getline(std::cin, password);

            std::vector<long long> cipher = encrypt(password, e, n);
            std::cout << "Encrypted message: ";
            for (long long c : cipher) {
                std::cout << c << " ";  // Display encrypted values
            }
            std::cout << std::endl;

            std::string decryptedMessage = decrypt(cipher, d, n);
            std::cout << "Decrypted message: " << decryptedMessage << std::endl;


            running = false;

        }

        

    }
    return 0;

}