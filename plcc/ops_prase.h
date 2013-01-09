#ifndef __OPS_PRASE_H__
#define __OPS_PRASE_H__

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



int  prase_plc_ld(std::string & line);
int  prase_plc_ldi(std::string & line);
int  prase_plc_and(std::string & line);
int  prase_plc_or(std::string & line);
int  prase_plc_ldp(std::string & line);
int  prase_plc_ldf(std::string & line);
int  prase_plc_andp(std::string & line);
int  prase_plc_andf(std::string & line);
int  prase_plc_orp(std::string & line);
int  prase_plc_orf(std::string & line);
int  prase_plc_set(std::string & line);
int  prase_plc_rst(std::string & line);
int  prase_plc_sei(std::string & line);
int  prase_plc_out(std::string & line);

#endif
