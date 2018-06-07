#ifndef SparseMatrix_H
#define SparseMatrix_H


#include<iostream>  
#include "DataProcessing.h"
using namespace std;  




struct Trituple{        //自定义数据结构：矩阵元素的行，列，值；  
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

class SparseMatrix{    //稀疏矩阵  
private:  
        
public:  
	int Rows,Cols,Terms;   //行数，列数，非零元素的个数  
    Trituple *smArray;   //存非零元素的三元数组 
	int maxTerms;
	SparseMatrix();//默认构造函数
    SparseMatrix(int maxSz,int rows,int cols);  //构造函数  
    SparseMatrix(SparseMatrix& SM);      //赋值构造函数  
    ~SparseMatrix(); //析构函数  
    SparseMatrix& operator=(SparseMatrix& SM); //赋值运算符重载  
    friend ostream& operator<<(ostream& ostr,SparseMatrix& SM); //矩阵的输出重载函数  
    friend istream& operator>>(istream& istr,SparseMatrix& SM); //矩阵的输入重载函数  
	void SMToFile(string filePath);
	
}; 

#endif