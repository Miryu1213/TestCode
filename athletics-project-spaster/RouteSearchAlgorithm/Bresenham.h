#pragma once
#include <vector>
#include "../common.h"

//ブレゼンハムアルゴリズム
class Bresenham
{
public:
    [[nodiscard]] static std::vector<Position2> GetFillPathFromBresenham(Position2 sPoint, Position2 ePoint)
    {
        std::vector<Position2> path;

        // 二点間の距離
        Position2 distance;
        distance.x_ = (ePoint.x_ > sPoint.x_) ? ePoint.x_ - sPoint.x_ : sPoint.x_ - ePoint.x_;
        distance.y_ = (ePoint.y_ > sPoint.y_) ? ePoint.y_ - sPoint.y_ : sPoint.y_ - ePoint.y_;

        // 二点の方向
        Position2 dir;
        dir.x_ = (ePoint.x_ > sPoint.x_) ? 1 : -1;
        dir.y_ = (ePoint.y_ > sPoint.y_) ? 1 : -1;

        // 傾きが1より小さい場合
        if (distance.x_ > distance.y_)
        {
            //値に意味はなし、符号のみを活用する(小数を消すため公倍数である2(x1-x0)をかけている)
            int sign = -distance.x_;
            for (int i = 0; i <= distance.x_; i++)
            {
                path.push_back(sPoint);
                sPoint.x_ += dir.x_;
                sign += 2 * distance.y_;

                if ((dir.x_ > 0 && ePoint.x_ < sPoint.x_) ||
                    (dir.x_ < 0 && ePoint.x_ > sPoint.x_))
                {
                    //処理終了時
                    break;
                }

                if (sign >= 0)
                {
                    //隙間埋め
                    path.push_back(sPoint);
                    sPoint.y_ += dir.y_;
                    sign -= 2 * distance.x_;
                }
            }
        }
        else
        {
            /* 傾きが1以上の場合 */
            int sign = -distance.y_;
            for (int i = 0; i <= distance.y_; i++)
            {
                path.push_back(sPoint);
                sPoint.y_ += dir.y_;
                sign += 2 * distance.x_;

                if ((dir.y_ > 0 && ePoint.y_ < sPoint.y_) ||
                    (dir.y_ < 0 && ePoint.y_ > sPoint.y_))
                {
                    //処理終了時
                    break;
                }

                if (sign >= 0)
                {
                    //隙間埋め
                    path.push_back(sPoint);
                    sPoint.x_ += dir.x_;
                    sign -= 2 * distance.y_;
                }
            }
        }
        return path;
    }
};