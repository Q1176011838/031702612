#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#define BOOL int
#define FALSE 1
#define TRUE 0
using namespace std;
 
 typedef struct node
{
    int col;
    int row;
    int value[10];
} Node;
 
int findvalue(int sudoku[9][9], Node * node);
BOOL general_inspection(int sudoku[9][9]);
int blank_num(int sudoku[9][9]);
Node * mem_alloc(int num_of_empty);
void trace(int sudoku[9][9], Node * node_stack, int num_of_empty);
void print_sudoku(int sudoku[9][9]);
 
int sudoku_num ; 
int sudoku_time ;
void doits(int sudoku[9][9]);
 
int main(int argc, char * argv[])
{
	int sudoku[9][9];
	sudoku_num = (int)*argv[2]-48;
	sudoku_time = (int)*argv[4]-48;
	
	if(sudoku_num!=9){
		cout<<"本程序仅支持九宫格。"<<endl;
		return 0;
	}
	
	
	
	ifstream myfile(argv[6]);
//	ofstream outfile(argv[8], ios::trunc);
 
	if (!myfile.is_open())
	{
		cout << "can not open this file" << endl;
		return 0;
	}
	
	
	
	for(int i=0;i<sudoku_time;i++){
		for(int a = 0;a<sudoku_num;a++){
			for(int b=0;b<sudoku_num;b++){
				myfile >> sudoku[a][b];
			}
		}
		//进行操作
		
		//..........................
		
		doits(sudoku);
		
		
		
		
		
		//...........................
		
		 //将数据输出至out.txt文件中
		//outfile << endl;
	}
 
	myfile.close();
	//outfile.close();
	return 0;
}
void doits(int sudoku[9][9]){
	    int num_of_empty;
    //为回溯栈分配空间
    Node * node_stack;
 
    if(general_inspection(sudoku))
    {
    	cout<<"此数独存在错误！请检查"<<endl;
        print_sudoku(sudoku);
    }
    num_of_empty = blank_num(sudoku);
    node_stack = mem_alloc(num_of_empty);
    trace(sudoku, node_stack, num_of_empty);
    print_sudoku(sudoku);
 
}
BOOL general_inspection(int sudoku[9][9])
{
    int temp[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int i, j, m, n;
    for(i=0; i<sudoku_num; i++)
        for(j=0; j<sudoku_num; j++)
            if(sudoku[i][j]!=0)
            {
                //检查所在行
                for(m=0; m<10; m++)
                    temp[m] = 0;
                for(m=0; m<sudoku_num; m++)
                    if(sudoku[i][m]!=0)
                    {
                        if(temp[sudoku[i][m]]==0)
                            temp[sudoku[i][m]] = 1;
                        else
                            return FALSE;
                    }
                //检查所在列
                for(m=0; m<10; m++)
                    temp[m] = 0;
                for(m=0; m<sudoku_num; m++)
                    if(sudoku[m][j]!=0)
                    {
                        if(temp[sudoku[m][j]]==0)
                            temp[sudoku[m][j]] = 1;
                        else
                            return FALSE;
                    }
                //检查所在九宫格
                for(m=0; m<10; m++)
                    temp[m] = 0;
                for(m=0; m<3; m++)
                    for(n=0; n<3; n++)
                        if(sudoku[i/3*3+m][j/3*3+n]!=0)
                        {
                            if(temp[sudoku[i/3*3+m][j/3*3+n]]==0)
                                temp[sudoku[i/3*3+m][j/3*3+n]] = 1;
                            else
                                return FALSE;
                        }
            }
    return TRUE;
}
 
int blank_num(int sudoku[9][9])
{
    //计算所给数独中待填入的空白数
    int i, j, num = 0;
    for(i=0; i<9; i++)
        for(j=0; j<9; j++)
            if(sudoku[i][j]==0)
                num++;
    return num;
}
 
Node * mem_alloc(int num_of_empty)
{
    Node * node_stack = (Node *)malloc(sizeof(struct node) * num_of_empty);
    return node_stack;
}
 
 
void trace(int sudoku[9][9], Node * node_stack, int num_of_empty)
{
    int i, j, index, k = 0;
    //回溯法求解数独
    while(num_of_empty)
    {
        for(i=0; i<sudoku_num; i++)
        {
            for(j=0; j<sudoku_num; j++)
            {
                if(sudoku[i][j]==0)
                {
                    (node_stack + k)->col = i;
                    (node_stack + k)->row = j;
                    sudoku[i][j] = findvalue(sudoku, node_stack+k);
                    if(sudoku[i][j]==-1)
                    {
                        sudoku[i][j] = 0;
                        k--;
                        while((node_stack + k)->value[0]==0)
                        {
                            //当栈空，说明数独错误，无解
                            if(k==0)
                            {
                            	cout<<"此数独无解！"<<endl;
                                //free(node_stack); //为啥这里一释放内存，就弹出debug assertion failed窗口啊！
                                exit(1);
                            }
                            sudoku[(node_stack + k)->col][(node_stack + k)->row] = 0;
                            num_of_empty++;
                            k--;
                        }
                        for(index=1; index<10; index++)
                            if((node_stack + k)->value[index]==0)
                            {
                                sudoku[(node_stack + k)->col][(node_stack + k)->row] = index;
                                (node_stack + k)->value[index] = 1;
                                (node_stack + k)->value[0]--;
                                break;
                            }
                        num_of_empty++;
                        i = (node_stack + k)->col;
                        j = (node_stack + k)->row;
                    }
                    k++;
                    num_of_empty--;
                }
            }
        }
    }
    //栈空间使用结束，释放
    free(node_stack);
    node_stack=NULL;
}
 
int findvalue(int sudoku[9][9], Node * node)
{
    int m, n, i = node->col, j = node->row;
    //初始化栈中存储候选值的数组
    for(m=0; m<10; m++)
        node->value[m] = 0;
    for(m=1; m<10; m++)
    {
        node->value[sudoku[i][m-1]] = 1;
        node->value[sudoku[m-1][j]] = 1;
    }
    for(m=0; m<3; m++)
        for(n=0; n<3; n++)
            node->value[sudoku[i/3*3+m][j/3*3+n]] = 1;
 
    //node->value[0]记录候选值个数，前面的循环可能会修改掉它，需要重新赋0值
    node->value[0] = 0;
    for(m=1; m<10; m++)
        if(node->value[m]==0)    node->value[0]++;
    for(m=1; m<10; m++)
        if(node->value[m]==0)
        {
            node->value[m] = 1;
            node->value[0]--;
            break;
        }
 
    //返回候选值m，若无候选值可用，返回错误标记-1
    if(m==10)
        return -1;
    else
        return m;
}
 
void print_sudoku(int sudoku[9][9])
{
	ofstream outfile("out.txt", ios::app);
	outfile<<endl;
	for(int i=0;i<sudoku_num;i++){
		for(int j=0;j<sudoku_num;j++){
			outfile << sudoku[i][j] << " ";
		}
		outfile<<endl;
	}
	outfile.close();
}
