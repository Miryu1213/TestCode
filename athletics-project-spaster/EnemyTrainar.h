#pragma once
#include <vector>
#include "common.h"

class EnemyTrainar
{
public:
    enum class STATE
    {
        NONE,
        BATTLED
    };

    EnemyTrainar(Direction dir, Position2 pos)
    {
        pos_ = pos;
        dir_ = dir;
        state_ = STATE::NONE;
    }

    void CreateSearchList(const vector<vector<int>>& map)
    {
        if (dir_ == Direction::NORTH)
        {
            //è„
            for (int y = pos_.y_ - 1; 0 <= y; y--)
            {
                if (map[y][pos_.x_] != ROADCOST)
                {
                    break;
                }
                searchList_.push_back(Position2(pos_.x_, y));
            }
        }
        if (dir_ == Direction::SOUTH)
        {
            //â∫
            for (int y = pos_.y_ + 1; y < map.size(); y++)
            {
                if (map[y][pos_.x_] != ROADCOST)
                {
                    break;
                }
                searchList_.push_back(Position2(pos_.x_, y));
            }
        }
        if (dir_ == Direction::WEST)
        {
            //ç∂
            for (int x = pos_.x_ - 1; 0 <= x; x--)
            {
                if (map[pos_.y_][x] != ROADCOST)
                {
                    break;
                }
                searchList_.push_back(Position2(x, pos_.y_));
            }
        }
        if (dir_ == Direction::EAST)
        {
            //âE
            for (int x = pos_.x_ + 1; x < map[pos_.y_].size(); x++)
            {
                if (map[pos_.y_][x] != ROADCOST)
                {
                    break;
                }
                searchList_.push_back(Position2(x, pos_.y_));
            }
        }
    }

    bool IsBattlePos(Position2 pos)
    {
        if (!IsNone())
        {
            //Ç∑Ç≈Ç…êÌÇ¡ÇƒÇ¢ÇÈèÍçá
            return false;
        }
        for (const auto& searchPos : searchList_)
        {
            if (pos == searchPos)
            {
                ToBattled();
                return true;
            }
        }
        return false;
    }

    const std::vector<Position2>& GetSearchList(void)
    {
        return searchList_;
    }

    bool IsNone(void)
    {
        return state_ == STATE::NONE;
    }
    void ToBattled(void)
    {
        state_ = STATE::BATTLED;
    }

private:

    Position2 pos_;
    Direction dir_;
    STATE state_;

    std::vector<Position2> searchList_;
};