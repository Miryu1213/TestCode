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
		//���v�R�X�g�̎Z�o
		return cost_ + heuristicCost_;
	}
	int GetHeuristicCost(void)
	{
		return heuristicCost_;
	}
	void CalcHeuristicCost(Position2 goalPos)
	{
		//�q���[���X�e�B�b�N�R�X�g�̌v�Z
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
		//�����̍��W��ǉ���A�e�̍��W��ǉ�����
		path->push_back(pos_);
		if (parent_ != nullptr)
		{
			parent_->GetPath(path);
		}
	}

private:
	//�m�[�h���
	STATE state_;
	//�m�[�h���W
	Position2 pos_;
	//�e�m�[�h
	ANode* parent_;
	//�q���[���X�e�B�b�N�R�X�g
	int heuristicCost_;
	//���R�X�g�i�X�^�[�g�n�_����̋����j
	int cost_;
};
