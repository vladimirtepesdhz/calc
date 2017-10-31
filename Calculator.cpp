#include <string.h>
#define	_USE_MATH_DEFINES
#include <cmath>

#include "Calculator.h"

#define	numof(a)	(sizeof(a)/sizeof((a)[0]))

static	wchar_t FromHexC(wchar_t hex_c)
{
	if(hex_c>='0'&&hex_c<='9')
		return	hex_c-'0';
	else if(hex_c>='A'&&hex_c<='F')
		return	hex_c-'A'+10;
	else if(hex_c>='a'&&hex_c<='f')
		return	hex_c-'a'+10;
	else
		return	0;
}

static	wchar_t ToHexC(wchar_t v)
{
	if(v>=0&&v<=9)
		return	v+'0';
	else if(v>=10&&v<=15)
		return	v-10+'A';
	else
		return	'0';
}

static	const	char * ParseEmpty(const char * p_str)
{
	if(!p_str)
		return	p_str;
	while(isspace(*p_str))
		++p_str;
	return	p_str;
}

/*
OPER_ADD = (int)'+'
,OPER_SUB = (int)'-'
,OPER_MULT = (int)'*'
,OPER_DIV = (int)'/'
,OPER_POWER = (int)'^'
,OPER_LOG = (int)'~'
,OPER_PAR_L = (int)'('
,OPER_PAR_R = (int)')'
,OPER_NEG = 0x0100	//负号

,OPER_FUNC_SIN = 0x0101	//sin函数
,OPER_FUNC_COS = 0x0102	//cos函数
,OPER_FUNC_TAN = 0x0103	//tan函数
,OPER_FUNC_EXP = 0x0104	//exp函数
,OPER_FUNC_LN = 0x0105	//ln函数
,OPER_FUNC_LOG10 = 0x0106	//log10函数
,OPER_FUNC_ASIN = 0x0107	//asin函数
,OPER_FUNC_ACOS = 0x0108	//acos函数
,OPER_FUNC_ATAN = 0x0109	//atan函数
*/

const char * SCalculator::Parse(const char * p_expr,SToken * p_data)
{
	struct	SFuncItem
	{
		const char * p_name;
		int	func_id;
	};
	const int	oper_table[] = 
	{
		OPER_ADD	//+
		,OPER_SUB	//-
		,OPER_MULT	//*
		,OPER_DIV	// /
		,OPER_POWER	//^
		,OPER_LOG	//~
		,OPER_PAR_L	// (
		,OPER_PAR_R	// )
	};
	static	const	SFuncItem	func_table[]=
	{
		{"abs",		OPER_FUNC_ABS}
		,{"sqrt",	OPER_FUNC_SQRT}
		,{"sin",	OPER_FUNC_SIN}
		,{"cos",	OPER_FUNC_COS}
		,{"tan",	OPER_FUNC_TAN}
		,{"exp",	OPER_FUNC_EXP}
		,{"ln",		OPER_FUNC_LN}
		,{"log10",	OPER_FUNC_LOG10}
		,{"asin",	OPER_FUNC_ASIN}
		,{"acos",	OPER_FUNC_ACOS}
		,{"atan",	OPER_FUNC_ATAN}
	};

	if(!p_expr||!p_data)
		return	p_expr;
	const char * p_old = p_expr;
	p_data->type = PARSE_TYPE_NULL;
//	while(*p_expr)
	if(*p_expr)
	{
		p_expr = ParseEmpty(p_expr);
		if(isdigit(*p_expr))	//
		{
			double sum = 0;
			TComplex vc(0,0);

			p_data->type = PARSE_TYPE_NUM;
			p_data->value = TComplex(0,0);

			if(p_expr[0] == '0'&&
					(
						p_expr[1] == 'b'||
						p_expr[1] == 'B'||
						p_expr[1] == 'x'||
						p_expr[1] == 'X'
					)
				)
			{
				//
				if(p_expr[1] == 'b'||p_expr[1] == 'B')
				{
					//
					p_expr += 2;
					while(*p_expr == '0'||*p_expr == '1')
					{
						sum = (sum*2) + (*p_expr - '0');
						++p_expr;
					}
					if(*p_expr == '.')
					{
						++p_expr;
						double mask = 0.5;
						while(*p_expr == '0'||*p_expr == '1')
						{
							sum += ((*p_expr)-'0')*mask;
							mask/=2;
							++p_expr;
						}
					}
				}
				else if(p_expr[1] == 'x'||p_expr[1] == 'X')
				{
					//
					p_expr += 2;
					while(isxdigit(*p_expr))
					{
						sum = (sum*16) + FromHexC(*p_expr);
						++p_expr;
					}
					if(*p_expr == '.')
					{
						++p_expr;
						double mask = 1.0/16;
						while(isxdigit(*p_expr))
						{
							sum += FromHexC(*p_expr)*mask;
							mask/=16;
							++p_expr;
						}
					}
				}
			}
			else
			{
				//
				while(isdigit(*p_expr))
				{
					sum = (sum*10) + (*p_expr-'0');
					++p_expr;
				}
				if(*p_expr == '.')
				{
					++p_expr;
					double mask = 0.1;
					while(isdigit(*p_expr))
					{
						sum += (*p_expr-'0')*mask;
						mask/=10;
						++p_expr;
					}
				}
			}
			if(*p_expr=='E'||*p_expr=='e')	//
			{
				bool exp_neg = false;
				double exp_value = 0;
				++p_expr;
				if(*p_expr=='-')
				{
					exp_neg = true;
					++p_expr;
				}
				else if(*p_expr == '+')
				{
					exp_neg = false;
					++p_expr;
				}
				while(isdigit(*p_expr))
				{
					exp_value = exp_value*10 + (*p_expr-'0');
					++p_expr;
				}
				if(exp_neg == true)
					exp_value = -exp_value;
				sum *= pow(10,exp_value);
			}

			if(*p_expr == 'i')
			{
				++p_expr;
				vc.imag(sum);
			}
			else
				vc.real(sum);
			p_data->value = vc;
			return	p_expr;
		}
		else if(isalpha(*p_expr)||*p_expr == '_')
		{
			string	str;
			while(isalpha(*p_expr)||isdigit(*p_expr)||*p_expr == '_')
			{
				str += (*p_expr);
				++p_expr;
			}
			if(str == "i")	//
			{
				p_data->type = PARSE_TYPE_NUM;
				p_data->value.imag(1);
				return	p_expr;
			}
			else if(strcasecmp(str.c_str(),"e") == 0)	//
			{
				p_data->type = PARSE_TYPE_NUM;
				p_data->value.real(M_E);
				return	p_expr;
			}
			else if(strcasecmp(str.c_str(),"PI") == 0)	//
			{
				p_data->type = PARSE_TYPE_NUM;
				p_data->value.real(M_PI);
				return	p_expr;
			}
			else
			{
				for(int iter=0;iter<numof(func_table);++iter)
				{
					if(strcasecmp(func_table[iter].p_name,str.c_str())==0)
					{
						p_data->type = PARSE_TYPE_OPER;	//
						p_data->oper = func_table[iter].func_id;
						return	p_expr;
					}
				}
				p_data->type = PARSE_TYPE_VAR;	//
				p_data->var_name = str;
			}
			return	p_expr;
		}
		else
		{
			for(int iter=0;iter<numof(oper_table);++iter)
			{
				if(oper_table[iter] == *p_expr)
				{
					++p_expr;
					p_data->type = PARSE_TYPE_OPER;	//
					p_data->oper = oper_table[iter];
					return	p_expr;
				}
			}
			p_data->type = PARSE_TYPE_NULL;	//
			return	p_expr;
		}
	}
	return	p_expr;
}

/*
*/

//
static	bool	IsArithmOper(int oper)
{
	switch(oper)
	{
		case	SCalculator::OPER_ADD:
		case	SCalculator::OPER_SUB:
		case	SCalculator::OPER_MULT:
		case	SCalculator::OPER_DIV:
		case	SCalculator::OPER_POWER:
		case	SCalculator::OPER_LOG:
			return	true;
		break;
	}
	return	false;
}
//
static	bool	IsFuncOper(int oper)
{
	switch(oper)
	{
		case	SCalculator::OPER_NEG:
		case	SCalculator::OPER_FUNC_ABS:
		case	SCalculator::OPER_FUNC_SQRT:
		case	SCalculator::OPER_FUNC_SIN:
		case	SCalculator::OPER_FUNC_COS:
		case	SCalculator::OPER_FUNC_TAN:
		case	SCalculator::OPER_FUNC_EXP:
		case	SCalculator::OPER_FUNC_LN:
		case	SCalculator::OPER_FUNC_LOG10:
		case	SCalculator::OPER_FUNC_ASIN:
		case	SCalculator::OPER_FUNC_ACOS:
		case	SCalculator::OPER_FUNC_ATAN:
			return	true;
		break;
		default:
			return	false;
		break;
	}
}

static int	LevelIn(int oper)
{
	switch(oper)
	{
		case	SCalculator::OPER_ADD:
		case	SCalculator::OPER_SUB:
			return	1;
		break;

		case	SCalculator::OPER_MULT:
		case	SCalculator::OPER_DIV:
			return	2;
		break;

		case	SCalculator::OPER_POWER:
		case	SCalculator::OPER_LOG:
			return	3;
		break;
		case	SCalculator::OPER_PAR_L:
			return	0;
		break;
		case	SCalculator::OPER_PAR_R:
			return	0;
		break;
		case	SCalculator::OPER_NEG:
		case	SCalculator::OPER_FUNC_ABS:
		case	SCalculator::OPER_FUNC_SQRT:
		case	SCalculator::OPER_FUNC_SIN:
		case	SCalculator::OPER_FUNC_COS:
		case	SCalculator::OPER_FUNC_TAN:
		case	SCalculator::OPER_FUNC_EXP:
		case	SCalculator::OPER_FUNC_LN:
		case	SCalculator::OPER_FUNC_LOG10:
		case	SCalculator::OPER_FUNC_ASIN:
		case	SCalculator::OPER_FUNC_ACOS:
		case	SCalculator::OPER_FUNC_ATAN:
			return	4;
		break;
		default:
			return	-1;
		break;
	}
}

static int	LevelOut(int oper)
{
	switch(oper)
	{
		case	SCalculator::OPER_ADD:
		case	SCalculator::OPER_SUB:
			return	1;
		break;

		case	SCalculator::OPER_MULT:
		case	SCalculator::OPER_DIV:
			return	2;
		break;

		case	SCalculator::OPER_POWER:
		case	SCalculator::OPER_LOG:
			return	4;
		break;
		case	SCalculator::OPER_PAR_L:
			return	6;
		break;
		case	SCalculator::OPER_PAR_R:
			return	0;
		break;
		case	SCalculator::OPER_NEG:
		case	SCalculator::OPER_FUNC_ABS:
		case	SCalculator::OPER_FUNC_SQRT:
		case	SCalculator::OPER_FUNC_SIN:
		case	SCalculator::OPER_FUNC_COS:
		case	SCalculator::OPER_FUNC_TAN:
		case	SCalculator::OPER_FUNC_EXP:
		case	SCalculator::OPER_FUNC_LN:
		case	SCalculator::OPER_FUNC_LOG10:
		case	SCalculator::OPER_FUNC_ASIN:
		case	SCalculator::OPER_FUNC_ACOS:
		case	SCalculator::OPER_FUNC_ATAN:
			return	5;
		break;
		default:
			return	-1;
		break;
	}
}

static	bool	Calculate(TComplex* p_v1,TComplex* p_v2,int oper)
{
	try
	{
		switch(oper)
		{
			case	SCalculator::OPER_ADD:
			{
				*p_v1 += *p_v2;
			}
			break;
			case	SCalculator::OPER_SUB:
			{
				*p_v1 -= *p_v2;
			}
			break;

			case	SCalculator::OPER_MULT:
			{
				*p_v1 *= *p_v2;
			}
			break;
			case	SCalculator::OPER_DIV:
			{
				*p_v1 /= *p_v2;
			}
			break;

			case	SCalculator::OPER_POWER:
			{
				if(p_v1->imag() == 0.0&&p_v2->imag()==0.0)
				{
					p_v1->real(pow(p_v1->real(),p_v2->real()));
				}
				else
				{
					*p_v1 = pow(*p_v1,*p_v2);
				}
			}
			break;
			case	SCalculator::OPER_LOG:
			{
				if(p_v1->imag() == 0.0&&p_v2->imag()==0.0&&
					p_v1->real()>0&&p_v2->real()>0)
				{
					p_v1->real(log(p_v2->real())/log(p_v1->real()));
				}
				else
				{
					*p_v1 = log(*p_v2)/log(*p_v1);
				}
			}
			break;
			case	SCalculator::OPER_NEG:
			{
				*p_v1 = -*p_v1;
			}
			break;
			case	SCalculator::OPER_FUNC_ABS:
			{
				if(p_v1->imag() == 0.0)
				{
					p_v1->real(fabs(p_v1->real()));
				}
				else
				{
					*p_v1 = abs(*p_v1);
				}
			}
			break;
			case	SCalculator::OPER_FUNC_SQRT:
			{
				if(p_v1->imag() == 0.0&&p_v1->real()>=0)
				{
					p_v1->real(sqrt(p_v1->real()));
				}
				else
				{
					*p_v1 = sqrt(*p_v1);
				}
			}
			break;
			case	SCalculator::OPER_FUNC_SIN:
			{
				if(p_v1->imag() == 0.0)
				{
					p_v1->real(sin(p_v1->real()));
				}
				else
				{
					*p_v1 = sin(*p_v1);
				}
			}
			break;
			case	SCalculator::OPER_FUNC_COS:
			{
				if(p_v1->imag() == 0.0)
				{
					p_v1->real(cos(p_v1->real()));
				}
				else
				{
					*p_v1 = cos(*p_v1);
				}
			}
			break;
			case	SCalculator::OPER_FUNC_TAN:
			{
				if(p_v1->imag() == 0.0)
				{
					p_v1->real(tan(p_v1->real()));
				}
				else
				{
					*p_v1 = tan(*p_v1);
				}
			}
			break;
			case	SCalculator::OPER_FUNC_EXP:
			{
				if(p_v1->imag() == 0.0)
				{
					p_v1->real(exp(p_v1->real()));
				}
				else
				{
					*p_v1 = exp(*p_v1);
				}
			}
			break;
			case	SCalculator::OPER_FUNC_LN:
			{
				if(p_v1->imag() == 0.0&&p_v1->real()>0)
				{
					p_v1->real(log(p_v1->real()));
				}
				else
				{
					*p_v1 = log(*p_v1);
				}
			}
			break;
			case	SCalculator::OPER_FUNC_LOG10:
			{
				if(p_v1->imag() == 0.0&&p_v1->real()>0)
				{
					p_v1->real(log10(p_v1->real()));
				}
				else
				{
					*p_v1 = log10(*p_v1);
				}
			}
			break;
			/*
			*/
			case	SCalculator::OPER_FUNC_ASIN:
			{
				if(p_v1->imag() == 0.0)
				{
					p_v1->real(asin(p_v1->real()));
				}
				else
				{
					//Arcsin(z)=Ln[iz+sqrt(1-z^2)]/i
					*p_v1 = log(TComplex(0.0,1.0)*(*p_v1)+sqrt(1.0-(*p_v1)*(*p_v1)))/TComplex(0.0,1.0);
				}
			}
			break;
			case	SCalculator::OPER_FUNC_ACOS:
			{
				if(p_v1->imag() == 0.0)
				{
					p_v1->real(acos(p_v1->real()));
				}
				else
				{
					//Arccos(z)=Ln[z+i*sqrt(1-z^2)]/i
					*p_v1 = log(*p_v1 + TComplex(0.0,1.0)*sqrt(1.0-(*p_v1)*(*p_v1)))/TComplex(0.0,1.0);
				}
			}
			break;
			case	SCalculator::OPER_FUNC_ATAN:
			{
				if(p_v1->imag() == 0.0)
				{
					p_v1->real(atan(p_v1->real()));
				}
				else
				{
				//Arctg(z)=Ln[(1+iz)/(1-iz)]/(2i)
					TComplex iz = TComplex(0.0,1.0)*(*p_v1);
					*p_v1 = log((1.0+iz)/(1.0-iz))/TComplex(0.0,2.0);
				}
			}
			break;
			default:
				return	false;
			break;
		}
	}
	catch(...)
	{
		return	false;
	}
	return	true;
}


bool	SCalculator::CreateExpression(const char * p_str)
{
	enum
	{
		PARSE_STATE_NUM
		,PARSE_STATE_OPER
	};
	int	parse_state = PARSE_STATE_NUM;
	vector<SStackData>	num_array;
	vector<int>	oper_array;
	const char * p_parse = p_str;
	const char * p_old = p_parse;
	this->expr = p_str;
//	this->suffix.clear();
	p_parse = ParseEmpty(p_parse);
	if(*p_parse == '\0')
	{
	//	this->error = PARSE_ERR_PROG;
	//	this->err_pos = 0;
	//	return	false;
		this->suffix.resize(1);
		this->suffix[0].type = PARSE_TYPE_NUM;
		this->suffix[0].value = TComplex(0,0);
		this->error = PARSE_ERR_NULL;
		this->err_pos = 0;
		return	true;
	}
	oper_array.push_back('#');
	for(;;)
	{
		SToken	token;
		SStackData  stack_data;
		p_old = p_parse;
		p_parse = Parse(p_parse,&token);
		if(parse_state == PARSE_STATE_NUM)
		{
			if(token.type == PARSE_TYPE_NULL)
			{
				if(*p_parse == '\0')
				{
					this->error = PARSE_ERR_PROG;
					this->err_pos = p_parse - p_str;
					return	false;
				}
				else
				{
					this->error = PARSE_ERR_CHAR;
					this->err_pos = p_parse - p_str;
					return	false;
				}
			}
			else if(token.type == PARSE_TYPE_NUM)
			{
				stack_data.type = PARSE_TYPE_NUM;
				stack_data.value = token.value;
				num_array.push_back(stack_data);
				parse_state = PARSE_STATE_OPER;	//
			}
			else if(token.type == PARSE_TYPE_OPER)
			{
				if(token.oper == OPER_ADD||token.oper == OPER_SUB)
				{
					//
					bool	neg = false;
					for(;;)
					{
						if(token.type == PARSE_TYPE_OPER)
						{
							if(token.oper == OPER_ADD||token.oper == OPER_SUB)
							{	
								if(token.oper == OPER_SUB)
									neg = !neg;
							}
							else
							{	
								p_parse = p_old;
								break;
							}
						}
						else
						{
							p_parse = p_old;
							break;
						}
						p_old = p_parse;
						p_parse = Parse(p_parse,&token);
					}
					if(neg)
						oper_array.push_back(OPER_NEG);
					parse_state = PARSE_STATE_NUM;
				}
				else
				{
					if(IsFuncOper(token.oper)||token.oper == OPER_PAR_L)
					{
						//
						oper_array.push_back(token.oper);
						parse_state = PARSE_STATE_NUM;
					}
					else
					{
						//
						this->error = PARSE_ERR_PROG;
						this->err_pos = p_old - p_str;
						return	false;
					}
				}
			}
			else if(token.type == PARSE_TYPE_VAR)
			{
				//
				TVarHandle index= var_map.find(token.var_name);
				if(index == var_map.end())
				{
					var_map[token.var_name] = TComplex(0,0);
					index = var_map.find(token.var_name);
				}
				stack_data.type = PARSE_TYPE_VAR;
				stack_data.var_info = index;
				num_array.push_back(stack_data);
				parse_state = PARSE_STATE_OPER;	//
			}
		}
		else if(parse_state == PARSE_STATE_OPER)
		{
			if(token.type == PARSE_TYPE_NULL)
			{
				if(*p_parse == '\0')
				{
				//	for(int iter=0;iter<num_array.size();++iter)
				//	{
				//		suffix.push_back(num_array[iter]);
				//	}
					for(int iter=oper_array.size()-1;iter>=1;--iter)
					{
						if(oper_array[iter]!=OPER_PAR_L)
						{
							stack_data.type = PARSE_TYPE_OPER;
							stack_data.oper = oper_array[iter];
							num_array.push_back(stack_data);
						}
					}
					this->suffix = num_array;
					this->error = PARSE_ERR_NULL;
					this->err_pos = 0;
					break;
				}
				else
				{
					this->error = PARSE_ERR_CHAR;
					this->err_pos = p_parse - p_str;
					return	false;
				}
			}
			else if(token.type == PARSE_TYPE_NUM||token.type == PARSE_TYPE_VAR)
			{
				p_parse = p_old;
				token.type == PARSE_TYPE_OPER;
				token.oper = OPER_MULT;
			}
			else if(token.type == PARSE_TYPE_OPER)
			{
				if(token.oper == OPER_PAR_L||IsFuncOper(token.oper))
				{
					p_parse = p_old;
					token.type == PARSE_TYPE_OPER;
					token.oper = OPER_MULT;
				}
			}

			if(oper_array.empty()||num_array.empty())
			{
				this->error = PARSE_ERR_PROG;
				this->err_pos = p_old - p_str;
				return	false;
			}
			int level_out = LevelOut(token.oper);
			int level_in = LevelIn(oper_array.back());
			if(level_out<=level_in)
			{
				for(;;)
				{

					int cur_oper = oper_array.back();

					level_out = LevelOut(token.oper);
					level_in = LevelIn(cur_oper);

					if(level_out>level_in)
						break;

					if(cur_oper!=OPER_PAR_L&&cur_oper!=OPER_PAR_R)
					{
						stack_data.type = PARSE_TYPE_OPER;
						stack_data.oper = cur_oper;
						num_array.push_back(stack_data);
					}
					oper_array.pop_back();
					if(level_out == level_in)
						break;
				}
			}
			if(token.oper!=OPER_PAR_R)
			{
				oper_array.push_back(token.oper);
				parse_state = PARSE_STATE_NUM;
			}
		}
	}
	Optimize();
	return	true;
}

void	SCalculator::Optimize()
{
	vector<SStackData>	opt_array;
	for(int iter=0;iter<this->suffix.size();++iter)
	{
		SStackData * p_data = &this->suffix[iter];
		if(p_data->type == PARSE_TYPE_OPER)
		{
			bool	opt_flag = false;
			if(IsArithmOper(p_data->oper))
			{
				int i1,i2;
				if(opt_array.size()<2)
					return;
				i2 = opt_array.size()-1;
				i1 = i2 - 1;
				if(opt_array[i1].type == PARSE_TYPE_NUM&&
					opt_array[i2].type == PARSE_TYPE_NUM)
				{
					opt_flag = true;
					Calculate(&opt_array[i1].value,&opt_array[i2].value,p_data->oper);
					opt_array.pop_back();
				}
			}
			else if(IsFuncOper(p_data->oper))
			{
				if(opt_array.back().type == PARSE_TYPE_NUM)
				{
					TComplex	tmp;
					opt_flag = true;
					Calculate(&opt_array.back().value,&tmp,p_data->oper);
				}
			}
			if(!opt_flag)
				opt_array.push_back(*p_data);
		}
		else
		{
			opt_array.push_back(*p_data);
		}
	}
	this->suffix = opt_array;
}

TComplex	SCalculator::Compute()	const
{
	vector<TComplex> num_array;
	vector<int>	oper_array;
	for(int iter=0;iter<suffix.size();++iter)
	{
		const SStackData * p_data = &suffix[iter];
		if(p_data->type == PARSE_TYPE_NUM)
		{
			num_array.push_back(p_data->value);
		}
		else if(p_data->type == PARSE_TYPE_VAR)
		{
			num_array.push_back(p_data->var_info->second);
		}
		else if(p_data->type == PARSE_TYPE_OPER)
		{
			if(IsArithmOper(p_data->oper))
			{
				int i1,i2;
				if(num_array.size()<2)
					return	TComplex(0,0);
				i2 = num_array.size()-1;
				i1 = i2 - 1;

				Calculate(&num_array[i1],&num_array[i2],p_data->oper);
				num_array.pop_back();
			}
			else if(IsFuncOper(p_data->oper))
			{
				TComplex	tmp;
				Calculate(&num_array.back(),&tmp,p_data->oper);
			}
		}
	}
	if(num_array.empty())
		return	TComplex(0.0,0.0);
	return	num_array.back();
}

SCalculator::EParseError	SCalculator::GetError(int * p_errpos)	const
{
	if(p_errpos)
		*p_errpos = this->err_pos;
	return	error;
}

bool	SCalculator::operator = (const wchar_t * p_wstr)
{
	vector<char>	a_buf;
	if(!p_wstr)
		return	false;
	a_buf.resize(wcslen(p_wstr)+1);
	int iter = 0;
	while(*p_wstr)
	{
		a_buf[iter] = *p_wstr;
		++iter;
		++p_wstr;
	}
	a_buf[iter] = '\0';
	return	this->CreateExpression(&a_buf[0]);
}

TComplex *	SCalculator::FindVar(const char * var_name)
{
	SCalculator::TVarHandle h = this->var_map.find(string(var_name));
	if(h == this->var_map.end())
		return	NULL;
	else
		return	&(h->second);
}
bool	SCalculator::AddVar(const char * var_name,TComplex value)
{
	string	name = var_name;
	SCalculator::TVarHandle h = this->var_map.find(name);
	if(h == this->var_map.end())
	{
		this->var_map[name] = value;
		return	true;
	}
	else
		return	false;
}
bool	SCalculator::DelVar(const char * var_name)
{
	string	name = var_name;
	SCalculator::TVarHandle h = this->var_map.find(name);
	if(h == this->var_map.end())
	{
		return	false;
	}
	else
	{
		this->var_map.erase(h);
		return	true;
	}
}

TComplex *	SCalculator::FindVarW(const wchar_t * var_name)
{
	vector<char>	buf;
	int iter;
	if(!var_name)
		return	NULL;
	int len = wcslen(var_name);
	buf.resize(len+1);
	for(iter=0;iter<len;++iter)
		buf[iter] = var_name[iter];
	buf[iter] = 0;
	return	this->FindVar(&buf[0]);
}
bool	SCalculator::AddVarW(const wchar_t * var_name,TComplex value)
{
	vector<char>	buf;
	int iter;
	if(!var_name)
		return	NULL;
	int len = wcslen(var_name);
	buf.resize(len+1);
	for(iter=0;iter<len;++iter)
		buf[iter] = var_name[iter];
	buf[iter] = 0;
	return	this->AddVar(&buf[0],value);
}
bool	SCalculator::DelVarW(const wchar_t * var_name)
{
	vector<char>	buf;
	int iter;
	if(!var_name)
		return	NULL;
	int len = wcslen(var_name);
	buf.resize(len+1);
	for(iter=0;iter<len;++iter)
		buf[iter] = var_name[iter];
	buf[iter] = 0;
	return	this->DelVar(&buf[0]);
}
