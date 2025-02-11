// Project identifier: 9504853406CBAC39EE89AA3AD238AA12CA198043
// EECS 281, W25: Project 2A
// kfukutom, Ken Fukutomi

#include <stdio.h>
#include <getopt.h>
#include "P2random.h"
using namespace std;

struct option longopts[] = {
    {"help", no_argument, NULL, 'h'},
    {"verbose", no_argument, NULL, 'v'},
    {"statistics", no_argument, NULL, 's'},
    {"median", no_argument, NULL, 'm'},
    {nullptr, no_argument, NULL, 0},
};

// My Comparators:
struct ZombieComparator {
  bool operator() (const Zombie& z1, const Zombie& z2) const {
    if (z1.zombies > z2.zombies) {
      return z1.zombies > z2.zombies;
    }
  }
}

// Initialize Structs for Varying Zombie(s) [States Included]
struct Zombie {
  string name;
  unsigned int hp;
  unsigned int dist;
  unsigned int round;
};

int main(int argc, char *argv[]) {

}