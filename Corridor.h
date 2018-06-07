#ifndef Corridor_H
#define Corridor_H


#include "DataProcessing.h"
#include"SparseMatrix.h"

using namespace std;

bool CMP_Cor(const CorridorSeg &a, const CorridorSeg &b);


class Corridor{     
private:

	vector<Stop>::size_type busLineCount;//公交线路个数
	vector<Stop>::size_type subwayLineCount;//地铁线路个数
	vector<vector<Stop>> busLineStops; //公交站点按线路组织
	vector<vector<Stop>> subwayLineStops;//地铁站点按线路组织
	vector<vector<Stop>> totalLineStops;//公交、地铁合并
	vector<vector<StopSide>> busStopSides;//公交站点边
	vector<vector<StopSide>> subwayStopSides;//地铁站点边
	vector<vector<StopSide>> totalStopSides;//公交、地铁站点边
	
	vector<Load> stopLoad;
	vector<Flow> stopFlow;
	


	int busStopCount;
	int subwayStopCount;
	vector<string> leg;
	int mPeakBegin;
	int mPeakEnd;
	int aPeakBegin;
	int aPeakEnd;

	int GetID(string name,string line);//得到站名在稀疏矩阵内的编号
	int ComparePts(vector<Pt> pt,vector<SideSeg> sharedLines);
	bool IfTerminus(string stop);
	int GetStopNum(string lineName,string aboardStop,string alightStop,string carNumber,int dir);
	vector<double> GetFlowWeight(int n);
	void CalFlowByWeight(vector<string> trip,vector<double> weight,time_t t);
	bool IfFlowShared(int cID,int _cID,int id,double p,int chTime,bool ifLoad);//判断两条走廊是否共享Flow,前一个连接或合并于后一个
	void DeleteCompleteOverlap();
	
public:  
	SparseMatrix *stopHopSM;
	vector<CorridorSeg> corridor;//走廊结构
	vector<StopLines> stopsLines;//按站点名称合并后，站点，及通过该站点的线路
	bool *ifCor;



	Corridor();
	~Corridor();


	void SetPeak(int a,int b,int c,int d);//设置两个时间段 a-b，c-d

	void InitLineStops(string busPath,string subwayPath);
	void InitLineStopSide(string busSegPath,string subwaySegPath);
	void InitFlowSide(string legPath,string referTime);
	void _InitFlowSide(string legPath,string referTime);//trip,权函数分配,计算流量
	void InitStopHop(string stopPath);
	void StopHopToFile(string filePath);
	int SearchHotLineFromStop(int startID,double threshold,double _threshold,int chooseTime,int corridorID,bool ifLoad);//从一个站点出发，搜索一条走廊，输入的应是站点在稀疏矩阵内的编号														//0,1,others代表上午，下午和其余
	int _SearchHotLineFromStop(int startID,double loadRateThre,double flowThre,int chooseTime,int corridorID);//共享Load搜索
	void CorridorToFile(string outPath);
	void SidePtToFile(string outBusPath,string outSubwayPath);
	void InitLineStopSideFromFile(string outBusPath,string outSubwayPath);
	void SearchCorridorFromLine(double threshold,double _threshold,int chooseTime,const bool ifLoad);//路段load搜索
	void _SearchCorridorFromLine(double coreThre,double loadRateThre,double flowThre,int chooseTime,const bool ifLoad);//共享load搜索
	vector<int> GetTopHotStop(int n,int chooseTime);
	void MergeCorridor(int chTime,bool ifLoad);
	double GetProValue(double p,bool ifLoad,int chTime);//times mean 倍数，give the pro, return the threshold value of corridor
	double _GetProValue(double p,bool ifLoad,int chTime);//得到站点load的分位数
	double GetMeanValue(double p,bool ifLoad,int chTime);//give the times of mean, return the threshold value of corridor
	int GetMostPopStop(int startID,double threshold,int chooseTime,const bool ifLoad);
	int _GetMostPopStop(int startID,double threshold,int chooseTime,const bool ifLoad);
	double CalPerLoad(bool ifLoadCor,int k);
	int TopKCorridorToFile(string filePath,int k,bool ifLoadCor);//得到排序之后的走廊，返回topK走廊的包含的站点数
	void CalTripOfTopKCorridor(double dis,int stopCount,int k,string referTime,int chT);
	bool ifCorContainTrip(vector<string> trip,int k);
	void InitialStopLoad();
	void SortCorridor(bool);
	void GetTopkCorridorTrip(int);
}; 





#endif