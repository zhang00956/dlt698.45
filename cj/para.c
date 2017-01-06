/*
 * para.c
 *
 *  Created on: Jan 5, 2017
 *      Author: admin
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "AccessFun.h"
#include "StdDataType.h"
#include "Objectdef.h"
/************************************
 * 函数说明：获取文件内容大小
 *************************************/
long get_file_size(const char *filename){

	long size=0;
    FILE* fp = fopen( filename, "rb" );
    if(fp==NULL){
        fprintf(stderr,"ERROR: Open file %s failed.\n", filename);
        return 0;
    }
    fseek( fp, 0L, SEEK_END );
    size=ftell(fp);
    fclose(fp);
    return size;
}

char *getenum(int type,int val)
{
	char name[64]={};

	strcpy(name,"");
	switch(type) {
	case 1:
		if(val==bps300)	strcpy(name,"300");
		if(val==bps600)	strcpy(name,"600");
		if(val==bps1200)	strcpy(name,"1200");
		if(val==bps2400)	strcpy(name,"2400");
		if(val==bps4800)	strcpy(name,"4800");
		if(val==bps7200)	strcpy(name,"7200");
		if(val==bps9600)	strcpy(name,"9600");
		if(val==bps19200)	strcpy(name,"19200");
		if(val==bps38400)	strcpy(name,"38400");
		if(val==bps57600)	strcpy(name,"57600");
		if(val==bps115200)	strcpy(name,"115200");
		if(val==autoa)		strcpy(name,"自适应");
		break;
	case 2:
		if(val==0)	strcpy(name,"未知");
		if(val==1)	strcpy(name,"DL/T645-1997");
		if(val==2)	strcpy(name,"DL/T645-2007");
		if(val==3)	strcpy(name,"DL/T698.45");
		if(val==4)	strcpy(name,"CJ/T18802004");
		break;
	case 3:
		if(val==0)	strcpy(name,"未知");
		if(val==1)	strcpy(name,"单相");
		if(val==2)	strcpy(name,"三相三线");
		if(val==3)	strcpy(name,"三相四线");
		break;
	}
	return name;
}
/*
 * 采集档案配置表
 * */
#define PARAFILE_6000		"/nand/para/meter6000.par"
void prtCollect6000()
{
	CLASS_6001	 meter={};
	int			i=0,blknum=0,blksize=0,sizenew=0;

	blksize = sizeof(meter);
	blknum = get_file_size(PARAFILE_6000)/(blksize+2);

	if(blksize%4==0)	sizenew = blksize+2;
	else sizenew = blksize+(4-blksize%4)+2;

	if(get_file_size(PARAFILE_6000)%sizenew !=0 ){
		fprintf(stderr,"采集档案表不是整数，检查文件完整性！！！ %ld-%d\n",get_file_size(PARAFILE_6000),sizenew);
	}
	fprintf(stderr,"采集档案配置单元6001个数：%d\n",blknum);
	fprintf(stderr,"基本信息:通信地址  波特率  规约  端口OAD  通信密码  费率个数  用户类型  接线方式  额定电压  额定电流 \n");
	fprintf(stderr,"扩展信息:采集器地址 资产号（PT CT）\n");
	fprintf(stderr,"附属信息:对象属性OAD  属性值\n");
	for(i=0;i<blknum;i++) {
		if(block_file_sync(PARAFILE_6000,&meter,blksize,i)==1) {
			if(meter.sernum!=0 || meter.sernum!=0xffff) {
				fprintf(stderr,"\n序号:%d ",meter.sernum);
				fprintf(stderr,"%02x%02x%02x%02x%02x%02x",
						meter.basicinfo.addr.addr[0],meter.basicinfo.addr.addr[1],meter.basicinfo.addr.addr[2],meter.basicinfo.addr.addr[3],
						meter.basicinfo.addr.addr[4],meter.basicinfo.addr.addr[5]);
				fprintf(stderr," %s",getenum(1,meter.basicinfo.baud));
				fprintf(stderr," %s",getenum(2,meter.basicinfo.protocol));
				fprintf(stderr," %04X_%02X%02X",meter.basicinfo.port.OI,meter.basicinfo.port.attflg,meter.basicinfo.port.attrindex);
				fprintf(stderr," %02x%02x%02x%02x%02x%02x",meter.basicinfo.pwd[0],meter.basicinfo.pwd[1],meter.basicinfo.pwd[2],
												meter.basicinfo.pwd[3],meter.basicinfo.pwd[4],meter.basicinfo.pwd[5]);
				fprintf(stderr," %d %d",meter.basicinfo.ratenum,meter.basicinfo.usrtype);
				fprintf(stderr," %s",getenum(3,meter.basicinfo.connectype));
				fprintf(stderr," %d %d",meter.basicinfo.ratedU,meter.basicinfo.ratedI);
				fprintf(stderr,"\n       %02x%02x%02x%02x%02x%02x ",
						meter.extinfo.cjq_addr.addr[0],meter.extinfo.cjq_addr.addr[1],meter.extinfo.cjq_addr.addr[2],
						meter.extinfo.cjq_addr.addr[3],meter.extinfo.cjq_addr.addr[4],meter.extinfo.cjq_addr.addr[5]);
				fprintf(stderr,"%02x%02x%02x%02x%02x%02x ",
						meter.extinfo.asset_code[0],meter.extinfo.asset_code[1],meter.extinfo.asset_code[2],meter.extinfo.asset_code[3],
						meter.extinfo.asset_code[4],meter.extinfo.asset_code[5]);
				fprintf(stderr," %d %d",meter.extinfo.pt,meter.extinfo.ct);
				fprintf(stderr,"\n       %04X_%02X%02X",meter.aninfo.oad.OI,meter.aninfo.oad.attflg,meter.aninfo.oad.attrindex);
			}
		}
	}
}

void para_process(char *type)
{
	if(strcmp(type,"6000")==0) {
		prtCollect6000();
	}
}
