
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



std::vector<plc_op_data_t> plc_ops;
std::vector<plc_op_data_t> plc_ops_unprase;
extern unsigned int        plc_op_index;

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

int  prase_plc_ld(std::string & line)
{
	typedef struct _ld_ops_t
	{
		unsigned char op;
		unsigned char addr_hi;
		unsigned char addr_lo;
	} ld_ops_t;
	plc_op_data_t op;
	std::vector<std::string> outlist;
	std::string sresult;
	op.index = plc_op_index++;
	op.opstr = line;
	std::string rgx = "LD +[XYMTC]\\d+";
	if(SearchIwantStringByRegex(line,rgx,outlist) == 1) {
		unsigned long addr;
		sresult = outlist[0];
	    rgx = "\\d+";
		if(SearchIwantStringByRegex(sresult,rgx,outlist) == 1) {
			addr = atol(outlist[0].c_str());
		} else {
			printf("error of addr!\n");
			return -1;
		}
		rgx = "[XYMTC]";
		if(SearchIwantStringByRegex(sresult,rgx,outlist) == 1) {
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
				op.opdat.resize(sizeof(ld_ops_t));
				ld_ops_t * po = (ld_ops_t *)&op.opdat[0];
				po->op = PLC_LD;
				po->addr_hi = (unsigned char)(addr>>8);
				po->addr_lo = (unsigned char)(addr&0xFF);
				plc_ops.push_back(op);
				return 0;
			} else {
				printf("error of LD addr too big!\n");
			}
		} else {
			printf("error of LD [XYTC]\n");
		}
	}
	return -1;
}



int  prase_plc_out(std::string & line)
{
	typedef struct _ld_ops_t
	{
		unsigned char op;
		unsigned char addr_hi;
		unsigned char addr_lo;
	} ld_ops_t;
	plc_op_data_t op;
	std::vector<std::string> outlist;
	std::string sresult;
	op.index = plc_op_index++;
	op.opstr = line;
	std::string rgx = "OUT +[XYMTC]\\d+";
	if(SearchIwantStringByRegex(line,rgx,outlist) == 1) {
		unsigned long addr;
		sresult = outlist[0];
	    rgx = "\\d+";
		if(SearchIwantStringByRegex(sresult,rgx,outlist) == 1) {
			addr = atol(outlist[0].c_str());
		} else {
			printf("error of addr!\n");
			return -1;
		}
		rgx = "[XYMTC]";
		if(SearchIwantStringByRegex(sresult,rgx,outlist) == 1) {
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
				op.opdat.resize(sizeof(ld_ops_t));
				ld_ops_t * po = (ld_ops_t *)&op.opdat[0];
				po->op = PLC_OUT;
				po->addr_hi = (unsigned char)(addr>>8);
				po->addr_lo = (unsigned char)(addr&0xFF);
				plc_ops.push_back(op);
				return 0;
			} else {
				printf("error of LD addr too big!\n");
			}
		} else {
			printf("error of LD [XYTC]\n");
		}
	}
	return -1;
}

