#include "stdafx.h"
#include <stdio.h>
#include <string>
#include <sstream>
#include <fstream>
#include <time.h>
#include <iostream>   
#include <iomanip>
#include <vector>
#include <math.h>
#include "Corridor.h"
#include <algorithm>
#include <iterator> 


using namespace std;


Corridor::Corridor(){
	busLineCount = 0;
	subwayLineCount = 0;
	busStopCount = 0;
	subwayStopCount = 0;
}

Corridor::~Corridor(){
	busLineStops.clear();
	subwayLineStops.clear();
	stopsLines.clear();
	if(stopHopSM)
		delete stopHopSM;
	if(ifCor)
		delete ifCor;
}

double Corridor::_GetProValue(double p,bool ifLoad,int chTime){
	vector<double> pt;
	for(int i =0;i<stopLoad.size();i++)
	{
		if(ifLoad)
		{
			if(chTime == 0&&stopLoad[i].m_load > 0)
				pt.push_back(stopLoad[i].m_load);
			else if(chTime == 1&&stopLoad[i].a_load > 0)
				pt.push_back(stopLoad[i].a_load);
			else if(stopLoad[i].e_load > 0)
				pt.push_back(stopLoad[i].e_load);
		}
		else
		{
			if(chTime == 0&&stopFlow[i].m_flow > 0)
				pt.push_back(stopFlow[i].m_flow);
			else if(chTime == 1&&stopFlow[i].a_flow > 0)
				pt.push_back(stopFlow[i].a_flow);
			else if(stopFlow[i].e_flow > 0)
				pt.push_back(stopFlow[i].e_flow);
		}
	}
	
	sort(pt.begin(),pt.end());

	
	
	int index = pt.size() * p;
	double value = pt[index];
	return value;
}

void Corridor::InitialStopLoad(){
	for (int i = 0;i<stopsLines.size();++i)
	{
		Load l;
		l.a_load = 0;
		l.e_load = 0;
		l.m_load = 0;
		stopLoad.push_back(l);
		Flow f;
		f.e_flow = 0;
		f.a_flow = 0;
		f.m_flow = 0;
		stopFlow.push_back(f);
	}
	for (int i=0;i<stopHopSM->Terms;i++)
	{
		int row = stopHopSM->smArray[i].row;
		if(row >=0)
		{
			stopLoad[row] = stopLoad[row]+stopHopSM->smArray[i].stopHop.totalLoad;
			stopFlow[row] = stopFlow[row]+stopHopSM->smArray[i].stopHop.totalFlow;	
		}
	}
}

void Corridor::SetPeak(int a,int b,int c,int d){
	mPeakBegin = a;
	mPeakEnd = b;
	aPeakBegin = c;
	aPeakEnd = d;
}


double Corridor::GetMeanValue(double times,bool ifLoad,int chTime){
	double *pt = new double[stopHopSM->Terms];
	for(int i =0;i<stopHopSM->Terms;i++)
	{
		if(ifLoad)
		{
			if(chTime == 0)
				pt[i] = stopHopSM->smArray[i].stopHop.totalLoad.m_load;
			else if(chTime == 1)
				pt[i] = stopHopSM->smArray[i].stopHop.totalLoad.a_load;
			else
				pt[i] = stopHopSM->smArray[i].stopHop.totalLoad.e_load;
		}
		else
		{
			if(chTime == 0)
				pt[i] = stopHopSM->smArray[i].stopHop.totalFlow.m_flow;
			else if(chTime == 1)
				pt[i] = stopHopSM->smArray[i].stopHop.totalFlow.a_flow;
			else
				pt[i] = stopHopSM->smArray[i].stopHop.totalFlow.e_flow;
		}
	}
	double sum = 0;
	for(int i = 0;i<stopHopSM->Terms;++i)
	{
		sum += pt[i];
	}

	double mean = sum / stopHopSM->Terms;
	delete []pt;
	return mean * times;
}


double Corridor::GetProValue(double p,bool ifLoad,int chTime){
	vector<double> pt;
	for(int i =0;i<stopHopSM->Terms;i++)
	{
		if(ifLoad)
		{
			if(chTime == 0&&stopHopSM->smArray[i].stopHop.totalLoad.m_load > 0)
				pt.push_back(stopHopSM->smArray[i].stopHop.totalLoad.m_load);
			else if(chTime == 1&&stopHopSM->smArray[i].stopHop.totalLoad.a_load > 0)
				pt.push_back(stopHopSM->smArray[i].stopHop.totalLoad.a_load);
			else if(stopHopSM->smArray[i].stopHop.totalLoad.e_load > 0)
				pt.push_back(stopHopSM->smArray[i].stopHop.totalLoad.e_load);
		}
		else
		{
			if(chTime == 0&&stopHopSM->smArray[i].stopHop.totalFlow.m_flow > 0)
				pt.push_back(stopHopSM->smArray[i].stopHop.totalFlow.m_flow);
			else if(chTime == 1&&stopHopSM->smArray[i].stopHop.totalFlow.a_flow > 0)
				pt.push_back(stopHopSM->smArray[i].stopHop.totalFlow.a_flow);
			else if(stopHopSM->smArray[i].stopHop.totalFlow.e_flow > 0)
				pt.push_back(stopHopSM->smArray[i].stopHop.totalFlow.e_flow);
		}
	}
	
	sort(pt.begin(),pt.end());

	/*ofstream outFile("C:\\Users\\LYC\\Desktop\\Distribution.csv",ios::out); 
	if(!outFile)
	{
	std::cout<<"error !"<<endl;
	}
	else
	{
	for(size_t i=0;i<pt.size();i++)
	{
	outFile<<i<<","<<pt[i]<<endl;
	}
	}
	outFile.close();
	*/
	int index = pt.size() * p;
	double value = pt[index];
	return value;
}

int Corridor::GetID(string name,string line){
	int flag = -1;
	int lineType;
	if(line.substr(0,4)=="地铁")
		lineType = 1;
	else
		lineType = 0;
	for(size_t i =0;i<stopsLines.size();i++)
	{
		if(name.compare(stopsLines[i].stopName)==0&&stopsLines[i].lineType == lineType)
		{
			flag = i;
			break;
		}
	}
	return flag;
}
	
vector<int> Corridor::GetTopHotStop(int n,int chooseTime){
	//chooseTime 0-上午 1-下午 其他-凌晨
	vector<int> _id;
	Trituple * temp = stopHopSM->smArray;
	if(chooseTime == 0)
		sort(temp,temp+stopHopSM->Terms-1,CMP_1);
	else if(chooseTime == 1)
		sort(temp,temp+stopHopSM->Terms-1,CMP_2);
	else
		sort(temp,temp+stopHopSM->Terms-1,CMP_3);
	for(int i = 0;i<n;++i)
	{
		int id = GetID(stopHopSM->smArray[i].stopHop.stopStart,stopHopSM->smArray[i].stopHop.sharedLines[0].lineName[0]);
		_id.push_back(id);
	}
	return _id;
}

void Corridor::StopHopToFile(string filePath){
	stopHopSM->SMToFile(filePath);
	std::cout <<"-----------------------------"+string(sizeof(filePath),'-')<< std::endl;
	cout<<"StopHop to File "<<filePath<<endl;
	std::cout <<"-----------------------------"+string(sizeof(filePath),'-')<< std::endl;
}
 
bool Corridor::IfTerminus(string stop){
	for(int i = 0;i<totalLineStops.size();i++)
	{
		if(totalLineStops[i][totalLineStops[i].size()-1].stopName == stop)
			return true;
	}
	return false;
}


int Corridor::_GetMostPopStop(int startID,double threshold,int chooseTime,const bool ifLoad){

	vector<int> popStopID;
	vector<double> popFlow;

	for(int i =0;i<stopHopSM->Terms;i++)
	{//chooseTime 0-上午 1-下午 其他-凌晨
		double flow = 0;
		if(chooseTime==0)
		{
			if(ifLoad)
				flow = stopHopSM->smArray[i].stopHop.totalLoad.m_load;
			else
				flow = stopHopSM->smArray[i].stopHop.totalFlow.m_flow;
		}
		else if(chooseTime==1)
		{
			if(ifLoad)
				flow = stopHopSM->smArray[i].stopHop.totalLoad.a_load;
			else
				flow = stopHopSM->smArray[i].stopHop.totalFlow.a_flow;
		}
		else
		{
			if(ifLoad)
				flow = stopHopSM->smArray[i].stopHop.totalLoad.e_load;
			else
				flow = stopHopSM->smArray[i].stopHop.totalFlow.e_flow;
		}
		if(stopHopSM->smArray[i].row == startID
			&&flow>threshold
			&&ifCor[stopHopSM->smArray[i].row*stopsLines.size()+stopHopSM->smArray[i].col]==false)//总的流量限制
		{
			if(corridor.size()>0)//防止反向搜索
				if(stopHopSM->smArray[i].col == corridor[corridor.size()-1].stopStartID)
					continue;
			popStopID.push_back(i);
			popFlow.push_back(flow);
		}
	}

	if(popStopID.size()<1)
		return -1;
	int flag = popStopID[0];
	double maxFlow = popFlow[0];
	for(int i = 1;i<popStopID.size();++i)
	{
		if(maxFlow < popFlow[i])
		{
			maxFlow = popFlow[i];
			flag = popStopID[i];
		}
	}
	return flag;
}



int Corridor::GetMostPopStop(int startID,double threshold,int chooseTime,const bool ifLoad){
	
	vector<int> popStopID;
	vector<double> popFlow;

	for(int i =0;i<stopHopSM->Terms;i++)
	{//chooseTime 0-上午 1-下午 其他-凌晨
		double flow = 0;
		if(chooseTime==0)
		{
			if(ifLoad)
				flow = stopHopSM->smArray[i].stopHop.totalLoad.m_load;
			else
				flow = stopHopSM->smArray[i].stopHop.totalFlow.m_flow;
		}
		else if(chooseTime==1)
		{
			if(ifLoad)
				flow = stopHopSM->smArray[i].stopHop.totalLoad.a_load;
			else
				flow = stopHopSM->smArray[i].stopHop.totalFlow.a_flow;
		}
		else
		{
			if(ifLoad)
				flow = stopHopSM->smArray[i].stopHop.totalLoad.e_load;
			else
				flow = stopHopSM->smArray[i].stopHop.totalFlow.e_flow;
		}
		if(stopHopSM->smArray[i].row == startID
			&&flow>threshold
			&&ifCor[stopHopSM->smArray[i].row*stopsLines.size()+stopHopSM->smArray[i].col]==false)//总的流量限制
		{
			if(corridor.size()>0)//防止反向搜索
				if(stopHopSM->smArray[i].col == corridor[corridor.size()-1].stopStartID)
					continue;
			popStopID.push_back(i);
			popFlow.push_back(flow);
		}
	}
	
	if(popStopID.size()<1)
		return -1;
	int flag = popStopID[0];
	double maxFlow = popFlow[0];
	for(int i = 1;i<popStopID.size();++i)
	{
		if(maxFlow < popFlow[i])
		{
			maxFlow = popFlow[i];
			flag = popStopID[i];
		}
	}
	return flag;
}

int Corridor::SearchHotLineFromStop(int startID,double threshold,double _threshold,int chooseTime,int corridorID,const bool ifLoad){
	int i = GetMostPopStop(startID,threshold,chooseTime,ifLoad);
	if(i==-1)
		return -1;
	double flow_1 = 0;
	double flow_2 = 0;
	for(int j = 0;j<stopHopSM->smArray[i].stopHop.sharedLines.size();j++)
	{
		if(chooseTime==0)
		{	
			if(ifLoad)
			{
				flow_1 = stopHopSM->smArray[i].stopHop.sharedLines[j].segLoad.m_load;
				flow_2 = stopHopSM->smArray[i].stopHop.sharedLines[j].segFlow.m_flow;
			}
			else
			{
				flow_1 = stopHopSM->smArray[i].stopHop.sharedLines[j].segFlow.m_flow;
				flow_2 = stopHopSM->smArray[i].stopHop.sharedLines[j].segLoad.m_load;
			}
		}
		else if(chooseTime==1)
		{	
			if(ifLoad)
			{
				flow_1 = stopHopSM->smArray[i].stopHop.sharedLines[j].segLoad.a_load;
				flow_2 = stopHopSM->smArray[i].stopHop.sharedLines[j].segFlow.a_flow;
			}
			else
			{
				flow_1 = stopHopSM->smArray[i].stopHop.sharedLines[j].segFlow.a_flow;
				flow_2 = stopHopSM->smArray[i].stopHop.sharedLines[j].segLoad.a_load;
			}
		}
		else
		{	
			if(ifLoad)
			{
				flow_1 = stopHopSM->smArray[i].stopHop.sharedLines[j].segLoad.e_load;
				flow_2 = stopHopSM->smArray[i].stopHop.sharedLines[j].segFlow.e_flow;
			}
			else
			{
				flow_1 = stopHopSM->smArray[i].stopHop.sharedLines[j].segFlow.e_flow;
				flow_2 = stopHopSM->smArray[i].stopHop.sharedLines[j].segLoad.e_load;
			}
		}
		if(flow_1>_threshold)
		{
			ifCor[stopHopSM->smArray[i].row*stopsLines.size()+stopHopSM->smArray[i].col] = true;
			CorridorSeg seg;
			seg.stopStartID = stopHopSM->smArray[i].row;
			seg.stopEndID = stopHopSM->smArray[i].col;
			for(size_t l = 0;l<stopHopSM->smArray[i].stopHop.sharedLines[j].sidePt.size();l++)
			{
				CPt p;
				p.x = stopHopSM->smArray[i].stopHop.sharedLines[j].sidePt[l].x;
				p.y = stopHopSM->smArray[i].stopHop.sharedLines[j].sidePt[l].y;
				p.stopStart = stopHopSM->smArray[i].stopHop.stopStart;
				p.stopEnd = stopHopSM->smArray[i].stopHop.stopEnd;
				p.value = flow_1;
				p._value = flow_2;
				p.line = stopHopSM->smArray[i].stopHop.sharedLines[0].lineName[0];
				seg.side.push_back(p);
			}
					
			seg.corridorID = corridorID;
			corridor.push_back(seg);
			if(IfTerminus(stopHopSM->smArray[i].stopHop.stopEnd))//搜索到了某条线路的终点站
				return -1;
			else
				return stopHopSM->smArray[i].col;
		}
	}		
}
	
	


int Corridor::_SearchHotLineFromStop(int startID,double loadRateThre,double flowThre,int chooseTime,int corridorID){
	if(startID <0)
		return -1;
	
	vector<double>loads;
	vector<int> candidates;

	for(int i =0;i<stopHopSM->Terms;i++)
	{
		if(stopHopSM->smArray[i].row == startID)
		{
			double load = 0;//起始站
			double _load = 0;//终点站
			double flow = 0;
			double _flow = 0;
			if(chooseTime==0)
			{
				load = stopLoad[stopHopSM->smArray[i].row].m_load;
				_load = stopLoad[stopHopSM->smArray[i].col].m_load;
				flow = stopFlow[stopHopSM->smArray[i].row].m_flow;
				_flow = stopFlow[stopHopSM->smArray[i].col].m_flow;
			}
			else if(chooseTime==1)
			{
				load = stopLoad[stopHopSM->smArray[i].row].a_load;
				_load = stopLoad[stopHopSM->smArray[i].col].a_load;
				flow = stopFlow[stopHopSM->smArray[i].row].a_flow;
				_flow = stopFlow[stopHopSM->smArray[i].col].a_flow;
			}
			else
			{
				load = stopLoad[stopHopSM->smArray[i].row].e_load;
				_load = stopLoad[stopHopSM->smArray[i].col].e_load;
				flow = stopFlow[stopHopSM->smArray[i].row].e_flow;
				_flow = stopFlow[stopHopSM->smArray[i].col].e_flow;
			}
			double rate = (_load - load)/load;
			if(rate > loadRateThre &&ifCor[stopHopSM->smArray[i].row*stopsLines.size()+stopHopSM->smArray[i].col] == false 
				&& flow > flowThre && _flow > flowThre)
			{
				if(corridor.size()>0)//防止反向搜索
					if(stopHopSM->smArray[i].col == corridor[corridor.size()-1].stopStartID)
						continue;
				loads.push_back(_load);
				candidates.push_back(i);
				
			}
		}
	}

	if(loads.size() < 1)
		return -1;

	int flag = candidates[0];
	double lo = loads[0];
	for (int i = 1;i <loads.size();i++)
	{
		if (lo<loads[i])
		{
			lo = loads[i];
			flag = candidates[i];
		}
	}

	if(flag < 0)
		return -1;

	double val = 0;//路段的load
	double _val = 0;//路段的flow

	if(chooseTime==0)
	{
		//val = stopHopSM->smArray[flag].stopHop.totalLoad.m_load;
		//_val = stopHopSM->smArray[flag].stopHop.totalFlow.m_flow;
		val = stopLoad[stopHopSM->smArray[flag].row].m_load;
		_val = stopFlow[stopHopSM->smArray[flag].row].m_flow;
	}
	else if(chooseTime==1)
	{
		val = stopHopSM->smArray[flag].stopHop.totalLoad.a_load;
		_val = stopHopSM->smArray[flag].stopHop.totalFlow.a_flow;
	}
	else
	{
		val = stopHopSM->smArray[flag].stopHop.totalLoad.e_load;
		_val = stopHopSM->smArray[flag].stopHop.totalFlow.e_flow;
	}


	ifCor[stopHopSM->smArray[flag].row*stopsLines.size()+stopHopSM->smArray[flag].col] = true;
	CorridorSeg seg;
	seg.stopStartID = stopHopSM->smArray[flag].row;
	seg.stopEndID = stopHopSM->smArray[flag].col;
	for(size_t l = 0;l<stopHopSM->smArray[flag].stopHop.sharedLines[0].sidePt.size();l++)
	{
		CPt p;
		p.x = stopHopSM->smArray[flag].stopHop.sharedLines[0].sidePt[l].x;
		p.y = stopHopSM->smArray[flag].stopHop.sharedLines[0].sidePt[l].y;
		p.stopStart = stopHopSM->smArray[flag].stopHop.stopStart;
		p.stopEnd = stopHopSM->smArray[flag].stopHop.stopEnd;
		p.value = val;
		p._value = _val;
		p.line = stopHopSM->smArray[flag].stopHop.sharedLines[0].lineName[0];
		seg.side.push_back(p);
	}

	seg.corridorID = corridorID;
	corridor.push_back(seg);
	if(IfTerminus(stopHopSM->smArray[flag].stopHop.stopEnd))//搜索到了某条线路的终点站
		return -1;
	else
		return stopHopSM->smArray[flag].col;

	return -1;
}

void Corridor::SearchCorridorFromLine(double threshold,double _threshold,int chooseTime,const bool ifLoad){
	std::cout <<"-------------------------------------------"<< std::endl;
	std::cout <<"Start Search Corridor From Line..."<< std::endl;
	for(int i = 0;i<totalLineStops.size();i++)
	{
		int startID = -1;
		for(int j = 0;j<totalLineStops[i].size()-1;j++)
		{	
			int row = GetID(totalLineStops[i][j].stopName,totalLineStops[i][j].lineName);
			for(int k =0;k<stopHopSM->Terms;k++)
			{
				double flow = 0;
				if(chooseTime==0)
				{
					if(ifLoad)
						flow = stopHopSM->smArray[k].stopHop.totalLoad.m_load;
					else
						flow = stopHopSM->smArray[k].stopHop.totalFlow.m_flow;
				}
				else if(chooseTime==1)
				{
					if(ifLoad)
						flow = stopHopSM->smArray[k].stopHop.totalLoad.a_load;
					else
						flow = stopHopSM->smArray[k].stopHop.totalFlow.a_flow;
				}
				else
				{
					if(ifLoad)
						flow = stopHopSM->smArray[k].stopHop.totalLoad.e_load;
					else
						flow = stopHopSM->smArray[k].stopHop.totalFlow.e_flow;
				}
				if(stopHopSM->smArray[k].row == row&&flow>threshold&&ifCor[stopHopSM->smArray[k].row*stopsLines.size()+stopHopSM->smArray[k].col]==false)
				{//寻找每条线路第一个满足条件的站点作为搜索起点
					startID = row;
					break;
				}
			}
			if(startID >= 0)
				break;
			
		}
		while(startID>=0)
			startID = SearchHotLineFromStop(startID,threshold,_threshold,chooseTime,i,ifLoad);
	}

	std::cout <<"Get..."<< std::endl;
	std::cout <<"-------------------------------------------"<< std::endl;
}

bool Corridor::IfFlowShared(int cID,int _cID,int id,double p,int chTime,bool ifLoad){
	double tFlow = 0;//totol Flow
	double sFlow = 0;//shared Flow
	for(size_t i =0;i<corridor[cID].side.size()-1;i++)
	{
		if(i==0)
		{
			if(ifLoad)
				tFlow += corridor[cID].side[0]._value;
			else
				tFlow += corridor[cID].side[0].value;
		}
		else 
		{
			if(corridor[cID].side[i].stopStart != corridor[cID].side[i+1].stopStart)
			{
				if(ifLoad)
					tFlow += corridor[cID].side[i+1]._value;
				else
					tFlow += corridor[cID].side[i+1].value;
			}
		}
	}

	int sIndex = GetID(corridor[cID].side[corridor[cID].side.size()-1].stopEnd,corridor[cID].side[corridor[cID].side.size()-1].line);
	int eIndex = GetID(corridor[_cID].side[id].stopStart,corridor[_cID].side[id].line);

	for(int i = 0;i<stopHopSM->Terms;i++)
	{
		if(stopHopSM->smArray[i].row == sIndex && stopHopSM->smArray[i].col == eIndex)
		{
			if(chTime == 0)
				sFlow = stopHopSM->smArray[i].stopHop.totalFlow.m_flow;
			else if(chTime == 1)
				sFlow = stopHopSM->smArray[i].stopHop.totalFlow.a_flow;
			else
				sFlow = stopHopSM->smArray[i].stopHop.totalFlow.e_flow;
			break;
		}
	}
//	std::cout<<sFlow<<","<<tFlow<<endl;
	if(sFlow/tFlow> 0.2)//notice how to set the judge
		return true;
	else
		return false;
}


void Corridor::DeleteCompleteOverlap(){
	//complete overlap

	for(size_t i=0;i<corridor.size();i++)
	{
		for(size_t j=0;j<corridor.size();j++)
		{
			if(corridor[i].corridorID != corridor[j].corridorID)
			{
				int num = -1;
				for(size_t m =0;m<corridor[i].side.size();m++)
				{
					num = ClosestPt(corridor[i].side[m],corridor[j].side,50);
					if(num == -1)
					{
						j  = corridor.size()-1;
						break;
					}
				}
				if(num>-1)
				{
					int b = ClosestPt(corridor[i].side[0],corridor[j].side,50);
					int e = ClosestPt(corridor[i].side[corridor[i].side.size()-1],corridor[j].side,50);
					if(CosSimilarity(corridor[i].side[0].x,corridor[i].side[0].y,
						corridor[i].side[corridor[i].side.size()-1].x,corridor[i].side[corridor[i].side.size()-1].y,
						corridor[j].side[b].x,corridor[j].side[b].y,corridor[j].side[e].x,corridor[e].side[b].y,0.5))
					{
						for(size_t n =b;n<=e;n++)
						{//这里有问题，因为完全重叠可能二者重合点的个数不一致，这里统一采用第一个点的相加，存在误差，但是我估计影响不大
							corridor[j].side[n].value += corridor[i].side[0].value;
						}
						corridor.erase(corridor.begin()+i);
						i=-1;
						break;
					}
				}
			}
		}
	}
}

void Corridor::MergeCorridor(int chTime,bool ifLoad){
	std::cout <<"-------------------------------------------"<< std::endl;
	std::cout <<"Merge Corridor..."<< std::endl;

	if (corridor.size()<1)
	{
		std::cout <<"No corridor"<< std::endl;
		abort();
	}
	//remake corridor

	for(size_t i=0;i<corridor.size()-1;i++)
	{
		corridor[i].stops.push_back(corridor[i].side[0].stopStart);
		corridor[i].stops.push_back(corridor[i].side[0].stopEnd);
		while(i<corridor.size()-1&&corridor[i].corridorID == corridor[i+1].corridorID)
		{
			copy(corridor[i+1].side.begin(),corridor[i+1].side.end(),back_inserter(corridor[i].side));
			corridor[i].stops.push_back(corridor[i+1].side[0].stopEnd);
			corridor.erase(corridor.begin()+i+1);
		}
	}

	DeleteCompleteOverlap();

	
	//connect
	for(size_t i=0;i<corridor.size();i++)
	{
		for(size_t j=0;j<corridor.size();j++)
		{
			if(i!=j&&((PtDis(corridor[i].side[corridor[i].side.size()-1].x,
				corridor[i].side[corridor[i].side.size()-1].y,
				corridor[j].side[0].x,corridor[j].side[0].y)< 5
				&&CosSimilarity(corridor[i].side[corridor[i].side.size()-2].x,corridor[i].side[corridor[i].side.size()-2].y,
				corridor[i].side[corridor[i].side.size()-1].x,corridor[i].side[corridor[i].side.size()-1].y,
				corridor[j].side[0].x,corridor[j].side[0].y,
				corridor[j].side[1].x,corridor[j].side[1].y,0.8))//站点重合
				||
				(PtDis(corridor[i].side[corridor[i].side.size()-1].x,
				corridor[i].side[corridor[i].side.size()-1].y,
				corridor[j].side[0].x,corridor[j].side[0].y)< 500
				&&(CosSimilarity(corridor[i].side[corridor[i].side.size()-2].x,corridor[i].side[corridor[i].side.size()-2].y,
				corridor[i].side[corridor[i].side.size()-1].x,corridor[i].side[corridor[i].side.size()-1].y,
				corridor[j].side[0].x,corridor[j].side[0].y,0.9)||
				CosSimilarity(corridor[i].side[corridor[i].side.size()-1].x,corridor[i].side[corridor[i].side.size()-1].y,
				corridor[j].side[0].x,corridor[j].side[0].y,
				corridor[j].side[1].x,corridor[j].side[1].y,0.9)))))//连接两个走廊,corridor[i]连接corridor[j]
			{
				if(IfFlowShared(i,j,0,0.3,chTime,ifLoad))
				{
					copy(corridor[j].side.begin(),corridor[j].side.end(),back_inserter(corridor[i].side));
					corridor.erase(corridor.begin()+j);
					i=-1;
					j=corridor.size();
					break;
				}
			}
			
		}	
	}

	DeleteCompleteOverlap();

	//combine
	for(size_t i=0;i<corridor.size();i++)
	{
		for(size_t j=0;j<corridor.size();j++)
		{
			bool flag = false;
			if(corridor[i].corridorID != corridor[j].corridorID)
			{
	
				for(int m =0;m<corridor[j].side.size()-1;++m)
				{
					if(PointToSegDist(corridor[i].side[corridor[i].side.size()-1].x,
						corridor[i].side[corridor[i].side.size()-1].y,corridor[j].side[m].x,corridor[j].side[m].y,
						corridor[j].side[m+1].x,corridor[j].side[m+1].y)<50
						&&CosSimilarity(corridor[i].side[corridor[i].side.size()-2].x,corridor[i].side[corridor[i].side.size()-2].y,
						corridor[i].side[corridor[i].side.size()-1].x,corridor[i].side[corridor[i].side.size()-1].y,
						corridor[j].side[m].x,corridor[j].side[m].y,
						corridor[j].side[m+1].x,corridor[j].side[m+1].y,0.1))
					{
						int n =0;
						for(n=m;n<corridor[j].side.size()-1&&corridor[j].side[n].stopStart == corridor[j].side[n+1].stopStart;++n)
						{}
						if(n==corridor[j].side.size()-1)
							n--;
						if(IfFlowShared(i,j,n+1,0.3,chTime,ifLoad))
						{
							corridor[j].corridorID = corridor[i].corridorID;
							copy(corridor[i].side.begin(),corridor[i].side.end(),back_inserter(corridor[j].side));
							corridor.erase(corridor.begin()+i);
							flag = true;
							break;
						}
					}
				}
			}
			if(flag)
			{
				i=-1;
				j=corridor.size();
				break;
			}
		}	
	}
	
	DeleteCompleteOverlap();

	//delete
	for(size_t i=0;i<corridor.size();i++)
	{
		if(corridor[i].side.size()<30)
		{
			corridor.erase(corridor.begin()+i);
			i=-1;
		}
	}

	
	std::cout <<"Finish..."<< std::endl;
	std::cout <<"-------------------------------------------"<< std::endl;
}


double Corridor::CalPerLoad(bool ifLoadCor,int k){
	double tDis = 0;
	double load = 0;
	double tempDis = 0;


	if (corridor.size()<k)
	{
		std::cout<<"Not enough corridors"<<endl;
		k = corridor.size();
		std::cout<<"corridor's size is "<<corridor.size()<<endl;
	}

	for(size_t i=0;i<k;i++)
	{
		if(ifLoadCor)
			load += corridor[i].side[0].value;
		else
			load += corridor[i].side[0]._value;
		for(size_t j=0;j<corridor[i].side.size()-1;j++)
		{	
			//cout<<tDis<<"QQQQ"<<PtDis(corridor[i].side[j].x,corridor[i].side[j].y,corridor[i].side[j+1].x,corridor[i].side[j+1].y)<<endl;
			tDis += PtDis(corridor[i].side[j].x,corridor[i].side[j].y,corridor[i].side[j+1].x,corridor[i].side[j+1].y);	
			if(corridor[i].side[j].stopStart != corridor[i].side[j+1].stopStart)
			{
				if(ifLoadCor)
					load += corridor[i].side[j+1].value;
				else
					load += corridor[i].side[j+1]._value;
			}
			else
			{
				
				tempDis += PtDis(corridor[i].side[j].x,corridor[i].side[j].y,corridor[i].side[j+1].x,corridor[i].side[j+1].y);	
			}
		}
		corridor[i].totoalDis = tempDis;
		tempDis = 0;
	}
	double res = load/tDis;
	cout<<setprecision(10)<<load<<","<<tDis<<","<<res<<endl;
	return tDis;
}


void Corridor::CorridorToFile(string filePath){
	ofstream outFile(filePath,ios::out); 
	if(!outFile)
	{
		cout<<"error !"<<endl;
	}
	else
	{
		int count = 0;
		for(size_t i=0;i<corridor.size();i++)
		{
			//outFile<<corridor[i].stopStart<<","<<corridor[i].stopStartID<<","<<corridor[i].stopEnd<<","<<corridor[i].stopEndID<<endl;
			for(size_t j=0;j<corridor[i].side.size();j++)
			{
				/*if(i>0&&j==0&&corridor[i].side[j].x==corridor[i-1].side[corridor[i-1].side.size()-1].x)
					continue;*/
				outFile<<count<<","<<corridor[i].corridorID<<","<<setprecision(12)<<corridor[i].side[j].x<<","<<corridor[i].side[j].y<<",";
				outFile<<corridor[i].side[j].stopStart<<","<<corridor[i].side[j].stopEnd<<","<<corridor[i].side[j].value<<","<<corridor[i].side[j]._value<<endl;
				count++;
			}
		}
		outFile.close();	
		std::cout <<"-----------------------------"+string(sizeof(filePath),'-')<< std::endl;
		cout<<"Corridor To File "<<filePath<<endl;
		std::cout <<"-----------------------------"+string(sizeof(filePath),'-')<< std::endl;
	}
}


bool Corridor::ifCorContainTrip(vector<string> trip,int k){
	
	
	double totalCount = 0.0;//trip包含的总站点数
	double cCount = 0.0;//该trip通过走廊的总站点数
	
	for(size_t i = 0;i<trip.size();i++)
	{
		vector<string> fields; 
		string field;  

		istringstream sin(trip[i]); 
		while (getline(sin, field, ','))   
		{  
			fields.push_back(field); 
		}  
		int id = atoi(fields[0].c_str());
		string aboardTime = fields[1].c_str();
		time_t aboardTime_T = StringToDatetime(aboardTime);
		string lineName = fields[2].c_str();
		string aboardStop = fields[3].c_str();
		string alightTime = fields[6].c_str();
		time_t alightTime_T = StringToDatetime(alightTime);
		string alightStop = fields[8].c_str();
		string carNumber = fields[11];
		int dir = atoi(fields[13].c_str());  

		if(carNumber.compare("SUBWAY")==0)
		{
			for(vector<Stop>::size_type i =0;i<subwayLineCount;i++)
			{
				if(subwayLineStops[i][0].lineName.compare(lineName)==0&&subwayLineStops[i][0].direction == dir)
				{
					int flag = -1;
					for(vector<Stop>::size_type j =0;j<subwayLineStops[i].size();j++)
					{
						if(subwayLineStops[i][j].stopName == aboardStop)
							flag = 1;
						if(subwayLineStops[i][j].stopName == alightStop)
							flag = 0;
						if(flag > 0)
						{
							totalCount++;
							for(size_t q=0;q<k;q++)
							{
								for(size_t w=0;w<corridor[q].side.size();w++)
								{

									if(corridor[q].side[w].stopStart == subwayLineStops[i][j].stopName)
									{
										cCount++;
										q=k;
										break;
									}
								}
							}
						}
						if(flag == 0)
							break;
					}
					break;
				}
			}
		}
		else
		{
			for(vector<Stop>::size_type i =0;i<busLineCount;i++)
			{
				if(busLineStops[i][0].lineName.compare(lineName)==0&&busLineStops[i][0].direction == dir)
				{
					int flag = -1;
					for(vector<Stop>::size_type j =0;j<busLineStops[i].size();j++)
					{						
						if(busLineStops[i][j].stopName == aboardStop)
							flag = 1;
						if(busLineStops[i][j].stopName == alightStop)
							flag = 0;
						if(flag > 0)
						{
							totalCount++;
							for(size_t q=0;q<k;q++)
							{
								for(size_t w=0;w<corridor[q].side.size();w++)
								{

									if(corridor[q].side[w].stopStart == busLineStops[i][j].stopName)
									{
										cCount++;
										q=k;
										break;
									}
								}
							}
						}
						if(flag == 0)
							break;
					}
					break;
				}
			}
		}
	}
	if(cCount/totalCount< 0.66667)
		return false;
	else
		return true;
}



void Corridor::CalTripOfTopKCorridor(double dis,int stopCount,int k,string referTime,int chT){
	

	int a = 0;
	int b = 0;

	if(chT == 0)
	{
		a = mPeakBegin;
		b = mPeakEnd;
	}
	else if(chT == 1)
	{
		a = aPeakBegin;
		b = aPeakEnd;
	}

	if(corridor.size()<k)
	{
		std::cout<<"Not enough corridors"<<endl;
		k = corridor.size();
	}


	time_t t = InitReferTime(referTime);//time
	int tripCount = 0;
	int totalTrip = 0;
	vector<string> trip;
	for(size_t i = 0;i<leg.size()-1;i++)
	{
		

		vector<string> fields; 
        string field;  
		
		istringstream sin(leg[i]); 
        while (getline(sin, field, ','))   
        {  
            fields.push_back(field); 
        }  
		int id = atoi(fields[0].c_str());
		string aboardTime = fields[1].c_str();
		time_t aboardTime_T = StringToDatetime(aboardTime);
		string lineName = fields[2].c_str();
		string aboardStop = fields[3].c_str();
		string alightTime = fields[6].c_str();
		time_t alightTime_T = StringToDatetime(alightTime);
		string alightStop = fields[8].c_str();
		string carNumber = fields[11];
		int dir = atoi(fields[13].c_str());  

		if(aboardStop==alightStop)
			continue;
		trip.push_back(leg[i]);	
		fields.clear();
		istringstream sin_1(leg[i+1]); 
        while (getline(sin_1, field, ','))   
        {  
            fields.push_back(field); 
        } 
		int _id = atoi(fields[0].c_str());
		string _aboardTime = fields[1].c_str();
		time_t _aboardTime_T = StringToDatetime(_aboardTime);
		string _lineName = fields[2].c_str();
		string _aboardStop = fields[3].c_str();
		string _alightTime = fields[6].c_str();
		time_t _alightTime_T = StringToDatetime(_alightTime);
		string _alightStop = fields[8].c_str();
		string _carNumber = fields[11];
		int _dir = atoi(fields[13].c_str());  

		if(id == _id&&(_aboardTime_T-alightTime_T)<1800)
		{
			
		}
		else
		{
			istringstream sin_2(trip[0]); 
			fields.clear();
			while (getline(sin_2, field, ','))   
			{  
				fields.push_back(field); 
			} 
			string aTime = fields[1].c_str();
			time_t aTime_T = StringToDatetime(aTime);
			if(aTime_T>t+3600*a && aTime_T<t+3600*b)
			{
				totalTrip++;
				if(ifCorContainTrip(trip,k))
					tripCount++;
			}
			
			trip.clear();
			
		}
		
	}
	std::cout<<"Total trip count is :"<<totalTrip<<","<<"Trip count is :"<<tripCount<<std::endl;
	std::cout<<"TopK Stop count is :"<<stopCount<<","<<"Trip per stop is :"<<tripCount/stopCount;
	std::cout<<","<<"Trip per dis is :"<<tripCount/dis<<std::endl;
}


bool CMP_Cor(const CorridorSeg &a, const CorridorSeg &b){
	return a.totalValue > b.totalValue;
}


void Corridor::SortCorridor(bool ifLoadCor){
	for(size_t i=0;i<corridor.size();i++)
	{
		double load = 0;
		double flow = 0;
		if(ifLoadCor)
		{
			load += corridor[i].side[0].value;
			flow += corridor[i].side[0]._value;
		}
		else
		{
			load += corridor[i].side[0]._value;
			flow += corridor[i].side[0].value;
		}
		for(size_t j=0;j<corridor[i].side.size()-1;j++)
		{
			if(corridor[i].side[j].stopStart != corridor[i].side[j+1].stopStart)
			{
				if(ifLoadCor)
					load += corridor[i].side[j+1].value;
				else
					load += corridor[i].side[j+1]._value;
			}
		}
		corridor[i].totalValue = load;
		corridor[i]._totalValue = flow;
	}

	sort(corridor.begin(),corridor.end(),CMP_Cor);
}


void Corridor::GetTopkCorridorTrip(int topK){

}


int Corridor::TopKCorridorToFile(string filePath,int k,bool ifLoadCor){

	//返回topK走廊包含的站点数

	int stopCount = k;//统计每条走廊的第一个点

	if (corridor.size()<k)
	{
		cout<<"Not enough corridors."<<endl;
		k = corridor.size();
	}

	for(size_t i=0;i<k;i++)
	{

		for(size_t j=1;j<corridor[i].side.size()-1;j++)//从第一个点开始统计
		{
			if (corridor[i].side[j].stopStart != corridor[i].side[j+1].stopStart)
			{
				stopCount++;
			}
			
		}
	}


	ofstream outFile(filePath,ios::out); 
	if(!outFile)
	{
		cout<<"error !"<<endl;
	}
	else
	{
		int count = 0;
		for(size_t i=0;i<k;i++)
		{
			for(size_t j=0;j<corridor[i].side.size();j++)
			{
				
				outFile<<count<<","<<corridor[i].corridorID<<","<<setprecision(12)<<corridor[i].side[j].x<<","<<corridor[i].side[j].y<<",";
				outFile<<corridor[i].side[j].stopStart<<","<<corridor[i].side[j].stopEnd<<","<<corridor[i].side[j].value<<","<<corridor[i]._totalValue/corridor[i].totoalDis<<endl;
				count++;
			}
		}
		outFile.close();	
		std::cout <<"-----------------------------"+string(sizeof(filePath),'-')<< std::endl;
		cout<<"Corridor To File "<<filePath<<endl;
		std::cout <<"-----------------------------"+string(sizeof(filePath),'-')<< std::endl;
	}
	return stopCount;
}


int Corridor::ComparePts(vector<Pt> pt,vector<SideSeg> sharedLines){
	for(size_t i =0;i<sharedLines.size();i++)
	{
		size_t size_1 = pt.size();
		size_t size_2 = sharedLines[i].sidePt.size();
		size_t minCount = MIN(size_1,size_2);
		for(size_t j = 0;j<minCount-1;j++)
		{
			double detx_1 = pt[j+1].x - pt[j].x;
			double detx_2 = sharedLines[i].sidePt[j+1].x - sharedLines[i].sidePt[j].x;
			double dety_1 = pt[j+1].y - pt[j].y;
			double dety_2 = sharedLines[i].sidePt[j+1].y - sharedLines[i].sidePt[j].y;
			if(!CosSimilarity(detx_1,dety_1,detx_2,dety_2,0.8))
			{
				return i;
			}
		}
	}
	return -1;
}

void Corridor::InitStopHop(string stopPath){
	std::cout <<"-------------------------------------------"<< std::endl;
	std::cout <<"Init Stop Hop...."<< std::endl;
	ifstream fin(stopPath); 
    string line;  
	while (getline(fin, line))   
	{
		istringstream sin(line); 
        string field;  
		StopLines stopLines;
		int flag = 0;
        while (getline(sin, field, ','))   
        {  
			if(flag == 0)
				stopLines.stopName = field;
			else if(flag>2)
				stopLines.lines.push_back(field);
			flag++;
        }  
		if(stopLines.lines[0].substr(0,4) == "地铁")
			stopLines.lineType = 1;
		else
			stopLines.lineType = 0;
		stopsLines.push_back(stopLines);
	}
 	fin.close();
	////
	ifCor = new bool[stopsLines.size()*stopsLines.size()];
	for(int i = 0;i<stopsLines.size()*stopsLines.size();i++)
		ifCor[i] = false;
	//////
	for(size_t i= 0;i<busLineStops.size();i++)
	{
		totalLineStops.push_back(busLineStops[i]);
		totalStopSides.push_back(busStopSides[i]);
	}
	for(size_t i= 0;i<subwayLineStops.size();i++)
	{
		totalLineStops.push_back(subwayLineStops[i]);
		totalStopSides.push_back(subwayStopSides[i]);
	}
	//create stop hop
	stopHopSM = new SparseMatrix(busStopCount,stopsLines.size(),stopsLines.size());
	for(size_t i= 0;i<totalLineStops.size();i++)
	{
		
		for(size_t j= 0;j<totalLineStops[i].size()-1;j++)//except the last
		{
			bool ifInsert = true;
			int row = GetID(totalLineStops[i][j].stopName,totalLineStops[i][j].lineName);
			int col = GetID(totalLineStops[i][j+1].stopName,totalLineStops[i][j+1].lineName);
			if(row <0 || col <0)
				continue;
			for(int k = 0;k<stopHopSM->Terms;k++)
			{
				
				if(row==stopHopSM->smArray[k].row&&col==stopHopSM->smArray[k].col)
				{
					//int index = ComparePts(totalStopSides[i][j].side,stopHopSM->smArray[k].stopHop.sharedLines);
					int index = 0;
					if(index>=0)//几何上同一条边
					{
						stopHopSM->smArray[k].stopHop.sharedLines[index].lineName.push_back(totalLineStops[i][j].lineName);
						stopHopSM->smArray[k].stopHop.sharedLines[index].segFlow.m_flow += totalLineStops[i][j].flow.m_flow;
						stopHopSM->smArray[k].stopHop.sharedLines[index].segFlow.a_flow += totalLineStops[i][j].flow.a_flow;
						stopHopSM->smArray[k].stopHop.sharedLines[index].segFlow.e_flow += totalLineStops[i][j].flow.e_flow;
						stopHopSM->smArray[k].stopHop.sharedLines[index].segLoad.m_load += totalLineStops[i][j].load.m_load;
						stopHopSM->smArray[k].stopHop.sharedLines[index].segLoad.a_load += totalLineStops[i][j].load.a_load;
						stopHopSM->smArray[k].stopHop.sharedLines[index].segLoad.e_load += totalLineStops[i][j].load.e_load;
					}
					else//几何上的新一条边
					{
						SideSeg seg;
						seg.lineName.push_back(totalLineStops[i][j].lineName);
						seg.segFlow.m_flow = totalLineStops[i][j].flow.m_flow;
						seg.segFlow.a_flow = totalLineStops[i][j].flow.a_flow;
						seg.segFlow.e_flow = totalLineStops[i][j].flow.e_flow;
						seg.sidePt = totalStopSides[i][j].side;
						stopHopSM->smArray[k].stopHop.sharedLines.push_back(seg);
					}
					stopHopSM->smArray[k].stopHop.totalFlow.m_flow += totalLineStops[i][j].flow.m_flow;
					stopHopSM->smArray[k].stopHop.totalFlow.a_flow += totalLineStops[i][j].flow.a_flow;
					stopHopSM->smArray[k].stopHop.totalFlow.e_flow += totalLineStops[i][j].flow.e_flow;
					stopHopSM->smArray[k].stopHop.totalLoad.m_load += totalLineStops[i][j].load.m_load;
					stopHopSM->smArray[k].stopHop.totalLoad.a_load += totalLineStops[i][j].load.a_load;
					stopHopSM->smArray[k].stopHop.totalLoad.e_load += totalLineStops[i][j].load.e_load;
					ifInsert = false;
					break;
				}	
			}	
			if(ifInsert)
			{
				stopHopSM->smArray[stopHopSM->Terms].row = row;
				stopHopSM->smArray[stopHopSM->Terms].col = col;
				stopHopSM->smArray[stopHopSM->Terms].stopHop.isExist = true;
				SideSeg seg;
				seg.lineName.push_back(totalLineStops[i][j].lineName);
				seg.segFlow.m_flow = totalLineStops[i][j].flow.m_flow;
				seg.segFlow.a_flow = totalLineStops[i][j].flow.a_flow;
				seg.segFlow.e_flow = totalLineStops[i][j].flow.e_flow;
				seg.segLoad.m_load = totalLineStops[i][j].load.m_load;
				seg.segLoad.a_load = totalLineStops[i][j].load.a_load;
				seg.segLoad.e_load = totalLineStops[i][j].load.e_load;
				seg.sidePt = totalStopSides[i][j].side;
				stopHopSM->smArray[stopHopSM->Terms].stopHop.sharedLines.push_back(seg);
				stopHopSM->smArray[stopHopSM->Terms].stopHop.stopStart = totalLineStops[i][j].stopName;
				stopHopSM->smArray[stopHopSM->Terms].stopHop.stopEnd = totalLineStops[i][j+1].stopName;
				stopHopSM->smArray[stopHopSM->Terms].stopHop.totalFlow.m_flow = totalLineStops[i][j].flow.m_flow;
				stopHopSM->smArray[stopHopSM->Terms].stopHop.totalFlow.a_flow = totalLineStops[i][j].flow.a_flow;
				stopHopSM->smArray[stopHopSM->Terms].stopHop.totalFlow.e_flow = totalLineStops[i][j].flow.e_flow;
				stopHopSM->smArray[stopHopSM->Terms].stopHop.totalLoad.m_load = totalLineStops[i][j].load.m_load;
				stopHopSM->smArray[stopHopSM->Terms].stopHop.totalLoad.a_load = totalLineStops[i][j].load.a_load;
				stopHopSM->smArray[stopHopSM->Terms].stopHop.totalLoad.e_load = totalLineStops[i][j].load.e_load;
				stopHopSM->Terms++;	
				
			}
			
		}
	}
	
	std::cout <<"Finish Stop Hop...."<<std::endl;
	std::cout <<"-------------------------------------------"<< std::endl;
}


int Corridor::GetStopNum(string lineName,string aboardStop,string alightStop,string carNumber,int dir){

	int num = 0;;
	if(carNumber.compare("SUBWAY")==0)
	{
		if(aboardStop == alightStop)
			return 0;
		for(vector<Stop>::size_type i =0;i<subwayLineCount;i++)
		{
			if(subwayLineStops[i][0].lineName.compare(lineName)==0&&subwayLineStops[i][0].direction == dir)
			{
				int flag = -1;
				for(vector<Stop>::size_type j =0;j<subwayLineStops[i].size();j++)
				{
					if(subwayLineStops[i][j].stopName == aboardStop)
						flag = 1;
					if(subwayLineStops[i][j].stopName == alightStop)
						flag = 0;
					if(flag == 1)
					{
						num++;
					}
					if(flag == 0)
						break;
				}
				break;
			}
		}
	}
	else
	{
		for(vector<Stop>::size_type i =0;i<busLineCount;i++)
		{
			if(busLineStops[i][0].lineName.compare(lineName)==0&&busLineStops[i][0].direction == dir)
			{
				int flag = -1;
				for(vector<Stop>::size_type j =0;j<busLineStops[i].size();j++)
				{						
					if(busLineStops[i][j].stopName == aboardStop)
						flag = 1;
					if(busLineStops[i][j].stopName == alightStop)
						flag = 0;
					if(flag == 1)
					{
						num++;
					}
					if(flag == 0)
						break;
				}
				break;
			}
		}
	}
	return num;
}

vector<double> Corridor::GetFlowWeight(int n){
	vector<double> r;
	double nn = 1.0*n;
	double q = 1.0/nn/nn;
	for(size_t i = 0;i<n;i++)
	{

		r.push_back(1.0*(i+1)*q + 1.0/nn-(1.0+nn)*0.5/nn/nn);
	}
	return r;
}


void Corridor::CalFlowByWeight(vector<string> trip,vector<double> weight,time_t t){
	int wCount = 0;//Load 基于leg来统计 Flow基于Trip（给每个stop加权得到）
	for(size_t i = 0;i<trip.size();i++)
	{
		vector<string> fields; 
        string field;  
		
		istringstream sin(trip[i]); 
        while (getline(sin, field, ','))   
        {  
            fields.push_back(field); 
        }  
		int id = atoi(fields[0].c_str());
		string aboardTime = fields[1].c_str();
		time_t aboardTime_T = StringToDatetime(aboardTime);
		string lineName = fields[2].c_str();
		string aboardStop = fields[3].c_str();
		string alightTime = fields[6].c_str();
		time_t alightTime_T = StringToDatetime(alightTime);
		string alightStop = fields[8].c_str();
		string carNumber = fields[11];
		int dir = atoi(fields[13].c_str());  

		if(carNumber.compare("SUBWAY")==0)
		{
			for(vector<Stop>::size_type i =0;i<subwayLineCount;i++)
			{
				if(subwayLineStops[i][0].lineName.compare(lineName)==0&&subwayLineStops[i][0].direction == dir)
				{
					int flag = -1;
					for(vector<Stop>::size_type j =0;j<subwayLineStops[i].size();j++)
					{
						if(subwayLineStops[i][j].stopName == aboardStop)
							flag = 1;
						if(subwayLineStops[i][j].stopName == alightStop)
							flag = 0;
						if(flag > 0)
						{
							if(aboardTime_T>t+3600*mPeakBegin && aboardTime_T<t+3600*mPeakEnd )//早高峰 7点到9点
							{
								subwayLineStops[i][j].flow.m_flow += weight.size()*weight[wCount];
								subwayLineStops[i][j].load.m_load += flag;
							}
							else if(aboardTime_T>t+3600*aPeakBegin && aboardTime_T<t+3600*aPeakEnd)
							{//晚高峰 17：30到19：30							{
								subwayLineStops[i][j].flow.a_flow += weight.size()*weight[wCount];
								subwayLineStops[i][j].load.a_load += flag;
							}
							else
							{
								subwayLineStops[i][j].flow.e_flow += weight.size()*weight[wCount];
								subwayLineStops[i][j].load.e_load += flag;
							}
							flag++;
							wCount++;
						}
						if(flag == 0)
							break;
					}
					break;
				}
			}
		}
		else
		{
			for(vector<Stop>::size_type i =0;i<busLineCount;i++)
			{
				if(busLineStops[i][0].lineName.compare(lineName)==0&&busLineStops[i][0].direction == dir)
				{
					int flag = -1;
					for(vector<Stop>::size_type j =0;j<busLineStops[i].size();j++)
					{						
						if(busLineStops[i][j].stopName == aboardStop)
							flag = 1;
						if(busLineStops[i][j].stopName == alightStop)
							flag = 0;
						if(flag > 0)
						{
							if(aboardTime_T>t+3600*mPeakBegin&&aboardTime_T<t+3600*mPeakEnd )
							{
								busLineStops[i][j].flow.m_flow += weight.size()*weight[wCount];
								busLineStops[i][j].load.m_load += flag;
							}
							else if(aboardTime_T>t+3600*aPeakBegin&&aboardTime_T<t+3600*aPeakEnd)
							{
								busLineStops[i][j].flow.a_flow += weight.size()*weight[wCount];
								busLineStops[i][j].load.a_load += flag;
							}
							else
							{
								busLineStops[i][j].flow.e_flow += weight.size()*weight[wCount];
								busLineStops[i][j].load.e_load += flag;
							}
							wCount++;
							flag++;
						}
						if(flag == 0)
							break;
					}
					break;
				}
			}
		}
	}
}

void Corridor::_InitFlowSide(string legPath,string referTime){
	std::cout <<"-------------------------------------------"<< std::endl;
	std::cout <<"Init Flow...."<< std::endl;
	time_t t = InitReferTime(referTime);//time
	ifstream fin(legPath); 
    string line; 
	int count = 0;
	
	while (getline(fin, line))   
	{
		leg.push_back(line);
	}
	fin.close();

	vector<string> trip;
	int stopSum = 0;
	vector<double> weight;

	for(size_t i = 0;i<leg.size()-1;i++)
	{
		count++;

		vector<string> fields; 
        string field;  
		
		istringstream sin(leg[i]); 
        while (getline(sin, field, ','))   
        {  
            fields.push_back(field); 
        }  
		int id = atoi(fields[0].c_str());
		string aboardTime = fields[1].c_str();
		time_t aboardTime_T = StringToDatetime(aboardTime);
		string lineName = fields[2].c_str();
		string aboardStop = fields[3].c_str();
		string alightTime = fields[6].c_str();
		time_t alightTime_T = StringToDatetime(alightTime);
		string alightStop = fields[8].c_str();
		string carNumber = fields[11];
		int dir = atoi(fields[13].c_str());  

		if(aboardStop==alightStop)
			continue;

		if(dir==0)
		{
			weight = GetFlowWeight(stopSum);
			CalFlowByWeight(trip,weight,t);
			trip.clear();
			stopSum = 0;
			continue;
		}

		trip.push_back(leg[i]);
		stopSum += GetStopNum(lineName,aboardStop,alightStop,carNumber,dir);

		fields.clear();
		istringstream sin_1(leg[i+1]); 
        while (getline(sin_1, field, ','))   
        {  
            fields.push_back(field); 
        } 
		int _id = atoi(fields[0].c_str());
		string _aboardTime = fields[1].c_str();
		time_t _aboardTime_T = StringToDatetime(_aboardTime);
		string _lineName = fields[2].c_str();
		string _aboardStop = fields[3].c_str();
		string _alightTime = fields[6].c_str();
		time_t _alightTime_T = StringToDatetime(_alightTime);
		string _alightStop = fields[8].c_str();
		string _carNumber = fields[11];
		int _dir = atoi(fields[13].c_str());  

		if(id == _id&&(_aboardTime_T-alightTime_T)<1800)
		{
			/*trip.push_back(leg[i+1]);
			stopSum += GetStopNum(_lineName,_aboardStop,_alightStop,_carNumber,_dir);*/
		}
		else
		{
			weight = GetFlowWeight(stopSum);
			CalFlowByWeight(trip,weight,t);
			trip.clear();
			stopSum = 0;
		}
		if(count%100000 ==0)
			std::cout <<"Init "<<count<<"...."<< std::endl;
		if(count%6000000 ==0)
			break;
	}
	
	std::cout <<"Finish Init Weight Flow of Stops...."<< std::endl;
	std::cout <<"-------------------------------------------"<< std::endl;
}


void Corridor::InitFlowSide(string legPath,string referTime){
	std::cout <<"-------------------------------------------"<< std::endl;
	std::cout <<"Init Flow...."<< std::endl;
	time_t m_t = 0;
	time_t a_t = 0;
	time_t e_t = 0;
	InitReferTime(referTime);//time
	ifstream fin(legPath); 
    string line; 
	int count = 0;
	while (getline(fin, line))   
    {  
		count++;
        istringstream sin(line); 
        vector<string> fields; 
        string field;  
        while (getline(sin, field, ','))   
        {  
            fields.push_back(field); 
        }  
		string aboardTime = fields[1].c_str();
		string lineName = fields[2].c_str();
		time_t aboardTime_T = StringToDatetime(aboardTime);
		string aboardStop = fields[3].c_str();
		string alightStop = fields[8].c_str();
		
		string carNumber = fields[11];
		int dir = atoi(fields[13].c_str());  
		int flag = -1;
		if(carNumber.compare("SUBWAY")==0)
		{
			if(aboardStop == alightStop)
				continue;
			for(vector<Stop>::size_type i =0;i<subwayLineCount;i++)
			{
				if(subwayLineStops[i][0].lineName.compare(lineName)==0&&subwayLineStops[i][0].direction == dir)
				{
					int flag = -1;
					for(vector<Stop>::size_type j =0;j<subwayLineStops[i].size();j++)
					{
						if(subwayLineStops[i][j].stopName == aboardStop)
							flag = 1;
						if(subwayLineStops[i][j].stopName == alightStop)
							flag = 0;
						if(flag == 1)
						{
							if(aboardTime_T>m_t&&aboardTime_T<a_t)
								subwayLineStops[i][j].flow.m_flow++;
							else if(aboardTime_T>a_t&&aboardTime_T<e_t)
								subwayLineStops[i][j].flow.a_flow++;
							else
								subwayLineStops[i][j].flow.e_flow++;
						}
						if(flag == 0)
							break;
					}
					break;
				}
			}
		}
		else
		{
			for(vector<Stop>::size_type i =0;i<busLineCount;i++)
			{
				if(busLineStops[i][0].lineName.compare(lineName)==0&&busLineStops[i][0].direction == dir)
				{
					int flag = -1;
					for(vector<Stop>::size_type j =0;j<busLineStops[i].size();j++)
					{						
						if(busLineStops[i][j].stopName == aboardStop)
							flag = 1;
						if(busLineStops[i][j].stopName == alightStop)
							flag = 0;
						if(flag == 1)
						{
							if(aboardTime_T>m_t&&aboardTime_T<a_t)
								busLineStops[i][j].flow.m_flow++;
							else if(aboardTime_T>a_t&&aboardTime_T<e_t)
								busLineStops[i][j].flow.a_flow++;
							else
								busLineStops[i][j].flow.e_flow++;
						}
						if(flag == 0)
							break;
					}
					break;
				}
			}
		}
		if(count%100000 ==0)
			std::cout <<"Init "<<count<<"...."<< std::endl;
	}
	fin.close();
	std::cout <<"Finish Init Flow of Stops...."<< std::endl;
	std::cout <<"-------------------------------------------"<< std::endl;
}

void Corridor::InitLineStops(string busPath,string subwayPath){
	std::cout <<"-------------------------------------------"<< std::endl;
	std::cout <<"Init Line Stops of Bus and Subway...."<< std::endl;
	ifstream fin(busPath); 
    string line;   
	vector<Stop> stops;
    while (getline(fin, line))   
    {  
		busStopCount++;
        std::istringstream sin(line); 
		Stop stop;
        string field;  
		vector<string> fields;
        while (getline(sin, field, ','))   
        {  
            fields.push_back(field); 
        }  
		if(stops.size()>0&&(stops[stops.size()-1].lineName!=fields[0].c_str()||stops[stops.size()-1].direction!=atoi(fields[4].c_str())))
		{
			busLineStops.push_back(stops);
			stops.clear();
			stop.lineName = fields[0].c_str();
			stop.stopName = fields[1].c_str();
			stop.x = atof(fields[2].c_str());
			stop.y = atof(fields[3].c_str());
			stop.direction = atoi(fields[4].c_str());
			stop.flow.a_flow = 0;
			stop.flow.e_flow = 0;
			stop.flow.m_flow = 0;
			stop.load.a_load = 0;
			stop.load.e_load = 0;
			stop.load.m_load = 0;
			stops.push_back(stop);
		}
		else
		{
			stop.lineName = fields[0].c_str();
			stop.stopName = fields[1].c_str();
			stop.x = atof(fields[2].c_str());
			stop.y = atof(fields[3].c_str());
			stop.direction = atoi(fields[4].c_str());
			stop.flow.a_flow = 0;
			stop.flow.e_flow = 0;
			stop.flow.m_flow = 0;
			stop.load.a_load = 0;
			stop.load.e_load = 0;
			stop.load.m_load = 0;
			stops.push_back(stop);
		}
	}
	busLineStops.push_back(stops);
	stops.clear();
	fin.close();
	ifstream fin_1(subwayPath);   
    while (getline(fin_1, line))   
    {  
		subwayStopCount++;
        istringstream sin(line); 
		Stop stop;
        string field;  
		vector<string> fields;
        while (getline(sin, field, ','))   
        {  
            fields.push_back(field); 
        }  
		if(stops.size()>0&&(stops[stops.size()-1].lineName!=fields[0].c_str()||stops[stops.size()-1].direction!=atoi(fields[4].c_str())))
		{
			subwayLineStops.push_back(stops);
			stops.clear();
			stop.lineName = fields[0].c_str();
			stop.stopName = fields[1].c_str();
			stop.x = atof(fields[2].c_str());
			stop.y = atof(fields[3].c_str());
			stop.direction = atoi(fields[4].c_str());
			stop.flow.a_flow = 0;
			stop.flow.e_flow = 0;
			stop.flow.m_flow = 0;
			stop.load.a_load = 0;
			stop.load.e_load = 0;
			stop.load.m_load = 0;
			stops.push_back(stop);
		}
		else
		{
			stop.lineName = fields[0].c_str();
			stop.stopName = fields[1].c_str();
			stop.x = atof(fields[2].c_str());
			stop.y = atof(fields[3].c_str());
			stop.direction = atoi(fields[4].c_str());
			stop.flow.a_flow = 0;
			stop.flow.e_flow = 0;
			stop.flow.m_flow = 0;
			stop.load.a_load = 0;
			stop.load.e_load = 0;
			stop.load.m_load = 0;
			stops.push_back(stop);
		}
	}
	subwayLineStops.push_back(stops);
	stops.clear();
	fin_1.close();
	busLineCount = busLineStops.size();
	subwayLineCount = subwayLineStops.size();

	std::cout <<"Finish Init Stops of Line...."<< std::endl;
	std::cout <<"-------------------------------------------"<< std::endl;
}

void Corridor::InitLineStopSide(string busSegPath,string subwaySegPath){
	std::cout <<"-------------------------------------------"<< std::endl;
	std::cout <<"Init Line Segs of Stop Side...."<< std::endl;
	ifstream fin(subwaySegPath); 
    string line;   
	vector<Pt> pts_Subway;
    while (getline(fin, line))   
    {  
        std::istringstream sin(line); 
		Pt pt;
        string field;  
		vector<string> fields;
        while (getline(sin, field, ','))   
        {  
            fields.push_back(field); 
        }  
		pt.x = atof(fields[0].c_str());
		pt.y = atof(fields[1].c_str());
		pt.lineName = fields[2].c_str();
		pt.direction = atoi(fields[3].c_str());
		pts_Subway.push_back(pt);
	}
	fin.close();

	int indexF = 0;
	for(size_t i=0;i<subwayLineStops.size();i++)
	{

		for(int m = 0;m<pts_Subway.size();++m)
		{
			if(pts_Subway[m].direction == subwayLineStops[i][0].direction&&pts_Subway[m].lineName == subwayLineStops[i][0].lineName)
			{
				indexF = m;
				break;
			}
		}

		vector<StopSide> sides;
		for(size_t j=0;j<subwayLineStops[i].size()-1;j++)
		{
			int s = indexF;
			int e = indexF;
			StopSide side;
			side.lineName = subwayLineStops[i][j].lineName;
			side.stopStart = subwayLineStops[i][j].stopName;
			side.stopEnd = subwayLineStops[i][j+1].stopName;
			Pt pt;
			pt.direction = subwayLineStops[i][j].direction;
			pt.lineName = subwayLineStops[i][j].lineName;
			pt.x = subwayLineStops[i][j].x;
			pt.y = subwayLineStops[i][j].y;
			side.side.push_back(pt);
			
			for(int m = indexF;m<pts_Subway.size()-1&&pts_Subway[m+1].direction == pt.direction&&pts_Subway[m+1].lineName== pt.lineName;++m)
			{
				if(m==indexF&&CosSimilarity(pt.x,pt.y,pts_Subway[m].x,pts_Subway[m].y,pts_Subway[m+1].x,pts_Subway[m+1].y,0.4)
					&&PtDis(pts_Subway[m].x,pts_Subway[m].y,pt.x,pt.y)<50)
				{
					s = indexF;
					break;
				}
				else if(PtDis(pts_Subway[m].x,pts_Subway[m].y,pt.x,pt.y)<5)
				{
					s = m+1;
					break;
				}
				else if(CosSimilarity(pts_Subway[m].x,pts_Subway[m].y,pt.x,pt.y,pts_Subway[m+1].x,pts_Subway[m+1].y,0)
						&&IfFront(pt.x,pt.y,pts_Subway[m].x,pts_Subway[m].y,pts_Subway[m+1].x,pts_Subway[m+1].y)
						&&IfFront(pt.x,pt.y,pts_Subway[m+1].x,pts_Subway[m+1].y,pts_Subway[m].x,pts_Subway[m].y))
				{
					s = m+1;
					break;
				}
				else
				{
					s = -1;
				}
			}


			pt.direction = subwayLineStops[i][j+1].direction;
			pt.lineName = subwayLineStops[i][j+1].lineName;
			pt.x = subwayLineStops[i][j+1].x;
			pt.y = subwayLineStops[i][j+1].y;
			for(int m = s;m<pts_Subway.size()-1&&pts_Subway[m+1].direction == pt.direction&&pts_Subway[m+1].lineName== pt.lineName;++m)
			{
				if(CosSimilarity(pts_Subway[m].x,pts_Subway[m].y,pt.x,pt.y,pts_Subway[m+1].x,pts_Subway[m+1].y,0.2)
					&&IfFront(pt.x,pt.y,pts_Subway[m].x,pts_Subway[m].y,pts_Subway[m+1].x,pts_Subway[m+1].y)
					&&IfFront(pt.x,pt.y,pts_Subway[m+1].x,pts_Subway[m+1].y,pts_Subway[m].x,pts_Subway[m].y))
				{
					e = m;
					break;
				}
				else if(PtDis(pts_Subway[m].x,pts_Subway[m].y,pt.x,pt.y)<5)
				{
					e = m-1;
					break;
				}
				else
				{
					e = -1;
				}
			}
			if(s>=indexF&&e>=indexF)
				for(int k = s;k<=e;k++)
					side.side.push_back(pts_Subway[k]);

			pt.direction = subwayLineStops[i][j+1].direction;
			pt.lineName = subwayLineStops[i][j+1].lineName;
			pt.x = subwayLineStops[i][j+1].x;
			pt.y = subwayLineStops[i][j+1].y;
			side.side.push_back(pt);

			sides.push_back(side);

		}
		subwayStopSides.push_back(sides);
	}

	ifstream fin_1(busSegPath); 
	vector<Pt> pts_Bus;
	pts_Bus.reserve(80000);
    while (getline(fin_1, line))   
    {  
        std::istringstream sin(line); 
		Pt pt;
        string field;  
		vector<string> fields;
        while (getline(sin, field, ','))   
        {  
            fields.push_back(field); 
        }  
		pt.x = atof(fields[0].c_str());
		pt.y = atof(fields[1].c_str());
		pt.lineName = fields[2].c_str();
		pt.direction = atoi(fields[3].c_str());
		pts_Bus.push_back(pt);
	}
	fin_1.close();

	indexF = 0;//上一次的索引
	
	for(size_t i=0;i<busLineStops.size();i++)
	{

		for(int m = 0;m<pts_Bus.size();++m)
		{
			if(pts_Bus[m].direction == busLineStops[i][0].direction&&pts_Bus[m].lineName == busLineStops[i][0].lineName)
			{
				indexF = m;
				break;
			}
		}

		vector<StopSide> sides;
		for(size_t j=0;j<busLineStops[i].size()-1;j++)
		{
			int s = indexF;
			int e = indexF;
			StopSide side;
			side.lineName = busLineStops[i][j].lineName;
			side.stopStart = busLineStops[i][j].stopName;
			side.stopEnd = busLineStops[i][j+1].stopName;
			Pt pt;
			pt.direction = busLineStops[i][j].direction;
			pt.lineName = busLineStops[i][j].lineName;
			pt.x = busLineStops[i][j].x;
			pt.y = busLineStops[i][j].y;
			side.side.push_back(pt);
			
			for(int m = indexF;m<pts_Bus.size()-1&&pts_Bus[m+1].direction == pt.direction&&pts_Bus[m+1].lineName== pt.lineName;++m)
			{
				if(m==indexF&&CosSimilarity(pt.x,pt.y,pts_Bus[m].x,pts_Bus[m].y,pts_Bus[m+1].x,pts_Bus[m+1].y,0.2)
					&&PtDis(pts_Bus[m].x,pts_Bus[m].y,pt.x,pt.y)<50)
				{
					s = indexF;
					break;
				}
				else if(PtDis(pts_Bus[m].x,pts_Bus[m].y,pt.x,pt.y)<5)
				{
					s = m+1;
					break;
				}
				else if(CosSimilarity(pts_Bus[m].x,pts_Bus[m].y,pt.x,pt.y,pts_Bus[m+1].x,pts_Bus[m+1].y,0)
						&&IfFront(pt.x,pt.y,pts_Bus[m].x,pts_Bus[m].y,pts_Bus[m+1].x,pts_Bus[m+1].y)
						&&IfFront(pt.x,pt.y,pts_Bus[m+1].x,pts_Bus[m+1].y,pts_Bus[m].x,pts_Bus[m].y))
				{
					s = m+1;
					break;
				}
				else
				{
					s = -1;
				}
			}


			pt.direction = busLineStops[i][j+1].direction;
			pt.lineName = busLineStops[i][j+1].lineName;
			pt.x = busLineStops[i][j+1].x;
			pt.y = busLineStops[i][j+1].y;
			for(int m = s;m<pts_Bus.size()-1&&pts_Bus[m+1].direction == pt.direction&&pts_Bus[m+1].lineName== pt.lineName;++m)
			{
				if(CosSimilarity(pts_Bus[m].x,pts_Bus[m].y,pt.x,pt.y,pts_Bus[m+1].x,pts_Bus[m+1].y,0)
					&&IfFront(pt.x,pt.y,pts_Bus[m].x,pts_Bus[m].y,pts_Bus[m+1].x,pts_Bus[m+1].y)
					&&IfFront(pt.x,pt.y,pts_Bus[m+1].x,pts_Bus[m+1].y,pts_Bus[m].x,pts_Bus[m].y))
				{
					e = m;
					break;
				}
				else if(PtDis(pts_Bus[m].x,pts_Bus[m].y,pt.x,pt.y)<5)
				{
					e = m-1;
					break;
				}
				else
				{
					e = -1;
				}
			}
			if(s>=indexF&&e>=indexF)
				for(int k = s;k<=e;k++)
					side.side.push_back(pts_Bus[k]);

			pt.direction = busLineStops[i][j+1].direction;
			pt.lineName = busLineStops[i][j+1].lineName;
			pt.x = busLineStops[i][j+1].x;
			pt.y = busLineStops[i][j+1].y;
			side.side.push_back(pt);

			sides.push_back(side);

		}
		busStopSides.push_back(sides);
	}


	std::cout <<"Finish Init Line Segs of Stop Side...."<< std::endl;
	std::cout <<"-------------------------------------------"<< std::endl;
}


void Corridor::_SearchCorridorFromLine(double coreThre,double loadRateThre,double flowThre,int chooseTime,const bool ifLoad){
	//第一个阈值表示种子站点的阈值
	//第二个阈值表示搜索共享load的阈值
	std::cout <<"-------------------------------------------"<< std::endl;
	std::cout <<"Start Search Corridor From Line..."<< std::endl;
	for(int i = 0;i<totalLineStops.size();i++)
	{
		int startID = -1;
		for(int j = 0;j<totalLineStops[i].size()-1;j++)
		{	
			int row = GetID(totalLineStops[i][j].stopName,totalLineStops[i][j].lineName);//地铁站和公交站有同名情况
			if(row <0 )
				continue;
			double load = 0;
			if(chooseTime==0)
			{
				load = stopLoad[row].m_load;
			}
			else if(chooseTime==1)
			{
				load = stopLoad[row].a_load;
			}
			else
			{
				load = stopLoad[row].e_load;
			}
			if(load>coreThre)
				//&&ifCor[row*stopsLines.size()+row]==false)
			{//寻找每条线路第一个满足条件的站点作为搜索起点
				//ifCor[row*stopsLines.size()+row] =true;
				startID = row;
				break;
			}

		}
		if(startID>=0)
			startID = _SearchHotLineFromStop(startID,loadRateThre,flowThre,chooseTime,i);
	}

	std::cout <<"Get..."<< std::endl;
	std::cout <<"-------------------------------------------"<< std::endl;
}


void Corridor::SidePtToFile(string outBusPath,string outSubwayPath){
	ofstream outFile(outBusPath,ios::out); 
	if(!outFile)
	{
		cout<<"error !";
	}
	else
	{
		for(size_t i = 0;i<busStopSides.size();i++)
		{
			for(size_t j = 0;j<busStopSides[i].size()-1;j++)
			{
				for(size_t k = 0;k<busStopSides[i][j].side.size();k++)
				{
					outFile<<setprecision(12)<<busStopSides[i][j].side[k].x<<","<<busStopSides[i][j].side[k].y;
					if(k<busStopSides[i][j].side.size()-1)
						outFile<<",";
				}
				outFile<<endl; 
			}
			    
		}
		outFile.close();
	}

	ofstream outFile_1(outSubwayPath,ios::out); 
	if(!outFile_1)
	{
		cout<<"error !";
	}
	else
	{
		for(size_t i = 0;i<subwayStopSides.size();i++)
		{
			for(size_t j = 0;j<subwayStopSides[i].size()-1;j++)
			{
				for(size_t k = 0;k<subwayStopSides[i][j].side.size();k++)
				{
					outFile_1<<setprecision(12)<<subwayStopSides[i][j].side[k].x<<","<<subwayStopSides[i][j].side[k].y;
					if(k<subwayStopSides[i][j].side.size()-1)
						outFile_1<<",";
				}
				outFile_1<<endl; 
			}
			    
		}
		outFile_1.close();
	}
}




void Corridor::InitLineStopSideFromFile(string inBusPath,string inSubwayPath){
	std::cout <<"Init Stop Side From "<<inBusPath<<"...."<<std::endl;
	ifstream fin(inBusPath); 
    string line;  
	busStopSides.clear();
	vector<StopSide> ss;
	int count = 0;
	int i = 0;
	while (getline(fin, line))   
	{
		count++;
		istringstream sin(line); 
        string field;  
		StopSide s;
		
		int flag = 0;
		Pt pt;
        while (getline(sin, field, ','))   
        {  
			if(flag%2==0)
				pt.x = atof(field.c_str());
			else
			{
				pt.y = atof(field.c_str());
				s.side.push_back(pt);
			}
			flag++;
			
        }  
		ss.push_back(s);
		if(count==busLineStops[i].size()-1)
		{
			count = 0;
			i++;
			busStopSides.push_back(ss);
			ss.clear();
		}
	}
 	fin.close();
}
