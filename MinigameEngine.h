#ifndef MINIGAME_ENGINE_H
#define MINIGAME_ENGINE_H

#include "N5110.h"
#include "Joystick.h"
#include "Utils.h"

class MinigameEngine {
public:
    MinigameEngine(N5110 &lcd, Joystick &joystick);
    void start_game();
    void update_game();
    void reset_game();
    void set_game_running(volatile bool gameRunning);
    void set_points_flag(bool _pointsFlag);
    bool get_points_flag();
    bool get_game_running();

private:
    N5110 &_lcd;
    Joystick &_joystick;
    static const int Apple[6][5];
    Position2D _player_pos;
    Position2D _collectible_pos;
    Timer _timer;
    volatile bool _game_running;
    bool _collectible_active;
    bool _points_flag;
    int _points;
    float _next_collectible_time;

    void move();
    void check_collect();
    void boundary();
}; 

#endif