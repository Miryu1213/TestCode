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
	//ノード生成,取得
	ANode* GetNode(Position2 pos);

	//ノードをオープンリストへ追加
	void AddOpenList(ANode* node);
	//ノードをオープンリストから削除
	void RemoveOpenList(ANode* node);

	//ノードオープン
	ANode* OpenNode(Position2 pos, int cost, ANode* parent, bool normal);
	//周りのノードオープン
	void OpenAround(ANode* parent, bool normal);
	//最小トータルコストのノード取得
	ANode* SearchMinTotalCostNodeFromOpenList(void);

	//座標を1元配列へ
	int ToOneArray(Position2 pos);

	//オープンノード以外をすべてNoneへ
	void ToNoneOtherOpenNode(void);

	void Release(void);

	//コストマップ
	std::vector<std::vector<int>> costMap_;
	//ノード管理
	std::map<int, ANode*> nodePool_;
	//オープンリスト
	std::list<ANode*> openList_;

	//ゴール
	Position2 goalPos_;
	//限界座標
	Position2 limitPos_;

	//ゴール後の状態回復
	bool recoveryFlag_;
};
