#ifndef SparseMatrix_H
#define SparseMatrix_H


#include<iostream>  
#include "DataProcessing.h"
using namespace std;  




struct Trituple{        //�Զ������ݽṹ������Ԫ�ص��У��У�ֵ��  
    int row,col;  
    StopHop stopHop;  
    Trituple& operator=(Trituple& x){   
        row=x.row;  
        col=x.col;  
        stopHop=x.stopHop;  
        return *this;  
    }  
}; 

bool CMP_1(const Trituple &a, const Trituple &b);
bool CMP_2(const Trituple &a, const Trituple &b);
bool CMP_3(const Trituple &a, const Trituple &b);

class SparseMatrix{    //ϡ�����  
private:  
        
public:  
	int Rows,Cols,Terms;   //����������������Ԫ�صĸ���  
    Trituple *smArray;   //�����Ԫ�ص���Ԫ���� 
	int maxTerms;
	SparseMatrix();//Ĭ�Ϲ��캯��
    SparseMatrix(int maxSz,int rows,int cols);  //���캯��  
    SparseMatrix(SparseMatrix& SM);      //��ֵ���캯��  
    ~SparseMatrix(); //��������  
    SparseMatrix& operator=(SparseMatrix& SM); //��ֵ���������  
    friend ostream& operator<<(ostream& ostr,SparseMatrix& SM); //�����������غ���  
    friend istream& operator>>(istream& istr,SparseMatrix& SM); //������������غ���  
	void SMToFile(string filePath);
	
}; 

#endif