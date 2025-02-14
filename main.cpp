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

// ------------------------------- STRUCT / COMPARATORS -------------------------------

struct Zombie {
  string name;
  uint32_t hp;
  uint32_t dist;
  uint32_t speed;
  size_t round;
  //bool alive_status;
};

// Comparators: 
struct ZombieComparator {
    bool operator()(const Zombie* lhs, const Zombie* rhs) const {
        uint32_t eta1 = (lhs->dist / lhs->speed);
        uint32_t eta2 = (rhs->dist / rhs->speed);
        return (eta1 > eta2) || (eta1 == eta2 && (lhs->hp > rhs->hp || (lhs->hp == rhs->hp && lhs->name < rhs->name)));
    }
};

struct AliveZomb {
    bool operator()(const Zombie* lhs, const Zombie* rhs) const {
        return (lhs->round < rhs->round) || (lhs->round == rhs->round && lhs->name < rhs->name);
    }
};

struct LeastAliveZomb {
    bool operator()(const Zombie* lhs, const Zombie* rhs) const {
        return (lhs->round > rhs->round) || (lhs->round == rhs->round && lhs->name > rhs->name);
    }
};

struct MedianSmaller {
    bool operator()(const Zombie* lhs, const Zombie* rhs) const {
        return lhs->round < rhs->round;
    }
};

struct MedianLarger {
    bool operator()(const Zombie* lhs, const Zombie* rhs) const {
        return lhs->round > rhs->round;
    }
};

// ------------------------------- MAIN -------------------------------

int main(int argc, char *argv[]) {
    std::ios_base::sync_with_stdio(false);
    bool verbose = false, median = false, stats =false;
    bool eof = false;

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
    if (median) {std::cout << "median enabled." << endl;}

    // Read in our file:    
    uint32_t quiver_capacity;
    uint32_t randomSeed;
    uint32_t max_random_dist;
    uint32_t max_random_speed;
    uint32_t max_random_health;
    uint32_t currRound;
    string junk, junk1;

    // first line of code which is not needed
    getline(cin, junk);
    cin >> junk1 >> quiver_capacity >> junk1 >> randomSeed >> junk1
        >> max_random_dist >> junk1 >> max_random_speed >> junk1 >> max_random_health >> junk1 >> junk1 >> currRound;
    P2random::initialize(randomSeed, max_random_dist, max_random_speed, max_random_health);

    // Begin the Game:
    bool status = false;
    uint32_t refill_arrows = 0;
    uint32_t round_count = 1;
    string culprit = "";
    deque<Zombie *> zombs;

    // single round operands
    uint32_t num_zbies = 0;
    uint32_t named_zbies = 0;
    priority_queue<Zombie*, deque<Zombie*>, ZombieComparator> pq_zb;

    while (status == false) {
        if (verbose == true) {
            std::cout << "Round: " << round_count << "\n";
        }
        refill_arrows = quiver_capacity;
        std::cout << refill_arrows << endl;

        if (zombs.size() != 0) {
            // Updated order by the created ordering:
            for (Zombie *i : zombs) {
                uint32_t new_dist = 0;
                if (i->hp != 0) {
                    i->round = i->round +1;
                    new_dist = i->dist - min(i->dist, i->speed);
                    if (i->speed >= i->dist) {
                        i->dist = 0;
                        if (status == false) {
                            culprit = i->name;
                        }
                        status = true;
                    } else{
                        i->dist = new_dist;
                    }

                    if (verbose) std::cout << "Moved: " << i->name
                    << " (distance: " << i->dist << ", speed: " << i->speed
                    << ", health: " << i->hp << ")\n";
                }
            }
        }

        if (status == true) {
            // we must've died one step latter above:
            std::cout << "DEFEAT IN ROUND " << round_count << "! "
            << culprit << " ate your brains!\n";
            break; // eof, while loop
        }
        //std::cout << "Debug past checking for defeat via advanced ordering: [SUCCESS]\n";
        string junk_extra;
        if (round_count == currRound) { // COMPARE WITH EXISTING ROUND PASSED In
            std::cin >> junk_extra >> num_zbies >> junk_extra >> named_zbies;
            //std:: cout << num_zbies << " and " << named_zbies << endl;

            if (num_zbies > 0) {
                // initialize our random zombies
                size_t i =0;
                for (; i < num_zbies; ++i) {
                    std::string name_zb  = P2random::getNextZombieName();
                    uint32_t dist_zb = P2random::getNextZombieDistance();
                    uint32_t speed_zb    = P2random::getNextZombieSpeed();
                    uint32_t hp_zb   = P2random::getNextZombieHealth();
                    //std::cout << name_zb << " has " << hp_zb << "hp.\n";
                    //std::cout << "And " << dist_zb << " and " << speed_zb << ".\n";

                    // reference zombie obj
                    Zombie *ptr_zb = new Zombie;
                    ptr_zb->name = name_zb;
                    ptr_zb->dist = dist_zb;
                    ptr_zb->speed = speed_zb;
                    ptr_zb->hp= hp_zb;

                    zombs.push_back(ptr_zb);
                    pq_zb.push(ptr_zb);

                    if (verbose) {
                        std::cout << "Created: " << ptr_zb->name << " (distance: " << ptr_zb->dist
                        << " , speed: " << ptr_zb->speed << " , health: " << ptr_zb->hp << ")\n";
                    }
                }
            }

            // now finish the named zbies:
            //std::cout << "custom zombie names:\n";
            string newline, junk_i;
            uint32_t zb_custom_dist;
            uint32_t zb_custom_speed;
            uint32_t zb_custom_hp;
            while (cin >> newline) {
                if (newline != "---") {
                    //std:: cout << "Found 1.\n";
                    cin >> junk_i >> zb_custom_dist >> junk_i >> zb_custom_speed >> junk_i >> zb_custom_hp;
                    Zombie *custom_zb = new Zombie;
                    custom_zb->name = newline;
                    custom_zb->dist = zb_custom_dist;
                    custom_zb->speed = zb_custom_speed;
                    custom_zb->hp = zb_custom_hp;

                    zombs.push_back(custom_zb);
                    pq_zb.push(custom_zb);
                    if (verbose) {
                        cout << "Created: " << newline << " (distance: " << zb_custom_dist
                        << ", speed: " << zb_custom_speed << ", health: " << zb_custom_hp << ")\n";
                    }
                } else {
                    break;
                }
            }
            cin >> junk_i >> currRound;
            //std::cout << currRound << "\n";
            if (cin.fail()) eof = true;

            if (pq_zb.size() != 0) {
                cout << "Holy shit! Zombies!\n";
                cout << pq_zb.top()->name << " has arrived.\n";
            }
        }
        if (eof) {
            std::cout << "VICTORY!" << "\n";
            break;
        }
        round_count+=1;
        status = true;
    }

    return 0;
}