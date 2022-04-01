#include <iostream>
#include <string>
#include <vector>
#include "EnemyTrainar.h"
#include "RouteSearchAlgorithm/AStarMng.h"

using namespace std;

//�}�b�v���
vector<vector<string>> map_;
Position2 limitPos_;
//���o���₷���悤�Ɉꎞ�ۑ�
Position2 townA_;
Position2 townB_;

//�G�g���[�i�[���X�g
vector<EnemyTrainar*> trainarList_;

//�����ړ����[�g
vector<Position2> satoshiPath_;

bool CreateMap(const string& str, Position2 pos)
{
    if (!(str == NORTH || str == SOUTH || str == WEST || str == EAST ||
        str == TOWN_A || str == TOWN_B || str == ROAD || str == WALL))
    {
        //���m�F�̕���
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
    //A��B�n�_��ۑ�
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
    //���͕�����
    string path;
    int index = STEP_LINE;

    while (index < GRID_START_LINE + limitPos_.y_)
    {
        getline(cin, path);
        //cout << "path[" << index++ << "]:" << path << "\n";

        if (index == GRID_HW_LINE)
        {
            //���E�l�𐮐��ł̎��o��
            limitPos_.y_ = atoi(path.substr(0, path.find_first_of(" ")).c_str());
            limitPos_.x_ = atoi(path.substr(path.find_first_of(" ")).c_str());

            //�z��v�f���̊m��
            map_.resize(limitPos_.y_);
            for (int i = 0; i < limitPos_.y_; i++)
            {
                map_[i].resize(limitPos_.x_);
            }
        }

        if (GRID_START_LINE <= index && index < (GRID_START_LINE + limitPos_.y_))
        {
            //�}�b�v�����i�s���ƂɁj
            for (int i = 0; i < limitPos_.x_; i++)
            {
                if (!CreateMap(path.substr(i, 1), Position2(i, index - GRID_START_LINE)))
                {
                    //�������s
                    cout << "�}�b�v�̐����Ɏ��s���܂���\n";
                    break;
                }
            }
        }
        index++;
    }

    //�^�E��AB�̈ʒu��ۑ�
    SaveTownAB();

    //�G�g���[�i�[�̒T���ʒu�m��
    for (const auto& trainar : trainarList_)
    {
        trainar->CreateSearchList(map_);
    }

    //lpAStarMng.Create(limitPos_, townB_, map_);
    AStarMng* asMng = new AStarMng(limitPos_, townB_, map_);

    //�T�g�V���[�g�m��
    satoshiPath_ = asMng->AStarCreate(townA_);

    delete asMng;
    //lpAStarMng.Destroy();

    //AB�ԋ���
    int distanceAB = static_cast<int>(satoshiPath_.size());

    //�o�g����
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

