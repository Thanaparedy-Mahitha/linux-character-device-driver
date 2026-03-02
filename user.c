#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>

int main(){

int fd;
char msg[30];
printf("enter the stg..\n");
scanf("%[^\n]",msg);
printf("the data =%s\n",msg);

fd=open("/proc/mahi_proc",O_WRONLY);

if(fd<0){
perror("open");
return 1;
}

write(fd,msg,strlen(msg));

return 0;


}
