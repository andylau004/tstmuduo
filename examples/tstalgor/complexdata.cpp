





#include "complexdata.h"





void tst_1() {

}

// 复杂数据处理总入口
int tst_ComplexDataEntry_() {
    FILE* fd;
    long dev;
    long offset;
    long length;
    char ch;
    double ts=0.000000;
    if((fd=fopen("fuzadata.txt","r"))==NULL)
    {
        printf("open the file is error!\n");
        exit(0);
    }
//    lseek(fd,0,SEEK_SET);
//    fseek()

    while(5==fscanf(fd,"%ld,%ld,%ld,%c,%lf\n",&dev,&offset,&length,&ch,&ts))
    {
        printf("%ld,%ld,%ld,%c,%lf\n",dev,offset,length,ch,ts);
    }
    fclose(fd);

    return 1;
}
