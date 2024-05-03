#include "MinigameEngine.h"

const int MinigameEngine::Apple[6][5]= {
    { 0,0,1,1,0, },
    { 0,0,1,0,0, },
    { 0,1,1,1,0, },
    { 1,1,1,1,1, },
    { 1,1,1,1,1, },
    { 0,1,1,1,0, },
    
};

MinigameEngine::MinigameEngine(N5110 &lcd, Joystick &joystick)
    : _lcd(lcd), _joystick(joystick), _player_pos({42, 24}), _game_running(false), _collectible_active(false), _points_flag(false), _points(0), _next_collectible_time(5.0) {}

void MinigameEngine::start_game() {
    _timer.start();
    _collectible_active = false;
    _points = 0;
    _next_collectible_time = 5.0;
}

void MinigameEngine::update_game() {
    //_timer.start();
    while (_timer.read() <= 25 && _points < 3) {
        _game_running = true;
        if (_timer.read() >= _next_collectible_time && !_collectible_active) {
            _collectible_pos.x = rand() % 77; //randomise pos
            _collectible_pos.y = rand() % 41;
            _collectible_active = true;
            _next_collectible_time += 5.0;
        }

        _lcd.clear();
        _lcd.drawRect(0, 0, 84, 48, FILL_TRANSPARENT);
        _lcd.drawRect(_player_pos.x, _player_pos.y, 2, 2, FILL_BLACK);

        if (_collectible_active) {
           _lcd.drawSprite(_collectible_pos.x, _collectible_pos.y, 6, 5, (int*)Apple);
        }

        move();
        check_collect();
        boundary();

        _lcd.refresh();
        ThisThread::sleep_for(30ms);

        if (_points == 3) {
            _points_flag = true;
        }
    }
    
}

void MinigameEngine::reset_game() {
    _timer.stop();
    _timer.reset();
    _collectible_active = false;
    _points = 0;
    _game_running = false;
    
}

void MinigameEngine::set_game_running(volatile bool game_running) {
    _game_running = game_running;

}

void MinigameEngine::set_points_flag(bool points_flag) {
    _points_flag = points_flag;
}

bool MinigameEngine::get_points_flag() {
    return _points_flag;
}

bool MinigameEngine::get_game_running() {
    return _game_running;
}

void MinigameEngine::move() {
    if (_joystick.get_direction() == N) {
        _player_pos.y--;
    }
    else if (_joystick.get_direction() == S) {
        _player_pos.y++;
    }
    else if (_joystick.get_direction() == E) {
        _player_pos.x++;
    }
    else if (_joystick.get_direction() == W) {
        _player_pos.x--;
    }
    else if (_joystick.get_direction() == NE) {
        _player_pos.y--;
        _player_pos.x++;
    }
    else if (_joystick.get_direction() == NW) {
        _player_pos.y--;
        _player_pos.x--;
    }
    else if (_joystick.get_direction() == SE) {
        _player_pos.y++;
        _player_pos.x++;
    }
    else if (_joystick.get_direction() == SW) {
        _player_pos.y++;
        _player_pos.x--;
    }
    
}

void MinigameEngine::check_collect() {
    if (_collectible_active && _player_pos.x >= _collectible_pos.x && _player_pos.x <= _collectible_pos.x + 5 && _player_pos.y >= _collectible_pos.y && _player_pos.y <= _collectible_pos.y + 6) {
        _collectible_active = false;
        _points++;
    }
}

void MinigameEngine::boundary() {
    if (_player_pos.x < 1) _player_pos.x = 1;
    else if (_player_pos.x > 81) _player_pos.x = 81;

    if (_player_pos.y < 1) _player_pos.y = 1;
    else if (_player_pos.y > 45) _player_pos.y = 45;
}