#ifndef	_CALCULATOR_H_
#define	_CALCULATOR_H_

#include <vector>
#include <string>
#include <complex>
#include <map>
using	std::vector;
using	std::string;
using	std::wstring;
using	std::map;
typedef	std::complex<double>	TComplex;

struct	SCalculator
{
	typedef	map<string,TComplex>::iterator	TVarHandle;
	enum	EParseType
	{
		PARSE_TYPE_NULL
		,PARSE_TYPE_NUM		//数字
		,PARSE_TYPE_OPER	//操作符
		,PARSE_TYPE_VAR		//变量
	};
	enum	EOperator
	{
		OPER_ADD = (int)'+'
		,OPER_SUB = (int)'-'
		,OPER_MULT = (int)'*'
		,OPER_DIV = (int)'/'
		,OPER_POWER = (int)'^'
		,OPER_LOG = (int)'~'
		,OPER_PAR_L = (int)'('
		,OPER_PAR_R = (int)')'
		,OPER_NEG = 0x0100	//负号

		,OPER_FUNC_ABS	//abs
		,OPER_FUNC_SQRT	//sqrt
		,OPER_FUNC_SIN	//sin
		,OPER_FUNC_COS	//cos
		,OPER_FUNC_TAN	//tan
		,OPER_FUNC_EXP	//exp
		,OPER_FUNC_LN	//ln
		,OPER_FUNC_LOG10	//log10
		,OPER_FUNC_ASIN	//asin
		,OPER_FUNC_ACOS	//acos
		,OPER_FUNC_ATAN	//atan
	};
	enum	EParseError
	{
		PARSE_ERR_NULL = 0	//
		,PARSE_ERR_CHAR = 1	//
		,PARSE_ERR_PROG = 2	//
	};

	struct	SToken
	{
		SToken()
		{
			type = PARSE_TYPE_NULL;
			oper = 0;
		}
		int	type;
		int	oper;
		string	var_name;
		TComplex value;
	};

	struct	SStackData
	{
		SStackData()
		{
			type = PARSE_TYPE_NULL;
			oper = 0;
		}
		int type;
		int oper;
		TVarHandle var_info;
		TComplex value;
	};
	SCalculator()
	{
		error = PARSE_ERR_NULL;
		err_pos = 0;
	}
	bool	CreateExpression(const char * p_str);
	void	Optimize();
	TComplex	Compute()	const;
	EParseError	GetError(int * p_errpos)	const;

	TComplex *	FindVar(const char * var_name);
	bool	AddVar(const char * var_name,TComplex value);
	bool	DelVar(const char * var_name);

	TComplex *	FindVarW(const wchar_t * var_name);
	bool	AddVarW(const wchar_t * var_name,TComplex value);
	bool	DelVarW(const wchar_t * var_name);

	int	GetVarCount()	const
	{
		return	this->var_map.size();
	}
	TVarHandle	GetVarFirst()
	{
		return	this->var_map.begin();
	}
	TVarHandle	GetVarEnd()
	{
		return	this->var_map.end();
	}
	TVarHandle	GetVarNext(TVarHandle h)
	{
		++h;
		return	h;
	}
	void	ClearVar()
	{
		this->var_map.clear();
	}
public:
	bool	operator = (const char * p_str)
	{
		return	CreateExpression(p_str);
	}
	bool	operator = (const wchar_t * p_wstr);
	static	const char * Parse(const char * p_expr,SToken * p_data);
	string	GetExpr()
	{
		return	expr;
	}
private:
	string	expr;
	map<string,TComplex>	var_map;
	vector<SStackData>	suffix;	//
	EParseError	error;
	int	err_pos;
};

#endif
