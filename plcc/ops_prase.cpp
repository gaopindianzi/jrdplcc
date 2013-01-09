
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

//根据正则表达式，把符合正则表达式的内容用空字符串替换掉，返回剩余的字符串
int ReplaceIwantStringByRegex(std::string src,std::string rgx,std::string & outstr)
{
	boost::regex e;
	e = rgx;
	outstr = boost::regex_replace(src,e,"");
	return (int)outstr.size();
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
		rgx = "LD +";
		ReplaceIwantStringByRegex(sresult,rgx,sresult);
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



int  prase_plc_ldi(std::string & line)
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
	std::string rgx = "LDI +[XYMTC]\\d+";
	if(SearchIwantStringByRegex(line,rgx,outlist) == 1) {
		unsigned long addr;
		sresult = outlist[0];
		rgx = "LDI +";
		ReplaceIwantStringByRegex(sresult,rgx,sresult);
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
				po->op = PLC_LDI;
				po->addr_hi = (unsigned char)(addr>>8);
				po->addr_lo = (unsigned char)(addr&0xFF);
				plc_ops.push_back(op);
				return 0;
			} else {
				printf("error of LDI addr too big!\n");
			}
		} else {
			printf("error of LDI [XYTC]\n");
		}
	}
	return -1;
}


int  prase_plc_and(std::string & line)
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
	std::string rgx = "AND +[XYMTC]\\d+";
	if(SearchIwantStringByRegex(line,rgx,outlist) == 1) {
		unsigned long addr;
		sresult = outlist[0];
		rgx = "AND +";
		ReplaceIwantStringByRegex(sresult,rgx,sresult);
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
				po->op = PLC_AND;
				po->addr_hi = (unsigned char)(addr>>8);
				po->addr_lo = (unsigned char)(addr&0xFF);
				plc_ops.push_back(op);
				return 0;
			} else {
				printf("error of AND addr too big!\n");
			}
		} else {
			printf("error of AND [XYTC]\n");
		}
	}
	return -1;
}

int  prase_plc_ani(std::string & line)
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
	std::string rgx = "ANI +[XYMTC]\\d+";
	if(SearchIwantStringByRegex(line,rgx,outlist) == 1) {
		unsigned long addr;
		sresult = outlist[0];
		rgx = "ANI +";
		ReplaceIwantStringByRegex(sresult,rgx,sresult);
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
				po->op = PLC_ANI;
				po->addr_hi = (unsigned char)(addr>>8);
				po->addr_lo = (unsigned char)(addr&0xFF);
				plc_ops.push_back(op);
				return 0;
			} else {
				printf("error of ANI addr too big!\n");
			}
		} else {
			printf("error of ANI [XYTC]\n");
		}
	}
	return -1;
}

int  prase_plc_or(std::string & line)
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
	std::string rgx = "OR +[XYMTC]\\d+";
	if(SearchIwantStringByRegex(line,rgx,outlist) == 1) {
		unsigned long addr;
		sresult = outlist[0];
		rgx = "OR +";
		ReplaceIwantStringByRegex(sresult,rgx,sresult);
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
				po->op = PLC_OR;
				po->addr_hi = (unsigned char)(addr>>8);
				po->addr_lo = (unsigned char)(addr&0xFF);
				plc_ops.push_back(op);
				return 0;
			} else {
				printf("error of OR addr too big!\n");
			}
		} else {
			printf("error of OR [XYTC]\n");
		}
	}
	return -1;
}


int  prase_plc_ori(std::string & line)
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
	std::string rgx = "ORI +[XYMTC]\\d+";
	if(SearchIwantStringByRegex(line,rgx,outlist) == 1) {
		unsigned long addr;
		sresult = outlist[0];
		rgx = "ORI +";
		ReplaceIwantStringByRegex(sresult,rgx,sresult);
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
				po->op = PLC_ORI;
				po->addr_hi = (unsigned char)(addr>>8);
				po->addr_lo = (unsigned char)(addr&0xFF);
				plc_ops.push_back(op);
				return 0;
			} else {
				printf("error of ORI addr too big!\n");
			}
		} else {
			printf("error of ORI [XYTC]\n");
		}
	}
	return -1;
}


int  prase_plc_ldp(std::string & line)
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
	std::string rgx = "LDP +[XYMTC]\\d+";
	if(SearchIwantStringByRegex(line,rgx,outlist) == 1) {
		unsigned long addr;
		sresult = outlist[0];
		rgx = "LDP +";
		ReplaceIwantStringByRegex(sresult,rgx,sresult);
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
				po->op = PLC_LDP;
				po->addr_hi = (unsigned char)(addr>>8);
				po->addr_lo = (unsigned char)(addr&0xFF);
				plc_ops.push_back(op);
				return 0;
			} else {
				printf("error of LDP addr too big!\n");
			}
		} else {
			printf("error of LDP [XYTC]\n");
		}
	}
	return -1;
}


int  prase_plc_ldf(std::string & line)
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
	std::string rgx = "LDF +[XYMTC]\\d+";
	if(SearchIwantStringByRegex(line,rgx,outlist) == 1) {
		unsigned long addr;
		sresult = outlist[0];
		rgx = "LDF +";
		ReplaceIwantStringByRegex(sresult,rgx,sresult);
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
				po->op = PLC_LDF;
				po->addr_hi = (unsigned char)(addr>>8);
				po->addr_lo = (unsigned char)(addr&0xFF);
				plc_ops.push_back(op);
				return 0;
			} else {
				printf("error of LDF addr too big!\n");
			}
		} else {
			printf("error of LDF [XYTC]\n");
		}
	}
	return -1;
}





int  prase_plc_andp(std::string & line)
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
	std::string rgx = "ANDP +[XYMTC]\\d+";
	if(SearchIwantStringByRegex(line,rgx,outlist) == 1) {
		unsigned long addr;
		sresult = outlist[0];
		rgx = "ANDP +";
		ReplaceIwantStringByRegex(sresult,rgx,sresult);
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
				po->op = PLC_ANDP;
				po->addr_hi = (unsigned char)(addr>>8);
				po->addr_lo = (unsigned char)(addr&0xFF);
				plc_ops.push_back(op);
				return 0;
			} else {
				printf("error of ANDP addr too big!\n");
			}
		} else {
			printf("error of ANDP [XYTC]\n");
		}
	}
	return -1;
}


int  prase_plc_andf(std::string & line)
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
	std::string rgx = "ANDF +[XYMTC]\\d+";
	if(SearchIwantStringByRegex(line,rgx,outlist) == 1) {
		unsigned long addr;
		sresult = outlist[0];
		rgx = "ANDF +";
		ReplaceIwantStringByRegex(sresult,rgx,sresult);
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
				po->op = PLC_ANDF;
				po->addr_hi = (unsigned char)(addr>>8);
				po->addr_lo = (unsigned char)(addr&0xFF);
				plc_ops.push_back(op);
				return 0;
			} else {
				printf("error of ANDF addr too big!\n");
			}
		} else {
			printf("error of ANDF [XYTC]\n");
		}
	}
	return -1;
}


int  prase_plc_orp(std::string & line)
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
	std::string rgx = "ORP +[XYMTC]\\d+";
	if(SearchIwantStringByRegex(line,rgx,outlist) == 1) {
		unsigned long addr;
		sresult = outlist[0];
		rgx = "ORP +";
		ReplaceIwantStringByRegex(sresult,rgx,sresult);
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
				po->op = PLC_ORP;
				po->addr_hi = (unsigned char)(addr>>8);
				po->addr_lo = (unsigned char)(addr&0xFF);
				plc_ops.push_back(op);
				return 0;
			} else {
				printf("error of ORP addr too big!\n");
			}
		} else {
			printf("error of ORP [XYTC]\n");
		}
	}
	return -1;
}



int  prase_plc_orf(std::string & line)
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
	std::string rgx = "ORF +[XYMTC]\\d+";
	if(SearchIwantStringByRegex(line,rgx,outlist) == 1) {
		unsigned long addr;
		sresult = outlist[0];
		rgx = "ORF +";
		ReplaceIwantStringByRegex(sresult,rgx,sresult);
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
				po->op = PLC_ORF;
				po->addr_hi = (unsigned char)(addr>>8);
				po->addr_lo = (unsigned char)(addr&0xFF);
				plc_ops.push_back(op);
				return 0;
			} else {
				printf("error of ORF addr too big!\n");
			}
		} else {
			printf("error of ORF [XYTC]\n");
		}
	}
	return -1;
}


int  prase_plc_set(std::string & line)
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
	std::string rgx = "SET +[XYMTC]\\d+";
	if(SearchIwantStringByRegex(line,rgx,outlist) == 1) {
		unsigned long addr;
		sresult = outlist[0];
		rgx = "SET +";
		ReplaceIwantStringByRegex(sresult,rgx,sresult);
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
				po->op = PLC_SET;
				po->addr_hi = (unsigned char)(addr>>8);
				po->addr_lo = (unsigned char)(addr&0xFF);
				plc_ops.push_back(op);
				return 0;
			} else {
				printf("error of SET addr too big!\n");
			}
		} else {
			printf("error of SET [XYTC]\n");
		}
	}
	return -1;
}



int  prase_plc_rst(std::string & line)
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
	std::string rgx = "RST +[XYMTC]\\d+";
	if(SearchIwantStringByRegex(line,rgx,outlist) == 1) {
		unsigned long addr;
		sresult = outlist[0];
		rgx = "RST +";
		ReplaceIwantStringByRegex(sresult,rgx,sresult);
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
				po->op = PLC_RST;
				po->addr_hi = (unsigned char)(addr>>8);
				po->addr_lo = (unsigned char)(addr&0xFF);
				plc_ops.push_back(op);
				return 0;
			} else {
				printf("error of RST addr too big!\n");
			}
		} else {
			printf("error of RST [XYTC]\n");
		}
	}
	return -1;
}



int  prase_plc_sei(std::string & line)
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
	std::string rgx = "SEI +[XYMTC]\\d+";
	if(SearchIwantStringByRegex(line,rgx,outlist) == 1) {
		unsigned long addr;
		sresult = outlist[0];
		rgx = "SEI +";
		ReplaceIwantStringByRegex(sresult,rgx,sresult);
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
				po->op = PLC_SEI;
				po->addr_hi = (unsigned char)(addr>>8);
				po->addr_lo = (unsigned char)(addr&0xFF);
				plc_ops.push_back(op);
				return 0;
			} else {
				printf("error of SEI addr too big!\n");
			}
		} else {
			printf("error of SEI [XYTC]\n");
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



int  prase_plc_ldkh(std::string & line)
{
	typedef struct _ld_ops_t
	{
		unsigned char op;
	} ld_ops_t;
	plc_op_data_t op;
	std::vector<std::string> outlist;
	std::string sresult;
	op.index = plc_op_index++;
	op.opstr = line;
	std::string rgx = "LDKH";
	if(SearchIwantStringByRegex(line,rgx,outlist) == 1) {
		op.opdat.resize(sizeof(ld_ops_t));
		ld_ops_t * po = (ld_ops_t *)&op.opdat[0];
		po->op = PLC_LDKH;
		plc_ops.push_back(op);
		return 0;
	}
	return -1;
}

int  prase_plc_ldkl(std::string & line)
{
	typedef struct _ld_ops_t
	{
		unsigned char op;
	} ld_ops_t;
	plc_op_data_t op;
	std::vector<std::string> outlist;
	std::string sresult;
	op.index = plc_op_index++;
	op.opstr = line;
	std::string rgx = "LDKL";
	if(SearchIwantStringByRegex(line,rgx,outlist) == 1) {
		op.opdat.resize(sizeof(ld_ops_t));
		ld_ops_t * po = (ld_ops_t *)&op.opdat[0];
		po->op = PLC_LDKL;
		plc_ops.push_back(op);
		return 0;
	}
	return -1;
}


