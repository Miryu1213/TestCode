#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "EnemyTrainar.h"
#include "RouteSearchAlgorithm/AStarMng.h"

#include <crtdbg.h>
#define malloc(X) _malloc_dbg(X.NORMAL_BLOCK,__FILE,__LINE__)
#define new ::new(_NORMAL_BLOCK,__FILE__,__LINE__)

using namespace std;

//マップ情報
vector<vector<int>> map_;
Position2 limitPos_;
//取り出しやすいように一時保存
Position2 townA_;
Position2 townB_;

//敵トレーナーリスト
vector<EnemyTrainar*> trainarList_;

//自分移動ルート
list<Position2> satoshiPath_;

int ConvertCost(const string& str)
{
    int cost = 0;
    if (str == NORTH)
    {
        cost = CNTMOVECOST;
    }
    if (str == SOUTH)
    {
        cost = CNTMOVECOST;
    }
    if (str == WEST)
    {
        cost = CNTMOVECOST;
    }
    if (str == EAST)
    {
        cost = CNTMOVECOST;
    }
    if (str == TOWN_A)
    {
        cost = TOWNCOST;
    }
    if (str == TOWN_B)
    {
        cost = TOWNCOST;
    }
    if (str == ROAD)
    {
        cost = ROADCOST;
    }
    if (str == WALL)
    {
        cost = CNTMOVECOST;
    }
    return cost;
}

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

    //街座標の保存
    if (str == TOWN_A)
    {
        townA_ = pos;
    }
    if (str == TOWN_B)
    {
        townB_ = pos;
    }

    map_[pos.y_][pos.x_] = ConvertCost(str);
    return true;
};

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
    _CrtSetDbgFlag(_CRTDBG_CHECK_ALWAYS_DF);
    _CrtSetDbgFlag(_CRTDBG_CHECK_CRT_DF);
    _CrtSetDbgFlag(_CRTDBG_DELAY_FREE_MEM_DF);
    _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF);

    //入力文字列
    //string buf;
    int index = STEP_LINE;

    std::ifstream ifs("basic/basic_3_1_random_00.in");
    if (!ifs) 
    {
        std::cerr << "ファイルオープンに失敗" << std::endl;
        std::exit(1);
    }

    //while (index < GRID_START_LINE + limitPos_.y_)
    while (index < GRID_START_LINE + limitPos_.y_)
    {
        //getline(cin, buf);
        //cout << "buf[" << index++ << "]:" << buf << "\n";

        std::string buf;
        std::getline(ifs, buf);

        if (index == GRID_HW_LINE)
        {
            //限界値を整数での取り出し
            limitPos_.y_ = atoi(buf.substr(0, buf.find_first_of(" ")).c_str());
            limitPos_.x_ = atoi(buf.substr(buf.find_first_of(" ")).c_str());

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
                if (!CreateMap(buf.substr(i, 1), Position2(i, index - GRID_START_LINE)))
                {
                    //生成失敗
                    cout << "マップの生成に失敗しました\n";
                    break;
                }
            }
        }
        index++;
    }

    //敵トレーナーの探索位置確定
    for (const auto& trainar : trainarList_)
    {
        trainar->CreateSearchList(map_);
    }

    //トレーナー索敵位置のコスト変更
    for (const auto& trainar : trainarList_)
    {
        for (const auto& searchPos : trainar->GetSearchList())
        {
            map_[searchPos.y_][searchPos.x_] = FRONTTRAINARCOST;
        }
    }

    //lpAStarMng.Create(limitPos_, townB_, map_);
    AStarMng* asMng = new AStarMng(limitPos_, townB_, map_);

    //サトシルート確定
    satoshiPath_ = asMng->AStarCreateSP(townA_, trainarList_);
    //satoshiPath_ = asMng->AStarCreateN(townA_);

    //画面出力
    for (int y = 0; y < map_.size(); y++)
    {
        for (int x = 0; x < map_[y].size(); x++)
        {
            bool flag = false;
            for (const auto& pos : satoshiPath_)
            {
                if (pos == Position2(x, y))
                {
                    flag = true;
                    break;
                }
            }
            if (flag)
            {
                cout << "o";
            }
            else
            {
                cout << map_[y][x];
            }
        }
        cout << "\n";
    }

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

    delete asMng;
    ifs.close();

    for (auto& trainar : trainarList_)
    {
        delete trainar;
    }
    trainarList_.clear();

    return 0;
}

