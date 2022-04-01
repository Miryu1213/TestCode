#include <iostream>
#include "AStarMng.h"
#include "../common.h"

//実態の宣言
//AStarMng* AStarMng::s_Instance = nullptr;

std::list<Position2> AStarMng::AStarCreate(Position2 startPos)
{
	std::list<Position2> goalPath;

	//AStarスタート
	ANode* node = OpenNode(startPos, 0, nullptr);

	//試行回数(1000回超えれば中断)
	int cnt = 0;
	while (cnt < 1000)
	{
		//自分をオープンリストから削除
		RemoveOpenList(node);
		node->CloseNode();

		//周囲を開く
		OpenAround(node);

		//最小合計コストのノードを探索
		node = SearchMinTotalCostNodeFromOpenList();
		if (node == nullptr)
		{
			//袋小路
			cout << "袋小路です\n";
			break;
		}
		if (node->GetPos() == goalPos_)
		{
			//ゴール
			RemoveOpenList(node);
			node->GetPath(&goalPath);

			//パスを反転
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
	//プールにある場合はそちらから取得
	if (nodePool_.count(arX) != 0)
	{
		return nodePool_[arX];
	}

	//ない場合生成
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
		//範囲外
		return nullptr;
	}
	if (!(costMap_[pos.y_][pos.x_] == TOWN_A || costMap_[pos.y_][pos.x_] == TOWN_B ||
		costMap_[pos.y_][pos.x_] == ROAD))
	{
		//移動不可マス
		return nullptr;
	}

	//ノードの取得
	auto node = GetNode(pos);
	if (!node->IsNone())
	{
		//すでにオープンしているので何もしない
		return nullptr;
	}

	//ノードオープン
	node->OpenNode(parent, cost);
	AddOpenList(node);

	return node;
}

void AStarMng::OpenAround(ANode* parent)
{
	Position2 pPos = parent->GetPos();
	int pCost = parent->GetCost();
	//4方向を開く.
	OpenNode(pPos + Position2(1, 0), pCost, parent); //右
	OpenNode(pPos - Position2(0, 1), pCost, parent); //上
	OpenNode(pPos - Position2(1, 0), pCost, parent); //左
	OpenNode(pPos + Position2(0, 1), pCost, parent); //下
}

ANode* AStarMng::SearchMinTotalCostNodeFromOpenList(void)
{
	//最小トータルコスト
	int minTotalCost = 10000;
	//最小ヒューリスティックコスト
	int minheuCost = 10000;
	//最小ノード
	ANode* minNode = nullptr;

	for (const auto& node : openList_)
	{
		int totalCost = node->GetTotalCost();
		if (totalCost > minTotalCost)
		{
			//現在より大きい
			continue;
		}
		if (totalCost == minTotalCost && node->GetHeuristicCost() >= minheuCost)
		{
			//トータルコストが同じかつゴールまでの距離が大きいとき
			continue;
		}

		//最小更新
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
