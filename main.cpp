#include <iostream>
#include <regex>
#include <map>
#include <vector>
#include <string>

struct Direction
{
    std::map<std::string, std::vector<int>> facingDirection = {
        {"NORTH", {1, 0}},
        {"WEST", {0, -1}},
        {"EAST", {0, 1}},
        {"SOUTH", {-1, 0}}};

    std::map<std::string, std::string> leftDirection = {
        {"NORTH", "WEST"},
        {"WEST", "SOUTH"},
        {"EAST", "NORTH"},
        {"SOUTH", "EAST"}};

    std::map<std::string, std::string> rightDirection = {
        {"NORTH", "EAST"},
        {"WEST", "NORTH"},
        {"EAST", "SOUTH"},
        {"SOUTH", "WEST"}};
};

class Table
{
public:
    int maxX = 5;
    int maxY = 5;
};

class Robot
{
private:
    int positionX;
    int positionY;
    std::string direction;

public:
    Robot()
    {
        positionX = -1;
        positionY = -1;
        direction = "";
    }

    void command();
    void place(std::string location);
    void move();
    void rotate(std::string rotation);
    void report();
    bool onTable(int x, int y);

    std::string commandField(std::string command, int fieldLoc, char delimeter);
    ~Robot(){}
};


int main()
{
    Robot r;
    r.command();

}
void Robot::command()
{
    std::string commandLine = "";
    std::regex reg("(PLACE (\\d+),(\\d+),(NORTH|SOUTH|EAST|WEST))|MOVE|LEFT|RIGHT|REPORT");

    do
    {
        std::cout << "Input command" << std::endl;
        std::getline(std::cin, commandLine);

        std::string command = Robot::commandField(commandLine, 1, ' ');

        if (command == "PLACE")
            this->place(Robot::commandField(commandLine, 2, ' '));
        else if (command == "MOVE")
            this->move();
        else if (command == "LEFT" || command == "RIGHT")
            this->rotate(command);
        else if (command == "REPORT")
            this->report();
            

    } while (std::regex_match(commandLine, reg));
}
void Robot::place(std::string location)
{
    std::cout << "ROBOT is in the table" << std::endl;
    this->positionX = std::stoi(Robot::commandField(location, 1, ','));
    this->positionY = std::stoi(Robot::commandField(location, 2, ','));
    this->direction = Robot::commandField(location, 3, ',');
    
    this->report();

}
void Robot::move()
{
    Direction d;
    std::map<std::string, std::vector<int>>::iterator itr = d.facingDirection.find(this->direction);
    int x = this->positionX + itr->second[0];
    int y = this->positionY + itr->second[1];

    std::cout << x << "," << y << std::endl;
    if (this->onTable(x, y))
    {
        this->positionX = x;
        this->positionY = y;
    }
}
void Robot::rotate(std::string rotation)
{
    Direction d;
    std::map<std::string, std::string>::iterator itr;

    if (rotation == "LEFT")
        itr = d.leftDirection.find(this->direction);
    else if (rotation == "RIGHT")
        itr = d.rightDirection.find(this->direction);

    this->direction = itr->second;
}
bool Robot::onTable(int newX, int newY)
{
    Table t;
    if (newX > -1 && newX < t.maxX && newY > -1 && newY < t.maxY)
        return true;
    else
        return false;
}
void Robot::report()
{
    std::cout << "OUTPUT: " << this->positionX << "," << this->positionY << "," << this->direction << std::endl;
}
std::string Robot::commandField(std::string command, int fieldLoc, char delimeter)
{
    int length = 0;
    int start = 0;
    int fieldNum = 1;

    for(int i = 0; i < command.length(); i++, length++)
    {
        if(command.at(i) == delimeter || (i == (command.length()-1)))
        {
            if((i == command.length() -1) && command.at(i) != delimeter )
                length++;
            if(fieldNum == fieldLoc)
                return command.substr(start,length);

            fieldNum++;
            start = i +1;
            length = -1;
        }
    }
    return "";
}