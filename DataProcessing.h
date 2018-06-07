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
	double value;//�������� load or flow(true)
	double _value;//�������� flow or load(false)
	string line;
}CPt;

typedef struct CorridorSeg{//С������
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
	Flow flow;//�õ�����һ�����ӵĵ������
	Load load;//վ��֮���Load�㼯
}Stop;

typedef struct StopLines{
	string stopName;
	int lineType;//0��ʾ������1��ʾ����
	vector<string> lines;
}StopLines;

typedef struct SideSeg{
	Flow segFlow;//ÿ���ߵķ�ʱ������
	Load segLoad;//ÿ���ߵķ�ʱ�θ���
	vector<Pt> sidePt;//���ϱ�
	vector<string> lineName;//�����ӱߵ���·��
}SideSeg;

typedef struct StopHop{
	string stopStart;//վ�����
	string stopEnd;//վ���յ�
	Flow totalFlow;//�ܼ�����
	Load totalLoad;//�ܼ�Load
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
bool IfFront(double x1,double y1,double x2,double y2 ,double x3,double y3);//�ж�һ���Ƿ���һ�������м�
double PtDis(double x1,double y1,double x2,double y2);
double PointToSegDist(double x, double y, double x1, double y1, double x2, double y2);
bool CosSimilarity(double x1,double y1,double x2,double y2 ,double x3,double y3,double x4,double y4,double maxSita);
int ClosestPt(CPt p,const vector<CPt>& pts,double maxDis);//-1��ʾû�з������������򷵻�pts�������ı��;


#endif