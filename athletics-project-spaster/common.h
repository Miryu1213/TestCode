#pragma once
#include <string>

using namespace std;

const int STEP_LINE = 1;
const int GRID_HW_LINE = 2;
const int GRID_START_LINE = 3;

const string NORTH = "N";
const string SOUTH = "S";
const string WEST = "W";
const string EAST = "E";
const string TOWN_A = "A";
const string TOWN_B = "B";
const string ROAD = ".";
const string WALL = "#";

//コスト
//移動不可
const int CNTMOVECOST = 10000;
//道
const int ROADCOST = 2;
//トレーナー前
const int FRONTTRAINARCOST = 1;
//街
const int TOWNCOST = 0;

enum class Direction
{
    NORTH,
    SOUTH,
    EAST,
    WEST
};

struct Position2
{
    Position2()
    {
        x_ = 0;
        y_ = 0;
    }
    Position2(int x, int y)
    {
        x_ = x;
        y_ = y;
    }

    bool operator ==(const Position2& vec) const
    {
        if (x_ == vec.x_ && y_ == vec.y_)
        {
            return true;
        }
        return false;
    }

    bool operator !=(const Position2& vec) const
    {
        if (x_ != vec.x_ || y_ != vec.y_)
        {
            return true;
        }
        return false;
    }

    bool operator <(const Position2& vec) const
    {
        if (x_ < vec.x_ && y_ < vec.y_)
        {
            return true;
        }
        return false;
    }

    bool operator <=(const Position2& vec) const
    {
        if (x_ <= vec.x_ && y_ <= vec.y_)
        {
            return true;
        }
        return false;
    }

    bool operator >(const Position2& vec) const
    {
        if (x_ > vec.x_ && y_ > vec.y_)
        {
            return true;
        }
        return false;
    }

    bool operator >=(const Position2& vec) const
    {
        if (x_ >= vec.x_ && y_ >= vec.y_)
        {
            return true;
        }
        return false;
    }

    Position2 operator +(const Position2& vec) const
    {
        return { x_ + vec.x_, y_ + vec.y_ };
    }
    Position2 operator -(const Position2& vec) const
    {
        return { x_ - vec.x_, y_ - vec.y_ };
    }
    Position2 operator *(const Position2& vec) const
    {
        return { x_ * vec.x_, y_ * vec.y_ };
    }
    Position2 operator /(const Position2& vec) const
    {
        return { x_ / vec.x_, y_ / vec.y_ };
    }

    int x_;
    int y_;
};
