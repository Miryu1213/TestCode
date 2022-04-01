#pragma once
#include <vector>
#include <list>
#include "Bresenham.h"

#include "../common.h"

class ANode
{
public:
	enum class STATE
	{
		NONE,
		OPEN,
		CLOSED
	};

	ANode(Position2 pos)
	{
		pos_ = pos;
		state_ = STATE::NONE;
		cost_ = 0;
		heuristicCost_ = 0;
		parent_ = nullptr;
	}

	Position2 GetPos(void)
	{
		return pos_;
	}
	int GetCost(void)
	{
		return cost_;
	}
	int GetTotalCost(void)
	{
		//合計コストの算出
		return cost_ + heuristicCost_;
	}
	int GetHeuristicCost(void)
	{
		return heuristicCost_;
	}
	void CalcHeuristicCost(Position2 goalPos)
	{
		//ヒューリスティックコストの計算
		heuristicCost_ = static_cast<int>(Bresenham::GetFillPathFromBresenham(pos_, goalPos).size());
	}

	bool IsNone(void)
	{
		return state_ == STATE::NONE;
	}
	void OpenNode(ANode* parent, int cost)
	{
		state_ = STATE::OPEN;
		cost_ = cost;
		parent_ = parent;
	}
	void CloseNode(void)
	{
		state_ = STATE::CLOSED;
	}

	void GetPath(std::list<Position2>* path)
	{
		//自分の座標を追加後、親の座標を追加する
		path->push_back(pos_);
		if (parent_ != nullptr)
		{
			parent_->GetPath(path);
		}
	}

private:
	//ノード状態
	STATE state_;
	//ノード座標
	Position2 pos_;
	//親ノード
	ANode* parent_;
	//ヒューリスティックコスト
	int heuristicCost_;
	//実コスト（スタート地点からの距離）
	int cost_;
};
