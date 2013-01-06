// plcc.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <boost/shared_ptr.hpp>
#include <boost/regex.hpp>
#include <iostream>  
#include <sstream>  
#include <fstream>  
#include <string>
#include <vector>
#include <string>
#include "plc_command_def.h"
#include "plc_prase.h"

typedef struct _plc_oplenmap_t
{
	unsigned char opnum;
	unsigned char len;
} plc_op_lenmap_t;

typedef struct _plc_op_data_t
{
	unsigned int  index;
	std::string   opstr;
	std::vector<unsigned char> opdat;
} plc_op_data_t;

const plc_op_lenmap_t plc_len_map[] = 
{
	{PLC_NONE, 1 },
	{PLC_LD  , 3 },
	{PLC_LDKH, 1 },
	{PLC_LDI, 3 },
	{PLC_OUT, 3 },
	{PLC_AND, 3 },
	{PLC_ANI, 3 },
	{PLC_OR, 3 },
	{PLC_ORI, 3 },
	{PLC_LDP, 3 },
	{PLC_LDF, 3 },
	{PLC_ANDP, 3 },
	{PLC_ANDF, 3 },
	{PLC_ORP, 3 },
	{PLC_ORF, 3 },
	{PLC_MPS, 3 },
	{PLC_MRD, 3 },
	{PLC_MPP, 3 },
	{PLC_SET, 3 },
	{PLC_RST, 3 },
	{PLC_INV, 1 },
	{PLC_OUTT, 5 },
	{PLC_OUTC, 5 },
	{PLC_ZRST, 5 },
	{PLC_BCMP, 6 },
	{PLC_BCMPE, 6 },
	{PLC_BCMPL, 6 },
	{PLC_BCMPB, 6 },
	{PLC_BACMPL, 255 }, ///可变长度
	{PLC_BACMPE, 255 },
	{PLC_BACMPB, 255 },
	{PLC_BZCP, 7 },
	{PLC_BZCPS, 7 },
	{PLC_BAZCP, 255 },
	{PLC_MOV, 0 },
	{PLC_CML, 0 },
	{PLC_BMOV,0},
	{PLC_FMOV,0},
	{PLC_NETRB,0},
	{PLC_NETWB,0},
	{PLC_NETRW,0},
	{PLC_NETWW,0},
	{PLC_LDKL,1},
	{PLC_SEI,3},
	{PLC_JMP,3},
	{PLC_JMPS,3},
	{PLC_JMPSU,3},
	{PLC_END,1},
	{0,0}
};

std::vector<plc_op_data_t> plc_ops;
std::vector<plc_op_data_t> plc_ops_unprase;
unsigned int               plc_op_index;

const std::string  reg_plc_ld   = "^ +LD +[XYMTC]\\d+ *(//.*)*";
const std::string  reg_plc_out  = "^ +OUT +[YMTC]\\d+ *(//.*)*";
const std::string  reg_plc_nop  = "^ *NOP *(//.*)*";
const std::string  reg_plc_end  = "^ *END *(//.*)*";

const std::string  reg_start    = "(^ *)";
const std::string  reg_device   ="(#device)";
const std::string  reg_define   ="(#define(\\(\\w+\\))*)";
const std::string  reg_operator = "((\\w+)( +\\w+)*( *)*)";
const std::string  reg_space    ="( +)";
const std::string  reg_comment1 = "/*([^\\*]|(\\*)*[^\\*/])*(\\*)*/";
const std::string  reg_comment2 = "(^ *//.*)";
const std::string  reg_comment  = "(" + reg_comment1 + "|" + reg_comment2 + ")*";
const std::string  reg_addr_ref = "^\\w+:";

std::vector<std::string>  plcline;
std::vector<std::string>  devicestring;
std::vector<std::string>  definestring;
std::vector<std::string>  plcopstr;
std::vector<std::string>  addr_str;
std::vector<std::string>  comment_str;


unsigned int read_in_plc(std::string filename)
{
	unsigned int linecount = 0;
	std::ifstream  fin(filename.c_str(), std::ios::in);  
	while(!fin.eof())
	{
		std::string linstr;
		getline(fin,linstr);
		linecount++;
		plcline.push_back(linstr);
    }
	return linecount;
}


//根据正则表达式，返回我符合正则表达式的字符串，可能有多项符合
int SearchIwantStringByRegex(std::string src,std::string rgx,std::vector<std::string> & outlist)
{
	outlist.clear();
	//
	boost::match_flag_type flags = boost::match_default; 
	boost::match_results<std::string::const_iterator> what;
	boost::regex e;
	std::string::const_iterator start,end; 
	//
	e = rgx;
	start = src.begin();end = src.end();
	while(boost::regex_search(start,end,what,e,flags)) {
		std::string tmp = what[0];
		start = what[0].second;
		outlist.push_back(tmp);
	}
	return (int)outlist.size();
}


void  prase_plc_ops(std::string & line)
{
	plc_op_data_t op;
	std::vector<std::string> outlist;
	if(boost::regex_match(line,boost::regex(reg_plc_ld))) {
		op.index = plc_op_index++;
		op.opstr = line;
		std::string rgx = "LD +[XYMTC]\\d+";
		if(SearchIwantStringByRegex(line,rgx,outlist) == 1) {
			unsigned long addr;
			rgx = "\\d+";
			if(SearchIwantStringByRegex(line,rgx,outlist) == 1) {
				addr = atol(outlist[0].c_str());
			} else {
				printf("error of addr!\n");
				return ;
			}
			rgx = "[XYMTC]";
			if(SearchIwantStringByRegex(line,rgx,outlist) == 1) {
				std::string addrtype = outlist[0];
				if(addrtype == "X") {
				} else if(addrtype == "Y") {
					addr += IO_OUTPUT_BASE;
				} else if(addrtype == "M") {
					addr += AUXI_RELAY_BASE;
				} else if(addrtype == "T") {
					addr += TIMING100MS_EVENT_BASE;
				} else if(addrtype == "C") {
					addr += COUNTER_EVENT_BASE;
				}
				if(addr <= 0xFFFF) {
				    op.opdat.clear();
				    op.opdat.push_back(PLC_LD);
				    op.opdat.push_back((unsigned char)(addr>>8));
				    op.opdat.push_back((unsigned char)(addr&0xFF));
					plc_ops.push_back(op);
					op.opstr.clear();
				} else {
					printf("error of LD addr too big!\n");
				}
			} else {
				printf("error of LD [XYTC]\n");
			}

		}
	} else if(boost::regex_match(line,boost::regex(reg_plc_out))) {
		//o.op = PLC_OUT;
		//o.opstr = line;
	} else if(boost::regex_match(line,boost::regex(reg_plc_nop))) {
		//o.op = PLC_NONE;
		//o.opstr = line;
	} else {
		//未知指令，报错
	}
	//plc_ops.push_back(o);
}

void prase_plc_ops(void)
{
	unsigned int i;
	for(i=0;i<plcline.size();i++) {
		std::string line = plcline[i];
		if(boost::regex_match(line,boost::regex(reg_device))) {
			devicestring.push_back(line);
		} else {
			prase_plc_ops(line);
		}
	}
}


void dempprintline(void)
{
	size_t i;
	for(i=0;i<devicestring.size();i++) {
		printf("%s\n",devicestring[i].c_str());
	}
	for(i=0;i<definestring.size();i++) {
		printf("%s\n",definestring[i].c_str());
	}
	for(i=0;i<comment_str.size();i++) {
		printf("%s\n",comment_str[i].c_str());
	}
	
}

int main(int argc, const char * argv[])
{
	//if(argc < 2) {
	//	printf("请输入需要编译的参数，如下：\n"
	//		   "  plcc filename.plc\n");
	//	return 0;
	//}
	//const char * pfilename = argv[1];
	//printf("plcc %s\n",pfilename);
	//printf("readline(%d)\n",read_in_plc(pfilename));

	plcline.push_back(" LD X123  //this is one\n");
	plcline.push_back("   LD Y123  //this is one\n");
	plcline.push_back(" LD T123     //this is one\n");
	plcline.push_back("  LD  C123  //this is one\n");
	plcline.push_back("  END  //this is one");


	plc_op_index = 0;
	prase_plc_ops();

	//getstring();
	dempprintline();
	return 0;
}

