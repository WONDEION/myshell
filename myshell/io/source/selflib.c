#include "selflib.h"

extern int screenlock;
int printf_fd = STDOUT_FILENO;

static int state = 0; // 0 = unlock 1 = lock 2 = freeing  
static int laststate = 0;
static int cachenum = 0;
static int get_icecha = 0;
static int add_icecha = 0;


char msh_getchar(int fd)   // fd is filedescriptor
{
	char buf[10];
	char output = 0;

	if(1 == state && 0 == screenlock)
	{
		state = 2;
	}

	switch(state)
	{
		case 0:	output = read(fd,buf,1);
				//printf("buf[0] : %d,output: %d\n",buf[0],output);
				break;
		case 1: output = read(fd,buf,1);
				break;
		case 2: buf[0] = getfromcache();
				break;
	}

	if( -1 == buf[0] )
	{
		state = 0;
		buf[0] = 17;
	}
	if((0 == state || 1 == state) && 0 == output)
	{
		//printf("here\n");
		return -1;
	}

	output = buf[0];

	return output;
}

char getfromcache()   // get char form cache
{
	int retchar = 0;

	if(get_icecha == cachenum)  // finished freeing
	{
		state = 0;
		cachenum = 0;
		get_icecha = 0;
		add_icecha = 0;
		return -1;
	}

	retchar = command_lockcache[get_icecha];
	get_icecha++;

	/*if(get_icecha == cachenum)  // finished freeing
	{
		state = 0;
		cachenum = 0;
		get_icecha = 0;
		add_icecha = 0;
		return -1;
	}*/

	return retchar;
}

int addchar_lockcache(char inputchar)  // add char to cache
{
	state = 1;
	if(19 == inputchar)
	{
		return 0;
	}
	// add
	//printf("\r\nwe cache: %d \r\n",inputchar);
	command_lockcache[add_icecha] = inputchar;
	// adjustment   
	add_icecha++;
	cachenum++;
	// 
	command_lockcache[cachenum] = '\0';

	if(CACHE_SIZE == cachenum)
	{
		PRINTERROR("Too many input of lock cache\n");
	}

	return 0;
}

int string_translation(char* string,int loc,int direction)    // 0< left   0> right  LOC is an array subscript
{
	int i = 0;
	int leng = 0;

	if(NULL == string)
	{
		return -1;
	}

	leng = strlen(string);

	if(loc < 0 || loc > leng)
	{
		return -1;
	}

	if(loc + direction < 0)
	{
		return -1;
	}

	if(0 > direction)   // left shift  
	{
		direction = -direction;
		for(i = loc; i <= leng; i++)
		{
			string[i - direction] = string[i];
		}
	}
	else if(direction > 0)   // right shift
	{
		for(i = leng; i>= loc; i--)
		{
			string[i + direction] = string[i];
		}
	}

	return 0;
}

int get_int_bit(int digit)
{
	int bitnumber = 0;

	if(digit == 0)
	{
		return 1;
	}

	while(digit != 0)
	{
		digit = digit /10;
		bitnumber++;
	}

	return bitnumber;
}

int issthex(char digit_bit)
{
	char sthex_c[] = "abcdefABCDEF";
	int i = 0;
	if(isdigit(digit_bit))
	{
		return digit_bit - '0';
	}
	for(i = 0; i < 12; i++)
	{
		if(sthex_c[i] == digit_bit)
		{
			return i > 5 ? digit_bit - 'A' + 10 : digit_bit - 'a' + 10;
		}
	}
	return 0;

}

int get_first_word(char* string, char* word)
{
	int i = 0;
	if(NULL == string || NULL == word)
	{
		return -1;
	}
	for( i = 0; string[i] != '\0' && string[i] != ' ';i++)
	{
		word[i] = string[i];
	}
	word[i] = '\0';
	return 0;
}

int find_word_from_string(char* string, char* word)
{
	int i = 0;
	int ret = 0;
	int j = 0;
	if(NULL == string || NULL == word)
	{
		return -1;
	}
	for(i = 0; string[i] != '\0'; i++)
	{
		if(string[i] == ' ')
		{
			while( string[i] == ' ')
			{
				i++;
			}
			ret = i;
			j = 0 ;
			while(word[j] != '\0' && string[i] == word[j])
			{
				i++;
				j++;
			}
			if('\0' == word[j])
			{
				return ret;
			}
		}
	}
	return -1;
}

int get_word_times(char* string,char* word)
{
	int i = 0;
	int times = 0;
	char* p_s = string;
	int wordleng = strlen(word);
	if(NULL == string || NULL == word)
	{
		return -1;
	}
	while(-1 != (i = find_word_from_string(p_s,word)))
	{
		times++;
		p_s = p_s + wordleng;
	}
	return times;
}
 
int string_to_arg(char* string, char** arg) // no malloc 
{
	int i = 0;
	int j = 0;
	int a_i = 0;

	if(NULL == string || NULL == arg)
	{
		return -1;
	}
	for(i = 0, j = 0, a_i = 0;  string[i] != '\0';i++, j++)
	{
		if(' ' == string[i])
		{
			arg[a_i][j] = '\0';
			j = -1;
			a_i++;
		}
		else
		{
			arg[a_i][j] = string[i];
		}
	}
	arg[a_i][j] = '\0';
	arg[a_i + 1] = NULL;

	return a_i +1;
}

int arg_to_string(char* arg[],char* string)
{
	int i = 0;
	int j = 0;
	int k = 0;
	if(NULL == arg || NULL == string)
	{
		return -1;
	}
	while(NULL != arg[i])
	{
		for(k = 0;arg[i][k] != '\0';k++,j++)
		{
			string[j] = arg[i][k];
		}
		string[j] = ' ';
		j++;
		i++;	
	}
	string[j - 1] = '\0';
	return 0;
}

int string_remove_word(char* string, char* word)
{
	int i = 0;
	if(NULL == string || NULL == word )
	{
		return -1;
	}
	if(-1 == (i = find_word_from_string(string,word)))
	{
		return 0;
	}
	string_translation(string,i,strlen(word) + 1);
	return 0;
}

int get_string_words(char* string)
{
	int i = 0;
	int num = 0;
	if(NULL == string)
	{
		return -1;
	}
	while(string[i] == ' ')
	{
		i++;
	}
	for(; string[i] != '\0' ; i++)
	{
		if(' ' == string[i])
		{
			num++;
			while(string[i] == ' ')
			{
				i++;
			}
			if(' ' != string[i])
			{
				i--;
			}	
		}
	}
	return num + 1;
}

int clear_intarray(int* ar, int n)
{
	int i = 0;
	if(NULL == ar || n < 0)
	{
		return -1;
	}
	for(i = 0; i < n;i++)
	{
		ar[i] = 0;
	}
	return 0;
}

int isalldigit(char* string)
{
	int i = 0;
	if(NULL == string)
	{
		return 0;
	}
	while('\0' != string[i] && isdigit(string[i]))
	{
		i++;
	}
	if('\0' != string[i])
	{
		return 0;
	}
	return 1;
}

int clear_space(char* string)
{
	int i = 0;
	int j = 0;
	char* buf = NULL;
	if(NULL == string)
	{
		return -1;
	}
	buf = malloc(sizeof(char) * strlen(string));

	i = 0;
	j = 0;
	while('\0' != string[i])
	{
		if(' ' != string[i] && '\t' != string[i])
		{
			buf[j] = string[i];
			j++;
		}
		i++;
	}
	buf[j] = '\0';
	strcpy(string,buf);
	return 0;
}

int msh_fprintf(int fd,char* format, ...) 
{
	va_list ap;
	char ptemp[512];
	char *ptarget = NULL;
	int iformat = 0;
	int itarget = 0;
	int itemp = 0;
	int numofturn = 0;

	// va_arg type;
	char* vaarg_s;
	unsigned char vaarg_uc;
	char vaarg_c;
	unsigned int vaarg_u;
	int  vaarg_d;
	unsigned long int vaarg_ul;
	long int  vaarg_l;
	float vaarg_f;
	double vaarg_lf;
	long double vaarg_llf;
	// 
	myprint_inf printtype;
	printtype.width = 0;
	printtype.accuracy = -1;
	printtype.align = 0; // 1 (-)   2 (+)  4 ( )  8(#)
	printtype.fill = 0;  // 1 -have filling     0 none  

	ptarget = (char*)malloc((strlen(format) + 512) * sizeof(char));
	va_start(ap,format);

    while(1)
    {
    	printtype.width = 0;
		printtype.accuracy = -1;
		printtype.align = 0; // 1 (-)   2 (+)  4 ( )  8(#)
		printtype.fill = 0;  // 1 -have filling     0 none 
    	// normal char
    	while(format[iformat] != '%' && '\0' != format[iformat])
    	{
    		ptarget[itarget] = format[iformat];
    		itarget++;
    		iformat++;
    	}

    	// if end 
    	if('\0' == format[iformat])
    	{
    		ptarget[itarget] = '\0';
    		break;
    	}

    	iformat++;    // skip '%'
    	// get mark
    	// if no output description 
    	/*
    	keep it
    	printtype.width = -1;
		printtype.accuracy = -1;
		printtype.align = 0;
		*/
    	printtype.align += mprint_getmark(format,&iformat); 
    	

    	//get width and filling
	    if(isdigit(format[iformat]))
    	{
    		if('0' == format[iformat])
    		{
    			printtype.fill = 1;
    		}

    		printtype.width = atoi(format+iformat);   //The functon atoi will return 0 when string have not  width 
    		while(isdigit(format[iformat]))
    		{
    			iformat++;
    		}
    	}

    	//get accuracy
    	if('.' == format[iformat])
    	{
    		format++;
    		if(isdigit(format[iformat]))
    		{
    			printtype.accuracy = atoi(format+iformat); 	//The functon atoi will return 0 when string have not  accuracy 
	    		while(isdigit(format[iformat]))
	    		{
	    			iformat++;
	    		}
    		}
    	}

    	switch(format[iformat])
    	{
    		case 's':	vaarg_s = va_arg(ap, char*);
    					append_s(ptarget, &itarget, vaarg_s, &printtype);
    					iformat = iformat + 1;
    					numofturn++;
    					break;

    		case 'c':	vaarg_c = va_arg(ap, int);
    					append_c(ptarget, &itarget, vaarg_c, &printtype);
    					iformat = iformat + 1;
    					numofturn++;
    					break;

    		case 'd':	vaarg_d = va_arg(ap, int);
    					append_d(ptarget, &itarget, vaarg_d, &printtype);
    					iformat = iformat + 1;
    					numofturn++;
    					break;

    		case 'i':	vaarg_d = va_arg(ap, int);
    					append_d(ptarget, &itarget, vaarg_d, &printtype);
    					iformat = iformat + 1;
    					numofturn++;
    					break;

    		case 'u':   vaarg_u = va_arg(ap, unsigned int);
    					append_u(ptarget, &itarget, vaarg_u, &printtype);
    					iformat = iformat + 1;
    					numofturn++;
    					break;

    		case 'X':	vaarg_d = va_arg(ap,unsigned int);
    					append_X(ptarget, &itarget, vaarg_d, &printtype);
    					iformat = iformat + 1;
    					numofturn++;
    					break;

    		case 'x':	vaarg_d = va_arg(ap,unsigned int);
    					append_x(ptarget, &itarget, vaarg_d, &printtype);
    					iformat = iformat + 1;
    					numofturn++;
    					break;

    		case 'o':   vaarg_d = va_arg(ap,unsigned int);
    					append_o(ptarget, &itarget, vaarg_d, &printtype);
    					iformat = iformat + 1;
    					numofturn++;
    					break;

    		case 'f':	vaarg_f = va_arg(ap, double);
    					append_f(ptarget, &itarget, vaarg_f, &printtype);
    					iformat = iformat + 1;
    					numofturn++;
    					break;

    		case 'e':   vaarg_f = va_arg(ap, double);
    					append_e(ptarget, &itarget, vaarg_f, &printtype);
    					iformat = iformat + 1;
    					numofturn++;
    					break;

    		case 'E':   vaarg_f = va_arg(ap, double);
    					append_E(ptarget, &itarget, vaarg_f, &printtype);
    					iformat = iformat + 1;
    					numofturn++;
    					break;

    		case 'g':   vaarg_f = va_arg(ap, double);
    					append_g(ptarget, &itarget, vaarg_f, &printtype);
    					iformat = iformat + 1;
    					numofturn++;
    					break;

    		case 'G':   vaarg_f = va_arg(ap, double);
    					append_f(ptarget, &itarget, vaarg_f, &printtype);
    					iformat = iformat + 1;
    					numofturn++;
    					break;

    		case 'p':   vaarg_l = va_arg(ap, long long int);
    					append_l(ptarget, &itarget, vaarg_l, &printtype);
    					iformat = iformat + 1;
    					numofturn++;
    					break;

    		case 'n':   append_d(ptarget, &itarget, numofturn, &printtype); 
    					iformat = iformat + 1;
    					break;

    		case 'l':	switch(format[iformat+1])
    					{
    						case 'f':	vaarg_lf = va_arg(ap, double);
    									append_lf(ptarget, &itarget, vaarg_lf, &printtype);
    									iformat = iformat + 2;
    									numofturn++;
    									break;
    									
    						case 'd':	vaarg_l = va_arg(ap, long int);
    									append_l(ptarget, &itarget, vaarg_l, &printtype);
    									iformat = iformat + 2;
    									numofturn++;
    									break;

    						default:    if('\0' != format[iformat])
    									{				
    										iformat = iformat + 1;
    										if('\0' != format[iformat])
	    									{				
	    										iformat = iformat + 1;
	    									}
    									}
    									break;			

    					}
    					break;

    		case '%':	ptarget[itarget] = '%';
    					itarget++;
    					iformat++;
    					break;

    		default: 	if('\0' != format[iformat])
    					{
    							iformat++;
    					}
    					break;
    	}
    }

    // keep itarget in range
    itarget = (itarget >= strlen(format) + 511) ? strlen(format) + 511 : itarget; 

    ptarget[itarget] = '\0';

    write(fd, ptarget, itarget);   // DO not need to print '\0'

	va_end(ap);
	free(ptarget);

	return numofturn;
}

int mprint_getmark(char* format,int* p_iformat) // return myprint_inf.align     '%' has been skip
{
	int iformat = *p_iformat;
	int returnalign = 0;

	while(format[iformat] == '-' || format[iformat] == '+' || format[iformat] == ' ' || format[iformat] == '#')
	{
		switch(format[iformat])
		{
			case '-':	returnalign += 1;
						break;
			case '+':	returnalign += 2;
						break;
			case ' ':	returnalign += 4;
						break;
			case '#':	returnalign += 8;
						break;
			default: 	break;
		}
		iformat++;
	}

	*p_iformat = iformat;
	return returnalign;
}

int append_s(char* pdest, int* p_idest , char* psource, myprint_inf* ptype)  // retrun number of characters in the actual copy
{
	int isource = 0;
	int idest = *p_idest;

	while('\0' != psource[isource])
	{
		pdest[idest] = psource[isource];
		idest++;
		isource++;
	}

	// no response '\0'

	*p_idest = idest;

	return isource;
}

int append_c(char* pdest, int* p_idest , char inputchar, myprint_inf* ptype)
{
	int i = 0;
	char filling = ' ';
	int idest = *p_idest;

	if(1 < ptype->width)   // all fill ' '
	{
		if( 1 == ptype->fill )
		{
			filling = '0';
		}
		for(i=0; i < ptype->width; i++)
		{
			pdest[idest] = filling;
			idest++;
		}
		idest--;    // keep idest is last of widthsize
	}

	if(1 == ptype->align %2)  // fill inputchar   align left 
	{
		pdest[*p_idest] = inputchar;
	}
	else      // right
	{
		pdest[idest] = inputchar;
		idest++;
	}
	*p_idest = idest;

	return 0;
}

int append_d(char* pdest, int* p_idest , int inputint, myprint_inf* ptype) // retrun number of characters in the actual copy
{
	char temp[22];
	char paim[512];
	int len = 0;
	int i = 0;
	int j = 0;
	int idest = *p_idest;
	char prefix[10];
	
	// get prefix
	prefix[0] = 0;
	if(inputint < 0) 
	{
		prefix[0] = '-';
	}
	else
	{
		if((ptype->align%8) > 3)
		{
			prefix[0] = ' ';
		}
		if((ptype->align%4) > 1)
		{
			prefix[0] = '+';
		}
	}
	prefix[1] = 0;

	// get string 
	int_to_string(inputint,temp,10);
 
 	// deal width and accuracy
    //int_deal_acc_and_width(char* string, char* paim, myprint_inf* ptype);
  	
	len = deal_acc_and_width(temp, paim, ptype, prefix);
  	
  	//  cpoy
	for(i = 0; i<len; i++)
	{
		pdest[idest] = paim[i];
		idest++;
	}
	
	*p_idest = idest;

	return len;
}

int append_u(char* pdest, int* p_idest , unsigned int inputunint, myprint_inf* ptype)
{
	char temp[22];
	char paim[512];
	int len = 0;
	int i = 0;
	int j = 0;
	int idest = *p_idest;
	char sign = -1;
	char prefix[10];
	
	//get prefix
	prefix[0] = 0;
	if((ptype->align%8) > 3)
	{
		prefix[0] = ' ';
	}
	if((ptype->align%4) > 1)
	{
		prefix[0] = '+';
	}
	prefix[1] = 0;
	//************
	
	//get string
	int_to_string(inputunint,temp,10);
	//**********
	
	// deal width and accuracy
	len = deal_acc_and_width(temp, paim, ptype, prefix);
	//*************
	
	//copy
	for(i = 0; i<len; i++)
	{
		pdest[idest] = paim[i];
		idest++;
	}
	//***************
	*p_idest = idest;
	
	return len;
}

int append_x(char* pdest, int* p_idest , unsigned int inputox, myprint_inf* ptype)
{
	char temp[22];
	char paim[512];
	int idest = *p_idest; 
	int len = 0;
	int i = 0;
	int j = 0;
	
	len = append_X(paim, &i ,inputox, ptype);
	
	for(i=0; i<len; i++ ,idest++)
	{
		if(isalpha(paim[i]))
		{
			paim[i] = paim[i] + 32;
		}
		pdest[idest] = paim[i];
	}
	
	*p_idest = idest;
	
	return len;
}

int append_X(char* pdest, int* p_idest , unsigned int inputox, myprint_inf* ptype)
{
	char temp[22];
	char paim[512];
	int len = 0;
	int i = 0;
	int j = 0;
	int idest = *p_idest;
	char prefix[10];
	
	// get prefix
	ptype->align = ptype->align & (~6);
	// only need '#'
	prefix[0] = 0;
	if(8 <= ptype->align)
	{
		prefix[0] = '0';
		prefix[1] = 'X';
	}
	prefix[2] = 0;
	// get string 
	int_to_string(inputox,temp,16);
 
 	// deal width and accuracy
    //int_deal_acc_and_width(char* string, char* paim, myprint_inf* ptype);
  	
	len = deal_acc_and_width(temp, paim, ptype, prefix);
  	
  	//  cpoy
	for(i = 0; i<len; i++)
	{
		pdest[idest] = paim[i];
		idest++;
	}
	
	*p_idest = idest;

	return len;
}

int append_o(char* pdest, int* p_idest , unsigned int inputo, myprint_inf* ptype)
{
	char temp[22];
	char paim[512];
	int len = 0;
	int i = 0;
	int j = 0;
	int idest = *p_idest;
	char prefix[10];
	
	// get prefix
	// get prefix
	ptype->align = ptype->align & (~6);
	// only need '#'
	prefix[0] = 0;
	if(8 <= ptype->align)
	{
		prefix[0] = '0';
	}
	prefix[1] = 0;

	// get string 
	int_to_string(inputo,temp,10);
 
 	// deal width and accuracy
    //int_deal_acc_and_width(char* string, char* paim, myprint_inf* ptype);
  	
	len = deal_acc_and_width(temp, paim, ptype, prefix);
  	
  	//  cpoy
	for(i = 0; i<len; i++)
	{
		pdest[idest] = paim[i];
		idest++;
	}
	
	*p_idest = idest;

	return len;
}

int append_f(char* pdest, int* p_idest , double inputdouble, myprint_inf* ptype)
{
	char temp[512];
	char paim[512];
	int len = 0;
	int i = 0;
	int j = 0;
	int idest = *p_idest;
	int frlen = 0;
	char prefix[10];
	myprint_inf temp_type;
	
	//get prefix
	prefix[0] = 0;
	if(inputdouble < 0) 
	{
		prefix[0] = '-';
	}
	else
	{
		if((ptype->align%8) > 3)
		{
			prefix[0] = ' ';
		}
		if((ptype->align%4) > 1)
		{
			prefix[0] = '+';
		}
	}
	prefix[1] = 0;
	
	//****************
	
	//get string
	gcvt(inputdouble, 511, temp);
	len = strlen(temp);
	//***************
	
	//accuracy and width
	
	//copy Integral part
	for(i=0; temp[i] != '.' && temp[i] != '\0'; i++)
	{
		paim[i] = temp[i];
 	}
 	// --------
 	// copy fractional part
 	frlen = ((ptype->accuracy == -1)? 6 : ptype->accuracy);
 	if(frlen <= 0)
 	{
 		paim[i] = '\0';
 		i++;
	}
	else
	{
		for(j=0; temp[i] != '\0' && j <= frlen; i++,j++)   // 7 times because wo need copy '.'
		{
			paim[i] = temp[i];
		}
		if(temp[i] == '\0') // fractional number < accuracy
		{
			for( ;j < frlen;i++,j++ )
			{
				paim[i] = '0';
			}
		}
		else	// fractional number > accuracy
		{
			
		}	
		paim[i] = '\0';
	}
	// deal ptype
	temp_type.width = ptype->width;
	temp_type.align = ptype->align;
	temp_type.fill = ptype->fill;
	temp_type.accuracy = -1;
	
	
	len = deal_acc_and_width(paim, paim, &temp_type, prefix);
	//***********
	
	//copy
	for(i = 0; i<len; i++)
	{
		pdest[idest] = paim[i];
		idest++;
	}
	
	//************
	*p_idest = idest;
	
	return len;
}

int deal_acc_and_width(char* string, char* ptarget, myprint_inf* ptype, char* prefix)
{
	char *ptemp; 
	char paccu[512];
	char pwidth[512];
	char filling = ' ';
	int len = 0;
	int prefixlen = 0;
	int realaccu = 0;
	int realwidth = 0;
	int i = 0;
	int j = 0;
	
	if(string == NULL || ptarget == NULL || ptype == NULL || prefix == NULL)
	{
		return -1;
	}
	
	len = strlen(string);
	prefixlen = strlen(prefix);	
	// Prefix is independent of precision
	ptemp = paccu + prefixlen;
	// add filling
	if( len < ptype->accuracy)    // deal accuracy
	{	
		realaccu = ptype->accuracy - len;
		for(i = 0 ; i < realaccu; i++)
		{
			ptemp[i] = '0';
		}
		for(j = 0; j<len ;j++,i++)
		{
			ptemp[i] = string[j];
		}

		len = ptype->accuracy ; // updata
	}
	else
	{
		for(i=0; i < len; i++)
		{
			ptemp[i] = string[i];
		}
		len = len ;
	}
	ptemp[len] = '\0'; 
	// add prefix	
	if(prefixlen > 0)   
	{
		for(i=0; i<prefixlen; i++)
		{
			ptemp[i - prefixlen] = prefix[i];
		}
		// Go back to paccu
		ptemp = ptemp - prefixlen;
		len = len + prefixlen;
	}

	if(len < ptype->width)   // due width 
	{ 
		if(1 == ptype->fill && ptype->accuracy == 0 && ptype->align%2 == 0)  // help filling is '0' and no accuracy and no right 
		{
			filling = '0';
		}
		realwidth = ptype->width - len;
		if(1 == ptype->align % 2)  // left
		{
			for(i = 0; i < len; i++)
			{
				pwidth[i] = ptemp[i];
			}
			for(j = 0; j < realwidth; j++, i++)
			{
				pwidth[i] = filling;
			}
		}
		else   // right
		{
			for(i = 0; i<realwidth; i++)
			{
				pwidth[i] = filling;
			}
			for(j = 0; j<len; j++, i++)
			{
				pwidth[i] = ptemp[j];
			}
		}
		// updata
		len = len + realwidth;
		ptemp = pwidth;
	}
	
	ptemp[len] = '\0';
	for(i=0; i < len; i++)
	{
		ptarget[i] = ptemp[i];
	}
	
	return len;
}

int int_to_string(long int target, char* string, int hexadecimal)
{
	char *p1 = NULL,*p2 = NULL;
	char temp = 0;
	int i = 0;
	int set = 0;
	
	if(target < 0)
	{
		set = 1;
		target = -target;
	}

	if(string == NULL || hexadecimal == 0 )
	{
		return -1;
	}

	if(target == 0)
	{
		string[0] = '0';
		string[1] = '\0';
		return 0;
	}

	i=0;
	if(1 == set)
	{
		string[i] = '-';
		i++;
	}

	while(target!=0)
	{
		string[i++]="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"[target % hexadecimal];
	    target = target/hexadecimal;
	}
	

	string[i] = '\0';

	for(p1 = string + set,p2 = string + strlen(string) - 1 ; p2 > p1 ; p1++,p2--)
	{
	    temp = *p1;
	    *p1 = *p2;
	    *p2 = temp;
	} 

	return i;
}

int print_arg(int argc, char* arg[])
{
	int i = 0;
	if(NULL == arg)
	{
		return -1;
	}
	printf("argc : %d\n",argc);
	for(i = 0;arg[i] != NULL;i++)
	{
		printf("arg[%d] : \"%s\"\n",i,arg[i]);
	}
	printf("end\n");
}

int findnum_from_arg(int argc,char* arg[])   //int
{
	int i = 0;
	int inti = 0;

	for(i = 0; i < argc; i++)
	{
		if(isdigit(arg[i][0]))
		{
			inti = atoi(arg[i]);
			if(get_int_bit(inti) == strlen(arg[i]))
			{
				return i;
			}
		}
	}

	return -1;
}

int findchar_from_arg(int argc,char* arg[],char c)   // char
{
	int i = 0;

	for(i = 0 ;i < argc; i++)
	{
		if(1 == strlen(arg[i]))
		{
			if(c == arg[i][1])
			{
				return i;
			}	
		}
	}

	return -1;
}

int findstring_from_arg(int argc,char* arg[],char* string)	// string
{
	int i = 0;

	for(i = 0 ;i < argc; i++)
	{
		if(!strcmp(arg[i],string))
		{
			return i;	
		}
	}

	return -1;
}

int find_num_from_arg(int argc,char* arg[])   // -int
{
	int i = 0;
	int inti = 0;

	for(i = 0; i < argc; i++)
	{
		if('-' == arg[i][0])
		{
			if(isdigit(arg[i][1]))
			{
				inti = atoi(arg[i] + 1);
				if(get_int_bit(inti) == strlen(arg[i] + 1))
				{
					return i;
				}
			}
		}
	}

	return -1;
}

int find_char_from_arg(int argc,char* arg[],char c)	// -char
{
	
	int i = 0;

	for(i = 0 ;i < argc; i++)
	{
		if(2 == strlen(arg[i]))
		{
			if('-' == arg[i][0])
			{
				if(c == arg[i][1])
				{
					return i;
				}
			}
		}
	}

	return -1;
}

int find_string_from_arg(int argc,char* arg[],char* string)	// -string
{
	int i = 0;

	for(i = 0 ;i < argc; i++)
	{
		if('-' == arg[i][0])
		{
			if(!strcmp(arg[i] + 1,string))
			{
				return i;	
			}
		}
	}

	return -1;
}

int append_e()
{
	return 0;
}
int append_E()
{
	return 0;
}
int append_g()
{
	return 0;
}

int append_l()
{
	return 0;
}
int append_lf()    // sizeof (long double) = 16
{
	return 0;
}