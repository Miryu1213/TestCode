#pragma once
#include <vector>
#include "../common.h"

//�u���[���n���A���S���Y��
class Bresenham
{
public:
    [[nodiscard]] static std::vector<Position2> GetFillPathFromBresenham(Position2 sPoint, Position2 ePoint)
    {
        std::vector<Position2> path;

        // ��_�Ԃ̋���
        Position2 distance;
        distance.x_ = (ePoint.x_ > sPoint.x_) ? ePoint.x_ - sPoint.x_ : sPoint.x_ - ePoint.x_;
        distance.y_ = (ePoint.y_ > sPoint.y_) ? ePoint.y_ - sPoint.y_ : sPoint.y_ - ePoint.y_;

        // ��_�̕���
        Position2 dir;
        dir.x_ = (ePoint.x_ > sPoint.x_) ? 1 : -1;
        dir.y_ = (ePoint.y_ > sPoint.y_) ? 1 : -1;

        // �X����1��菬�����ꍇ
        if (distance.x_ > distance.y_)
        {
            //�l�ɈӖ��͂Ȃ��A�����݂̂����p����(�������������ߌ��{���ł���2(x1-x0)�������Ă���)
            int sign = -distance.x_;
            for (int i = 0; i <= distance.x_; i++)
            {
                path.push_back(sPoint);
                sPoint.x_ += dir.x_;
                sign += 2 * distance.y_;

                if ((dir.x_ > 0 && ePoint.x_ < sPoint.x_) ||
                    (dir.x_ < 0 && ePoint.x_ > sPoint.x_))
                {
                    //�����I����
                    break;
                }

                if (sign >= 0)
                {
                    //���Ԗ���
                    path.push_back(sPoint);
                    sPoint.y_ += dir.y_;
                    sign -= 2 * distance.x_;
                }
            }
        }
        else
        {
            /* �X����1�ȏ�̏ꍇ */
            int sign = -distance.y_;
            for (int i = 0; i <= distance.y_; i++)
            {
                path.push_back(sPoint);
                sPoint.y_ += dir.y_;
                sign += 2 * distance.x_;

                if ((dir.y_ > 0 && ePoint.y_ < sPoint.y_) ||
                    (dir.y_ < 0 && ePoint.y_ > sPoint.y_))
                {
                    //�����I����
                    break;
                }

                if (sign >= 0)
                {
                    //���Ԗ���
                    path.push_back(sPoint);
                    sPoint.x_ += dir.x_;
                    sign -= 2 * distance.y_;
                }
            }
        }
        return path;
    }
};