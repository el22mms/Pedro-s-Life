#ifndef PET_H
#define PET_H

#include "N5110.h"
#include "Utils.h"

class Pet {
public:
    Pet(N5110 &lcd, BusOut &leds_bus);
    void hunger_update();
    void energy_update();
    void happiness_update();
    void health_update();
    void set_hunger(int hunger);
    void set_energy(int energy);
    void set_happiness(int happiness);
    void set_health(int health);
    int get_hunger();
    int get_energy();
    int get_happiness();
    int get_health();

private:
    N5110 &_lcd;
    BusOut &_leds_bus;
    static const int LightningBolt[10][10];
    static const int DrumStick[10][10];
    static const int SmileyHappy[10][10];
    static const int SmileyNeutral[10][10];
    static const int SmileySad[10][10];
    static const int Pedro[32][44];
    int _hunger;
    int _energy;
    int _happiness;
    int _health;
}; 

#endif