#include<cstdio>
#include<cstdlib>
#include<list>
#include<cstring>
#include<map>

using namespace std;

int main()
{
	int frame_size[4] = {64, 128, 256, 512};
	char input[1000];
	printf("Please enter the name of trace file : ");
	scanf("%s",input);
	
	printf("%6s%6s%6s%18s\n", "size", "miss", "hit", "page fault ratio");
	int i;
	for(i=0; i<4; i++)
	{
		FILE * trace = fopen(input, "r");
		list<unsigned long long> page;
		map<unsigned long long, list<unsigned long long>::iterator> table;
		int hit = 0, miss = 0;
		unsigned long long address; 
		char op[20];
		while(fscanf(trace, "%s", op) != EOF)
		{	
			char addr_size[20];
			fscanf(trace, "%xl", &address);
			fscanf(trace, "%s", addr_size);
			
			address >>= 12;					//4KB	
			if(table.count(address))		//hit
			{
				page.erase(table[address]);		//move to the last
				page.push_front(address);		//recently used
				table[address] = page.begin();	
				hit++;
			}
			else							//miss
			{
				if(page.size() == frame_size[i])
				{
					table.erase(page.back());	//delete table 
					page.pop_back();			//pop the last one
				}	
				page.push_front(address);		//recently used
				table[address] = page.begin();	
				miss++;
			}
		}	
		printf("%6d%6d%6d%18.10lf\n", frame_size[i], miss, hit, miss / (double) (hit + miss));		
		fclose(trace);
	}
	return 0;
}
