#include <iostream>
#include <vector>
#include <cstring>
#include <time.h>
#include <stdlib.h>
using namespace std; 
int point,edge;//图的点数，边数 
vector<vector<int> > map;
int **tabu_table,**adjacement_color_table;
int *solution,*best_solution;
int f,best_f;
int k;//color;
int vi,vj,u;
int iter;//迭代次数 
int best_delt;
void Initialization()
{
	solution=new int[point];
	best_solution=new int[point];
		srand((int)time(0));
	for(int i=0;i<point;i++)
	{
		solution[i]=rand()%k;
		//cout<<solution[i]<<endl;
		best_solution[i]=solution[i];
	} 
	tabu_table=new int*[point];
	adjacement_color_table=new int*[point];
	for(int i=0;i<point;i++)
	{
		tabu_table[i]=new int[k];
		adjacement_color_table[i]=new int[k];
	}
	for(int i=0;i<point;i++)
	for(int j=0;j<k;j++)
	{
		tabu_table[i][j]=0;
		adjacement_color_table[i][j]=0;
	}
	for(int i=0;i<point;i++)//初始化adjacement_color_table
	for(int j=0;j<k;j++)
	for(int m=0;m<map[i].size();m++)
	if(solution[map[i][m]]==j) adjacement_color_table[i][j]++;
	//计算f,best_f
	f=0;
	for(int i=0;i<point;i++)
	f=f+adjacement_color_table[i][solution[i]];
	f=f/2;
//	cout<<f;
	best_f=f;
} 

void print()
{
	cout<<"point"<<point<<endl;
	cout<<"solution "<<endl;
	for(int i=0;i<point;i++)
	cout<<"solution "<<i<<" "<<solution[i]<<endl;
	cout<<"adjace_color_table"<<endl;
	for(int i=0;i<point;i++)
	{
		cout<<"apoint "<<i<<" ";
		for(int j=0;j<k;j++)
		{
			cout<<adjacement_color_table[i][j]<<" ";
		}
		cout<<endl;
	}
	cout<<"tabu_table"<<endl;
	for(int i=0;i<point;i++)
	{
		cout<<"tpoint "<<i<<" ";
		for(int j=0;j<k;j++)
		{
			cout<<tabu_table[i][j]<<" ";
		}
		cout<<endl;
	}
	cout<<"f: "<<f<<" best_f: "<<best_f<<endl;
} 





void findmove(int* vi,int* vj,int *u,int *best_delt)
{
	int non_tabu_count,tabu_count;
	int delt,nontabu_best_delt,tabu_best_delt;
	int non_tabu_vi,non_tabu_vj,non_tabu_u;
	int tabu_vi,tabu_vj,tabu_u;
	int nontabu_randomcount,tabu_randomcount;
	non_tabu_count=0; 
	tabu_count=0;
	nontabu_randomcount=0;
	tabu_randomcount=0;
	nontabu_best_delt=25536;tabu_best_delt=25536;
	for(int i=0;i<point;i++)
	{
		if(adjacement_color_table[i][solution[i]]>0)
		{
			for(int j=0;j<k;j++)
			{
				if(j!=solution[i])
				{
					delt=adjacement_color_table[i][j]-adjacement_color_table[i][solution[i]];
					//为了调试程序写的cout 
				//	cout<<i<<":"<<"从" <<solution[i]<<"到"<<j<<":delt=" <<delt;
					if (tabu_table[i][j]<=iter)//non_tabu
					{
						//bebug
						//cout<<"non_tabu"<<" ";//debug
						non_tabu_count++;
						if(non_tabu_count==1) nontabu_best_delt=delt;
						if(delt<nontabu_best_delt)
						{
							non_tabu_vi=solution[i];
							non_tabu_vj=j;
							non_tabu_u=i;
							nontabu_best_delt=delt;
						}
						else if(delt==nontabu_best_delt)
						{
							nontabu_randomcount++;
						//	cout<<"!!!!!!!!!!!!!!!";
							if(rand()%nontabu_randomcount==0)//随机更新 
							{
								non_tabu_vi=solution[i];
								non_tabu_vj=j;
								non_tabu_u=i;
							}
						}	 
					}
					else//tabu
					{
						//bebug
					//	cout<<"tabu"<<" ";//debug
						tabu_count++;
						if(tabu_count==1) tabu_best_delt=delt;
						if(delt<tabu_best_delt)
						{
							tabu_vi=solution[i];
							tabu_vj=j;
							tabu_u=i;
							tabu_best_delt=delt;
						}
						else if(delt==tabu_best_delt)
						{
						//	cout<<"!!!!!!!!!!!!!!!";
							tabu_randomcount++;
							if(rand()%tabu_randomcount==0)//随机更新 
							{
								tabu_vi=solution[i];
								tabu_vj=j;
								tabu_u=i;
							}
						}	 
					}
				//debug cout
//				cout<<"tabu_best_delt "<<tabu_best_delt<<"  nontabu_best_delt "<<nontabu_best_delt<<endl;
				}
			}
		}	
	}
	if(f+tabu_best_delt<best_f&&tabu_best_delt<nontabu_best_delt)//满足破忌准则
	{
		*vi=tabu_vi;
		*vj=tabu_vj;
		*u=tabu_u;
		*best_delt=tabu_best_delt;
//		cout<<"选择 tabo";
//		cout<<" u "<<*u<<" vi "<<*vi<<" vj "<<*vj<<" best_delt "<<*best_delt<<endl;
	}
	else
	{
	
		*vi=non_tabu_vi;
		*vj=non_tabu_vj;
		*u=non_tabu_u;
		*best_delt=nontabu_best_delt;
	//	cout<<"选择 nontabo";
	//	cout<<" u "<<*u<<" vi "<<*vi<<" vj "<<*vj<<" best_delt "<<*best_delt<<endl;
	}	 
}
void makemove()
{
	iter++;
	solution[u]=vj;
	f=f+best_delt;
	if(f<best_f)
	{
		best_f=f;
		for(int i=0;i<point;i++)
		{
			best_solution[i]=solution[i];
		}
	}
	tabu_table[u][vi]=iter+f+rand()%10;
	//更新adjacement_color_table
	for(int i=0;i<map[u].size();i++)
	{
		adjacement_color_table[map[u][i]][vj]++;
		if(adjacement_color_table[map[u][i]][vi]>=1) adjacement_color_table[map[u][i]][vi]--;
	} 
}
void check()
{
	int mark=0;
	for(int i=0;i<point;i++)
	{
		if(adjacement_color_table[i][solution[i]]!=0)
		{
			mark=1;
			break;
		}
	}
	if(mark==1) cout<<"no"<<endl;
	else cout<<"yes"<<endl;
} 
int main()
{
	freopen("DSJC500.5.col","r",stdin);
	//freopen("o.txt","w",stdout);
	char str[1001];
	while(cin.getline(str,1001))//对文件数据处理初始化map
	{
		
		if(str[0]=='p')
		{
			int i=7;
			int num=0;
			while(str[i]!=' ')
			{
				num=num*10+str[i]-'0';
				i++;
			}
			point=num;
			i++; 
			num=0;
			while(str[i]!='\0')
			{
				num=num*10+str[i]-'0';
				i++;
			}
			edge=num;
			map.resize(point);
		}
		else if(str[0]=='e')
		{
			//start是边的起点，end是边的终点  
			int i=2;
			int start=0;
			while(str[i]!=' ')
			{
				start=start*10+str[i]-'0';
				i++;
			}
			start--;//目的是让点从0开始 
		    int end=0;
			i++; 
			while(str[i]!='\0')
			{
				end=end*10+str[i]-'0';
				i++;
			}
			end--;
			map[start].push_back(end);
			map[end].push_back(start);
		}
	}
	iter=0;
	k=49;
	Initialization();
//	print();
	while(f>0)
	{
		findmove(&vi,&vj,&u,&best_delt);
		//cout<<u<<" "<<vi<<" "<<vj<<" "<<best_delt<<endl;
		makemove(); 
	//	print();
	}
	cout<<iter<<endl;
	check();
	return 0;
} 




