#include <iostream>

using namespace std;
int main()
{
	int n=5;
	int A[5]={3,2,5,4,1};
		
		int count=0;
		int v,j;
    
  	for(int i=1;i<=n-1;i++)
	{
		 v=A[i];
		 j=i-1;
		 cout<<"j£º"<<j<<endl; 
		
		while(j>0 && A[j]>v){
		
				count=count+1;
				
				cout<<"j£º"<<j<<endl; 
				
			
				A[j+1]=A[j];
			
				
				j=j+1;
		}
		A[j+1]=v;
		
	}
	     for(i=0;i<n;i++){
     	
     	cout<<A[i]<<endl;
     	
     }
	
	
	
	cout<<"±Èİ^´Î”µend£º"<<count<<endl; 
	
	return 0;
}