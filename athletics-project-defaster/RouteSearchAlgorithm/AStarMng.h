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
//	//ノード生成,取得
//	ANode* GetNode(Position2 pos);
//
//	//ノードをオープンリストへ追加
//	void AddOpenList(ANode* node);
//	//ノードをオープンリストから削除
//	void RemoveOpenList(ANode* node);
//
//	//ノードオープン
//	ANode* OpenNode(Position2 pos, int cost, ANode* parent);
//	//周りのノードオープン
//	void OpenAround(ANode* parent);
//	//最小トータルコストのノード取得
//	ANode* SearchMinTotalCostNodeFromOpenList(void);
//
//	//座標を1元配列へ
//	int ToOneArray(Position2 pos);
//
//	//唯一のインスタンス
//	static AStarMng* s_Instance;
//
//	//コストマップ
//	std::vector<std::vector<std::string>> costMap_;
//	//ノード管理
//	std::map<int, ANode*> nodePool_;
//	//オープンリスト
//	std::list<ANode*> openList_;
//
//	//ゴール
//	Position2 goalPos_;
//	//限界座標
//	Position2 limitPos_;
//};

class AStarMng
{
public:
	AStarMng(Position2 limitPos, Position2 goalPos, const std::vector<std::vector<std::string>>& costMap);

	std::list<Position2> AStarCreate(Position2 startPos);
private:
	//ノード生成,取得
	ANode* GetNode(Position2 pos);

	//ノードをオープンリストへ追加
	void AddOpenList(ANode* node);
	//ノードをオープンリストから削除
	void RemoveOpenList(ANode* node);

	//ノードオープン
	ANode* OpenNode(Position2 pos, int cost, ANode* parent);
	//周りのノードオープン
	void OpenAround(ANode* parent);
	//最小トータルコストのノード取得
	ANode* SearchMinTotalCostNodeFromOpenList(void);

	//座標を1元配列へ
	int ToOneArray(Position2 pos);

	//コストマップ
	std::vector<std::vector<std::string>> costMap_;
	//ノード管理
	std::map<int, ANode*> nodePool_;
	//オープンリスト
	std::list<ANode*> openList_;

	//ゴール
	Position2 goalPos_;
	//限界座標
	Position2 limitPos_;
};
