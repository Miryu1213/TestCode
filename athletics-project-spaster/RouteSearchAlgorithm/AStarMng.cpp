#include <iostream>
#include <chrono>
#include "AStarMng.h"
#include "../common.h"

//���Ԃ̐錾
//AStarMng* AStarMng::s_Instance = nullptr;

using namespace std::chrono;

AStarMng::AStarMng(Position2 limitPos, Position2 goalPos, const std::vector<std::vector<int>>& costMap)
{
	limitPos_ = limitPos;
	goalPos_ = goalPos;
	costMap_ = costMap;

	recoveryFlag_ = false;
}

std::list<Position2> AStarMng::AStarCreateN(Position2 startPos)
{
	//�ʏ�A*
	std::list<Position2> goalPath;

	//AStar�X�^�[�g
	ANode* node = OpenNode(startPos, 0, nullptr, true);

	//���s��(1000�񒴂���Β��f)
	int cnt = 0;
	while (1)
	{
		//�������I�[�v�����X�g����폜
		RemoveOpenList(node);
		node->CloseNode();

		//���͂��J��
		OpenAround(node, true);

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
			goalPath.reverse();

			break;
		}
		cnt++;
	}
	
	Release();

	return goalPath;
}

std::list<Position2> AStarMng::AStarCreateSP(Position2 startPos, const std::vector<EnemyTrainar*>& trainarList)
{
	//���ׂĂ̍ŒZ�o�H�T��A*
	std::list<std::list<Position2>> goalPathList;

	//���O�ɍŒZ�o�H�������Ă���
	auto prePath = AStarCreateN(startPos);
	goalPathList.push_back(prePath);
	int toGoalDistance = prePath.size();

	//AStar�X�^�[�g
	ANode* node = OpenNode(startPos, 0, nullptr, false);

	//���ԕۑ��n
	system_clock::time_point StartTime = system_clock::now();									//�J�n���̓���
	microseconds microstarttime = duration_cast<milliseconds>(StartTime.time_since_epoch());	//���݂̃V�X�e������
	system_clock::time_point nowTime;															//���݂̓���
	microseconds elaptime = microstarttime;
	microseconds oldelaptime;

	double delta = 0;
	double totalDelta = 0;
	

	//���s��(1000�񒴂���Β��f)
	int cnt = 0;
	while (1)
	{
		oldelaptime = elaptime;																	//��O�̎��Ԃ̍X�V
		nowTime = system_clock::now();															//���݂̓���
		elaptime = duration_cast<microseconds>(nowTime.time_since_epoch());
		delta = static_cast<double>((elaptime - oldelaptime).count() * 0.000001f);
		totalDelta += delta;

		if (openList_.empty())
		{
			//�T���I��
			break;
		}
		//�������I�[�v�����X�g����폜
		RemoveOpenList(node);
		node->CloseNode();

		//�p�X�̐��ɂ���đł��؂�
		int pathCnt = 0;
		node->GetPathNum(pathCnt);
		if (pathCnt <= prePath.size())
		{
			//���͂��J��
			OpenAround(node, false);
		}

		//�ŏ����v�R�X�g�̃m�[�h��T��
		node = SearchMinTotalCostNodeFromOpenList();

		if (node == nullptr)
		{
			//�I�[�v���m�[�h�I��
			break;
		}

		//�S�[����̏�ԉ�
		if (recoveryFlag_)
		{
			node->CloseNodeParent();
		}

		if (node->GetPos() == goalPos_)
		{
			//�S�[��
			std::list<Position2> tmpPath;
			node->GetPath(&tmpPath);
			
			if (tmpPath.size() <= toGoalDistance)
			{
				//�S�[���ւ̃p�X���X�g�֒ǉ�
				goalPathList.push_back(tmpPath);
				toGoalDistance = tmpPath.size();
				
				//�I�[�v���m�[�h�ȊO��None��
				ToNoneOtherOpenNode();
				recoveryFlag_ = true;

				//�S�[���܂ł̃m�[�h�����ׂĉ��
				//node->ReleaseNode(openList_);
			}
		}
		cnt++;

		if (cnt % 10000 == 0)
		{
			cout << "10000\n";
		}
	}

	cout << totalDelta << "\n";

	//�S�[���܂ł̃p�X�Ńg���[�i�[�Ɠ�����񐔂��������̂����o��
	std::list<Position2> tmpPath;
	int macCnt = 0;

	for (const auto& goalPath : goalPathList)
	{
		int cnt = 0;
		for (const auto& trainar : trainarList)
		{
			bool flag = false;
			for (const auto& pPos : goalPath)
			{
				for (const auto& searchPos : trainar->GetSearchList())
				{
					if (searchPos == pPos)
					{
						cnt++;
						flag = true;
						break;
					}
				}
				if (flag)
				{
					//�������W���l�̃g���[�i�[�����邱�Ƃ͂Ȃ�����
					break;
				}
			}
		}
		if (cnt > macCnt && goalPath.size() <= toGoalDistance)
		{
			//�g���[�i�[�Ɠ�����񐔂��������̂����o��
			macCnt = cnt;
			tmpPath = goalPath;
		}
	}

	if (tmpPath.empty())
	{
		//�g���[�i�[�Ɠ�����Ȃ��ꍇ
		tmpPath = goalPathList.back();
	}

	tmpPath.pop_back();
	tmpPath.reverse();

	//std::vector<Position2> reversePath;
	////�p�X�𔽓](�X�^�[�g�̃}�X�͊܂߂Ȃ�)
	//for (auto itr = tmpPath. .end() - 1; itr != tmpPath.begin(); itr--)
	//{
	//	reversePath.push_back((*itr));
	//}

	Release();

	return tmpPath;
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

ANode* AStarMng::OpenNode(Position2 pos, int cost, ANode* parent, bool normal)
{
	if (pos.x_ < 0 || limitPos_.x_ <= pos.x_ || pos.y_ < 0 || limitPos_.y_ <= pos.y_)
	{
		//�͈͊O
		return nullptr;
	}
	if (costMap_[pos.y_][pos.x_] == CNTMOVECOST)
	{
		//�ړ��s�}�X
		return nullptr;
	}

	//�m�[�h�̎擾
	auto node = GetNode(pos);
	//�S�[���̏ꍇ�̂ݗ�O�Ƃ��ĊJ��
	if (normal)
	{
		if (!node->IsNone())
		{
			//���łɃI�[�v�����Ă���̂ŉ������Ȃ�
			return nullptr;
		}
	}
	else
	{
		if (node->GetPos() != goalPos_)
		{
			if (node->IsClosed())
			{
				return nullptr;
			}
			if (node->IsOpen() && node->GetCost() < cost)
			{
				//�m�[�h���I�[�v���Ŏ������R�X�g���Ⴂ�ꍇ
				return nullptr;
			}

			//if (!node->IsNone())
			//{
			//	//���łɃI�[�v�����Ă���̂ŉ������Ȃ�
			//	return nullptr;
			//}
		}
	}

	//�m�[�h�I�[�v��
	node->OpenNode(parent, cost);
	AddOpenList(node);

	return node;
}

void AStarMng::OpenAround(ANode* parent, bool normal)
{
	if (parent->GetPos() == goalPos_)
	{
		return;
	}
	Position2 pPos = parent->GetPos();
	int pCost = parent->GetCost() + 1;
	//4�������J��.
	OpenNode(pPos + Position2(1, 0), pCost, parent, normal); //�E
	OpenNode(pPos - Position2(0, 1), pCost, parent, normal); //��
	OpenNode(pPos - Position2(1, 0), pCost, parent, normal); //��
	OpenNode(pPos + Position2(0, 1), pCost, parent, normal); //��
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
		//int totalCost = node->GetTotalCost();
		int totalCost = node->GetTotalCost();
		if (totalCost > minTotalCost)
		{
			//���݂��傫��
			continue;
		}
		if (totalCost == minTotalCost && node->GetHeuristicCost() >= minheuCost)
		//if (totalCost == minTotalCost && node->GetCost() >= minheuCost)
		{
			//�g�[�^���R�X�g���������S�[���܂ł̋������傫���Ƃ�
			continue;
		}

		//�ŏ��X�V
		minTotalCost = totalCost;
		minheuCost = node->GetHeuristicCost();
		//minheuCost = node->GetCost();
		minNode = node;
	}
	return minNode;
}

int AStarMng::ToOneArray(Position2 pos)
{
	return pos.y_ * limitPos_.x_ + pos.x_;
}

void AStarMng::ToNoneOtherOpenNode(void)
{
	for (auto& nodePool : nodePool_)
	{
		if(!nodePool.second->IsOpen())
		{
			nodePool.second->NoneNode();
		}
	}
}

void AStarMng::Release(void)
{
	//�����o�̉��
	for (auto& node : nodePool_)
	{
		delete node.second;
	}
	nodePool_.clear();
	openList_.clear();
}
