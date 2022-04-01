#pragma once
#include <list>
#include <map>
#include "ANode.h"
#include "Bresenham.h"
#include "../common.h"

//#define lpAStarMng AStarMng::GetInstance()
//
//class AStarMng
//{
//public:
//	static void Create(Position2 limitPos, Position2 goal, const std::vector<std::vector<std::string>>& costMap)
//	{
//		if (s_Instance == nullptr)
//		{
//			s_Instance = new AStarMng(limitPos, goal, costMap);
//		}
//	}
//	static void Destroy(void)
//	{
//		if (s_Instance != nullptr)
//		{
//			delete s_Instance;
//			s_Instance = nullptr;
//		}
//	}
//	static AStarMng& GetInstance(void)
//	{
//		return (*s_Instance);
//	}
//
//	std::vector<Position2> AStarCreate(Position2 startPos);
//
//private:
//	AStarMng(Position2 limitPos, Position2 goalPos, const std::vector<std::vector<std::string>>& costMap);
//
//	//�m�[�h����,�擾
//	ANode* GetNode(Position2 pos);
//
//	//�m�[�h���I�[�v�����X�g�֒ǉ�
//	void AddOpenList(ANode* node);
//	//�m�[�h���I�[�v�����X�g����폜
//	void RemoveOpenList(ANode* node);
//
//	//�m�[�h�I�[�v��
//	ANode* OpenNode(Position2 pos, int cost, ANode* parent);
//	//����̃m�[�h�I�[�v��
//	void OpenAround(ANode* parent);
//	//�ŏ��g�[�^���R�X�g�̃m�[�h�擾
//	ANode* SearchMinTotalCostNodeFromOpenList(void);
//
//	//���W��1���z���
//	int ToOneArray(Position2 pos);
//
//	//�B��̃C���X�^���X
//	static AStarMng* s_Instance;
//
//	//�R�X�g�}�b�v
//	std::vector<std::vector<std::string>> costMap_;
//	//�m�[�h�Ǘ�
//	std::map<int, ANode*> nodePool_;
//	//�I�[�v�����X�g
//	std::list<ANode*> openList_;
//
//	//�S�[��
//	Position2 goalPos_;
//	//���E���W
//	Position2 limitPos_;
//};

class AStarMng
{
public:
	AStarMng(Position2 limitPos, Position2 goalPos, const std::vector<std::vector<std::string>>& costMap);

	std::list<Position2> AStarCreate(Position2 startPos);
private:
	//�m�[�h����,�擾
	ANode* GetNode(Position2 pos);

	//�m�[�h���I�[�v�����X�g�֒ǉ�
	void AddOpenList(ANode* node);
	//�m�[�h���I�[�v�����X�g����폜
	void RemoveOpenList(ANode* node);

	//�m�[�h�I�[�v��
	ANode* OpenNode(Position2 pos, int cost, ANode* parent);
	//����̃m�[�h�I�[�v��
	void OpenAround(ANode* parent);
	//�ŏ��g�[�^���R�X�g�̃m�[�h�擾
	ANode* SearchMinTotalCostNodeFromOpenList(void);

	//���W��1���z���
	int ToOneArray(Position2 pos);

	//�R�X�g�}�b�v
	std::vector<std::vector<std::string>> costMap_;
	//�m�[�h�Ǘ�
	std::map<int, ANode*> nodePool_;
	//�I�[�v�����X�g
	std::list<ANode*> openList_;

	//�S�[��
	Position2 goalPos_;
	//���E���W
	Position2 limitPos_;
};
