#ifndef DataProcessing_H
#define DataProcessing_H

#define MIN(a,b) ((a)<(b)?(a):(b))
#define MAX(a,b) ((a)>(b)?(a):(b))

#include <string>
#include <time.h>
#include <vector>

using namespace std;

typedef struct Flow{
	double m_flow;
	double a_flow;
	double e_flow;
	Flow operator+(const Flow &l) const{
		Flow flow;
		flow.m_flow = m_flow + l.m_flow;
		flow.a_flow = a_flow + l.a_flow;
		flow.e_flow = e_flow + l.e_flow;
		return flow;
	}
}Flow;

typedef struct Load{
	double m_load;
	double a_load;
	double e_load;
	Load operator+(const Load &l) const{
		Load load;
		load.m_load = m_load + l.m_load;
		load.a_load = a_load + l.a_load;
		load.e_load = e_load + l.e_load;
		return load;
	
	} 
}Load;
typedef struct Pt{
	double x;
	double y;
	string lineName;
	int direction;
}Pt;

typedef struct CPt{
	double x;
	double y;
	string stopStart;
	string stopEnd;
	double value;//搜索依据 load or flow(true)
	double _value;//后处理依据 flow or load(false)
	string line;
}CPt;

typedef struct CorridorSeg{//小段走廊
	vector<string> stops;

	int stopStartID;
	int stopEndID;
	int corridorID; 
	vector<CPt> side;
	double totalValue;
	double _totalValue;
	double totoalDis;
}CorridorSeg;

typedef struct Stop{
	string stopName;
	string lineName;
	int direction;
	double x;
	double y;
	Flow flow;//该点与下一条连接的点的流量
	Load load;//站点之间的Load汇集
}Stop;

typedef struct StopLines{
	string stopName;
	int lineType;//0表示公交，1表示地铁
	vector<string> lines;
}StopLines;

typedef struct SideSeg{
	Flow segFlow;//每个边的分时段流量
	Load segLoad;//每个边的分时段负载
	vector<Pt> sidePt;//集合边
	vector<string> lineName;//该连接边的线路名
}SideSeg;

typedef struct StopHop{
	string stopStart;//站点起点
	string stopEnd;//站点终点
	Flow totalFlow;//总计流量
	Load totalLoad;//总计Load
	vector<SideSeg> sharedLines;
	bool isExist;
}StopHop;


typedef struct StopSide{
	string lineName;
	string stopStart;
	string stopEnd;
	vector<Pt> side;
}StopSide;


typedef struct LineSegment{
	string lineName;
	int direction;
	double x;
	double y;
	LineSegment *next;
}LineSegment;

void ShowTime();
time_t StringToDatetime(string str);
time_t InitReferTime(string str);
void GetStopsDirection(std::string filePath,std::string outputPath);
void SimplizeLine(string filePath,string outPath);
bool CosSimilarity(double x1,double y1,double x2,double y2 ,double x3,double y3,double maxSita);
bool CosSimilarity(double x1,double y1,double x2,double y2,double maxSita);
bool IfFront(double x1,double y1,double x2,double y2 ,double x3,double y3);//判断一点是否在一个向量中间
double PtDis(double x1,double y1,double x2,double y2);
double PointToSegDist(double x, double y, double x1, double y1, double x2, double y2);
bool CosSimilarity(double x1,double y1,double x2,double y2 ,double x3,double y3,double x4,double y4,double maxSita);
int ClosestPt(CPt p,const vector<CPt>& pts,double maxDis);//-1表示没有符合条件，否则返回pts中最近点的编号;


#endif