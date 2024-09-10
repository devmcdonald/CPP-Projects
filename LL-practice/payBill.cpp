#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

void openFile(ifstream& instream){
    string filename;
    cout << "Enter the filename: ";
    cin >> filename;
    instream.open(filename);
    while(!instream){
        cout << "Bad File Name. Please input new filename: " << endl;
        cin >> filename;
        instream.clear();
        instream.open(filename);
    }
}

class LList; //declaration

class GuestNode{
private:
    double paid;
    string fullName;
    GuestNode * next;
    GuestNode * prev;
public:
    GuestNode(): paid(0), fullName(""), next(nullptr), prev(nullptr){}
    GuestNode(double payment, string name, GuestNode * ptr, GuestNode * prevptr): paid(payment), fullName(name), next(ptr), prev(prevptr){}
    friend class LList;
    double getPayment(){return paid;}
    void setPayment(double newMoney){paid = newMoney;}
    string getName(){return fullName;}
    GuestNode* getNext() const{return next;}
    GuestNode* getPrev() const{return prev;}
};

class LList{
private:
    GuestNode* head;
    GuestNode* tail;
public:
    LList();
    ~LList();
    LList(const LList& rhs);
    LList &operator = (const LList &rhs);
    bool isEmpty(){return head->next == tail;}
    void popFront();
    void popBack();
    void pushBack(double money, string name);
    void pushFront(double money, string name);
    void clear();
    int size();
    void outputList();
    double correctPayment();
    void friend reorganizeLists(const double &correctPayment, LList &startingList, LList &extra, LList& payUp, LList &done);
    GuestNode* retHead() const{return head;}
    GuestNode* retTail() const{return tail;}
};

LList::LList(){
    head = new GuestNode();
    tail = new GuestNode();
    head->next = tail;
    tail->prev = head;
}

LList::~LList(){
    clear();
    delete head;
    delete tail;
}

LList::LList(const LList& rhs){
    head = new GuestNode();
    tail = new GuestNode();
    head->next = tail;
    tail ->prev = head;
    *this = rhs;
}

LList& LList::operator = (const LList &rhs){
    if(this == &rhs){
        return *this;
    }
    clear();
    GuestNode *ptr = rhs.head->next;
    while(ptr != rhs.tail){
        pushBack(ptr->paid, ptr->fullName);
    }
    return *this;
}

void LList::clear(){
    //iterate through list and delete nodes
    while(!isEmpty()){
        popFront();
    }
}

//delete current node and move head to next
void LList::popFront(){
    if(isEmpty())
        return; //empty list
    
    GuestNode * temp = head;
    head = head->next;
    head->next->prev = head;
    delete temp;
}

void LList::popBack(){
    if(isEmpty())
        return;
    
    GuestNode * temp = tail;
    tail = tail->prev;
    tail->prev->next = tail;
    delete temp;
}

void LList::pushFront(double money, string name){
    head->next = new GuestNode(money, name, head->next, head);
    head->next->next->prev = head->next;
}

void LList::pushBack(double money, string name){
    tail->prev = new GuestNode(money, name, tail, tail->prev);
    tail->prev->prev->next = tail->prev;
}

int LList::size(){
    GuestNode *ptr = head->next;
    int count = 0;
    while(ptr!=tail){
        count++;
        ptr = ptr->next;
    }
    return count;
}

void readInput(ifstream &infile, LList &list){
    double money;
    string name;
    char nextChar;
    while(infile >> money){
        do{
            infile.get(nextChar);
        }while(nextChar == ' ' || nextChar == '\t');
        infile.putback(nextChar);
        getline(infile, name);
        list.pushBack(money, name);
    }
}

void LList::outputList(){
    GuestNode *ptr = head->next;
    while(ptr != tail){
        cout << ptr->paid << '\t' << ptr->fullName << endl;
        ptr = ptr->next;
    }
}

double LList::correctPayment(){
    GuestNode *ptr = head->next;
    double sum = 0, average;

    while(ptr!=tail){
        sum += ptr->paid;
        ptr=ptr->next;
    }
    average = sum/static_cast<double>(size());
    return average;
}

void reorganizeLists(const double &correctPayment, LList &startingList, LList &extra, LList& payUp, LList &done){

    GuestNode* tempPtr;

    while(!startingList.isEmpty()){
        tempPtr = startingList.head->getNext();
        if(tempPtr->getPayment() < correctPayment){
            GuestNode temp = *tempPtr; //copy constructor
            startingList.popFront(); //deletes old object
            payUp.pushBack(temp.getPayment(), temp.getName());
        }
        else if(tempPtr->getPayment() > correctPayment){
            GuestNode temp = *tempPtr;
            startingList.popFront(); //deletes old object
            extra.pushBack(temp.getPayment(), temp.getName());
        }
        else{ 
            GuestNode temp = *tempPtr;
            startingList.popFront(); //deletes old object
            done.pushBack(temp.getPayment(), temp.getName());
        }
    }
}

double round(double value){
    int temp = value*100;
    double diff = value*100 - (int)(value*100);
    if(diff > 0.9998)
        temp++;
    
    double truncated = (double)temp/100.0;
    return truncated;
}

void payPeople(GuestNode* &givers, const LList &paidLess, GuestNode* &receivers, const LList &paidMore, double payment){
    while(givers != paidLess.retTail()){
        double giverDifference = round(payment - givers->getPayment());
        double receiverDifference = round(receivers->getPayment() - payment);
        //3 cases
        if(giverDifference < receiverDifference){
            cout << givers->getName() << ", you give " << receivers->getName() << " $" << giverDifference << endl;
            givers = givers->getNext();
            receivers->setPayment(receivers->getPayment() - giverDifference);
        }
        else if(giverDifference > receiverDifference){
            cout << givers->getName() << ", you give " << receivers->getName() << " $" << receiverDifference << endl;
            receivers = receivers->getNext();
            givers->setPayment(givers->getPayment() + receiverDifference);
            if(payment - givers->getPayment() == 0.00){
                givers = givers->getNext();
            }
        }
        else{ //diff is equal, advance both pointers
            cout << givers->getName() << ", you give " << receivers->getName() << " $" << giverDifference << endl;
            givers = givers->getNext();
            receivers = receivers->getNext();
        }
    }
}

int main(){
    ifstream inFile;
    openFile(inFile);
    
    LList inputList, paidMore, paidLess, paidEqual;
    readInput(inFile, inputList);

    cout.setf(ios::fixed);
    cout.setf(ios::showpoint);
    cout.precision(2);

    double correctPayment = inputList.correctPayment();
    
    reorganizeLists(correctPayment, inputList, paidMore, paidLess, paidEqual);

    //Testing
    /*
    cout << "Input: " << endl;
    inputList.outputList();
    cout << endl;
    cout << "Paid Extra: " << endl;
    paidMore.outputList();
    cout<< endl;
    cout << "Not paid enough: " << endl;
    paidLess.outputList();
    cout << endl;
    cout << "Paid correct amount: " << endl;
    paidEqual.outputList();
    cout << endl;
    */

    GuestNode* equalPeople = paidEqual.retHead()->getNext();
    GuestNode* lessPeople = paidLess.retHead()->getNext();
    GuestNode* morePeople = paidMore.retHead()->getNext();

    //Output people who don't need to do anything
    for(int i=0; i<paidEqual.size();i++){
        cout << equalPeople->getName() << ", you don't need to do anything" << endl;
        equalPeople = equalPeople->getNext();
    }

    payPeople(lessPeople, paidLess, morePeople, paidMore, correctPayment);

    cout << "In the end, you should all have spent around $" << correctPayment << endl;

    return 0;
}
