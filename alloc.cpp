#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>

#define NUMRES 5
int fd;
char *data;
off_t len;
char yesNo= 'y';
int type, units;

//used for displaying mapped data
struct stat sb;

int main(void){

	//memory map
	fd = open("res.txt", O_RDWR);


    if (fstat (fd, &sb) == -1) {
            perror ("fstat");
   	        return 1;
    }
    //is mapped, checked
	data = static_cast<char*>(mmap (0, sb.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0));
    
    while (yesNo == 'y'){
    	//getting inputs
	    printf("Allocate more resources(y/n)?");
	    scanf(" %c", &yesNo);
	    getchar();
	    if (yesNo == 'y'){
	    	printf("Enter the resource number and number of units needed: \n");
	    	scanf("%d %d", &type, &units);
	    	getchar();
	    	for (int i=0; i<=sb.st_size; i+=4 ){
	    		//printf("%c \n",static_cast<char>(data[i+1]) );
				if (data[i]== type +'0'){
					//in specific resource type 
					//check if enough resources
					if (data[i+2] < units+ '0'){
						printf("Not enough resources, only %c units left in resource type %c . \n", data[i], data[i+2]);
					break;
					}
					//enough units

					else{
						//subtract from mapped file
						data[i+2] -= units;
					}
				}
				
			}

	    }

    }
    //get page size
    size_t page_size = (size_t) sysconf (_SC_PAGESIZE);

       /* use msync to write changes to disk. */
     if ( msync( data, page_size , MS_ASYNC ) < 0 ) {
          perror("msync failed with error:");
          return -1;
      }
      else {} ;


    close(fd);
	return 0;
}

