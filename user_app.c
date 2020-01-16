#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

#define BUF_LEN 80

int main()
{
    int file_desc;
    int ret_val;
    char entry;
    char buffer[BUF_LEN];

	while(1)
	{
		scanf("%c", &entry);
		if(entry == 'o' || entry == 'c' || entry == 's')
		{
			file_desc = open("garage", O_RDWR);
			if(file_desc < 0)
   			{
       				printf("Error, 'garage' not opened\n");
        			return -1;
    			}

			ret_val = write(file_desc, &entry, 1);
			close(file_desc);
		}
		if(entry == 'r')
		{
			file_desc = open("garage", O_RDWR);
			if(file_desc < 0)
		        {
        			printf("Error, 'garage' not opened\n");
        			return -1;
    		        }
			write(file_desc, &entry, 1);
                        read(file_desc, buffer, sizeof(buffer));
			printf("%s", buffer);
                        close(file_desc);
                }

		if(entry == 'q')
		{
			break;
		}
	}
    return 0;
}
