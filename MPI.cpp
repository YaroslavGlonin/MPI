
#include "stdafx.h"
#include "stdlib.h"
#include "math.h"
#include "windows.h"
#include "mpi.h"

class Particle
{
public :

	float x,y,alpha;// x,y - ����������, alpha - ���� �����������
	int r,nomer;// r - ������, nomer - ����� ��������������� ��������

	void Set(float vX,float vY,int R, int A, int N)
	{x=vX; y=vY; r=R; alpha=A; nomer=N;}

	void Show()
	{printf("  N=%d  ",nomer);}

	void Move()
	{
		x=x+15*sin(alpha*180/3.14);y=y+15*cos(alpha*180/3.14);
		if (x <= 0 || x>=485){alpha = -alpha;}
		if (y <= 10 || y>=495){ alpha = 360 - alpha;}
	}

};
int _tmain(int argc, char *argv[])
{
	int dta0[10],dta1[10],dta2[10],dta3[10];//������� ����������� ���������� ���� �����
	int rank, size,stolknovenie;
	MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size( MPI_COMM_WORLD, &size );
	MPI_Status status;   
	double starttime, endtime;
    starttime = MPI_Wtime();
	int testmassiv[10][10];
	int i,j,n,T,nom;//i,j - ��� �����, n - ��� ��������� X,Y, T - ����� �������������, nom - ����� ��������
	float l,m,k;//l,m - ���������� ��� ��������� ���������, k - ��� ��������� ���� �����������
	Particle p[10];//������ ������
	FILE *f;
	f = fopen("file.txt","w");
	char * szString = new char[10];//������ ��� ������ � ����
		for (i=0;i<10;i++)
		{
			dta0[i]=0;dta1[i]=0;dta2[i]=0;dta3[i]=0;
			l=rand()%400;
			m=rand()%400;
			n=rand()%10;
			k=rand()%360;
			for(j=0;j<size;j++)
			{
				if (l>=(j*(400/size)) && l<((j+1)*(400/size)) ) nom=j;
			}
			p[i].Set(l,m,n,k,nom);//��������� ���������� ������
			p[i].Show();
			itoa(n,szString,10);//��������� �� int � char
			fputs(szString, f);
			fputs("\n",f);
			
		}
	for(i=0;i<size;i++)
	{
		if (rank==i)
		{
			for (T=0;T<10;T++)
			{
				for (j=0;j<10;j++)
				{	
					if(p[j].nomer==i)
					{

						p[j].Move();
					    for (stolknovenie = 0; stolknovenie < 10; stolknovenie++)
						{
						   if (sqrt((p[i].x - p[stolknovenie].x) * (p[i].x -p[stolknovenie].x) + (p[stolknovenie].y - p[i].y) * (p[stolknovenie].y - p[i].y)) < 10 && i != stolknovenie)
						     {
								    p[i].alpha = -p[i].alpha; 
								  p[stolknovenie].alpha = -p[stolknovenie].alpha; 
						    }
				    	  }
					}
				}

			}
		}
	}


	fclose(f);
	endtime   = MPI_Wtime();
    printf("\nThat took %.20f seconds, by process %d\n",endtime-starttime,rank);
	MPI_Finalize();
	return 0;
}

