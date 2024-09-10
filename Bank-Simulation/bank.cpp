#include <iostream>

using namespace std;

class Money{
public:
    friend Money operator +(const Money& amount1, const Money& amount2);
    friend Money operator -(const Money& amount1, const Money& amount2);
    friend Money operator -(const Money& amount);
    friend bool operator ==(const Money& amount1, const Money& amount2);
    friend bool operator <(const Money& amount1, const Money& amount2);
    Money(long dollars, int cents);
    Money(long dollars): all_cents(dollars*100){};
    Money(): all_cents(0){};
    double get_value() const {return all_cents*0.01;};
    void set_value(double newValue);
    friend istream& operator >> (istream& ins, Money& amount);
    friend ostream& operator <<(ostream& outs, const Money& amount);
private:
    long all_cents;
};

class Check{
public:
    Check(): checkNumber(0), amount(0), wasCashed(0){};
    Check(int num, Money amt, bool cashed): checkNumber(num), amount(amt), wasCashed(cashed){};
    int getNumber ()const {return checkNumber;};
    bool getCashed ()const {return wasCashed;};
    Money getValue()const {return amount;};
    void setNumber (int newNumber){checkNumber = newNumber;};
    void setCashed (){wasCashed = true;};
    void setValue(long newValue){amount.set_value(newValue);};
    void inputCheck();
    void outputCheck();


private:
    int checkNumber;
    Money amount;
    bool wasCashed;
};

Check inputCheck(){
    int tempNumber;
    Money tempAmount;
    char cashed;
    char first_char;
    char dollarSign;
    bool tempWasCashed;

    cout << "Enter check number: ";
    cin >> tempNumber;
    if(tempNumber < 0){
        Check end;
        return end;
    }
    cout << "Enter check amount: ";
    cin >> tempAmount;

    cout << "Was the check cashed (Y/N)? ";
    cin >> cashed;

    if(cashed =='Y' || cashed == 'y')
        tempWasCashed = true;
    else if (cashed == 'N' || cashed == 'n')
        tempWasCashed = false;
    else {   
        cout << "Invalid input.";
        exit(1);
    }

    Check temp(tempNumber, tempAmount, tempWasCashed);

    return temp;
}

void Check::outputCheck(){
    cout << checkNumber << '\t';
    cout << amount << '\t';
    if(wasCashed == 0)
        cout << "No";
    else
        cout << "Yes";
}

Money::Money(long dollars, int cents){
    if(dollars * cents < 0){
        cout << "Illegal values for dollars and cents. \n";
        exit(1);
    }
    all_cents = dollars*100+cents;
}

int digit_to_int(char c){
    return (static_cast<int> (c) - static_cast<int> ('0'));
}

void Money::set_value(double newValue){
    char one_char, decimal_point, digit1, digit2;
    long dollars;
    int cents;
    bool negative;
    cin>>one_char;
    if(one_char == '-'){
        negative = true;
        cin >> one_char;
    }
    else{
        negative = false;
    }

    cin >> dollars >> decimal_point >> digit1 >> digit2;

    if(one_char != '$' || decimal_point != '.' || !isdigit(digit1) || !isdigit(digit2)){
        cout << "Error illegal form for money input\n";
        exit(1);
    }

    cents = digit_to_int(digit1) *10 + digit_to_int(digit2);

    all_cents = dollars*100 + cents;

}
Money operator +(const Money& amount1, const Money& amount2){
    Money temp;
    temp.all_cents = amount1.all_cents + amount2.all_cents;
    return temp;
}

bool operator ==(const Money& amount1, const Money& amount2){
    return (amount1.all_cents == amount2.all_cents);
}

Money operator -(const Money& amount1, const Money& amount2){
    Money temp;
    temp.all_cents = amount1.all_cents - amount2.all_cents;
    return temp;
}

Money operator -(const Money& amount){
    Money temp;
    temp.all_cents = -amount.all_cents;
    return temp;
}

istream& operator >>(istream& ins, Money& amount){
    char one_char, decimal_point, digit1, digit2;
    long dollars;
    int cents;
    bool negative;
    ins>>one_char;
    if(one_char == '-'){
        negative = true;
        ins >> one_char;
    }
    else{
        negative = false;
    }

    ins >> dollars >> decimal_point >> digit1 >> digit2;

    if(one_char != '$' || decimal_point != '.' || !isdigit(digit1) || !isdigit(digit2)){
        cout << "Error illegal form for money input\n";
        exit(1);
    }

    cents = digit_to_int(digit1) *10 + digit_to_int(digit2);

    amount.all_cents = dollars*100 + cents;
    if(negative)
        amount.all_cents = -amount.all_cents;
    return ins;
}

ostream& operator <<(ostream& outs, const Money& amount){
    long positive_cents, dollars, cents;
    positive_cents = labs(amount.all_cents);
    dollars = positive_cents/100;
    cents = positive_cents%100;

    if(amount.all_cents < 0){
        outs << "-$" << dollars << '.';
    }
    else    
        outs << "$" << dollars << '.';
    
    if(cents < 10)
        outs << '0';
    outs << cents;

    return outs;
}

void sortChecks(Check startArr[], int arrSize, Check cashedArr[], int& cashedSize, Check uncashedArr[], int& uncashedSize){
    //sort start array by check no.
    Check temp;

    for(int i=0; i<arrSize; i++){
        for(int j=0; j<arrSize-1; j++){
            if(startArr[j].getNumber() > startArr[j+1].getNumber()){
                Check temp = startArr[j];
                startArr[j] = startArr[j+1];
                startArr[j+1] = temp;
            }
        }
    }

    //sort into cashed and uncashed arrays
    cashedSize = 0, uncashedSize = 0;
    for(int i=0; i<arrSize; i++){
        if(startArr[i].getCashed()){
            cashedArr[cashedSize] = startArr[i];
            cashedSize++;
        }
        else{
            uncashedArr[uncashedSize] = startArr[i];
            uncashedSize++;
        }
    }
}

void expandCheckArray(Check *&arr, int &arrSize){
    arrSize *= 2;
    Check *temp = new Check[arrSize];
    for(int i=0; i<arrSize/2; i++){
        temp[i] = arr[i];
    }
    delete [] arr;
    arr = temp;
}


void inputDeposits(Money &deposits){

    bool toDeposit = true;
    char input;
    Money nextDeposit;
    cout << "Would you like to enter a cash deposit? (Y/N): ";
    cin >> input;
    do{
        if(input == 'Y' || input == 'y'){
            cout << "Enter amount you would like to deposit: ";
            cin >> nextDeposit;
            deposits = deposits + nextDeposit;
            cout << "Would you like to enter another deposit? ";
            cin >> input;
        }
        else if(input == 'N' || input == 'n'){
            toDeposit = false;
        }
        else{
            cout << "Invalid input";
            exit(1);
        }

    }while(toDeposit);

}


int main(){

    Money startBalance, totalDeposits;
    cout << "Please enter starting balance of the account in the form ($dollars.cents): ";
    cin >> startBalance;
    inputDeposits(totalDeposits);
    Money afterDeps = startBalance + totalDeposits;
    cout << "New balance: " << afterDeps << endl;
    cout << "----------------------" << endl;
    cout << endl;

    Check *userChecks = new Check[1];
    int checksArrSize = 1;
    char checkInput;
    cout << "Would you like to add check information? (Y/N): ";
    cin >> checkInput;
    int i=0;
    bool moreChecks = true;
    while(moreChecks){

        if(checkInput == 'y' || checkInput == 'Y'){
            if(i>=checksArrSize){
                expandCheckArray(userChecks, checksArrSize);
            }

            userChecks[i] = inputCheck();
            i++;
            cout << "Do you have more checks to input? ";
            cin >> checkInput;
        }

        else if(checkInput == 'n' || checkInput == 'N'){
            //final index = number of elements in check array
            moreChecks = false;
            break;
        }

        else{
            cout << "Invalid input";
            exit(1);
        }
    }

    cout << "----------------------" << endl;
    cout << endl;

    Check *cashedCheckArr = new Check[i];
    Check *uncashedCheckArr = new Check[i];
    int uncashedSize = 0, cashedSize = 0;
    sortChecks(userChecks, i, cashedCheckArr, cashedSize, uncashedCheckArr, uncashedSize);
    //Output checks
    cout << "Checks Entered:" << endl;
    cout << "No." << '\t' << "Amount" << '\t' << "Cashed?" << endl;
    for(int j=0; j<i; j++){
        userChecks[j].outputCheck();
        cout << endl;
    }

    cout << "----------------------" << endl;
    cout << endl;
    
    

    Money cashedTotal;
    cout << "Cashed checks:" << endl;
    cout << "No." << '\t' << "Amount" << '\t' << "Cashed?" << endl;
    for(int j=0; j<cashedSize; j++){
        cashedTotal = cashedTotal + cashedCheckArr[j].getValue();
        cashedCheckArr[j].outputCheck();
        cout << endl;
    }
    
    cout << "Total of cashed checks: " << cashedTotal << endl;
    cout << "----------------------" << endl;
    cout << endl;

    Money uncashedTotal;
    cout << "Uncashed checks:" << endl;
    cout << "No." << '\t' << "Amount" << '\t' << "Cashed?" << endl;
    for(int j=0; j<uncashedSize; j++){
        uncashedTotal = uncashedTotal + uncashedCheckArr[j].getValue();
        uncashedCheckArr[j].outputCheck();
        cout << endl;
    }
    cout << "Total of uncashed checks: " << uncashedTotal << endl;
    cout << "----------------------" << endl;
    cout << endl;

    Money afterCashedChecks = afterDeps - cashedTotal;
    Money afterAllChecks = afterCashedChecks - uncashedTotal;
    Money difference = afterCashedChecks - afterAllChecks;
    cout << "Your available balance: " << afterCashedChecks << endl;
    cout << "Your checkbook balance is: " << afterAllChecks << endl;
    cout << "The difference between your available balance and checkbook balance: " << difference << endl;
    cout << "----------------------" << endl;
    cout << "Exiting account, have a good day!" << endl << endl;
    
    return 0;
}
