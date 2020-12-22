#include <iostream>
#include <fstream>
#include <vector>
#include <string>

/*
--- Day 17: Conway Cubes ---
As your flight slowly drifts through the sky, the Elves at the Mythical Information Bureau at the North Pole contact you. They'd like some help debugging a malfunctioning experimental energy source aboard one of their super-secret imaging satellites.

The experimental energy source is based on cutting-edge technology: a set of Conway Cubes contained in a pocket dimension! When you hear it's having problems, you can't help but agree to take a look.

The pocket dimension contains an infinite 3-dimensional grid. At every integer 3-dimensional coordinate (x,y,z), there exists a single cube which is either active or inactive.

In the initial state of the pocket dimension, almost all cubes start inactive. The only exception to this is a small flat region of cubes (your puzzle input); the cubes in this region start in the specified active (#) or inactive (.) state.

The energy source then proceeds to boot up by executing six cycles.

Each cube only ever considers its neighbors: any of the 26 other cubes where any of their coordinates differ by at most 1. For example, given the cube at x=1,y=2,z=3, its neighbors include the cube at x=2,y=2,z=2, the cube at x=0,y=2,z=3, and so on.

During a cycle, all cubes simultaneously change their state according to the following rules:

If a cube is active and exactly 2 or 3 of its neighbors are also active, the cube remains active. Otherwise, the cube becomes inactive.
If a cube is inactive but exactly 3 of its neighbors are active, the cube becomes active. Otherwise, the cube remains inactive.
The engineers responsible for this experimental energy source would like you to simulate the pocket dimension and determine what the configuration of cubes should be at the end of the six-cycle boot process.

For example, consider the following initial state:

.#.
..#
###
Even though the pocket dimension is 3-dimensional, this initial state represents a small 2-dimensional slice of it. (In particular, this initial state defines a 3x3x1 region of the 3-dimensional space.)

Simulating a few cycles from this initial state produces the following configurations, where the result of each cycle is shown layer-by-layer at each given z coordinate:

Before any cycles:

                          z=0
                          .#.
                          ..#
                          ###


After 1 cycle:

              z=-1        z=0        z=1
              #..         #.#        #..
              ..#         .##        ..#
              .#.         .#.        .#.
        

After 2 cycles:

 z=-2        z=-1        z=0         z=1         z=2
 .....       ..#..       ##...       ..#..       .....
 .....       .#..#       ##...       .#..#       .....
 ..#..       ....#       #....       ....#       ..#..
 .....       .#...       ....#       .#...       .....
 .....       .....       .###.       .....       .....


After 3 cycles:

z=-2        z=-1        z=0         z=1         z=2
.......     ..#....     ...#...     ..#....     .......
.......     ...#...     .......     ...#...     .......
..##...     #......     #......     #......     ..##...
..###..     .....##     .......     .....##     ..###..
.......     .#...#.     .....##     .#...#.     .......
.......     ..#.#..     .##.#..     ..#.#..     .......
.......     ...#...     ...#...     ...#...     .......


After the full six-cycle boot process completes, 112 cubes are left in the active state.

Starting with your given initial configuration, simulate six cycles. How many cubes are left in the active state after the sixth cycle?
*/

std::vector<std::vector<std::vector<bool>>> source(8, std::vector<std::vector<bool>>(8, std::vector<bool>(9)));

void initialise() {
    std::ifstream file("../input/day17.txt");
    std::string row;

    getline(file, row);
    for(int y = 0; y < source.size(); y++) {
        for(int x = 0; x < source.size(); x++) {
            source[(source.size()/2) + 1][y][x] = (row[x] == '#');
        }
        getline(file, row);
    }
}

bool returnValue(int x, int y, int z) {
    if(z >= source.size() || y >= source[z].size() || x >= source[z][y].size()) {
        return false;
    }

    return source[z][y][x];
}

int getActiveAround(int x, int y, int z) {
    
    if(z <= 1 || z >= source.size() || y <= 1 || y >= source[z].size() || x <= 1 || x >= source[z][y].size()) {
        return 0;
    }

    int active = 0;

    // Top
    active += returnValue(x-1, y+1, z+1);
    active += returnValue(x, y+1, z+1);
    active += returnValue(x+1, y+1, z+1);

    active += returnValue(x-1, y+1, z);
    active += returnValue(x, y+1, z);
    active += returnValue(x+1, y+1, z);

    active += returnValue(x-1, y+1, z-1);
    active += returnValue(x, y+1, z-1);
    active += returnValue(x+1, y+1, z-1);

    // Bottom
    active += returnValue(x-1, y-1, z+1);
    active += returnValue(x, y-1, z+1);
    active += returnValue(x+1, y-1, z+1);

    active += returnValue(x-1, y-1, z);
    active += returnValue(x, y-1, z);
    active += returnValue(x+1, y-1, z);

    active += returnValue(x-1, y-1, z-1);
    active += returnValue(x, y-1, z-1);
    active += returnValue(x+1, y-1, z-1);

    // Around
    active += returnValue(x-1, y, z-1);
    active += returnValue(x-1, y, z);
    active += returnValue(x-1, y, z+1);
    active += returnValue(x, y, z-1);
    active += returnValue(x, y, z+1);
    active += returnValue(x+1, y, z-1);
    active += returnValue(x+1, y, z);
    active += returnValue(x+1, y, z+1);

    return active;
}

int cycle() {
    std::vector<std::vector<std::vector<bool>>> updatedSource(source.size()+2, std::vector<std::vector<bool>>(source.size()+2, std::vector<bool>(source.size()+3)));

    for(int z = 0; z < source.size(); z++) {
        for(int y = 0; y < source[z].size(); y++) {
            for(int x = 0; x < source[y].size(); x++) {
                updatedSource[z+1][y+1][x+1] = source[z][y][x];
            }
        }
    }

    int totalActive = 0;

    for(int z = 0; z < updatedSource.size(); z++) {
        for(int y = 0; y < updatedSource[z].size(); y++) {
            for(int x = 0; x < updatedSource[y].size(); x++) {
                int activeAround = getActiveAround(x, y, z);

                // count number around it
                if(updatedSource[z][y][x] && activeAround != 2 && activeAround != 3) {
                    updatedSource[z][y][x] == false;
                }
                else if(!updatedSource[z][y][x] && activeAround == 3) {
                    updatedSource[z][y][x] = true;
                }

                totalActive += updatedSource[z][y][x];
            }
        }
    }

    source = updatedSource;

    return totalActive;
}

int part1() {
    initialise();
    
    int totalActive = 0;

    for(int i = 0; i < 6; i++) {
        totalActive = cycle();
    }

    return totalActive;
}

int main()
{    
    std::cout << "Part 1: " << part1() << std::endl;
}

// x: source.size()
// y: source[x].size()
// z: source[y][x].size()
// value: source[z][y][x]
