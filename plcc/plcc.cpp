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
#include "ops_prase.h"



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


const std::string  reg_plc_addr  = " +[XYMTC]\\d+[BDOH]* *(//.*)*";
const std::string  red_plc_op_pre = "^ +";
const std::string  reg_plc_out  = "^ +OUT +[XYM]\\d+[BDOH]* *(//.*)*";
const std::string  reg_plc_outt = "^ +OUT +T\\d+[BDOH]* +K\\d+[BDOH]* *(//.*)*";
const std::string  reg_plc_outc = "^ +OUT +C\\d+[BDOH]* +K\\d+[BDOH]* *(//.*)*";
const std::string  reg_plc_op_jmp  = "^ +JMP +\\d+[BDOH]* *(//.*)*";
const std::string  reg_plc_op_jmps  = "^ +JMPS +\\d+[BDOH]* *(//.*)*";
const std::string  reg_plc_nop  = "^ *NOP *(//.*)*";
const std::string  reg_plc_end  = "^ *END *(//.*)*";
//编译器相关
const std::string  reg_start    = "(^ *)";
const std::string  reg_device   ="(#device)";
const std::string  reg_define   ="(#define(\\(\\w+\\))*)";
const std::string  reg_operator = "((\\w+)( +\\w+)*( *)*)";
const std::string  reg_space    ="( +)";
const std::string  reg_comment1 = "/*([^\\*]|(\\*)*[^\\*/])*(\\*)*/";
const std::string  reg_comment2 = "(^ *//.*)";
const std::string  reg_comment  = "(" + reg_comment1 + "|" + reg_comment2 + ")*";
const std::string  reg_addr_ref = "^\\w+: *(//.*)*";

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

void  prase_plc_ops(void)
{
	size_t i;
	for(i=0;i<plcline.size();i++) {
		std::string line = plcline[i];
	    if(boost::regex_match(line,boost::regex(red_plc_op_pre + "LD" + reg_plc_addr))) {
		    prase_plc_ld(line);
		} else if(boost::regex_match(line,boost::regex(red_plc_op_pre + "LDI" + reg_plc_addr))) {
			prase_plc_ldi(line);
		} else if(boost::regex_match(line,boost::regex(red_plc_op_pre + "AND" + reg_plc_addr))) {
			prase_plc_and(line);
		} else if(boost::regex_match(line,boost::regex(red_plc_op_pre + "OR" + reg_plc_addr))) {
			prase_plc_or(line);
		} else if(boost::regex_match(line,boost::regex(red_plc_op_pre + "LDP" + reg_plc_addr))) {
			prase_plc_ldp(line);
		} else if(boost::regex_match(line,boost::regex(red_plc_op_pre + "LDF" + reg_plc_addr))) {
			prase_plc_ldf(line);
		} else if(boost::regex_match(line,boost::regex(red_plc_op_pre + "ANDP" + reg_plc_addr))) {
			prase_plc_andp(line);
		} else if(boost::regex_match(line,boost::regex(red_plc_op_pre + "ANDF" + reg_plc_addr))) {
			prase_plc_andf(line);
		} else if(boost::regex_match(line,boost::regex(red_plc_op_pre + "ORP" + reg_plc_addr))) {
			prase_plc_orp(line);
		} else if(boost::regex_match(line,boost::regex(red_plc_op_pre + "ORF" + reg_plc_addr))) {
			prase_plc_orf(line);
		} else if(boost::regex_match(line,boost::regex(red_plc_op_pre + "SET" + reg_plc_addr))) {
			prase_plc_set(line);
		} else if(boost::regex_match(line,boost::regex(red_plc_op_pre + "RST" + reg_plc_addr))) {
			prase_plc_rst(line);
		} else if(boost::regex_match(line,boost::regex(red_plc_op_pre + "SEI" + reg_plc_addr))) {
			prase_plc_sei(line);
	    } else if(boost::regex_match(line,boost::regex(reg_plc_out))) {
		    prase_plc_out(line);
	    } else if(boost::regex_match(line,boost::regex(reg_plc_outt))) {
			prase_plc_outt(line);
		} else if(boost::regex_match(line,boost::regex(reg_plc_outc))) {
			prase_plc_outc(line);
		} else if(boost::regex_match(line,boost::regex(reg_comment1))) {
			//注释，忽略s
	    } else {
		    //未知指令，报错
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

unsigned int               plc_op_index;

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


	plcline.push_back("   OUT   C123  K500D  //this is one\n");
	plcline.push_back("    OUT   T123  K200D    //this is one\n");
	

	plcline.push_back("   AND Y123  //this is one\n");
	plcline.push_back("   OR Y123  //this is one\n");
	plcline.push_back("   LDP Y123  //this is one\n");
	plcline.push_back("   LDF Y123  //this is one\n");
	plcline.push_back("   ANDP Y123  //this is one\n");
	plcline.push_back("   ANDF Y123  //this is one\n");
	plcline.push_back("   ORP Y123  //this is one\n");
	plcline.push_back("   ORF Y123  //this is one\n");
	plcline.push_back("   SET Y123  //this is one\n");
	plcline.push_back("   RST Y123  //this is one\n");
	plcline.push_back("   SEI Y123  //this is one\n");


	
	plcline.push_back(" LD T123     //this is one\n");
	plcline.push_back("  LD  C123  //this is one\n");
	plcline.push_back("  END  //this is one");


	plc_op_index = 0;
	prase_plc_ops();

	dempprintline();
	return 0;
}

