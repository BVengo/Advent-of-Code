#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <ctime> 
#include <vector>
#include <map>
#include <ctype.h>

/*
--- Day 16: Ticket Translation ---
As you're walking to yet another connecting flight, you realize that one of the legs of your re-routed trip coming up is on a high-speed train. However, the train ticket you were given is in a language you don't understand. You should probably figure out what it says before you get to the train station after the next flight.

Unfortunately, you can't actually read the words on the ticket. You can, however, read the numbers, and so you figure out the fields these tickets must have and the valid ranges for values in those fields.

You collect the rules for ticket fields, the numbers on your ticket, and the numbers on other nearby tickets for the same train service (via the airport security cameras) together into a single document you can reference (your puzzle input).

The rules for ticket fields specify a list of fields that exist somewhere on the ticket and the valid ranges of values for each field. For example, a rule like class: 1-3 or 5-7 means that one of the fields in every ticket is named class and can be any value in the ranges 1-3 or 5-7 (inclusive, such that 3 and 5 are both valid in this field, but 4 is not).

Each ticket is represented by a single line of comma-separated values. The values are the numbers on the ticket in the order they appear; every ticket has the same format. For example, consider this ticket:

.--------------------------------------------------------.
| ????: 101    ?????: 102   ??????????: 103     ???: 104 |
|                                                        |
| ??: 301  ??: 302             ???????: 303      ??????? |
| ??: 401  ??: 402           ???? ????: 403    ????????? |
'--------------------------------------------------------'
Here, ? represents text in a language you don't understand. This ticket might be represented as 101,102,103,104,301,302,303,401,402,403; of course, the actual train tickets you're looking at are much more complicated. In any case, you've extracted just the numbers in such a way that the first number is always the same specific field, the second number is always a different specific field, and so on - you just don't know what each position actually means!

Start by determining which tickets are completely invalid; these are tickets that contain values which aren't valid for any field. Ignore your ticket for now.

For example, suppose you have the following notes:

class: 1-3 or 5-7
row: 6-11 or 33-44
seat: 13-40 or 45-50

your ticket:
7,1,14

nearby tickets:
7,3,47
40,4,50
55,2,20
38,6,12
It doesn't matter which position corresponds to which field; you can identify invalid nearby tickets by considering only whether tickets contain values that are not valid for any field. In this example, the values on the first nearby ticket are all valid for at least one field. This is not true of the other three nearby tickets: the values 4, 55, and 12 are are not valid for any field. Adding together all of the invalid values produces your ticket scanning error rate: 4 + 55 + 12 = 71.

Consider the validity of the nearby tickets you scanned. What is your ticket scanning error rate?

--- Part Two ---
Now that you've identified which tickets contain invalid values, discard those tickets entirely. Use the remaining valid tickets to determine which field is which.

Using the valid ranges for each field, determine what order the fields appear on the tickets. The order is consistent between all tickets: if seat is the third field, it is the third field on every ticket, including your ticket.

For example, suppose you have the following notes:

class: 0-1 or 4-19
row: 0-5 or 8-19
seat: 0-13 or 16-19

your ticket:
11,12,13

nearby tickets:
3,9,18
15,1,5
5,14,9
Based on the nearby tickets in the above example, the first position must be row, the second position must be class, and the third position must be seat; you can conclude that in your ticket, class is 12, row is 11, and seat is 13.

Once you work out which field is which, look for the six fields on your ticket that start with the word departure. What do you get if you multiply those six values together?
*/

std::vector<std::vector<int>> fields;
std::vector<std::vector<int>> tickets;
std::vector<int> myTicket;

std::vector<int> getTicketValues(std::string ticket) {
    std::vector<int> positions;

    while(!ticket.empty()) {
        int val = std::stoi(ticket.substr(0, ticket.find(',')));
        
        positions.push_back(val);
        if(ticket.find(',') == std::string::npos) {
            break;
        }

        ticket.erase(0, (ticket.find(',')+1));
    }

    return positions;
}

void initialise() {
    std::ifstream file("../input/day16.txt");
    std::string row;
    std::getline(file, row);
    
    while(!row.empty()) {
        std::vector<int> ranges(4);

        std::string field = row.substr(0, row.find(':'));

        row.erase(0, field.size()+2);
        ranges[0] = std::stoi(row.substr(0, row.find('-')));
        row.erase(0, row.find('-')+1);
        ranges[1] = std::stoi(row.substr(0, row.find(' ')));
        row.erase(0, row.find(' ')+4);
        ranges[2] = std::stoi(row.substr(0, row.find('-')));
        row.erase(0, row.find('-') + 1);
        ranges[3] = std::stoi(row);

        fields.push_back(ranges);

        std::getline(file, row);
    }

    std::getline(file, row);
    std::getline(file, row);
    myTicket = getTicketValues(row);
    std::getline(file, row);
    std::getline(file, row);

    do {
        std::getline(file, row);
        tickets.push_back(getTicketValues(row));
    }
    while(!file.eof());
}

int part1()
{
    int sumWrong = 0;
    for(int i = 0; i < tickets.size(); i++) 
    {
        for(int j = 0; j < fields.size(); j++) 
        {    
            bool isWrong = true;

            for(int k = 0; k < fields.size(); k++) 
            {
                if(!(tickets[i][j] < fields[k][0] || (tickets[i][j] > fields[k][1] && tickets[i][j] < fields[k][2]) || tickets[i][j] > fields[k][3]))
                {
                    isWrong = false;
                }
            }

            if(isWrong) 
            {
                sumWrong += tickets[i][j];
                tickets.erase(tickets.begin() + i);
            }
        }
    }
    return sumWrong;
}
    
bool isValid(int value, std::vector<int> rules) {
    return false;
}

int part2()
{
    std::vector<int> mapping(fields.size());

    for(int i = 0; i < mapping.size(); i++) {
        for(int j = 0; j < fields.size(); j++) {
            for(int k = 0; k < tickets.size(); k++) {
                if(isValid(tickets[k][j], fields[j])) {
                    
                }
            }
        }
        
    }
    return(0);

    int mult = 1;

    for(int i = 0; i < 6; i++) {
        mult *= mapping[i];
    }

    return mult;
}

int main()
{
    initialise();
    std::cout << part1() << std::endl;
    std::cout << part2() << std::endl;
    
}