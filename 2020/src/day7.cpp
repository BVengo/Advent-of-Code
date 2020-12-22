#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <iterator>
#include <algorithm>

/*
--- Day 7: Handy Haversacks ---
You land at the regional airport in time for your next flight. In fact, it looks like you'll even have time to grab some food: all flights are currently delayed due to issues in luggage processing.

Due to recent aviation regulations, many rules (your puzzle input) are being enforced about bags and their contents; bags must be color-coded and must contain specific quantities of other color-coded bags. 
Apparently, nobody responsible for these regulations considered how long they would take to enforce!

For example, consider the following rules:

light red bags contain 1 bright white bag, 2 muted yellow bags.
dark orange bags contain 3 bright white bags, 4 muted yellow bags.
bright white bags contain 1 shiny gold bag.
muted yellow bags contain 2 shiny gold bags, 9 faded blue bags.
shiny gold bags contain 1 dark olive bag, 2 vibrant plum bags.
dark olive bags contain 3 faded blue bags, 4 dotted black bags.
vibrant plum bags contain 5 faded blue bags, 6 dotted black bags.
faded blue bags contain no other bags.
dotted black bags contain no other bags.
These rules specify the required contents for 9 bag types. In this example, every faded blue bag is empty, every vibrant plum bag contains 11 bags (5 faded blue and 6 dotted black), and so on.

You have a shiny gold bag. If you wanted to carry it in at least one other bag, how many different bag colors would be valid for the outermost bag? (In other words: how many colors can, eventually, contain at least one shiny gold bag?)

In the above rules, the following options would be available to you:

A bright white bag, which can hold your shiny gold bag directly.
A muted yellow bag, which can hold your shiny gold bag directly, plus some other bags.
A dark orange bag, which can hold bright white and muted yellow bags, either of which could then hold your shiny gold bag.
A light red bag, which can hold bright white and muted yellow bags, either of which could then hold your shiny gold bag.
So, in this example, the number of bag colors that can eventually contain at least one shiny gold bag is 4.

How many bag colors can eventually contain at least one shiny gold bag? (The list of rules is quite long; make sure you get all of it.)

--- Part Two ---
It's getting pretty expensive to fly these days - not because of ticket prices, but because of the ridiculous number of bags you need to buy!

Consider again your shiny gold bag and the rules from the above example:

faded blue bags contain 0 other bags.
dotted black bags contain 0 other bags.
vibrant plum bags contain 11 other bags: 5 faded blue bags and 6 dotted black bags.
dark olive bags contain 7 other bags: 3 faded blue bags and 4 dotted black bags.
So, a single shiny gold bag must contain 1 dark olive bag (and the 7 bags within it) plus 2 vibrant plum bags (and the 11 bags within each of those): 1 + 1*7 + 2 + 2*11 = 32 bags!

Of course, the actual rules have a small chance of going several levels deeper than this example; be sure to count all of the bags, even if the nesting becomes topologically impractical!

Here's another example:

shiny gold bags contain 2 dark red bags.
dark red bags contain 2 dark orange bags.
dark orange bags contain 2 dark yellow bags.
dark yellow bags contain 2 dark green bags.
dark green bags contain 2 dark blue bags.
dark blue bags contain 2 dark violet bags.
dark violet bags contain no other bags.
In this example, a single shiny gold bag must contain 126 other bags.

How many individual bags are required inside your single shiny gold bag?

*/

struct Bag {
    std::string color;
    std::map<std::string, int> contains;

    Bag(std::string color, std::map<std::string, int> bags) : 
        color(color), contains(bags) {};
};

std::vector<Bag> bags;

void initialiseMap()
{
    std::ifstream file("../input/day7.txt");
    std::string rule;

    while(std::getline(file, rule))
    {
        std::map<std::string, int> contains;

        std::string color = rule.substr(0, rule.find(" bags"));
        std::string canHold = rule.substr(rule.find("contain")+8);

         while(!canHold.empty()) {
            // Empty bag
            if(canHold.find("no ") != std::string::npos)
            {
                break;
            }

            std::string currentBag = canHold.substr(0, canHold.find(" bag"));

            contains.emplace(currentBag.substr(currentBag.find(' ')+1), std::stoi(currentBag.substr(0, currentBag.find(' '))));
            
            canHold.erase(0, canHold.find(" bag") + 6);
            
            
            if(canHold[0] == ' ')
            {
                canHold.erase(0, 1);
            }
        }

        bags.push_back(Bag(color, contains));
    }
}

void printMap()
{
    for(Bag bag : bags)
    {
        std::cout << bag.color << " contains ";
        
        std::map<std::string, int>::iterator it;

        for ( it = bag.contains.begin(); it != bag.contains.end(); it++ )
        {
            std::cout << it->second << " " << it->first << ", ";
        }

        std::cout << std::endl;
    }
}

bool canHold(Bag outsideBag, std::string findBag)
{
    if(outsideBag.contains.empty())
    {
        return false;
    }

    if(outsideBag.contains.find(findBag) != outsideBag.contains.end())
    {
        return true;
    }

    std::map<std::string, int>::iterator it;
    for ( it = outsideBag.contains.begin(); it != outsideBag.contains.end(); it++ )
    {
        for(Bag b : bags)
        {
            if(b.color.compare(it->first) == 0)
            {
                if(canHold(b, findBag)) {
                    return true;
                }
            }
        }        
    }

    return false;
}

int contains(Bag bag)
{
    int count = 1;

    if(bag.contains.empty())
    {
        return 1;
    }

    std::map<std::string, int>::iterator it;
    for ( it = bag.contains.begin(); it != bag.contains.end(); it++ )
    {
        for(Bag b : bags)
        {
            if(b.color.compare(it->first) == 0)
            {
                count += (it->second) * contains(b);
            }
        }        
    }

    return count;
}

int part1()
{
    int count = 0;
    for(Bag bag : bags)
    {
        count += canHold(bag, "shiny gold");
    }

    return count;
}

int part2()
{
    for(Bag bag : bags)
    {
        if(bag.color.compare("shiny gold") == 0)
        {
            return contains(bag) - 1;
        }
    }
    return -1;
}

int main()
{
    initialiseMap();
    // printMap();

    // std::cout << part1() << std::endl;
    std::cout << part2() << std::endl;
}

