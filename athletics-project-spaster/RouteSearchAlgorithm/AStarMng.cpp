#include <iostream>
#include <chrono>
#include "AStarMng.h"
#include "../common.h"

//実態の宣言
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
	//通常A*
	std::list<Position2> goalPath;

	//AStarスタート
	ANode* node = OpenNode(startPos, 0, nullptr, true);

	//試行回数(1000回超えれば中断)
	int cnt = 0;
	while (1)
	{
		//自分をオープンリストから削除
		RemoveOpenList(node);
		node->CloseNode();

		//周囲を開く
		OpenAround(node, true);

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
	//すべての最短経路探索A*
	std::list<std::list<Position2>> goalPathList;

	//事前に最短経路を見つけておく
	auto prePath = AStarCreateN(startPos);
	goalPathList.push_back(prePath);
	int toGoalDistance = prePath.size();

	//AStarスタート
	ANode* node = OpenNode(startPos, 0, nullptr, false);

	//時間保存系
	system_clock::time_point StartTime = system_clock::now();									//開始時の日時
	microseconds microstarttime = duration_cast<milliseconds>(StartTime.time_since_epoch());	//現在のシステム時間
	system_clock::time_point nowTime;															//現在の日時
	microseconds elaptime = microstarttime;
	microseconds oldelaptime;

	double delta = 0;
	double totalDelta = 0;
	

	//試行回数(1000回超えれば中断)
	int cnt = 0;
	while (1)
	{
		oldelaptime = elaptime;																	//一つ前の時間の更新
		nowTime = system_clock::now();															//現在の日時
		elaptime = duration_cast<microseconds>(nowTime.time_since_epoch());
		delta = static_cast<double>((elaptime - oldelaptime).count() * 0.000001f);
		totalDelta += delta;

		if (openList_.empty())
		{
			//探索終了
			break;
		}
		//自分をオープンリストから削除
		RemoveOpenList(node);
		node->CloseNode();

		//パスの数によって打ち切り
		int pathCnt = 0;
		node->GetPathNum(pathCnt);
		if (pathCnt <= prePath.size())
		{
			//周囲を開く
			OpenAround(node, false);
		}

		//最小合計コストのノードを探索
		node = SearchMinTotalCostNodeFromOpenList();

		if (node == nullptr)
		{
			//オープンノード終了
			break;
		}

		//ゴール後の状態回復
		if (recoveryFlag_)
		{
			node->CloseNodeParent();
		}

		if (node->GetPos() == goalPos_)
		{
			//ゴール
			std::list<Position2> tmpPath;
			node->GetPath(&tmpPath);
			
			if (tmpPath.size() <= toGoalDistance)
			{
				//ゴールへのパスリストへ追加
				goalPathList.push_back(tmpPath);
				toGoalDistance = tmpPath.size();
				
				//オープンノード以外をNoneへ
				ToNoneOtherOpenNode();
				recoveryFlag_ = true;

				//ゴールまでのノードをすべて解放
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

	//ゴールまでのパスでトレーナーと当たる回数が多いものを取り出す
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
					//同じ座標を二人のトレーナーが見ることはないため
					break;
				}
			}
		}
		if (cnt > macCnt && goalPath.size() <= toGoalDistance)
		{
			//トレーナーと当たる回数が多いものを取り出す
			macCnt = cnt;
			tmpPath = goalPath;
		}
	}

	if (tmpPath.empty())
	{
		//トレーナーと当たらない場合
		tmpPath = goalPathList.back();
	}

	tmpPath.pop_back();
	tmpPath.reverse();

	//std::vector<Position2> reversePath;
	////パスを反転(スタートのマスは含めない)
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

ANode* AStarMng::OpenNode(Position2 pos, int cost, ANode* parent, bool normal)
{
	if (pos.x_ < 0 || limitPos_.x_ <= pos.x_ || pos.y_ < 0 || limitPos_.y_ <= pos.y_)
	{
		//範囲外
		return nullptr;
	}
	if (costMap_[pos.y_][pos.x_] == CNTMOVECOST)
	{
		//移動不可マス
		return nullptr;
	}

	//ノードの取得
	auto node = GetNode(pos);
	//ゴールの場合のみ例外として開く
	if (normal)
	{
		if (!node->IsNone())
		{
			//すでにオープンしているので何もしない
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
				//ノードがオープンで自分よりコストが低い場合
				return nullptr;
			}

			//if (!node->IsNone())
			//{
			//	//すでにオープンしているので何もしない
			//	return nullptr;
			//}
		}
	}

	//ノードオープン
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
	//4方向を開く.
	OpenNode(pPos + Position2(1, 0), pCost, parent, normal); //右
	OpenNode(pPos - Position2(0, 1), pCost, parent, normal); //上
	OpenNode(pPos - Position2(1, 0), pCost, parent, normal); //左
	OpenNode(pPos + Position2(0, 1), pCost, parent, normal); //下
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
		//int totalCost = node->GetTotalCost();
		int totalCost = node->GetTotalCost();
		if (totalCost > minTotalCost)
		{
			//現在より大きい
			continue;
		}
		if (totalCost == minTotalCost && node->GetHeuristicCost() >= minheuCost)
		//if (totalCost == minTotalCost && node->GetCost() >= minheuCost)
		{
			//トータルコストが同じかつゴールまでの距離が大きいとき
			continue;
		}

		//最小更新
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
	//メンバの解放
	for (auto& node : nodePool_)
	{
		delete node.second;
	}
	nodePool_.clear();
	openList_.clear();
}
