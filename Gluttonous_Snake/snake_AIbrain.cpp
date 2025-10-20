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
	priority_queue<pair<coordinate, int>, vector<pair<coordinate, int>>, compare> Fdistance;//f���������ȶ���
	Fdistance.push(make_pair(start, 0));//����ʼ��������ȶ���

	this_parent[start.y * CANVAS_XBLOCK + start.x] = start;//������˫�׽����Ϊ����
	this_G[start.y * CANVAS_XBLOCK + start.x] = 0;//����g��Ϊ0

	while (!Fdistance.empty()) {//���ȶ��зǿգ���δ������ͼ�����е�
		coordinate nowblock = Fdistance.top().first;//���ȶ��г��ӣ��ҵ�f��С����һ����
		if (nowblock == end)//��һ��������յ�
			break;//���ҽ���
		Fdistance.pop();

		coordinate around[4] = { nowblock,nowblock ,nowblock ,nowblock };//��������
		--around[0].x;
		++around[1].x;
		--around[2].y;
		++around[3].y;

		for (i = 0; i < 4; ++i) {//���β������������ĸ���
			if (around[i].x >= 0 && around[i].x < CANVAS_XBLOCK && around[i].y >= 0 && around[i].y < CANVAS_YBLOCK && TERRITORY[around[i].y * CANVAS_XBLOCK + around[i].x] < SOFT_WALL && f.JudgeFood(around[i], CANVAS_XBLOCK) != BAD) {//�õ������ڵ�ͼ�����������ҿ���ͨ��������ײ����
				nowblockG = this_G[nowblock.y * CANVAS_XBLOCK + nowblock.x] + GetSpeed(TERRITORY[around[i].y * CANVAS_XBLOCK + around[i].x]);//��Χ���g���ڱ�����g + ���ߵ���ʱ��
				if (this_G.find(around[i].y * CANVAS_XBLOCK + around[i].x) == this_G.end() || nowblockG < this_G.find(around[i].y * CANVAS_XBLOCK + around[i].x)->second) {//���ȶ������Ҳ���nowblock��Χ���������nowblock��nowblock��Χ��������gֵ�ȸõ�ԭgֵС
					this_G[around[i].y * CANVAS_XBLOCK + around[i].x] = nowblockG;//���¸õ�gֵ
					nowblockF = nowblockG + abs(end.x - around[i].x) + abs(end.y - around[i].y);//����õ�fֵ
					Fdistance.push(make_pair(around[i], nowblockF));//���¸õ�fֵ
					this_parent[around[i].y * CANVAS_XBLOCK + around[i].x] = nowblock;//���¸õ��father
				}
			}
		}
	}
}
bool Map::GetNextStep(char& direction, int& step, const coordinate& start, const coordinate& end, Food& f)
{//�����Ƿ��ܳԵ��ù���,��ñ�����ǰ����ʱ�����ѣ���һ���ķ���
	unordered_map<int, coordinate> this_parent;//��ǰ��ĸ��׿�
	unordered_map<int, int> this_G;//��ǰ���Gֵ
	AsearchList(start, end, this_parent, this_G, f);
	if (this_G.find(end.y * CANVAS_XBLOCK + end.x) == this_G.end())//�յ�δ�����ϣ�����޷����ӵ�
		return false;
	step = 1;
	coordinate previous, now = end;
	for (previous = this_parent.find(now.y * CANVAS_XBLOCK + now.x)->second; previous != start; previous = this_parent.find(now.y * CANVAS_XBLOCK + now.x)->second) {
		++step;//��now��previous����һ��
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
	//����ƽ�������

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