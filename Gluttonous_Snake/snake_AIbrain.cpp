#include <queue>
#include "snake_class.h"
char GetSpeed(const char type)
{
	switch (type) {
		case GROUND:
			return NORMAL + 1;
		case FAST_GROUND:
			return FAST + 1;
		case SLOW_GROUND:
			return SLOW + 1;
		default:
			return 100;
	}
}
class compare {
public:
	bool operator()(pair<coordinate, int> a, pair<coordinate, int> b) {
		return a.second > b.second;
	}
};
void Map::AsearchList(const coordinate& start, const coordinate& end, unordered_map<int, coordinate>& this_parent, unordered_map<int, int>& this_G, Food& f)
{
	int i, nowblockG, nowblockF;
	priority_queue<pair<coordinate, int>, vector<pair<coordinate, int>>, compare> Fdistance;//f函数的优先队列
	Fdistance.push(make_pair(start, 0));//将起始点加入优先队列

	this_parent[start.y * CANVAS_XBLOCK + start.x] = start;//将起点的双亲结点标记为本身
	this_G[start.y * CANVAS_XBLOCK + start.x] = 0;//起点的g设为0

	while (!Fdistance.empty()) {//优先队列非空，即未遍历地图上所有点
		coordinate nowblock = Fdistance.top().first;//优先队列出队，找到f最小的下一个点
		if (nowblock == end)//下一个点等于终点
			break;//查找结束
		Fdistance.pop();

		coordinate around[4] = { nowblock,nowblock ,nowblock ,nowblock };//上下左右
		--around[0].x;
		++around[1].x;
		--around[2].y;
		++around[3].y;

		for (i = 0; i < 4; ++i) {//依次查找上下左右四个点
			if (around[i].x >= 0 && around[i].x < CANVAS_XBLOCK && around[i].y >= 0 && around[i].y < CANVAS_YBLOCK && TERRITORY[around[i].y * CANVAS_XBLOCK + around[i].x] < SOFT_WALL && f.JudgeFood(around[i], CANVAS_XBLOCK) != BAD) {//该点坐标在地图合理区间内且可以通过（不会撞死）
				nowblockG = this_G[nowblock.y * CANVAS_XBLOCK + nowblock.x] + GetSpeed(TERRITORY[around[i].y * CANVAS_XBLOCK + around[i].x]);//周围点的g等于本结点的g + 两者到达时间
				if (this_G.find(around[i].y * CANVAS_XBLOCK + around[i].x) == this_G.end() || nowblockG < this_G.find(around[i].y * CANVAS_XBLOCK + around[i].x)->second) {//优先队列中找不到nowblock周围的这个点或从nowblock到nowblock周围的这个点的g值比该点原g值小
					this_G[around[i].y * CANVAS_XBLOCK + around[i].x] = nowblockG;//更新该点g值
					nowblockF = nowblockG + abs(end.x - around[i].x) + abs(end.y - around[i].y);//计算该点f值
					Fdistance.push(make_pair(around[i], nowblockF));//更新该点f值
					this_parent[around[i].y * CANVAS_XBLOCK + around[i].x] = nowblock;//更新该点的father
				}
			}
		}
	}
}
bool Map::GetNextStep(char& direction, int& step, const coordinate& start, const coordinate& end, Food& f)
{//返回是否能吃到该果子,获得本果子前进的时间消费，下一步的方向
	unordered_map<int, coordinate> this_parent;//当前块的父亲块
	unordered_map<int, int> this_G;//当前块的G值
	AsearchList(start, end, this_parent, this_G, f);
	if (this_G.find(end.y * CANVAS_XBLOCK + end.x) == this_G.end())//终点未加入哈希表，即无法连接到
		return false;
	step = 1;
	coordinate previous, now = end;
	for (previous = this_parent.find(now.y * CANVAS_XBLOCK + now.x)->second; previous != start; previous = this_parent.find(now.y * CANVAS_XBLOCK + now.x)->second) {
		++step;//从now到previous走了一步
		now = previous;
	}
	if (previous.x == now.x - 1)
		direction = 0x44;
	else if (previous.x == now.x + 1)
		direction = 0x41;
	else if (previous.y == now.y - 1)
		direction = 0x53;
	else if (previous.y == now.y + 1)
		direction = 0x57;
	return true;
}
char Food::WhichDirection(Map& m, const coordinate& start)
{
	const int EXCELLENT_VALUE = 3;
	const int GOOD_VALUE = 4;
	const int ORDINARY_VALUE = 5;
	//比例平衡的问题

	char thisdirec, direction;
	int thisstep, step;
	thisstep = step = INT_MAX;
	thisdirec = direction = -1;
	for (unordered_map<int, int>::iterator p = allfood.begin(); p != allfood.end(); ++p) {
		if (p->second == BAD)
			continue;
		coordinate end = { p->first % m.CANVAS_XBLOCK ,p->first / m.CANVAS_XBLOCK };
		m.GetNextStep(thisdirec, thisstep, start, end, *this);
		if (allfood[p->second] == EXCELLENT)
			thisstep *= EXCELLENT_VALUE;
		else if (allfood[p->second] == GOOD)
			thisstep *= GOOD_VALUE;
		else if (allfood[p->second] == SIMPLE)
			thisstep *= ORDINARY_VALUE;
		if (thisstep < step) {
			step = thisstep;
			direction = thisdirec;
		}
	}
	return direction;
}