#include <stdlib.h> 
#include <stdio.h> 
#include <string.h>
#include <vector> 
#include <unordered_map> 

int main() 
{
	FILE *fp; 
	char buf[255];
	int i = 0; 
	std::unordered_map<unsigned long, int> accessed_pages; 
	int matches = 0; 

	fp = fopen("/home/jason/Desktop/mem_data.txt", "r"); 
	
	while(fgets(buf, 255, fp) != NULL){
		if(strcmp(buf, "DONE\n") == 0){ 
			printf("Workload %i: %i / %li\n", i, matches, accessed_pages.size());  
			i++;
			matches = 0;
			continue;  
		}
		
		unsigned long pfn = std::stoul(buf); 
		if( i == 0){
			accessed_pages[pfn] = i;
		}else{
			if(accessed_pages.find(pfn) != accessed_pages.end()
				&& accessed_pages[pfn] == i-1){
				accessed_pages[pfn] = i;
				matches++; 
			}
			
		}
	}
}
