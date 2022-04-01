#include <iostream>
#include <string>
#include <vector>
#include "EnemyTrainar.h"
#include "RouteSearchAlgorithm/AStarMng.h"

using namespace std;

//マップ情報
vector<vector<string>> map_;
Position2 limitPos_;
//取り出しやすいように一時保存
Position2 townA_;
Position2 townB_;

//敵トレーナーリスト
vector<EnemyTrainar*> trainarList_;

//自分移動ルート
vector<Position2> satoshiPath_;

bool CreateMap(const string& str, Position2 pos)
{
    if (!(str == NORTH || str == SOUTH || str == WEST || str == EAST ||
        str == TOWN_A || str == TOWN_B || str == ROAD || str == WALL))
    {
        //未確認の文字
        cout << str << "\n";
        return false;
    }

    if (str == NORTH || str == SOUTH || str == WEST || str == EAST)
    {
        Direction dir;
        if (str == NORTH)
        {
            dir = Direction::NORTH;
        }
        if (str == SOUTH)
        {
            dir = Direction::SOUTH;
        }
        if (str == WEST)
        {
            dir = Direction::WEST;
        }
        if (str == EAST)
        {
            dir = Direction::EAST;
        }
        trainarList_.push_back(new EnemyTrainar(dir, pos));
    }

    map_[pos.y_][pos.x_] = str;
    return true;
};

void SaveTownAB(void)
{
    //AとB地点を保存
    Position2 defPos = Position2(-1, -1);
    Position2 townA = defPos;
    Position2 townB = defPos;

    bool flag = false;
    for (int i = 0; i < limitPos_.y_; i++)
    {
        for (int j = 0; j < limitPos_.x_; j++)
        {
            if (map_[i][j] == TOWN_A)
            {
                townA = Position2(j, i);
                if (townB != defPos)
                {
                    flag = true;
                    break;
                }
            }

            if (map_[i][j] == TOWN_B)
            {
                townB = Position2(j, i);
                if (townA != defPos)
                {
                    flag = true;
                    break;
                }
            }
        }
        if (flag)
        {
            break;
        }
    }

    townA_ = townA;
    townB_ = townB;
}

int GetBattleNum(Position2 pos)
{
    int cnt = 0;
    for (const auto& trainar : trainarList_)
    {
        if (trainar->IsBattlePos(pos))
        {
            cnt++;
        }
    }
    return cnt;
}

int main(int argc, char* argv[])
{
    //入力文字列
    string path;
    int index = STEP_LINE;

    while (index < GRID_START_LINE + limitPos_.y_)
    {
        getline(cin, path);
        //cout << "path[" << index++ << "]:" << path << "\n";

        if (index == GRID_HW_LINE)
        {
            //限界値を整数での取り出し
            limitPos_.y_ = atoi(path.substr(0, path.find_first_of(" ")).c_str());
            limitPos_.x_ = atoi(path.substr(path.find_first_of(" ")).c_str());

            //配列要素数の確定
            map_.resize(limitPos_.y_);
            for (int i = 0; i < limitPos_.y_; i++)
            {
                map_[i].resize(limitPos_.x_);
            }
        }

        if (GRID_START_LINE <= index && index < (GRID_START_LINE + limitPos_.y_))
        {
            //マップ生成（行ごとに）
            for (int i = 0; i < limitPos_.x_; i++)
            {
                if (!CreateMap(path.substr(i, 1), Position2(i, index - GRID_START_LINE)))
                {
                    //生成失敗
                    cout << "マップの生成に失敗しました\n";
                    break;
                }
            }
        }
        index++;
    }

    //タウンABの位置を保存
    SaveTownAB();

    //敵トレーナーの探索位置確定
    for (const auto& trainar : trainarList_)
    {
        trainar->CreateSearchList(map_);
    }

    //lpAStarMng.Create(limitPos_, townB_, map_);
    AStarMng* asMng = new AStarMng(limitPos_, townB_, map_);

    //サトシルート確定
    satoshiPath_ = asMng->AStarCreate(townA_);

    delete asMng;
    //lpAStarMng.Destroy();

    //AB間距離
    int distanceAB = static_cast<int>(satoshiPath_.size());

    //バトル回数
    int battleNum = 0;
    for (const auto& satoshiPos : satoshiPath_)
    {
        battleNum += GetBattleNum(satoshiPos);
    }

    cout << distanceAB << " " << battleNum;
    //{
    //   for (const auto& x : y)
    //    {
    //        cout << x;
    //    }
    //    cout << "\n";
    //}

    for (auto& trainar : trainarList_)
    {
        delete trainar;
    }
    trainarList_.clear();

    return 0;
}

