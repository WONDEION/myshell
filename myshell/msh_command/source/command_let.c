#include "command_let.h"

int command_let(int argc , char* arg[])
{
	char calculation[MAXCOMMAND_SIZE];
	int i = 0;
	int state  = 0;
	int value = 0;
	if(0 == argc || NULL == arg)
	{
		return -1;
	}
	if(strcmp(arg[0],"let"))
	{
		shellcommandNOcall(STDERR_FILENO,"let");
	}

	for(i = 1; arg[i] != NULL ;i++)
	{
		if(1 == i)
		{
			strcpy(calculation,arg[i]);
		}
		else
		{
			strcat(calculation,arg[i]);
		}
	}
	clear_space(calculation);
	//printf("calculation: %s\n", calculation);
	i = 0;
	state = analusis_cal(calculation,strlen(calculation),&i,&value);
	return state;
}

int analusis_cal(char* calculation,int strleng,int* local, int* pvalue)
{
	int value = 0;
	int i = 0;
	int calleng = 0;
	int number = 0;
	int begin = 0;
	int leng = 0;
	int type = 0;
	number = get_expressnumber(calculation,local,strleng);
	i = *local;
	leng = 0;
	begin = i;
	//printf("number : %d\n", number);
	while(number)
	{
		for (calleng = 0; calculation[i] != '\0' && calculation[i] != ','
						&& leng < strleng; ++i)
		{
			leng++;
			calleng++;
			/* code */
		}
		if(',' == calculation[i])
		{
			i++;
		}
		if(isassignment(calculation + begin ,calleng))
		{
			//printf("is assignment\n");
			if(-1 == resolve_assignment(calculation + begin,calleng,&value))
			{
				return -1;
			}
		}
		else
		{
			//printf("is compute\n");
			if(-1 == compute(calculation + begin, calleng, &value))
			{
				return -1;
			}
		}	
		begin = i; // step ','
		number--;
	}
	*local = i;
	*pvalue = value;
	return 0;
}

int get_expressnumber(char* string,int *local, int strleng)
{
	int i = 0;
	int number = 0;
	int leng_i = 0;
	if(NULL == string || 0 == strlen(string))
	{
		return 0;
	}
	number = 1;
	for(i = *local , leng_i = 0 ; '\0' != string[i] && leng_i < strleng; leng_i++,i++)
	{
		if(',' == string[i])
		{
			number++;
		}
	}
	return number;
}

int iscalculate(char* string,int strleng) // barring number of strleng
{
	int i = 0;
	if((0 == isassignment(string,strleng)) && (0 == iscompare(string,strleng)))
	{
		return 1;
	}
	return 0;
}

int isassignment(char* string,int strleng) // barring number of strleng
{
	int i = 0;
	char temp_ope[3];
	if(NULL == string || 0 == strleng)
	{
		return 0;
	}
	if(-1 == assignmenttype_and_local(string,strleng,&i,temp_ope))
	{
		return 0;
	}
	return 1;
}

int assignmenttype_and_local(char* string, int strleng, int* local,char *ope)
{
	int i = 0;
	if(NULL == string || 0 == strleng || NULL == ope)
	{
		return -1;
	}
	//printf(" assignmenttype_and_local string: %s\n", string);
	for(i = 0 ; i < strleng ;i ++)
	{
		if('=' == string[i])
		{
			if(i > 0)
			{
				if(string[i - 1] != string[i] && string[i + 1] != string[i]) 
				{
					switch (string[i-1])
					{
						case '+': ope[0] = string[i-1];
								  ope[1] = '\0';
								  *local = i - 1;
								  return 0;
								break;
						case '-': ope[0] = string[i-1];
								  ope[1] = '\0';
								  *local = i - 1;
								  return 0;
								break;
						case '*': ope[0] = string[i-1];
								  ope[1] = '\0';
								  *local = i - 1;
								  return 0;
								break;
						case '/': ope[0] = string[i-1];
								  ope[1] = '\0';
								  *local = i - 1;
								  return 0;
								break;
						case '%': ope[0] = string[i-1];
								  ope[1] = '\0';
								  *local = i - 1;
								  return 0;
								break;
						case '&': ope[0] = string[i-1];
								  ope[1] = '\0';
								  *local = i - 1;
								  return 0;
								break;
						case '^': ope[0] = string[i-1];
								  ope[1] = '\0';
								  *local = i - 1;
								  return 0;
								break;
						case '|': ope[0] = string[i-1];
								  ope[1] = '\0';
								  *local = i - 1;
								  return 0;
								break;
						default :
								if('>' != string[i - 1] && '<' != string[i - 1])
								{
									ope[0] = '=';
									ope[1] = '\0';
									*local = i;
									return 0;
								}
								break;
					} 
					if(i - 1 > 0)
					{
						if(('>' == string[i - 1] || '<' == string[i - 1])
								&& string[i - 1] == string[i - 2])
						{
							ope[0] = string[i - 2];
							ope[1] = string[i - 1];
							ope[2] = '\0';
							return 0;
						}
					}
				}
			}
			else
			{
				printf("Expression format error!!\n");
				return -1;
			}
		}
	}
	return -1;
}

int iscompare(char* string,int strleng) // barring number of strleng
{
	int i = 0;
	char temp_ope[3];
	if(NULL == string)
	{
		return 0;
	}
	if(0 < comparetype_and_local(string,strleng,&i,temp_ope))
	{
		return 1;
	}
	return 0;
}	

int comparetype_and_local(char* string, int strleng, int* local,char *ope)  // return type
// set local    type : 1 ==;  2 !=;  3 >=;  4 <=;  5 >; 6 <;
{
	int i = 0;
	if(NULL == string || NULL == local || NULL == ope)
	{
		return -1;
	}
	for(i = 0; i < strleng; i++)
	{
		if('=' == string[i])
		{
			if(i > 0)
			{
				if('=' == string[i + 1])
				{
					*local = i + 1;
					strcpy(ope,"==");
					return 1;
				}
				if('!' == string[i - 1])
				{
					*local = i - 1;
					strcpy(ope,"!=");
					return 2;
				}
				if('>' == string[i - 1])
				{
					*local = i - 1;
					strcpy(ope,">=");
					return 3;
				}
				if('<' == string[i - 1])
				{
					*local = i - 1;
					strcpy(ope,"<=");
					return 4;
				}
			}
			else
			{
				printf("Expression format error\n");
				return -1;
			}
		}
		if('>' == string[i] || '<' == string[i])
		{
			if(i > 0)
			{
				if((string[i] != string[i - 1]) && (string[i] != string[i + 1]))
				{
					*local = i;
					ope[0] = string[i];
					ope[1] = '\0';
					return string[i] == '>' ? 5 : 6;
				}
			}
			else
			{
				if(string[i] != string[i + 1])
				{
					*local = i;
					ope[0] = string[i];
					ope[1] = '\0';
					return string[i] == '>' ? 5 : 6;
				}
			}
		}
	}
	return 0;
}	

int resolve_assignment(char* string, int strleng, int* pvalue)
{
	int i = 0;
	int j = 0;
	int local = 0;
	int relocal = 0;
	int last_local = 0;
	int leftvalue_container_size = 0;
	int leng = 0;
	int rightvalue_leng = 0;
	int compute_value = 0;
	char save_operator[3];
	char rightvalue[MAXCOMMAND_SIZE];
	char temp_envvalue[MAXENV_SIZE];
	leftvalue_container_type leftvalue[MAXCOMMAND_SIZE];
	if(NULL == string || NULL == pvalue)
	{
		return -1;
	}
	// get all left value
	leng = strleng;
	local = 0;
	last_local = 0;
	i = 0;
	while(0 == assignmenttype_and_local(string + local,leng,&relocal,save_operator))
	{
		//printf("string + local : %s| local : %d| save_operator : %s\n", 
		//	string + local,local,save_operator);
		// save left value infomation
		local = local + relocal;
		for(i = last_local, j = 0; i < local; i++,j++)
		{
			leftvalue[leftvalue_container_size].name[j] = string[i];
		}
		leftvalue[leftvalue_container_size].name[j] = '\0';
		strcpy(leftvalue[leftvalue_container_size].ope,save_operator);
		// Adjust each value
		last_local = local + strlen(save_operator);
		local = last_local;
		leng = leng - strlen(leftvalue[leftvalue_container_size].name) 
					- strlen(save_operator);
		leftvalue_container_size++;
	}
	
	/*for(i = 0; i < leftvalue_container_size; i++)
	{
		printf("leftvalue[%d].name : %s| leftvalue[%d].ope : %s\n", 
			i,leftvalue[i].name,i,leftvalue[i].ope);
	}*/
	// get rightvalue
	for(i = local,j = 0; i < strleng; i++ ,j++)
	{
		rightvalue[j] = string[i];
	}
	rightvalue[j] = '\0';
	//printf("rightvalue : %s|\n", rightvalue);
	// compute rightvalue
	rightvalue_leng = strlen(rightvalue);
	if(-1 == compute(rightvalue, rightvalue_leng, &compute_value))
	{
		return -1;
	}
	//printf("compute over\n");
	// normal compute  ,begin to assignment
	for(i = leftvalue_container_size - 1; i >= 0; i--)
	{
		//printf("process: leftvalue[%d].name : %s| leftvalue[%d].ope : %s\n", 
		//	i,leftvalue[i].name,i,leftvalue[i].ope);
		// inspect leftvalue
		for(j = 0; '\0' != leftvalue[i].name[j]; j++)
		{
			if(!isenvnamechar(leftvalue[i].name[j]))
			{
				printf("Invalid vraiable name : \"%s\"!!\n",leftvalue[i].name);
				return -1;
			}
		}
		// find variable
		if(-1 == (j = get_from_all_env(leftvalue[i].name,temp_envvalue)))
		{	
			//new variable
			if(strcmp("=",leftvalue[i].ope))   // != "="
			{
				printf("Build vraiable \"%s\" only can be \"=\"\n",leftvalue[i].name);
				return 0;
			}
			int_to_string(compute_value,temp_envvalue,10);
			if(-1 == buildenv_charvalue(&msh_envtable,leftvalue[i].name,temp_envvalue))
			{
				return -1;
			}
		}
		else // updata variable
		{
			if(!isalldigit(temp_envvalue))
			{
				printf("Variable \"%s\" value type error!!\n", leftvalue[i].name);
				return -1;
			}
			//printf("leftvalue[%d].ope : \"%s\"\n",i, leftvalue[i].ope);
			if(strcmp("=",leftvalue[i].ope))   // != "="
			{
				// remove '='
				leftvalue[i].ope[strlen(leftvalue[i].ope) - 1] = '\0';
				// 
				if(-1 == calculator_value(atoi(temp_envvalue), leftvalue[i].ope, 
						compute_value, &compute_value))
				{
					return -1;
				}
			}
			int_to_string(compute_value,temp_envvalue,10);
			updatemshenv(leftvalue[i].name,temp_envvalue);
			// update rightvalue
			strcpy(rightvalue,temp_envvalue);
		}
	}
	//printf("end string : %s\n",string);
	return 0;
}

int compute(char* string, int strleng,int* pvalue)
{
	int i = 0;
	char* bufexp = NULL;
	int value = 0;
	int local = 0;
	int dev_leng = 0;
	expression_type expression; 
	if(NULL == string || 0 == strleng)
	{
		return -1;
	}
	bufexp = (char*)malloc(sizeof(string) * strleng);
	if(NULL == bufexp)
	{
		printf("let : compute : bufexp malloc error\n");
		return -1;
	}
	// get bufexp
	for(i = 0; i < strleng ; i++)
	{
		bufexp[i] = string[i]; 
	}
	bufexp[i] = '\0';

	// develop env
	if(-1 == develop_envselfadd(bufexp, strleng, &local,&dev_leng))
	{
		return 0;
	}
	// resolve
	if(-1 == let_resolve(bufexp, dev_leng, &local ,&value))
	{
		return -1;
	}
	printf("value : %d\n", value);
	//free(bufexp);
	*pvalue = value;
	return 0;
}


int let_resolve(char* string, int strleng, int* local, int* pvalue)
{
	int i = 0;
	int ri = 0;
	int expi = 0;
	int value = 0;
	int state = 0;
	char ope[3];
	expression_type expression;
	expression.size = 0;
	if(NULL == string || 0 == strleng || NULL == local || NULL == pvalue)
	{
		return -1;
	}
//	printf("let_resolve :");
/*	for(i = 0; i < strleng; i++)
	{
		printf("%c", string[i + *local]);
	}*/
//	printf(" | strleng : %d\n", strleng);
	for(i = *local,expi = 0; expi < strleng && string[i] != '\0'
							&& string[i] != ')' && string[i] != ',';)
	{
		//get number
		ri = i;
		if(-1 == (state = let_get_number(string,&i,&value)))
		{
			return -1;
		}
		if(0 == state)
		{
			expi = expi + i - ri;
			//printf("value : %d\n", value);
			pushexpression_d(&expression,value,"\0\0\0",'i');
		}
		if(expi >= strleng)
		{
			break;
		}
		//get operator
		ri = i;
		//printf("string[%d] : %c| expi :%d\n", i,string[i],expi);
		if(-1 == (state = let_get_operator(string,&i,ope)))
		{
			return -1;
		}
		if(0 == state)
		{
			//printf("operator: %s\n",ope);
			pushexpression_d(&expression,value,ope,'c');
			expi = expi + i - ri;
		}
	}
//	printexpression(&expression);
	infix_to_postfix(&expression,&expression);
//	printf("infix :\n");
//	printexpression(&expression);
	state = calculator_postfix(&expression, &value);
	if(-1 == state)
	{
		return -1;
	}
	*local = i;
	*pvalue = value;
	//printf("value : %d\n", value);
	return 0;
}

int let_get_number(char* string, int* local, int* pvalue)
{
	int i = 0;
	int j = 0;
	int value = 0;
	int remember_i = 0;
	char unaryop_char = 0;
	int state = 0;
	if(NULL == string || NULL == local)
	{
		return -1;
	}
	i = *local;
	if('\0' == string[i])
	{
		return 1;
	}
	if(isdigit(string[i]))  // normal
	{
		while(isdigit(string[i]))
		{
			value = value * 10 + (string[i] - '0');
			i++;
			state = 0;
		}
	}
	else if('(' == string[i]) 
	{
		i++;
		int leng = 0;
		int bracerecord = 0;
		for(j = i; string[j] != '\0';j++)
		{
			//printf("string[%d] : %c | bracerecord ; %d\n",j,string[j],bracerecord );
			if(0 == bracerecord && ')' == string[j])
			{
				//printf("leng : %d | j : %d | string[j] : %c\n", leng,j,string[j]);
				break;
			}
			if('(' == string[j])
			{
				bracerecord++;
			}
			if(')' == string[j])
			{
				bracerecord--;
			}
			leng++;
		}
		if(-1 == analusis_cal(string,leng,&i,&value))
		{
			return -1;
		}
		//printf("value : %d ; string[%d] : %c\n",value,i,string[i]);
		i++; // step ')'
	}
	else if(isunaryoperator(string[i])) 
	{
		unaryop_char = string[i];
		i++;
		if((!isdigit(string[i])) && '(' == string[i])
		{
			printf("expression format error at : \"%s\"\n", string + i);
			return -1;
		}
		if(-1 == let_get_number(string,&i,&value))
		{
			return -1;
		}
		switch(unaryop_char)
		{
			case '+': value = value;
					break;
			case '-': value = -value;
					break;
			case '!': value = !value;
					break;
			case '~': value = ~value;
					break;
		}
	}
	else
	{	
		printf("expression format error at : \"%s\"\n", string + i);
		return -1;
	}

	*local = i;
	*pvalue = value;
	return 0;
}

int let_get_operator(char* string, int* local, char* poperator)
{
	int i = 0;

	if(NULL == string || NULL == local || NULL == poperator)
	{
		return -1;
	}
	i = *local;
	if('\0' == string[i])
	{
		return 1;
	}
	if('+' == string[i] || '-' == string[i] || '/' == string[i]
		|| '%' == string[i] || '^' == string[i])  // 
	{
		poperator[0] = string[i];
		poperator[1] = '\0';
		i++;
	}
	else if('*' == string[i] || '&' == string[i] || '|' == string[i])
	{
		if(string[i] != string[i + 1])
		{
			poperator[0] = string[i];
			poperator[1] = '\0';
			i++;
		}
		else
		{
			poperator[0] = string[i];
			i++;
			poperator[1] = string[i];
			poperator[2] = '\0';
			i++;
		}
	}
	else if('<' == string[i] || '>' == string[i])
	{
		if(string[i] == string[i + 1])
		{
			poperator[0] = string[i];
			i++;
			poperator[1] = string[i];
			poperator[2] = '\0';
			i++;
		}
		else if('=' == string[i + 1])
		{
			poperator[0] = string[i];
			i++;
			poperator[1] = string[i];
			poperator[2] = '\0';
			i++;
		}
		else if(isdigit(string[i + 1]))
		{
			poperator[0] = string[i];
			poperator[1] = '\0';
			i++;
		}
		else
		{
			printf("expression format error at : \"%s\"\n", string + i);
			return -1;
		}
	}
	else if('!' == string[i] || '=' == string[i])
	{
		if('=' == string[i + 1])
		{
			poperator[0] = string[i];
			i++;
			poperator[1] = string[i];
			poperator[2] = '\0';
			i++;
		}
		else if('!' == string[i] && !isdigit(string[i + 1]))
		{
			printf("expression format error at : \"%s\"\n", string + i);
			return -1;
		}
	}
	else
	{
		printf("expression format error at : \"%s\"\n", string + i);
		return -1;
	}
	*local = i;
	return 0;
}

int infix_to_postfix(expression_type* expdest,expression_type* expsource)
{
	int i = 0;
	int relation = 0;
	expression_type tempdest;
	expression_type stack_exp;
	expression_info_type tempinfo;
	stack_exp.size = 0;
	tempdest.size = 0;

	if(NULL == expdest || NULL == expsource)
	{
		return -1;
	}
	for(i = 0; i < expsource->size; i++)
	{
		if('i' == expsource->express[i].type)
		{
			if(-1 == pushexpression(&tempdest,expsource->express + i))
			{
				printf("expression format error!!\n");
				return -1;
			}
		}
		else if('c' == expsource->express[i].type)
		{
			if( 0 == stack_exp.size) //
			{
				pushexpression(&stack_exp,expsource->express + i);
			}
			else
			{
				relation = compara_operator(expsource->express[i].ope,
								stack_exp.express[stack_exp.size - 1].ope);
				if(relation < 0)
				{
					//printf("now stack_exp :\n");
					//printexpression(&stack_exp);
					return -1;
				}
				if(0 == relation) // out
				{
					while(0 < compara_operator(stack_exp.express[stack_exp.size - 1].ope,
									expsource->express[i].ope))
					{
						int popstate = 0;
						popstate = popexpression(&stack_exp,&tempinfo);
						pushexpression(&tempdest,&tempinfo);
						if(-1 == popstate || 0 == stack_exp.size)
						{
							break;
						}
					}
					//printf("now stack_exp :\n");
					//printexpression(&stack_exp);
					pushexpression(&stack_exp,expsource->express + i);
				}
				else   // >= in
				{
					pushexpression(&stack_exp,expsource->express + i);
				}
			}
		}
		else
		{
			printf("command_let / infix_to_postfix : program error!!!\n");
		}
	}
	while(-1 != popexpression(&stack_exp,&tempinfo))
	{
		pushexpression(&tempdest,&tempinfo);
	}
	//printf("here\n");
	//printexpression(&tempdest);
	expression_copy(expdest,&tempdest);
	return 0;	
}

int calculator_postfix(expression_type* expression, int* pvalue)
{
	int i = 0;
	int temp_value_1 = 0;
	int temp_value_2 = 0;
	int temp_value = 0;
	stack_int_type number_stack;
	number_stack.sp = 0;
	if(NULL == expression || NULL == pvalue)
	{
		return -1;
	}
	for(i = 0; i < expression->size; i++)
	{
		//printf("here\n");
		if('i' == expression->express[i].type)
		{
			//printf("push :%d \n", expression->express[i].value);
			push_int(&number_stack,expression->express[i].value);
		}
		else if('c' == expression->express[i].type)
		{
			if(-1 == pop_int(&number_stack,&temp_value_2)||
					 -1 == pop_int(&number_stack,&temp_value_1))
			{
				printf("Expression format error!!\n");
				return -1;
			}
			if(-1 == calculator_value(temp_value_1,expression->express[i].ope,
											temp_value_2, &temp_value))
			{
				printf("Operator error!!\n");
				return -1;
			}
			//printf("temp_value : [%d] = temp_value_1 : [%d]  [%s] temp_value_2 : [%d]\n",
			//			temp_value,temp_value_1,expression->express[i].ope,temp_value_2 );
			//printf("push :%d \n", temp_value);
			push_int(&number_stack,temp_value);
		}
		else
		{
			printf("Program error\n");
		}
	}
	if(-1 == pop_int(&number_stack,&temp_value) || number_stack.sp != 0)
	{
		return -1;
	}

	*pvalue = temp_value;
	//printf("value : %d\n", *pvalue);
	return 0;
}

int calculator_value(int value_1, char* ope, int value_2, int* pvalue)
{
	int value = 0;
	if(NULL == ope || NULL == pvalue)
	{
		return -1;
	}
	if(1 == strlen(ope))
	{
		switch(ope[0])
		{
			case '+' : 	value = value_1 + value_2;
						break;
			case '-' :  value = value_1 - value_2;
						break;
			case '*' :  value = value_1 * value_2;
						break;
			case '/' :  value = value_1 / value_2;
						break;
			case '%' :  value = value_1 % value_2;
						break;
			case '&' :  value = value_1 & value_2;
						break;
			case '^' :  value = value_1 ^ value_2;
						break;
			case '|' :  value = value_1 | value_2;
						break;
			case '>' :  value = value_1 > value_2;
						break;
			case '<' :  value = value_1 < value_2;
						break;
			default : 	printf("Operator error!!\n");
						return -1;
						break;
		}
	}else if(2 == strlen(ope))
	{
		if(!strcmp(ope,"**"))
		{
			int i = 0;
			for(i = 0, value = 1;i < value_2;i++)
			{
				value = value * value_1;
			}
		}
		if(!strcmp(ope,"||"))
		{
			value = value_1 || value_2;			
		}
		if(!strcmp(ope,"&&"))
		{
			value = value_1 && value_2;
		}
		if(!strcmp(ope,"<<"))
		{
			value = value_1 << value_2;
		}
		if(!strcmp(ope,">>"))
		{
			value = value_1 >> value_2;
		}
		if(!strcmp(ope,">="))
		{
			value = value_1 >= value_2;
		}
		if(!strcmp(ope,"<="))
		{
			value = value_1 <= value_2;
		}
		if(!strcmp(ope,"=="))
		{
			value = value_1 == value_2;
		}
		if(!strcmp(ope,"!="))
		{
			value = value_1 != value_2;
		}
	}
	else
	{
		printf("Operator error!!\n");
		return -1;
	}
	*pvalue = value;
	return 0;
}

int compara_operator(char* ope1, char* ope2)  //  ope1 > ope2 : 2
// ope1 == ope2 : 1     ope1 < ope2 : 0
{
	int prioritys_1 = 0;
	int prioritys_2 = 0;

	if(NULL == ope1 || NULL == ope2)
	{
		return -1;
	}

	prioritys_1 = operator_priority(ope1);
	prioritys_2 = operator_priority(ope2);

	if(prioritys_1 < 1 || prioritys_2 < 1)
	{
//		printf("ope1 : %s| ope2 : %s|\n",ope1,ope2);
		printf("Invalid operator!!\n");
		return -1;
	}

	if(prioritys_1 < prioritys_2)
	{
		return 2;
	}
	else if(prioritys_1 == prioritys_2)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int operator_priority(char* ope) // 1, 2, 3, 4, 5, ,6, 7, 8, 9
{
	if(NULL == ope)
	{
		return -1;
	}
	if(!strcmp(ope,"**"))
	{
		return 1;
	}
	if('\0' == ope[1] && ('*' == ope[0] || '/' == ope[0] || '%' == ope[0]))
	{
		return 3;
	}
	if('\0' == ope[1] && ('+' == ope[0] || '-' == ope[0]))
	{
		return 4;
	}
	if(!strcmp(ope,"<<") || !strcmp(ope,">>"))
	{
		return 5;
	}
	if(!strcmp(ope,"<=") || !strcmp(ope,">=")||!strcmp(ope,"<") || !strcmp(ope,">"))
	{
		return 6;
	}
	if(!strcmp(ope,"==") || !strcmp(ope,"!="))
	{
		return 7;
	}
	if('\0' == ope[1] && '&' == ope[0])
	{
		return 8;
	}
	if('\0' == ope[1] && '^' == ope[0])
	{
		return 9;
	}
	if('\0' == ope[1] && '|' == ope[0])
	{
		return 10;
	}
	if(!strcmp(ope,"&&"))
	{
		return 11;
	}
	if(!strcmp(ope,"||"))
	{
		return 12;
	}
	return 0;
}

int develop_envselfadd(char* string,int strleng, int* local,int* pleng)
{
	int i = 0;
	int leng = 0;
	int begin = 0;
	int j = 0;
	int realleng = 0;
	int env_head = 0;
	int env_tail = 0;
	int env_digit = 0;
	int space_value = 0;
	int space_leng = 0;
	int space_begin = 0;
	int trans_loacl = 0;
	int env_name_leng = 0;
	char env_name[MAXNAME_SIZE];
	char env_value[MAXENV_SIZE];
	if(NULL == string || NULL == local || NULL == pleng)
	{
		return -1;
	}
//	printf("develop_envselfadd : ");
	/*for( i = 0; i < strleng ; i++)
	{
		printf("%c",string[i + *local] );
	}*/
	//printf("\n");
	begin = *local;
	realleng = strleng;
	for(i = *local, leng = 0; string[i] != '\0' && leng < strleng 
					&& i - *local < strleng; i++,leng++)
	{
		if(isalpha(string[i]) || '_' == string[i])
		{
			j = 0;
			env_head = i;
			while(isenvnamechar(string[i]))
			{
				env_name[j] = string[i];
				j++;
				i++;
				leng++;
			}
			env_name[j] = '\0';
			env_tail = i;
			if(-1 == get_from_all_env(env_name, env_value))
			{
				//if no env
				printf("No variable : \"%s\"\n", env_name);
				return -1;
			}
			//printf("env_value : %s\n", env_value);
			if(0 == isalldigit(env_value))
			{
				// if type error
				printf("Vraiable type error : \"%s\"\n", env_name);
				return -1;
			}
			env_digit = atoi(env_value);
			// mode
			if(env_head - 2 >= begin && 
				('-' == string[env_head - 1] || '+' == string[env_head - 1])
				&& string[env_head - 1] == string[env_head - 2])
				// add or reduction before used
			{
				//printf("in before\n");
				if('-' == string[env_head - 1])
				{
					env_digit--;
				}
				else if('+' == string[env_head - 1])
				{
					env_digit++;
				}
				else
				{
					printf("develop_envselfadd : program error!!\n");
					return -1;
				}
				space_value = env_digit;
				env_name_leng = 2;
				space_begin = env_head - 2;
				trans_loacl = env_tail;
				int_to_string(env_digit, env_value,10);
				if(-1 == updateallenv(env_name,env_value))
				{
					printf("updateallenv error\n");
				}
			}
			else if (env_head + 2 < begin + strleng && 
					('-' == string[env_head + 1] || '+' == string[env_head + 1])
					&& string[env_head + 1] == string[env_head + 2]) 
			// add or reduction after used
			{
				//printf("in after\n");
				space_value = env_digit;
				if('-' == string[env_head + 1])
				{
					env_digit--;
				}
				else if('+' == string[env_head + 1])
				{
					env_digit++;
				}
				else
				{
					printf("develop_envselfadd : program error!!\n");
					return -1;
				}
				env_name_leng = 2;
				space_begin = env_head;
				trans_loacl = env_tail + 2;
				// back set
				int_to_string(env_digit, env_value,10);
				if(-1 == updateallenv(env_name,env_value))
				{
					printf("updateallenv error\n");
				}
				leng = leng + 2;
			}
			else //normal
			{
				env_name_leng = 0;
				space_value = env_digit;
				space_begin = env_head;
				trans_loacl = env_tail;
			}
			// *********************************
			// resplace
			int_to_string(space_value,env_value,10);
			space_leng = get_int_bit(space_value);
			//printf("space_leng : %d\n", space_leng);
			env_name_leng = env_name_leng + strlen(env_name);
			//printf("env_name_leng : %d\n", env_name_leng);
			//printf("space_begin : %d\n", space_begin);
			string_translation(string,trans_loacl, space_leng - env_name_leng);
			for(j = 0; j < space_leng; j++)
			{
				string[space_begin + j] = env_value[j];
			}
			// set leng
			realleng = realleng - env_name_leng + space_leng;
			//printf("after develop string : %s\n", string);  
		}	
	}
	*pleng = realleng;
	return 0;
}

int is_expressionlegality(char* expression, int mode)  // mode == 1 mead : include '='
// mode == 0 means Barring '='
{
	int i = 0;
	if(NULL == expression)
	{
		return 0;
	}

	i = 0;
	while('\0' != expression[i])
	{
		if(!is_calculatorchar(expression[i]))
		{
			if(!(1 == mode && '=' == expression[i]))
			{
				printf("led : invalid input at \"%s\"\n", expression + i);
				return 0;
			}
		}
	}
}

int is_calculatorchar(char ch)
{
	if(isdigit(ch))
	{
		return 1;
	}
	if(ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%' ||
		ch == '&' || ch == '|' || ch == '<' || ch == '>' || ch == '(' || ch == ')' ||
		ch == '!' || ch == '~' || ch == '?' || ch == ','|| ch == ':' )
	{
		return 1;
	}
	return 0;
}

int expression_copy(expression_type* expdest,expression_type* expsource)
{
	int i = 0;
	if(NULL == expdest || NULL == expsource)
	{
		return -1;
	}
	for(i = 0; i < expsource->size; i++)
	{
		expression_info_copy(expdest->express + i,expsource->express + i);
	}
	expdest->size = expsource->size;
	return 0;
}

int expression_info_copy(expression_info_type* expdest, expression_info_type* expsource)
{
	int i = 0;
	if(NULL == expdest || NULL == expsource)
	{
		return -1;
	}
	expdest->value = expsource->value;
	expdest->type = expsource->type;
	strcpy(expdest->ope,expsource->ope);

	return 0;
}

int isunaryoperator(char ch)
{
	if('+' == ch || '-' == ch || '!' == ch || '~' == ch)
	{
		return 1;
	}
	return 0;
}

int pushexpression_d(expression_type* pexp,int digit,char* ope,char type)
{
	if(NULL == pexp || NULL == ope)
	{
		return -1;
	}
	if(pexp->size == MAXCOMMAND_SIZE)
	{
		return -1;
	}
	if('i' == type)
	{
		pexp->express[pexp->size].value = digit;
		pexp->express[pexp->size].type = type;
		strcpy(pexp->express[pexp->size].ope,"\0\0\0");
	}
	else if('c' == type)
	{
		pexp->express[pexp->size].value = 0;
		pexp->express[pexp->size].type = type;
		strcpy(pexp->express[pexp->size].ope,ope);
	}
	else
	{
		return -1;
	}
	pexp->size++;
	return 0;
}

int popexpression_d(expression_type* pexp,int* pdi, char* ope, char* ptype)
{
	if(NULL == pexp || NULL == pdi || NULL == ope || NULL == ptype)
	{
		return -1;
	}
	if(0 == pexp->size)
	{
		return -1;
	}
	pexp->size--;
	*pdi = pexp->express[pexp->size].value;
	*ptype = pexp->express[pexp->size].type;
	strcpy(ope,pexp->express[pexp->size].ope);
	return 0;
}

int pushexpression(expression_type* pexp,expression_info_type* pinfo)
{
	if(NULL == pexp || NULL == pinfo)
	{
		return -1;
	}
	if(MAXCOMMAND_SIZE == pexp->size)
	{
		return -1;
	}
	expression_info_copy(pexp->express + pexp->size, pinfo);
	pexp->size++;
	return 0;
}

int popexpression(expression_type* pexp,expression_info_type* pinfo)
{
	if(NULL == pexp || NULL == pinfo)
	{
		return -1;
	}
	if(0 == pexp->size)
	{
		return -1;
	}
	pexp->size--;
	expression_info_copy(pinfo,pexp->express + pexp->size);
	return 0;
}

int printexpression(expression_type* expression)
{
	int i = 0;
	if(NULL == expression)
	{
		return -1;
	}
	for(i = 0; i < expression->size; i++)
	{
		if('i' == expression->express[i].type)
		{
			printf("%d ", expression->express[i].value);
		}
		else
		{
			printf("%s ", expression->express[i].ope);
		}
	}
	printf("\n");
	return 0;
}