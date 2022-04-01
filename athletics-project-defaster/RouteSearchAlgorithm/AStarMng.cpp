#include <iostream>
#include "AStarMng.h"
#include "../common.h"

//���Ԃ̐錾
//AStarMng* AStarMng::s_Instance = nullptr;

std::list<Position2> AStarMng::AStarCreate(Position2 startPos)
{
	std::list<Position2> goalPath;

	//AStar�X�^�[�g
	ANode* node = OpenNode(startPos, 0, nullptr);

	//���s��(1000�񒴂���Β��f)
	int cnt = 0;
	while (cnt < 1000)
	{
		//�������I�[�v�����X�g����폜
		RemoveOpenList(node);
		node->CloseNode();

		//���͂��J��
		OpenAround(node);

		//�ŏ����v�R�X�g�̃m�[�h��T��
		node = SearchMinTotalCostNodeFromOpenList();
		if (node == nullptr)
		{
			//�܏��H
			cout << "�܏��H�ł�\n";
			break;
		}
		if (node->GetPos() == goalPos_)
		{
			//�S�[��
			RemoveOpenList(node);
			node->GetPath(&goalPath);

			//�p�X�𔽓]
			goalPath.pop_back();
			goalPath.reverse();

			break;
		}
		cnt++;
	}
	for (auto& node : nodePool_)
	{
		delete node.second;
	}
	nodePool_.clear();

	return goalPath;
}

ANode* AStarMng::GetNode(Position2 pos)
{
	int arX = ToOneArray(pos);
	//�v�[���ɂ���ꍇ�͂����炩��擾
	if (nodePool_.count(arX) != 0)
	{
		return nodePool_[arX];
	}

	//�Ȃ��ꍇ����
	nodePool_[arX] = new ANode(pos);
	nodePool_[arX]->CalcHeuristicCost(goalPos_);
	return nodePool_[arX];
}

void AStarMng::AddOpenList(ANode* node)
{
	openList_.push_back(node);
}

void AStarMng::RemoveOpenList(ANode* node)
{
	openList_.remove(node);
}

ANode* AStarMng::OpenNode(Position2 pos, int cost, ANode* parent)
{
	if (pos.x_ < 0 || limitPos_.x_ <= pos.x_ || pos.y_ < 0 || limitPos_.y_ <= pos.y_)
	{
		//�͈͊O
		return nullptr;
	}
	if (!(costMap_[pos.y_][pos.x_] == TOWN_A || costMap_[pos.y_][pos.x_] == TOWN_B ||
		costMap_[pos.y_][pos.x_] == ROAD))
	{
		//�ړ��s�}�X
		return nullptr;
	}

	//�m�[�h�̎擾
	auto node = GetNode(pos);
	if (!node->IsNone())
	{
		//���łɃI�[�v�����Ă���̂ŉ������Ȃ�
		return nullptr;
	}

	//�m�[�h�I�[�v��
	node->OpenNode(parent, cost);
	AddOpenList(node);

	return node;
}

void AStarMng::OpenAround(ANode* parent)
{
	Position2 pPos = parent->GetPos();
	int pCost = parent->GetCost();
	//4�������J��.
	OpenNode(pPos + Position2(1, 0), pCost, parent); //�E
	OpenNode(pPos - Position2(0, 1), pCost, parent); //��
	OpenNode(pPos - Position2(1, 0), pCost, parent); //��
	OpenNode(pPos + Position2(0, 1), pCost, parent); //��
}

ANode* AStarMng::SearchMinTotalCostNodeFromOpenList(void)
{
	//�ŏ��g�[�^���R�X�g
	int minTotalCost = 10000;
	//�ŏ��q���[���X�e�B�b�N�R�X�g
	int minheuCost = 10000;
	//�ŏ��m�[�h
	ANode* minNode = nullptr;

	for (const auto& node : openList_)
	{
		int totalCost = node->GetTotalCost();
		if (totalCost > minTotalCost)
		{
			//���݂��傫��
			continue;
		}
		if (totalCost == minTotalCost && node->GetHeuristicCost() >= minheuCost)
		{
			//�g�[�^���R�X�g���������S�[���܂ł̋������傫���Ƃ�
			continue;
		}

		//�ŏ��X�V
		minTotalCost = totalCost;
		minheuCost = node->GetHeuristicCost();
		minNode = node;
	}
	return minNode;
}

int AStarMng::ToOneArray(Position2 pos)
{
	return pos.y_ * limitPos_.x_ + pos.x_;
}

AStarMng::AStarMng(Position2 limitPos, Position2 goalPos, const std::vector<std::vector<std::string>>& costMap)
{
	limitPos_ = limitPos;
	goalPos_ = goalPos;
	costMap_ = costMap;
}
