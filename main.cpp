#include "mbed.h"
#include "N5110.h"
#include <cstdio>
#include "Joystick.h" 
#include "MinigameEngine.h"
#include "Pet.h"
#include "Utils.h"

N5110 lcd(PC_7, PA_9, PB_10, PB_5, PB_3, PA_10);
//                  y     x
Joystick joystick(PC_3, PC_2);  

InterruptIn joystick_button(PC_10);
InterruptIn button_blue(BUTTON1); 
InterruptIn button1(PC_11);
InterruptIn button2(PD_2);
InterruptIn button3(PC_12);
BusOut leds_bus(PC_0, PC_1, PB_0);  //LSB, ..., MSB

Pet pet(lcd, leds_bus);
MinigameEngine minigame(lcd, joystick);

Ticker hunger_ticker;
Ticker health_ticker;
Ticker happiness_ticker;

int m_hunger = 0;
int m_energy = 0;
int m_happiness = 0;
int m_health = 0;
volatile int g_button1_flag = 0;
volatile int g_button2_flag = 0;
volatile int g_button3_flag = 0;
volatile int g_button_blue_flag = 0;

void hunger_timer_isr();
void health_timer_isr();
void energy_timer_isr();
void happiness_timer_isr();
void init();
void main_menu();
void button_blue_isr();
void button1_isr();
void button2_isr();
void button3_isr();

int main() {
    button_blue.mode(PullUp);
    button_blue.fall(&button_blue_isr);
    
    init();
    main_menu();
    hunger_ticker.attach(&hunger_timer_isr, 5s);
    health_ticker.attach(&health_timer_isr, 5s);
    happiness_ticker.attach(&happiness_timer_isr, 5s);
    while (1) {
        button1.rise(&button1_isr);
        button2.rise(&button2_isr);
        button3.rise(&button3_isr);
        if (minigame.get_game_running() == false) {

            pet.hunger_update();
            pet.energy_update();
            pet.happiness_update();
            pet.health_update();
        
            if (g_button1_flag && pet.get_health() > 0) {
                g_button1_flag = 0;
                m_hunger = 2;
                pet.set_hunger(3);
                hunger_ticker.attach(&hunger_timer_isr, 5s);
            }
            if (g_button2_flag && pet.get_health() > 0) {
                g_button2_flag = 0;
                m_energy = 3;
                pet.set_energy(3);
            }
            if (g_button3_flag && pet.get_health() > 0 && minigame.get_points_flag() == true) {
                g_button3_flag = 0;
                minigame.set_points_flag(false);
                m_happiness = 2;
                pet.set_happiness(3);
                happiness_ticker.attach(&happiness_timer_isr, 5s);
            }
            if (pet.get_hunger() == 3 && pet.get_energy() > 0 && pet.get_happiness() > 0) {
                m_health = 2;
            }
            if (pet.get_hunger() > 0 && pet.get_energy() == 3 && pet.get_happiness() > 0) {
                m_health = 2;
            }
            if (pet.get_hunger() > 0 && pet.get_energy() > 0 && pet.get_happiness() == 3) {
                m_health = 2;
            }            
        }
        
        if (minigame.get_game_running() == true) {
            minigame.start_game();
            minigame.update_game();
            minigame.reset_game();
        }
        ThisThread::sleep_for(100ms);
    }
}


void hunger_timer_isr(){
    if (pet.get_hunger() > 0 && g_button1_flag == 0 && minigame.get_game_running() == false) { 
        pet.set_hunger(m_hunger--);
    }
    if(pet.get_hunger() == 0) {
        hunger_ticker.detach();
    }
}

void health_timer_isr(){
    if(pet.get_hunger() == 0 || pet.get_energy() == 0 || pet.get_happiness() == 0) {
        if (minigame.get_game_running() == false) {
            pet.set_health(m_health--);
        }
    }
    if(pet.get_health() == 0) {
        health_ticker.detach();
    }
}

void happiness_timer_isr() {
    if (pet.get_happiness() > 0 && g_button3_flag == 0 && minigame.get_game_running() == false) {
        pet.set_happiness(m_happiness--);
    }
    if (pet.get_happiness() == 0) {
        happiness_ticker.detach();
    }
}

void init(){
    lcd.init(LPH7366_1);
    lcd.setContrast(0.5);
    lcd.setBrightness(0.45);
    lcd.clear();
    button1.mode(PullNone);
    button2.mode(PullNone);
    button3.mode(PullNone);
    joystick.init();
    joystick_button.mode(PullUp);
    m_hunger = 2; //somehow setting this to 3 makes the first decrease take 10s instead of 5s but setting it to 2 makes it works as it should
    m_energy = 3;
    m_happiness = 2;
    m_health = 2;
}

void main_menu(){
    init();
    lcd.printString(" Pedro's Life", 0, 0);
    lcd.printString("================", 0, 1);
    lcd.printString("     Press", 0, 3);
    lcd.printString("   Joystick", 0, 4);
    lcd.refresh();

    while (joystick_button.read() == 1) {
        ThisThread::sleep_for(100ms);
    }
}

void button_blue_isr(){
    g_button_blue_flag = 1;   
}

void button1_isr(){
    g_button1_flag = 1;
}

void button2_isr() {
    g_button2_flag = 1;
}

void button3_isr() {        
    g_button3_flag = 1;
    minigame.set_game_running(true);
    if (pet.get_energy() > 0) {
        m_energy--;
        pet.set_energy(m_energy);
    }
} 
