#include <iostream>
#include "string"
#include <cstdlib>
#include "ctime"
#include "fstream"
#include "vector"
using namespace std;

// user
class User
{
public:
    string username;
    string password;
    int level = 1;

    User() {}

    User(string username, string password)
    {
        this->username = username;
        this->password = password;
        level = 1;
    }

    User(string username, string password, int level)
    {
        this->username = username;
        this->password = password;
        this->level = level;
    }

    void updateLevel()
    {
        level++;
    }
};

// global variables
string spacing = "                                                                       ";
string emptyBox = "      ";
User currentUser;

// helper functions
void cls()
{
    system("clear"); // use cls for windows. clear only works with mac
    // system("cls");
}

int getRandomNumber(int limit)
{
    return rand() % limit;
}

string pad(string item)
{
    while (item.length() < emptyBox.length())
    {
        item += " ";
    }
    return item;
}

// Data Structures
struct Node
{
    string data;
    Node *next = NULL;
};

class StringLinkedList
{
private:
    Node *head;
    int size = 0;

public:
    StringLinkedList()
    {
        head = NULL;
    }

    void insertAtBegin(string data)
    {
        size++;
        Node *newNode = new Node;
        newNode->data = data;
        if (head == NULL)
        {
            head = newNode;
        }
        else
        {
            newNode->next = head;
            head = newNode;
        }
    }

    string removeAtBegin()
    {
        if (head == NULL)
        {
            return "";
        }
        else if (head->next == NULL)
        {
            size--;
            string returnData = head->data;
            head = NULL;
            return returnData;
        }
        else
        {
            size--;
            string returnData = head->data;
            head = head->next;
            return returnData;
        }
    }

    Node *getHead()
    {
        return head;
    }

    string *getArray()
    {
        string *ptr = new string[size];
        int index = 0;

        for (Node *i = head; i != NULL; i = i->next)
        {
            ptr[index] = i->data;
            index++;
        }
        return ptr;
    }

    int getSize()
    {
        return size;
    }
};

class StackContainer
{
private:
    int maxSize;
    StringLinkedList colors;

public:
    StackContainer(int size)
    {
        maxSize = size;
        for (int i = 0; i < maxSize; i++)
        {
            colors.insertAtBegin(emptyBox);
        }
    }

    StackContainer() {}

    int getActualSize()
    {
        int count = 0;
        string *list = colors.getArray();
        for (int i = 0; i < colors.getSize(); i++)
        {
            if (list[i] != emptyBox)
            {
                count++;
            }
        }
        return count;
    }

    int getSize()
    {
        return colors.getSize();
    };

    bool isFilled()
    {
        return maxSize == getActualSize();
    }

    string peek()
    {
        if (colors.getHead() != NULL)
        {
            return colors.getHead()->data;
        }
        else
        {
            return "";
        }
    }

    void push(string color)
    {
        while (peek() == emptyBox)
        {
            colors.removeAtBegin();
        }
        if (!isFilled())
        {
            colors.insertAtBegin(pad(color));
            while (colors.getSize() < maxSize)
            {
                colors.insertAtBegin(emptyBox);
            }
        }
    }

    string pop()
    {
        if (getActualSize() == 0)
        {
            return "empty";
        }
        while (peek() == emptyBox)
        {
            colors.removeAtBegin();
        }
        string popped = colors.removeAtBegin();
        while (colors.getSize() < maxSize)
        {
            colors.insertAtBegin(emptyBox);
        }
        return popped;
    }

    void clear()
    {
        colors = StringLinkedList();
        for (int i = 0; i < maxSize; i++)
        {
            colors.insertAtBegin(emptyBox);
        }
    }

    string *getArray()
    {
        return colors.getArray();
    }

    bool isSorted()
    {
        bool ret = true;
        if (getSize() > 0)
        {
            string *array = getArray();
            string item = array[0];
            for (int i = 0; i < getSize(); i++)
            {
                if (array[i] != item)
                {
                    ret = false;
                    break;
                }
            }
        }
        return ret;
    }
};

// classes for containers to keep containers in linkedList (2D linked-list)
struct StackNode
{
    StackContainer stack;
    StackNode *next = NULL;
};

class LevelLinkedList
{
private:
    StackNode *head;
    int size = 0;

public:
    LevelLinkedList()
    {
        head = NULL;
    }

    void insertAtBegin(StackContainer stack)
    {
        size++;
        StackNode *newNode = new StackNode;
        newNode->stack = stack;
        if (head == NULL)
        {
            head = newNode;
        }
        else
        {
            newNode->next = head;
            head = newNode;
        }
    }

    StackNode *getIndex(int index)
    {
        StackNode *ptr = head;
        for (int i = 0; i < index; i++)
        {
            if (ptr != NULL && ptr->next != NULL)
            {
                ptr = ptr->next;
            }
        }
        return ptr;
    }

    StackContainer *getArray()
    {
        StackContainer *ptr = new StackContainer[size];
        int index = 0;
        for (StackNode *i = head; i != NULL; i = i->next)
        {
            ptr[index] = i->stack;
            index++;
        }
        return ptr;
    }

    int getSize()
    {
        return size;
    }
};

// file handling
void createUser(User user)
{
    fstream file;
    file.open("users.txt", ios::app);
    if (file.is_open())
    {
        file << user.username << endl;
        file << user.password << endl;
        file << user.level << endl;
        file.close();
    }
    else
    {
        cout << "\nError while handling files\n";
    }
}

void updateUser(User user)
{
    vector<User> users;

    // taking all the users
    ifstream file("users.txt");
    if (file.is_open())
    {
        string fileUserName;
        string filePassword;
        string temp;
        int level;
        int index = 0;

        while (getline(file, temp))
        {
            if (index == 0)
            {
                fileUserName = temp;
                index++;
            }
            else if (index == 1)
            {
                filePassword = temp;
                index++;
            }
            else
            {
                level = stoi(temp);
                users.push_back(User(fileUserName, filePassword, level));
                index = 0;
            }
        }
        file.close();
    }
    else
    {
        cout << "\nError while handling files\n";
    }

    string outString = "";
    User tempUser;
    for (int i = 0; i < users.size(); i++)
    {
        tempUser = users.at(i);
        if (tempUser.username == user.username)
        {
            outString += user.username + '\n';
            outString += user.password + '\n';
            outString += to_string(user.level) + '\n';
        }
        else
        {
            outString += tempUser.username + '\n';
            outString += tempUser.password + '\n';
            outString += to_string(tempUser.level) + '\n';
        }
    }
    ofstream outFile("users.txt");
    outFile << outString;
    outFile.close();
}

bool validate(string username, string password)
{
    bool isValid = false;
    ifstream file("users.txt");
    if (file.is_open())
    {
        string fileUserName;
        string filePassword;
        string temp;
        int level;
        int index = 0;

        while (getline(file, temp))
        {
            if (index == 0)
            {
                fileUserName = temp;
                index++;
            }
            else if (index == 1)
            {
                filePassword = temp;
                index++;
            }
            else
            {
                if (username == fileUserName && password == filePassword)
                {
                    level = stoi(temp);
                    currentUser = User(fileUserName, filePassword, level);
                    isValid = true;
                    break;
                }
                else
                {
                    index = 0;
                }
            }
        }
        file.close();
    }
    else
    {
        cout << "\nError while handling files\n";
        isValid = false;
    }
    return isValid;
}

// game logic functions
bool isWon(LevelLinkedList level)
{
    bool won = true;
    StackContainer *stacks = level.getArray();
    for (int i = 0; i < level.getSize(); i++)
    {
        if (!stacks[i].isSorted())
        {
            won = false;
        }
    }
    return won;
}

void showColor(string color)
{

    if (color == pad("Red"))
    {
        cout << "\033[1;31;41m" << color << "\033[0;0m";
    }
    else if (color == pad("Green"))
    {
        cout << "\033[1;32;42m" << color << "\033[0;0m";
    }
    else if (color == pad("Yellow"))
    {
        cout << "\033[1;33;43m" << color << "\033[0;0m";
    }
    else if (color == pad("Purple"))
    {
        cout << "\033[1;34;44m" << color << "\033[0;0m";
    }
    else if (color == pad("Pink"))
    {
        cout << "\033[1;95;105m" << color << "\033[0;0m";
    }
    else if (color == pad("White"))
    {
        cout << "\033[1;97;107m" << color << "\033[0;0m";
    }
    else
    {
        cout << color;
    }
}

void printGame(LevelLinkedList level, string popped, int poppedIndex, string errorMessage)
{
    StackContainer *stack = level.getArray();
    cout << "Level : " << currentUser.level << endl;
    cout << "Name  : " << currentUser.username << endl;
    if (errorMessage.length() > 0)
    {
        cout << "Error : " << errorMessage;
    }

    cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
    if (poppedIndex != -1)
    {
        cout << spacing;
        for (int i = 0; i < level.getSize(); i++)
        {
            if (i == poppedIndex)
            {
                cout << "  ";
                showColor(popped);
                cout << "  \t";
            }
            else
            {
                cout << "         \t";
            }
        }
    }
    // colors
    cout << "\n\n\n\n";
    for (int i = 0; i < stack[0].getSize(); i++)
    {
        cout << spacing;
        for (int j = 0; j < level.getSize(); j++)
        {
            cout << "| ";
            showColor(stack[j].getArray()[i]);
            cout << " |\t";
        }
        cout << endl;
    }
    // for under line
    cout << spacing;
    for (int i = 0; i < level.getSize(); i++)
    {
        cout << " -";
        for (int j = 0; j < emptyBox.length(); j++)
        {
            cout << "-";
        }
        cout << "- \t";
    }
    cout << endl;
    // for indexs
    cout << spacing;
    for (int j = 0; j < level.getSize(); j++)
    {
        cout << "    " << j + 1 << "    \t";
    }
    cout << "\n\n";
}

void play(LevelLinkedList level)
{
    string popped = "";
    int poppedIndex = -1;
    string errorMessage = "";

game:
    cls();
    printGame(level, popped, poppedIndex, errorMessage);
    if (isWon(level) && poppedIndex == -1)
    {
        currentUser.updateLevel();
        updateUser(currentUser);
        cout << spacing << "All colors are sorted you won (Press enter to continue).... ";
        cin.get();
        cin.get();
        return;
    }
    else if (poppedIndex == -1)
    {
        cout << spacing << "Pop color from : ";
        char option = cin.get();
        if (option > '0' && option - 48 <= level.getSize())
        {
            errorMessage = "";
            poppedIndex = option - 48 - 1;
            popped = level.getIndex(poppedIndex)->stack.pop();
            if (popped == "empty")
            {
                errorMessage = "Empty stack";
                poppedIndex = -1;
                popped = "";
            }
            goto game;
        }
        else
        {

            goto game;
        }
    }
    else
    {
        cout << spacing << "Push color to : ";
        char option = cin.get();
        if (option > '0' && option - 48 <= level.getSize())
        {
            int pushIndex = option - 48 - 1;
            if (level.getIndex(pushIndex)->stack.isFilled())
            {
                errorMessage = "Stack " + to_string(pushIndex + 1) + " is full!";
                goto game;
            }
            else
            {
                errorMessage = "";
                level.getIndex(pushIndex)->stack.push(popped);
                // cleaning popped
                popped = "";
                poppedIndex = -1;
                goto game;
            }
        }
        else
        {
            goto game;
        }
    }
}

// game levels
LevelLinkedList levelOne()
{
    StackContainer stack(2);
    LevelLinkedList level;
    stack.push("Yellow");
    stack.push("Red");
    level.insertAtBegin(stack);
    stack.clear();
    stack.push("Red");
    stack.push("Yellow");
    level.insertAtBegin(stack);
    stack.clear();
    level.insertAtBegin(stack);
    return level;
}

LevelLinkedList levelTwo()
{
    StackContainer stack(3);
    LevelLinkedList level;

    stack.push("Yellow");
    stack.push("Yellow");

    level.insertAtBegin(stack);
    stack.clear();

    stack.push("Red");
    stack.push("Red");

    level.insertAtBegin(stack);
    stack.clear();

    stack.push("Red");

    level.insertAtBegin(stack);
    stack.clear();

    stack.push("Yellow");
    level.insertAtBegin(stack);
    return level;
}

LevelLinkedList levelThree()
{
    StackContainer stack(4);
    LevelLinkedList level;
    stack.push("Yellow");
    stack.push("Red");
    stack.push("Yellow");
    stack.push("Red");
    level.insertAtBegin(stack);
    stack.clear();
    stack.push("Red");
    stack.push("Yellow");
    stack.push("Red");
    stack.push("Yellow");
    level.insertAtBegin(stack);
    stack.clear();
    level.insertAtBegin(stack);
    return level;
}

LevelLinkedList levelFour()
{
    StackContainer stack(4);
    LevelLinkedList level;
    stack.push("Yellow");
    stack.push("Green");
    stack.push("Red");
    stack.push("Yellow");
    level.insertAtBegin(stack);
    stack.clear();
    stack.push("Red");
    stack.push("Red");
    stack.push("Yellow");
    stack.push("Green");
    level.insertAtBegin(stack);
    stack.clear();
    stack.push("Green");
    stack.push("Red");
    stack.push("Yellow");
    stack.push("Green");
    level.insertAtBegin(stack);
    stack.clear();
    level.insertAtBegin(stack);
    stack.clear();
    level.insertAtBegin(stack);
    return level;
}

LevelLinkedList levelFive()
{
    StackContainer stack(4);
    LevelLinkedList level;
    stack.push("Green");
    stack.push("Green");
    stack.push("Red");
    stack.push("Yellow");
    level.insertAtBegin(stack);
    stack.clear();
    stack.push("Pink");
    stack.push("Purple");
    stack.push("Red");
    stack.push("Yellow");
    level.insertAtBegin(stack);
    stack.clear();
    stack.push("Purple");
    stack.push("Red");
    stack.push("Pink");
    stack.push("Red");
    level.insertAtBegin(stack);
    stack.clear();
    stack.push("Pink");
    stack.push("Green");
    stack.push("Yellow");
    stack.push("Purple");
    level.insertAtBegin(stack);
    stack.clear();
    stack.push("Green");
    stack.push("Pink");
    stack.push("Yellow");
    stack.push("Purple");
    level.insertAtBegin(stack);
    stack.clear();
    level.insertAtBegin(stack);
    stack.clear();
    level.insertAtBegin(stack);
    return level;
}

LevelLinkedList generateLevel(int levelNo)
{
    srand(time(0));

    // size of stack
    int size = 7;
    int stackSize = 5;
    if (levelNo > 10)
    {
        stackSize = 5 + levelNo / 10;
        // max size is 15
        if (stackSize > 15)
        {
            stackSize = 15;
        }
    }

    string colors[5] = {"Green", "Pink", "Yellow", "Red", "White"};
    StackContainer stack(stackSize);
    LevelLinkedList level;

    // creating stack
    for (int i = 0; i < size; i++)
    {
        if (i < size - 2)
        {
            for (int j = 0; j < stackSize; j++)
            {
                stack.push(colors[i]);
            }
        }
        level.insertAtBegin(stack);
        stack.clear();
    }

    // shuffling stack
    int randomNumber = 0;
    string popped = "";

    for (int i = 0; i < 100 * levelNo; i++)
    {
        randomNumber = getRandomNumber(size);
        popped = level.getIndex(randomNumber)->stack.pop();
        if (popped != "empty")
        {
            randomNumber = getRandomNumber(size);
            // until we get  empty stack
            while (level.getIndex(randomNumber)->stack.isFilled())
            {
                randomNumber = getRandomNumber(size);
            }
            level.getIndex(randomNumber)->stack.push(popped);
        }
        else
        {
            i--;
        }
    }

    return level;
}

LevelLinkedList getLevel(int level)
{
    switch (level)
    {
    case 1:
        return levelOne();
    case 2:
        return levelTwo();
    case 3:
        return levelThree();
    case 4:
        return levelFour();
    case 5:
        return levelFive();
    default:
        return generateLevel(level);
    }
}

// graphics code
void printLineWithColor(string line, string color)
{

    if (color == "Red")
    {
        cout << "\033[1;31m" << line << "\033[0m";
    }
    else if (color == "Green")
    {
        cout << "\033[1;32m" << line << "\033[0m";
    }
    else if (color == "Yellow")
    {
        cout << "\033[1;33m" << line << "\033[0m";
    }
    else if (color == "Purple")
    {
        cout << "\033[1;34m" << line << "\033[0m";
    }
    else if (color == "Pink")
    {
        cout << "\033[1;95m" << line << "\033[0m";
    }
    else if (color == "White")
    {
        cout << "\033[1;97m" << line << "\033[0m";
    }
    else
    {
        cout << line;
    }
}

void printIntro()
{
    string colors[5] = {"Green", "Pink", "Yellow", "Red", "White"};
    StackContainer tempStack(10);
    LevelLinkedList level;

    // creating stack
    for (int i = 0; i < 7; i++)
    {
        if (i < 5)
        {
            for (int j = 0; j < 10; j++)
            {
                tempStack.push(colors[i]);
            }
        }
        level.insertAtBegin(tempStack);
        tempStack.clear();
    }
    StackContainer *stack = level.getArray();
    // colors
    cout << "\n\n\n\n";
    for (int i = 0; i < stack[0].getSize(); i++)
    {
        cout << spacing;
        for (int j = 0; j < level.getSize(); j++)
        {
            cout << "| ";
            showColor(stack[j].getArray()[i]);
            cout << " |\t";
        }
        cout << endl;
    }
    // for under line
    cout << spacing;
    for (int i = 0; i < level.getSize(); i++)
    {
        cout << " -";
        for (int j = 0; j < emptyBox.length(); j++)
        {
            cout << "-";
        }
        cout << "- \t";
    }
    cout << endl;
}

void intro()
{

    cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
    printLineWithColor("                                                                              888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888                                 ", "Green");
    cout << endl;
    printLineWithColor("                                                                         8888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888                            ", "Green");
    cout << endl;
    printLineWithColor("                                                        88    8888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888   88             ", "Green");
    cout << endl;
    printLineWithColor("                                                     88                                                                                                                                      88          ", "Green");
    cout << endl;
    printLineWithColor("                                                  88           888888888   88888888   88         88888888   8888888        8888888    88888888   8888888    88888888  88888888  8888888         88       ", "Green");
    cout << endl;
    printLineWithColor("                                               88             88          88      88  88        88      88  88    88      88         88      88  88    88      88     88        88    88           88    ", "Green");
    cout << endl;
    printLineWithColor("                                            88                88          88      88  88        88      88  8888888        888888    88      88  8888888       88     888888    8888888               88 ", "Green");
    cout << endl;
    printLineWithColor("                                               88             88          88      88  88        88      88  88    88             88  88      88  88    88      88     88        88    88            88   ", "Green");
    cout << endl;
    printLineWithColor("                                                  88           888888888   88888888   888888888  88888888   88    88      8888888     88888888   88    88      88     88888888  88    88         88      ", "Green");
    cout << endl;
    printLineWithColor("                                                     88                                                                                                                                      88          ", "Green");
    cout << endl;
    printLineWithColor("                                                        88    8888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888   88             ", "Green");
    cout << endl;
    printLineWithColor("                                                                         88888888888888888->  Mubashir Ali | Rana Irtaza Ahmed | Syeda Marium Tauqeer <- 88888888888888888888                            ", "Green");
    cout << endl;
    printLineWithColor("                                                                              888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888                                 ", "Green");
    cout << endl
         << endl;

    printIntro();
    cout << "\n\n\n";
    cout << "                                                            Press enter to continue... ";
    cin.get();
}

char loginOrSignupPage()
{
    cls();
    cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
    printLineWithColor("                                                                              88                                                                                         88          ", "Green");
    cout << endl;
    printLineWithColor("                                                                           88      88          8888888888  888888888888  8888888888            88888888      88888888       88       ", "Green");
    cout << endl;
    printLineWithColor("                                                                        88         88          88               88       88                  88        88  88        88        88    ", "Green");
    cout << endl;
    printLineWithColor("                                                                     88            88          8888888888       88       8888888888          88            88        88           88 ", "Green");
    cout << endl;
    printLineWithColor("                                                                        88         88          88               88               88          88    888888  88        88        88    ", "Green");
    cout << endl;
    printLineWithColor("                                                                           88      8888888888  8888888888       88       8888888888            88888   88    88888888       88       ", "Green");
    cout << endl;
    printLineWithColor("                                                                              88                                                                                         88          ", "Green");
    cout << endl
         << endl;

    cout << "                                                                                                  1. Log In\n";
    cout << "                                                                                                  2. Sign Up\n";
    cout << "                                                                                                  3. Exit\n";
    cout << "                                                                                                  Select an option : ";
    char opt;
    cin >> opt;
    if (opt >= '1' && opt <= '3')
    {
        return opt;
    }
    return loginOrSignupPage();
}

bool login()
{
    string username;
    string password;
    cls();

    cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
    printLineWithColor("                                                                                     88                                                                          88          ", "Green");
    cout << endl;
    printLineWithColor("                                                                                  88        88             88888888      88888888      88888888  888       88       88       ", "Green");
    cout << endl;
    printLineWithColor("                                                                               88           88           88        88  88        88       88     88 88     88          88    ", "Green");
    cout << endl;
    printLineWithColor("                                                                            88              88           88        88  88                 88     88   88   88             88 ", "Green");
    cout << endl;
    printLineWithColor("                                                                               88           88           88        88  88    888888       88     88     88 88          88    ", "Green");
    cout << endl;
    printLineWithColor("                                                                                  88        8888888888     88888888      88888   88    88888888  88       888       88       ", "Green");
    cout << endl;
    printLineWithColor("                                                                                     88                                                                          88          ", "Green");
    cout << endl;
    cout << "                                                                                                                                                                                                  " << endl
         << endl;
    cout << "                                                                                                                                                                                                  " << endl
         << endl;
    cout << "                                                                                                    Username : ";
    cin >> username;
    cout << "                                                                                                    Password : ";
    cin >> password;
    bool isValid = validate(username, password);
    if (isValid)
    {
        cout << "\t\tLogin successful :)  (Press enter to continue) : ";
    }
    else
    {
        cout << "\t\tLogin failed :(  (Press enter to continue) : ";
    }
    cin.get();
    return isValid;
}

void signUp()
{
    string username;
    string password;
    string confirmPassword;
    cls();
    cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
    cout << "                                                                                                                                                                                                               " << endl;
    printLineWithColor("                                                                                   88                                                                                   88          ", "Green");
    cout << endl;
    printLineWithColor("                                                                                88        8888888888   88888888   88888888    888        88     88      88   88888888      88       ", "Green");
    cout << endl;
    printLineWithColor("                                                                             88           88              88    88        88  88  88     88     88      88   88    88         88    ", "Green");
    cout << endl;
    printLineWithColor("                                                                          88              8888888888      88    88            88    88   88     88      88   88888888            88 ", "Green");
    cout << endl;
    printLineWithColor("                                                                             88                   88      88    88    888888  88      88 88     88      88   88               88    ", "Green");
    cout << endl;
    printLineWithColor("                                                                                88        8888888888   88888888   88888   88  88        888     8888888888   88            88       ", "Green");
    cout << endl;
    printLineWithColor("                                                                                   88                                                                                   88          ", "Green");
    cout << endl;
    cout << "                                                                                                                                                                                                               " << endl
         << endl;
    cout << "                                                                                        Enter your username (without spaces) : ";
    cin >> username;
password:
    cout << "                                                                                        Create a password : ";
    cin >> password;
    cout << "                                                                                        Confirm a password : ";
    cin >> confirmPassword;
    if (password != confirmPassword)
    {
        cout << "\t\tPasswords doesn't match!\n";
        goto password;
    }
    createUser(User(username, password));
}

char home()
{
    cls();
    cout << "Level : " << currentUser.level << endl;
    cout << "Name  : " << currentUser.username;
    cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";

    printLineWithColor("                                                                              888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888                                 ", "Green");
    cout << endl;
    printLineWithColor("                                                                         8888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888                            ", "Green");
    cout << endl;
    printLineWithColor("                                                        88    8888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888   88             ", "Green");
    cout << endl;
    printLineWithColor("                                                     88                                                                                                                                      88          ", "Green");
    cout << endl;
    printLineWithColor("                                                  88           888888888   88888888   88         88888888   8888888        8888888    88888888   8888888    88888888  88888888  8888888         88       ", "Green");
    cout << endl;
    printLineWithColor("                                               88             88          88      88  88        88      88  88    88      88         88      88  88    88      88     88        88    88           88    ", "Green");
    cout << endl;
    printLineWithColor("                                            88                88          88      88  88        88      88  8888888        888888    88      88  8888888       88     888888    8888888               88 ", "Green");
    cout << endl;
    printLineWithColor("                                               88             88          88      88  88        88      88  88    88             88  88      88  88    88      88     88        88    88            88   ", "Green");
    cout << endl;
    printLineWithColor("                                                  88           888888888   88888888   888888888  88888888   88    88      8888888     88888888   88    88      88     88888888  88    88         88      ", "Green");
    cout << endl;
    printLineWithColor("                                                     88                                                                                                                                      88          ", "Green");
    cout << endl;
    printLineWithColor("                                                        88    8888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888   88             ", "Green");
    cout << endl;
    printLineWithColor("                                                                         88888888888888888->                         Fatima Illimdin                   <- 88888888888888888888                            ", "Green");
    cout << endl;
    printLineWithColor("                                                                              888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888                                 ", "Green");
    cout << endl
         << endl;

    cout << "                                                                                                  1.Play\n";
    cout << "                                                                                                  2.Logout\n";
    cout << "                                                                                                  3.Exit\n";
    cout << "                                                                                                  Select an option : ";
    char opt;
    cin >> opt;
    if (opt >= '1' && opt <= '3')
    {
        return opt;
    }
    return home();
}

// main

int main()
{

    char opt;
    // call the intro function which displays some sort of introduction or splash screen
    intro();
loggedOut:
    // call the loginOrSignupPage function which prompts the user to either log in or sign up
    opt = loginOrSignupPage();
    // check if user chose to log in
    if (opt == '1')
    {
        // attempt to log in
        if (login())
        {
        home:
            // call the home function which presents the user with options for interacting with the game
            opt = home();
            // check if user chose to play the game
            if (opt == '1')
            {
                // call the play function and pass it the level selected by the user
                play(getLevel(currentUser.level));
                // jump back to the beginning of the home loop
                goto home;
            }
            // check if user chose to log out
            else if (opt == '2')
            {
                // set the current user to an empty user
                currentUser = User();
                // jump back to the beginning of the loggedOut loop
                goto loggedOut;
            }
        }
        // if the login attempt was unsuccessful, jump back to the beginning of the loggedOut loop
        else
        {
            goto loggedOut;
        }
    }
    // check if user chose to sign up
    else if (opt == '2')
    {
        // call the signUp function
        signUp();
        // jump back to the beginning of the loggedOut loop
        goto loggedOut;
    }
}
