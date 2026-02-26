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

enum Outputs{checkbalance = 0, deposit=1, checktermdeposit=2,maketermdeposit=3, makeapayment=4, checkallcontacts=5,addnewcontact=6};

class Bank{

    private:
        std::string username;   
        double bankBalance=100.0;
        double interestRate=0.0;
        std::vector<std::string> savedContacts={};
        std::string password;
        double termDepositBalance=0.0;
        double effectiveTermDepositBalance=0.0;
        int termDepositTime=0;
        
    
    public:

        Bank(std::string username = "Test", std::string password = "password"){
            this->username=username;
            this->bankBalance= bankBalance;
            this->interestRate = interestRate;
            this->savedContacts=savedContacts;
            if(savedContacts.size()==0){

                std::ifstream reader("savedContacts.txt");
                bool found = false;
                if(reader.is_open()){
                    std::string line;
                    while(std::getline(reader,line)){
                                
                        std::string currentContact="";
                        if(line.find(username)!=std::string::npos){
                            
                            for(auto ch: line){
                                if(ch==':'){
                                    found=true;
                                }
                                else if(found){
                                    if(ch!=','){
                                        currentContact=currentContact+ch;
                                    }
                                    else{
                                        savedContacts.push_back(currentContact);
                                        currentContact="";
                                    }
                                }
                            }
                        }
                    }


                }
                reader.close();
                if(!found){

                    std::ifstream reader("savedContacts.txt");
                    bool found = false;
                    if(reader.is_open()){
                        found=true;
                    }
                    else{
                        found=false;
                    }
                    reader.close();

                    std::ofstream writer;
                    writer.open("savedContacts.txt",std::ios::app);

                    if(!writer){
                        std::cout<<"error opening file"<<std::endl;
                    }

                    if(found){
                        writer<<'\n'<<username<<":"<<'\n';

                        
                    }
                    else{
                        writer<<username<<":"<<'\n';
                    }
                    writer.close();

                }
            }
        
            std::string year="";
            std::string month="";
            std::string day ="";
            std::string hour="";
            std::string min="";
            std::string termDepositBalanceValue="";
            std::string interestRateValue="";
            std::string termDepositTimeValue="";
            std::ifstream reader("termDeposit.txt");
            bool found = false;
            if(reader.is_open()){
                std::string line;
                int count=0;
                
                while(std::getline(reader,line)){
                                
                    std::string currentContact="";
                    if(line.find(username)!=std::string::npos){  
                        
                        for(auto ch: line){
                            if(ch==':'){
                                found=true;
                            }
                            else if(found){
                                if(ch!=','){
                                    switch(count){
                                        case 0:
                                            termDepositBalanceValue=termDepositBalanceValue+ch;
                                            break;
                                        case 1: 
                                            interestRateValue=interestRateValue+ch;
                                            break;  
                                        case 2:
                                            termDepositTimeValue=termDepositTimeValue+ch;
                                            break;
                                        case 3:
                                            year=year+ch;
                                            break;
                                        case 4:
                                            month = month+ch;
                                            break;
                                        case 5:
                                            day = day+ch;
                                            break;
                                        case 6:
                                            hour=hour+ch;
                                            break;
                                        case 7:
                                            min = min +ch;
                                            break;

                                    }

                                }
                                else{
                                    count++;
                                    
                                }
                            }
                        }
                        std::cout<<'\n';
                    }
                }
            }
            reader.close();
            if(found){
                double newInterestRate=0.0;
                int newTermDepositTime=0;
                double newTermDepositBalance=0.0;

                double expectedTermDeposit = round(stod(termDepositBalanceValue) * pow((1.0+(stod(interestRateValue)/100.0)),(stod(termDepositTimeValue)/12.0)));

            //    std::cout<<"term deposit balance: "<<termDepositBalanceValue<<" interest rate: "<<interestRateValue<<" term deposit time: "<<termDepositTimeValue<<" Expected term deposit amount at end of term: "<<expectedTermDeposit<<'\n';

                time_t timeDate;
                time(&timeDate);
                struct tm *localTime = localtime(&timeDate);
                int currentYear = localTime->tm_year+1900;
                int currentMonth = localTime->tm_mon +1;
                int currentDay = localTime->tm_mday;
                int netDifferenceMonths = ((currentYear-stod(year))*12) + (currentMonth-stod(month)) + ((currentDay-stod(day))/12);
                effectiveTermDepositBalance =((expectedTermDeposit-stod(termDepositBalanceValue)) *(netDifferenceMonths/stod(month))) + stod(termDepositBalanceValue);

                if(netDifferenceMonths>=stod(month)){
                    termDepositBalance=effectiveTermDepositBalance;
                }
                else{
                    termDepositBalance=stod(termDepositBalanceValue);
                }
            }
            

        }

        

        void checkBalance(){
            std::cout<<"You're balance is: $"<<bankBalance<<std::endl;
        }

        void checkAllContacts(){
            std::cout<<"Saved contact size: "<<savedContacts.size()<<std::endl;
            std::cout<<"The following are your contacts"<<std::endl;
            for(auto contact : savedContacts){
                std::cout<<contact<<std::endl;
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::string userInput="";
            std::cout<<"Do you wish to sort your contact list? If so please enter one of the following(N to exit)\n"<<"    - Sort Alphebetically\n"<<"    - Sort by bank balance"<<std::endl;
            std::cin>>userInput;
            std::getline(std::cin, userInput);
            userInput.erase(std::remove(userInput.begin(), userInput.end(), ' '), userInput.end());  
            std::transform(userInput.begin(), userInput.end(),userInput.begin(),[](unsigned char c){return std::tolower(c);});
       

        }

        void makeTermDeposit(){
            double userInputTermDeposit=0.0;
            bool running = true;
            while(running){
                std::cout<<"How much money from your balance would you like to dedicate towards a term deposit?"<<std::endl;
                std::cin >> userInputTermDeposit;
                if(userInputTermDeposit<=bankBalance){
                    bankBalance = bankBalance - termDepositBalance;
                    termDepositBalance=userInputTermDeposit;
                    running = false;
                }
                else{
                    std::cout<<"Please enter a valid amount OR make sure you have enough money in the bank"<<std::endl;
                }
            }
            running = true;
            std::cout<<"What interest rate would you like to have?(as a %)"<<std::endl;
            std::cin >> interestRate;
            std::cout<<"And for how long? (in months)"<<std::endl;
            std::cin>>termDepositTime;

            std::ofstream writer;
            writer.open("termDeposit.txt",std::ios::app);

            if(!writer){
                std::cout<<"error opening file"<<std::endl;
            }  

            time_t timeDate;
            time(&timeDate);
            struct tm *localTime = localtime(&timeDate);
            int year = localTime->tm_year+1900;
            int month = localTime->tm_mon +1;
            int day = localTime->tm_mday;
            int hour = localTime->tm_hour;
            int min= localTime->tm_min;

            


            writer<<username<<":"<<termDepositBalance<<","<<interestRate<<","<<termDepositTime<<","<<year<<","<<month<<","<<day<<","<<hour<<","<<min<<","<<'\n';
            writer.close();


        }

        void checkTermDeposit(){
            if(termDepositBalance==0){
                std::cout<<"You do not have a term deposit"<<std::endl;
            }
            else{
                std::cout<<"You're effective term deposit value is: " <<effectiveTermDepositBalance<<std::endl;
                std::cout<<"You're current term deposit actual value is: "<<termDepositBalance<<std::endl;
            }


        }

        
        void addNewContact(){

            std::string newContact="";
            std::cout<<"What new contact would you like to add?"<<std::endl;
            std::cin >> newContact;


            savedContacts.push_back(newContact);
            std::cout<<"Saved contact size: "<<savedContacts.size()<<std::endl;

            if(savedContacts.back()==newContact){
                std::cout<<"Contact has been succesfully added"<<std::endl;
                std::vector<std::string> savedContactContent;
                std::ifstream reader("savedContacts.txt");
                bool newAdded=false;
                if(reader.is_open()){
                    std::string line;

                    while(std::getline(reader,line)){
                            
                            
                        if(line.find(username)!=std::string::npos){
                            savedContactContent.push_back(line+newContact+",");
                            newAdded=true;
                        }
                        else{
                            savedContactContent.push_back(line);
                        }
                    }
                }
                reader.close();
                if(newAdded){
                    std::ofstream writer("savedContacts.txt");

                    if(!writer){
                        std::cout<<"error opening file"<<std::endl;
                    }
                        
                    for(auto current : savedContactContent){
                        writer<<current<<'\n';
                    }
                    writer.close();
                        
                }

            }
            
            else{std::cout<<"Contact has not been succesfully added, try again"<<std::endl;}
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
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::string userInput="";
        std::cout<<"You are logged in"<<'\n'<<"What would you like to do?"<<'\n'<<"    - Check Balance"<<'\n'<<"    - Deposit"<<'\n'<<"    - Check Term deposit"<<'\n'<<"    - Make term deposit"<<'\n'<<"    - Make a payment"<<'\n'<<"    - Check all contacts"<<'\n'<<"    - Add new Contact"<<std::endl;

        std::getline(std::cin, userInput);
        userInput.erase(std::remove(userInput.begin(), userInput.end(), ' '), userInput.end());  
        std::transform(userInput.begin(), userInput.end(),userInput.begin(),[](unsigned char c){return std::tolower(c);});
       
        Outputs userChoice = convert(userInput);

        switch(userChoice){
            case checkbalance:
                user.checkBalance();
                break;
            case deposit:
                break;
            case checktermdeposit:
                user.checkTermDeposit();
                break;
            case maketermdeposit:
                user.makeTermDeposit();
                break;
            case makeapayment:
                
                break;
            case checkallcontacts:
                user.checkAllContacts();
                break;
            case addnewcontact:
                user.addNewContact();
                break;
            
        }


        


    }
}


int main(){

    clear();
    bool running = true;
    std::string username="";
    std::string userPassword;
    while (running){
        std::string userInput="";
        std::cout<<"Welcome to the banking app."<<'\n'<<"Please type one of the following options"<<'\n'<<"    - Login"<<'\n'<<"    - Sign up"<<std::endl;
        std::getline(std::cin, userInput);

        std::transform(userInput.begin(), userInput.end(),userInput.begin(),[](unsigned char c){return std::tolower(c);});

        if(userInput == "login"){
            bool usernameFound = false;
            bool finished= false;
            std::string password;
            

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
                
        

                
                std::cout<<"Please enter your password"<<std::endl;
                

                disableEcho();
                std::cin>>userPassword;
                enableEcho();
                
                if(userPassword==password){
                    running = false;
                    break;
                    


                }
                if(userPassword!=password){
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
    loggedIn(username,userPassword); 


    return 0;

}