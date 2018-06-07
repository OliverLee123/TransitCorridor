#ifndef Corridor_H
#define Corridor_H


#include "DataProcessing.h"
#include"SparseMatrix.h"

using namespace std;

bool CMP_Cor(const CorridorSeg &a, const CorridorSeg &b);


class Corridor{     
private:

	vector<Stop>::size_type busLineCount;//������·����
	vector<Stop>::size_type subwayLineCount;//������·����
	vector<vector<Stop>> busLineStops; //����վ�㰴��·��֯
	vector<vector<Stop>> subwayLineStops;//����վ�㰴��·��֯
	vector<vector<Stop>> totalLineStops;//�����������ϲ�
	vector<vector<StopSide>> busStopSides;//����վ���
	vector<vector<StopSide>> subwayStopSides;//����վ���
	vector<vector<StopSide>> totalStopSides;//����������վ���
	
	vector<Load> stopLoad;
	vector<Flow> stopFlow;
	


	int busStopCount;
	int subwayStopCount;
	vector<string> leg;
	int mPeakBegin;
	int mPeakEnd;
	int aPeakBegin;
	int aPeakEnd;

	int GetID(string name,string line);//�õ�վ����ϡ������ڵı��
	int ComparePts(vector<Pt> pt,vector<SideSeg> sharedLines);
	bool IfTerminus(string stop);
	int GetStopNum(string lineName,string aboardStop,string alightStop,string carNumber,int dir);
	vector<double> GetFlowWeight(int n);
	void CalFlowByWeight(vector<string> trip,vector<double> weight,time_t t);
	bool IfFlowShared(int cID,int _cID,int id,double p,int chTime,bool ifLoad);//�ж����������Ƿ���Flow,ǰһ�����ӻ�ϲ��ں�һ��
	void DeleteCompleteOverlap();
	
public:  
	SparseMatrix *stopHopSM;
	vector<CorridorSeg> corridor;//���Ƚṹ
	vector<StopLines> stopsLines;//��վ�����ƺϲ���վ�㣬��ͨ����վ�����·
	bool *ifCor;



	Corridor();
	~Corridor();


	void SetPeak(int a,int b,int c,int d);//��������ʱ��� a-b��c-d

	void InitLineStops(string busPath,string subwayPath);
	void InitLineStopSide(string busSegPath,string subwaySegPath);
	void InitFlowSide(string legPath,string referTime);
	void _InitFlowSide(string legPath,string referTime);//trip,Ȩ��������,��������
	void InitStopHop(string stopPath);
	void StopHopToFile(string filePath);
	int SearchHotLineFromStop(int startID,double threshold,double _threshold,int chooseTime,int corridorID,bool ifLoad);//��һ��վ�����������һ�����ȣ������Ӧ��վ����ϡ������ڵı��														//0,1,others�������磬���������
	int _SearchHotLineFromStop(int startID,double loadRateThre,double flowThre,int chooseTime,int corridorID);//����Load����
	void CorridorToFile(string outPath);
	void SidePtToFile(string outBusPath,string outSubwayPath);
	void InitLineStopSideFromFile(string outBusPath,string outSubwayPath);
	void SearchCorridorFromLine(double threshold,double _threshold,int chooseTime,const bool ifLoad);//·��load����
	void _SearchCorridorFromLine(double coreThre,double loadRateThre,double flowThre,int chooseTime,const bool ifLoad);//����load����
	vector<int> GetTopHotStop(int n,int chooseTime);
	void MergeCorridor(int chTime,bool ifLoad);
	double GetProValue(double p,bool ifLoad,int chTime);//times mean ������give the pro, return the threshold value of corridor
	double _GetProValue(double p,bool ifLoad,int chTime);//�õ�վ��load�ķ�λ��
	double GetMeanValue(double p,bool ifLoad,int chTime);//give the times of mean, return the threshold value of corridor
	int GetMostPopStop(int startID,double threshold,int chooseTime,const bool ifLoad);
	int _GetMostPopStop(int startID,double threshold,int chooseTime,const bool ifLoad);
	double CalPerLoad(bool ifLoadCor,int k);
	int TopKCorridorToFile(string filePath,int k,bool ifLoadCor);//�õ�����֮������ȣ�����topK���ȵİ�����վ����
	void CalTripOfTopKCorridor(double dis,int stopCount,int k,string referTime,int chT);
	bool ifCorContainTrip(vector<string> trip,int k);
	void InitialStopLoad();
	void SortCorridor(bool);
	void GetTopkCorridorTrip(int);
}; 





#endif