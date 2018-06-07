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
#include "DataProcessing.h"
using namespace std;



double PointToSegDist(double x, double y, double x1, double y1, double x2, double y2)  
{  
	double cross = (x2 - x1) * (x - x1) + (y2 - y1) * (y - y1);  
	if (cross <= 0) 
		return sqrt((x - x1) * (x - x1) + (y - y1) * (y - y1));  
  
	double d2 = (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1);  
	if (cross >= d2) 
		return sqrt((x - x2) * (x - x2) + (y - y2) * (y - y2));  
   
	double r = cross / d2;  
	double px = x1 + (x2 - x1) * r;  
	double py = y1 + (y2 - y1) * r;  
	return sqrt((x - px) * (x - px) + (py - y1) * (py - y1));  
}  


void ShowTime()
{
    time_t timep;
    time (&timep);
    char tmp[64];
    strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S",localtime(&timep) );
    std::cout <<tmp<< std::endl;
}

time_t StringToDatetime(string str)
{
    char *cha = (char*)str.data();             
    tm tm_;                                    
    int year, month, day, hour, minute, second;
    sscanf(cha, "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second);
    tm_.tm_year = year - 1900;                
    tm_.tm_mon = month - 1;                   
    tm_.tm_mday = day;                        
    tm_.tm_hour = hour;                       
    tm_.tm_min = minute;                      
    tm_.tm_sec = second;                      
    tm_.tm_isdst = 0;                         
    time_t t_ = mktime(&tm_);                
    return t_;                                
}

bool IfFront(double x1,double y1,double x2,double y2 ,double x3,double y3){//利用象限判断三点前后关系，即1是否在2和3中间
	if(y3>=y2&&x3>=x2)
	{//1
		if(y1>=y2||x1>=x2)
			return true;
		else
			return false;
	}	
	else if(y3>=y2&&x3<=x2)
	{//2
		if(y1>=y2||x1<=x2)
			return true;
		else
			return false;
	}
	if(y3<=y2&&x3<=x2)
	{//3
		if(y1<=y2||x1<=x2)
			return true;
		else
			return false;
	}
	else
	{//4
		if(y1<=y2||x1>=x2)
			return true;
		else
			return false;
	}
}

time_t InitReferTime(string str)
{
    char *cha = (char*)str.data();             
    tm tm_;                                    
    int year, month, day, hour, minute, second;
    sscanf(cha, "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second);// 将string存储的日期时间，转换为int临时变量。
    hour = 0;
	minute = 0;
	second = 0;
	tm_.tm_year = year - 1900;              
    tm_.tm_mon = month - 1;                 
    tm_.tm_mday = day;                        
    tm_.tm_hour = hour;                       
    tm_.tm_min = minute;                      
    tm_.tm_sec = second;                      
    tm_.tm_isdst = 0;                         
    time_t t_ = mktime(&tm_);  
	return t_;
}

void GetStopsDirection(std::string filePath,std::string outputPath)
{
	std::cout <<"Get Direction...."<< std::endl;

	std::ofstream outFile(outputPath,std::ios::out); 
	std::ifstream fin(filePath); 
    std::string line;   
	std::vector<Stop> stops;
    while (std::getline(fin, line))   
    {  
        std::istringstream sin(line); 
		Stop stop;
        std::string field;  
		std::vector<std::string> fields;
        while (getline(sin, field, ','))   
        {  
            fields.push_back(field); 
        }  
		stop.lineName = fields[0].c_str();
		stop.stopName = fields[1].c_str();
		stop.x = atof(fields[2].c_str());
		stop.y = atof(fields[3].c_str());
		stops.push_back(stop);
	}
	fin.close();
	for(size_t i = 0;i<stops.size();i++)
	{
		if(i>0)
			i--;
		int direction;
		double x_start = stops[i].x;
		double y_start = stops[i].y;
		int id_start = i;
		double x_end = 0.0;
		double y_end = 0.0;
		int id_end = 0;
		while(stops[i].lineName.compare(stops[i+1].lineName)==0&&stops[i].stopName.compare(stops[i+1].stopName)!=0)
		{
			x_end = stops[i+1].x;
			y_end = stops[i+1].y;
			id_end = i+1;
			i++;
			if(i == stops.size()-1)
				break;
		}
		if(abs(x_end - x_start)<2&&abs(y_end - y_start)<2)
			direction = 0;
		else if (y_end > y_start)
			direction = 1;
		else 
			direction = 2;
		for(i = id_start;i<= id_end;i++)
		{
			outFile<<stops[i].lineName<<","<<stops[i].stopName<<",";
			outFile<<std::setprecision(12)<<stops[i].x<<","<<stops[i].y<<",";
			outFile<<direction<<std::endl;
		}
	}
	outFile.close();

	std::cout <<"Finish...."<< std::endl;
}

bool CosSimilarity(double x1,double y1,double x2,double y2 ,double x3,double y3,double maxSita)
{
	double fx = x2 - x1;
	double fy = y2 - y1;
	double lx = x3 - x2;
	double ly = y3 - y2;
	double m = sqrt(fx*fx+fy*fy);
	double n = sqrt(lx*lx+ly*ly);
	double p = fx*lx+fy*ly;
	double cosSita = p/(m*n);
	if(cosSita > maxSita)
		return true;
	else
		return false;
}


bool CosSimilarity(double x1,double y1,double x2,double y2 ,double x3,double y3,double x4,double y4,double maxSita)
{
	double fx = x2 - x1;
	double fy = y2 - y1;
	double lx = x4 - x3;
	double ly = y4 - y3;
	double m = sqrt(fx*fx+fy*fy);
	double n = sqrt(lx*lx+ly*ly);
	double p = fx*lx+fy*ly;
	double cosSita = p/(m*n);
	if(cosSita > maxSita)
		return true;
	else
		return false;
}


bool CosSimilarity(double x1,double y1,double x2,double y2,double maxSita)
{
	double fx = x1;
	double fy = y1;
	double lx = x2;
	double ly = y2;
	double m = sqrt(fx*fx+fy*fy);
	double n = sqrt(lx*lx+ly*ly);
	double p = fx*lx+fy*ly;
	double cosSita = p/(m*n);
	if(abs(cosSita) > maxSita)
		return true;
	else
		return false;
}

double PtDis(double x1,double y1,double x2,double y2){

	return sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}

int ClosestPt(CPt p,const vector<CPt>& pts,double maxDis){
	int num = -1;
	for(size_t i = 0;i<pts.size();i++)
	{
		if(PtDis(p.x,p.y,pts[i].x,pts[i].y)<maxDis)
		{
			num =i;
			maxDis = PtDis(p.x,p.y,pts[i].x,pts[i].y);
		}
	}
	return num;
}


void SimplizeLine(string filePath,string outPath){
	ifstream fin(filePath); 
    string line;   
	LineSegment *head;
	LineSegment *cur = new LineSegment;
	head = cur;
    while (getline(fin, line))   
    {  
        std::istringstream sin(line); 
		Pt pt;
        string field;  
		vector<string> fields;
        while (getline(sin, field, ' '))   
        {  
            fields.push_back(field); 
        }  
		LineSegment *seg = new LineSegment;
		seg->x = atof(fields[0].c_str());
		seg->y = atof(fields[1].c_str());
		seg->lineName = fields[2].c_str();
		seg->direction = atoi(fields[3].c_str());
		seg->next = NULL;
		cur->next = seg;
		cur = seg;
	}
	fin.close();
	cur = head->next;
	int i =0;
	//while(cur->next->next!=NULL)
	//{
	//	bool flag = true;
	//	if(cur->lineName==cur->next->lineName&&cur->next->lineName==cur->next->next->lineName&&
	//		cur->direction==cur->next->direction&&cur->next->direction==cur->next->next->direction&&
	//		CosSimilarity(cur->x,cur->y,cur->next->x,cur->next->y,cur->next->next->x,cur->next->next->y,0.95))
	//	{
	//		i++;
	//		LineSegment * temp = cur->next;
	//		cur->next = cur->next->next;
	//		delete temp;
	//		flag = false;
	//		cout<<i<<endl;
	//	}
	//	if(flag)
	//		cur = cur->next;
	//}

	cur = head->next;
	ofstream outFile(outPath,ios::out); 
	if(!outFile)
	{
		cout<<"error !";
	}
	else
	{
		while(cur)
		{
			outFile<<setprecision(12)<<cur->x<<","<<cur->y<<",";
			outFile<<cur->lineName<<","<<cur->direction<<endl;     
			cur = cur->next;
		}
		outFile.close();
	}
}

//void SimplizeLine(string filePath,string outPath){
//	ifstream fin(filePath); 
//    string line;   
//	vector<Pt> pts;
//    while (getline(fin, line))   
//    {  
//        std::istringstream sin(line); 
//		Pt pt;
//        string field;  
//		vector<string> fields;
//        while (getline(sin, field, ' '))   
//        {  
//            fields.push_back(field); 
//        }  
//		pt.x = atof(fields[0].c_str());
//		pt.y = atof(fields[1].c_str());
//		pt.lineName = fields[2].c_str();
//		pt.direction = atoi(fields[3].c_str());
//		pts.push_back(pt);
//	}
//	fin.close();
//	for(size_t i = 0;i<pts.size()-2;i++)
//	{
//		if(pts[i].lineName==pts[i+1].lineName&&pts[i+1].lineName==pts[i+2].lineName&&
//			pts[i].direction==pts[i+1].direction&&pts[i+1].direction==pts[i+2].direction&&
//			CosSimilarity(pts[i].x,pts[i].y,pts[i+1].x,pts[i+1].y,pts[i+2].x,pts[i+2].y,0.9))
//			pts.erase(pts.begin()+i);
//	}
//
//	ofstream outFile(outPath,ios::out); 
//	if(!outFile)
//	{
//		cout<<"error !";
//	}
//	else
//	{
//		for(size_t i = 0;i<pts.size();i++)
//		{
//			outFile<<setprecision(12)<<pts[i].x<<","<<pts[i].y<<",";
//			outFile<<pts[i].lineName<<","<<pts[i].direction<<endl;     
//		}
//		outFile.close();
//	}
//}