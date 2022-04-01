#pragma once
#include <list>
#include <map>
#include "ANode.h"
#include "Bresenham.h"
#include "../EnemyTrainar.h"
#include "../common.h"

class AStarMng
{
public:
	AStarMng(Position2 limitPos, Position2 goalPos, const std::vector<std::vector<int>>& costMap);

	std::list<Position2> AStarCreateSP(Position2 startPos, const std::vector<EnemyTrainar*>& trainarList);
	std::list<Position2> AStarCreateN(Position2 startPos);
private:
	//�m�[�h����,�擾
	ANode* GetNode(Position2 pos);

	//�m�[�h���I�[�v�����X�g�֒ǉ�
	void AddOpenList(ANode* node);
	//�m�[�h���I�[�v�����X�g����폜
	void RemoveOpenList(ANode* node);

	//�m�[�h�I�[�v��
	ANode* OpenNode(Position2 pos, int cost, ANode* parent, bool normal);
	//����̃m�[�h�I�[�v��
	void OpenAround(ANode* parent, bool normal);
	//�ŏ��g�[�^���R�X�g�̃m�[�h�擾
	ANode* SearchMinTotalCostNodeFromOpenList(void);

	//���W��1���z���
	int ToOneArray(Position2 pos);

	//�I�[�v���m�[�h�ȊO�����ׂ�None��
	void ToNoneOtherOpenNode(void);

	void Release(void);

	//�R�X�g�}�b�v
	std::vector<std::vector<int>> costMap_;
	//�m�[�h�Ǘ�
	std::map<int, ANode*> nodePool_;
	//�I�[�v�����X�g
	std::list<ANode*> openList_;

	//�S�[��
	Position2 goalPos_;
	//���E���W
	Position2 limitPos_;

	//�S�[����̏�ԉ�
	bool recoveryFlag_;
};
