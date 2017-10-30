//============================================================================
// Name        : calc.cpp
// Author      : D.H.Z.
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C, Ansi-style
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Calculator.h"

void	CalcProc(SCalculator* p_calc,char const * p_expr)
{
	TComplex	res;
	int	error_code = 0;
	int	error_pos = 0;

	*p_calc = p_expr;
	error_code = p_calc->GetError(&error_pos);
	if(error_code == SCalculator::PARSE_ERR_NULL)
	{
		res = p_calc->Compute();
		printf("(%.18g)+(%.18g)i\n",res.real(),res.imag());
	}
	else
	{
		if(error_code == SCalculator::PARSE_ERR_CHAR)
			printf("表达式词法错误！");
		else if(error_code == SCalculator::PARSE_ERR_PROG)
			printf("表达式语法错误！");
		printf("错误位置:%d\n",error_pos);
	}
}

int main(int argc,char * argv[])
{
	SCalculator	calc;

	if(argc > 1)
	{
		for(int iter=1;iter<argc;++iter)
		{
			CalcProc(&calc,argv[iter]);
		}
	}
	else
	{
		char	input_buf[512];
		for(;;)
		{
			printf("输入表达式:");

			gets(input_buf);
			if(stricmp(input_buf,"exit") == 0)
				break;
			CalcProc(&calc,input_buf);
		}
	}
	return EXIT_SUCCESS;
}
