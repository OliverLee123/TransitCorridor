#include "stdafx.h"
#include"SparseMatrix.h"
#include<iostream>  
#include<assert.h>  
#include<fstream> 
using namespace std;


SparseMatrix::SparseMatrix(){
	Rows=0;
	Cols=0;
	Terms=0;  
}

SparseMatrix::SparseMatrix(int maxSz,int rows,int cols):maxTerms(maxSz){     //���캯��������һ����СΪmaxTerm����Ԫ�飬�������ͷ���Ԫ�ظ���������  
    if(maxSz<1){  
        cerr<<"�����ʼ������"<<endl;  
        exit(1);  
    }  
    smArray=new Trituple[maxSz];  
    assert(smArray!=NULL);  
    Rows=rows;
	Cols=cols;
	Terms=0;  
}  
SparseMatrix::SparseMatrix(SparseMatrix& SM){  //���ƹ��캯��  
    Rows=SM.Rows;      //��ֵ���������  
    Cols=SM.Cols;  
    Terms=SM.Terms;  
    maxTerms=SM.maxTerms;  
    smArray=new Trituple[maxTerms];  //������Ԫ�鲢����SM��ͬ��ֵ  
    assert(smArray!=NULL);  
    for(int i=0;i<Terms;i++)  
        smArray[i]=SM.smArray[i];  
}  
SparseMatrix::~SparseMatrix(){   //�����������ͷ����д洢  
    if(smArray)
		delete[]smArray;  
}  
SparseMatrix& SparseMatrix::operator=(SparseMatrix& SM){ //��ֵ���������  
    Rows=SM.Rows;      //Ԫ�����ʵĸ�ֵ  
    Cols=SM.Cols;  
    Terms=SM.Terms;  
    maxTerms=SM.maxTerms;  
    for(int i=0;i<Terms;i++)      //��Ԫ������Ԫ�ظ�ֵ  
        smArray[i]=SM.smArray[i];  
    return *this;    //���ص��ǶԵ��øú����Ķ�������ã�����ʽʹ��thisָ�룻  
}  
ostream& operator<<(ostream& ostr,SparseMatrix& SM){  //������������ ��Ϊɶ��ģ��Ͳ��ܵ���row�� ��  
    ostr<<"# Rows="<<SM.Rows<<endl;    //����þ��������  
    ostr<<"# Cols="<<SM.Cols<<endl;  
    ostr<<"# Terms="<<SM.Terms<<endl;  
	for(int i=0;i<SM.Terms;i++)     
	{
		ostr<<i+1<<": "<<"SM: "<<SM.smArray[i].row<<","<<SM.smArray[i].col<<endl;
		ostr<<SM.smArray[i].stopHop.stopStart<<endl;
		for(size_t j=0;j<SM.smArray[i].stopHop.sharedLines.size();j++)
		{
			for(size_t k=0;k<SM.smArray[i].stopHop.sharedLines[j].lineName.size();k++)
				ostr<<"  "<<SM.smArray[i].stopHop.sharedLines[j].lineName[k];
			ostr<<endl;
		}
		ostr<<endl;
		ostr<<"Flow"<<endl;
		ostr<<"06:00-12:00:"<<SM.smArray[i].stopHop.totalFlow.m_flow<<endl;
		ostr<<"12:00-22:00:"<<SM.smArray[i].stopHop.totalFlow.a_flow<<endl;
		ostr<<"00:00-06:00&22:00-24:00:"<<SM.smArray[i].stopHop.totalFlow.e_flow<<endl;
		ostr<<SM.smArray[i].stopHop.stopEnd<<endl;
		ostr<<endl;
	}	
    return ostr;  
}  
istream& operator>> (istream& istr,SparseMatrix& SM){  //�������������  
    cout<<"Please enter number of rows,columns,and terms of Matrix"<<endl;  
    istr>>SM.Rows>>SM.Cols>>SM.Terms;  //����Ԫ�ص�����  
    if(SM.Terms>SM.maxTerms){  
        cerr<<"Numbers of Terms overflow!"<<endl;  
        exit(1);  
    }  
    for(int i=0;i<SM.Terms;i++){   //�����������Ԫ�ص������ֵ  
        cout<<"Enter row,column,and value of term:"<<i+1<<endl;  
        cin>>SM.smArray[i].row>>SM.smArray[i].col;  
    }  
    return istr;  
}  

void SparseMatrix::SMToFile(string filePath){
	ofstream outFile(filePath,ios::out); 
	if(!outFile)
	{
		cout<<"error !"<<endl;
	}
	else
	{
		outFile<<"# Rows="<<Rows<<endl;    //����þ��������  
		outFile<<"# Cols="<<Cols<<endl;  
		outFile<<"# Terms="<<Terms<<endl;  
		for(int i=0;i<Terms;i++)     
		{
			outFile<<i+1<<": "<<"SM: "<<smArray[i].row<<","<<smArray[i].col<<endl;
			outFile<<smArray[i].stopHop.stopStart<<endl;
			for(size_t j=0;j<smArray[i].stopHop.sharedLines.size();j++)
			{
				for(size_t k=0;k<smArray[i].stopHop.sharedLines[j].lineName.size();k++)
					outFile<<"  "<<smArray[i].stopHop.sharedLines[j].lineName[k];
				outFile<<endl;
			}
			outFile<<smArray[i].stopHop.stopEnd<<endl;
			outFile<<"Flow"<<endl;
			outFile<<"Time_1:"<<smArray[i].stopHop.totalFlow.m_flow<<","<<smArray[i].stopHop.totalLoad.m_load<<endl;
			outFile<<"Time_2:"<<smArray[i].stopHop.totalFlow.a_flow<<","<<smArray[i].stopHop.totalLoad.a_load<<endl;
			outFile<<"Time_3:"<<smArray[i].stopHop.totalFlow.e_flow<<","<<smArray[i].stopHop.totalLoad.e_load<<endl;	
			outFile<<endl;
		}	
		outFile.close();	
	}
}

bool CMP_1(const Trituple &a, const Trituple &b){
	return a.stopHop.totalFlow.m_flow > b.stopHop.totalFlow.m_flow;
}

bool CMP_2(const Trituple &a, const Trituple &b){
	return a.stopHop.totalFlow.a_flow > b.stopHop.totalFlow.a_flow;
}

bool CMP_3(const Trituple &a, const Trituple &b){
	return a.stopHop.totalFlow.e_flow > b.stopHop.totalFlow.e_flow;
}

