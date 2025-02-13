// Project identifier: 9504853406CBAC39EE89AA3AD238AA12CA198043
// EECS 281, W25: Project 2A
// kfukutom, Ken Fukutomi

#include <stdio.h>
#include <getopt.h>
#include <iostream>
#include "P2random.h"
using namespace std;

struct option longOpts[] = {
    {"help", no_argument, NULL, 'h'},
    {"verbose", no_argument, NULL, 'v'},
    {"statistics", no_argument, NULL, 's'},
    {"median", no_argument, NULL, 'm'},
    {nullptr, no_argument, NULL, 0},
};

// Initialize Structs for Varying Zombie(s) [States Included]
struct Zombie {
  string name;
  uint32_t hp;
  uint32_t dist;
  uint32_t speed;
  size_t round;
  bool alive_status;
};

// My Comparators:
struct ZombieComparator {
    bool operator()(const Zombie& lhs, const Zombie& rhs) const {
        if (lhs.dist < rhs.dist) {
            return true;
        } return false;
    }
};

struct LexicographicComp {
    // Compaere the Zombie Names Lexicographically:
    bool operator()(const Zombie& lhs, const Zombie& rhs) const {
        return lhs.name < rhs.name;
    }
};



// ------------------------------- MAIN -------------------------------

int main(int argc, char *argv[]) {
    std::ios_base::sync_with_stdio(false);
    bool verbose = false, median = false, stats =false;

    int index = 0, opt = 0;
    while ((opt = getopt_long(argc, argv, "hvsm", longOpts, &index)) != -1) {
        switch (opt) {
            case 'h':
                cout << "Usage: " << argv[0] << "[OPTIONS]\n"
                << "Options:\n"
                << "-h, --help                Show this help message\n"
                << "-v, --verbose             Enable verbos output\n"
                << "-s, --statistics          Enable statistics output\n"
                << "-m, --median              Enable median output\n";
                return 0;

            case 'v':
                verbose = true;
                break;
            
            case 's':
                stats = true;
                break;
            
            case 'm':
                median = true;
                break;
            
            default:
                std::cerr << "Please run --help / -h to see valid options.\n";
                return 1;
        }
    }

    if (verbose) {std::cout << "verbose enabled." << endl;}
    if (stats) {std::cout << "stats enabled." << endl; }
    if (median) std::cout << "median enabled." << endl;

    return 0;
}