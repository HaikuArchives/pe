/*	$Id$
	
	Copyright 1996, 1997, 1998, 2002
	        Hekkelman Programmatuur B.V.  All rights reserved.
	
	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions are met:
	1. Redistributions of source code must retain the above copyright notice,
	   this list of conditions and the following disclaimer.
	2. Redistributions in binary form must reproduce the above copyright notice,
	   this list of conditions and the following disclaimer in the documentation
	   and/or other materials provided with the distribution.
	3. All advertising materials mentioning features or use of this software
	   must display the following acknowledgement:
	   
	    This product includes software developed by Hekkelman Programmatuur B.V.
	
	4. The name of Hekkelman Programmatuur B.V. may not be used to endorse or
	   promote products derived from this software without specific prior
	   written permission.
	
	THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES,
	INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
	FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
	AUTHORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
	EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
	PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
	OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
	WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
	OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
	ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 	

	Created: 12/07/97 22:01:11 by Maarten Hekkelman
	Modified from CLang_cpp.cpp for pascal by Kelvin W Sherlock
	Merged with BiPolar's CLang_pascal.cpp based on m7m's "Pe on steroids" CLang_cpp.cpp
*/

#include "CLanguageAddOn.h"
#include <stack>
#include <ctype.h>

extern "C" {
_EXPORT const char kLanguageName[] = "Pascal";
_EXPORT const char kLanguageExtensions[] = "pas;pp;inc";
_EXPORT const char kLanguageCommentStart[] = "//";
_EXPORT const char kLanguageCommentEnd[] = "";
_EXPORT const char kLanguageKeywordFile[] = "keywords.pas";
}

enum {
	START, 
	IDENT, 
	COMMENT1, 		// { ... }
	COMMENT2, 		// (* ... *)
	STRING,
	LCOMMENT, 
	NUMERIC, 
	OPERATOR, 
	SYMBOL, 
	COMPILERDIRECTIVE,
};

#define GETCHAR			(c = (i++ < size) ? text[i - 1] : 0)

bool isOperator(char c)
{
	if (c == '+' || c == '-' || c == '*' || c == '/' || c == ':' || c == '=' ||
		c == '<' || c == '>' || c == '@' || c == '^')
		return true;
			
	return false;
}

bool isSymbol(char c)
{
	if (c == '(' || c == ')' || c == '[' || c == ']' || c == '&' || 
		c == '.' || c == ',' || c == ';' || c == '$' || c == '#')
		return true;
	
	return false;
}

bool isNumeric(char c)
{
	if (c >= '0' && c <= '9')
		return true;

	return false;
}

bool isHexNum(char c)
{
	if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))
		return true;

	return false;
}

_EXPORT void ColorLine(CLanguageProxy& proxy, int& state)
{
	const char *text = proxy.Text();
	int size = proxy.Size();
	int i = 0, s = 0, kws = 0, esc = 0;
	char c;
	bool leave = false;
	bool floating_point = false;
	bool hex_num = false;

	if (state == COMMENT1 || state == COMMENT2 || state == LCOMMENT)
		proxy.SetColor(0, kLCommentColor);
	else
		proxy.SetColor(0, kLTextColor);
	
	if (size <= 0)
		return;
	
	while (!leave)
	{
		GETCHAR;
		
		switch (state) {
			case START:

				if (isalpha(c) || c == '_')
				{
					kws = proxy.Move(tolower(c), 1);
					state = IDENT;
				}
				else if (c == '{')
				{
					if ((text[i] == '$') && isalpha(text[i+1]))
					{
						kws = proxy.Move(c, 1);
						state = COMPILERDIRECTIVE;
					}
					else
					{
						state = COMMENT1;	
					}
				}
				else if (c == '(' && text[i] == '*')
				{
					i++;
					state = COMMENT2;
				}
				else if (c == '/' && text[i] == '/')
				{
					i++;
					state = LCOMMENT;
				}
				else if (c == '\'')
					state = STRING;

				else if (isNumeric(c) || (c == '$' && isHexNum(text[i])))
				{
					state = NUMERIC;	
				}
				else if (isOperator(c))
				{
					state = OPERATOR;	
				}
				else if (isSymbol(c))
				{
					state = SYMBOL;
				}
				else if (c == '\n' || c == 0)
					leave = true;
					
				if (leave || (state != START && s < i))
				{
					proxy.SetColor(s, kLTextColor);
					s = i - 1;
				}
				break;
			
			// {. .. } format comments
			case COMMENT1:
				if (c == '}')
				{
					proxy.SetColor(s, kLCommentColor);
					s = i;
					state = START;
				}
				else if (c == '\n' || c == 0)
				{
					proxy.SetColor(s, kLCommentColor);
					leave = true;	
				}
				break;
			// (* ... *) format comments
			case COMMENT2:
				if ((s == 0 || i > s + 1) && c == '*' && text[i] == ')')
				{
					proxy.SetColor(s - 1, kLCommentColor);
					s = i + 1;
					state = START;
				}
				else if (c == 0 || c == '\n')
				{
					proxy.SetColor(s - 1, kLCommentColor);
					leave = true;
				}
				break;
			// // format comments
			case LCOMMENT:
				proxy.SetColor(s - 1, kLCommentColor);
				leave = true;
				if (text[size - 1] == '\n')
					state = START;
				break;

			
			case IDENT:
				if (!isalnum(c) && c != '_')
				{
					int kwc;

					if (i > s + 1 && (kwc = proxy.IsKeyWord(kws)) != 0)
					{
						switch (kwc)
						{
							case 1:	proxy.SetColor(s, kLKeyWordColor); break;
							case 2:	proxy.SetColor(s, kLUser1); break;
							case 3:	proxy.SetColor(s, kLUser2); break;
							case 4:	proxy.SetColor(s, kLUser3); break;
							case 5:	proxy.SetColor(s, kLUser4); break;
//							default:	ASSERT(false);
						}
						s = --i;
						state = START;
					}
					else if (c != '.')
					{
						proxy.SetColor(s, kLTextColor);
						s = --i;
						state = START;
					}
					
				}
				else if (kws)
					kws = proxy.Move((int)(unsigned char)tolower(c), kws);
				break;
			
			case COMPILERDIRECTIVE:
				if (c == '}')
				{
					proxy.SetColor(s, kLCharConstColor);
					s = i;
					state = START;
				}
				else if (c == '\n' || c == 0)
				{
					proxy.SetColor(s, kLCharConstColor);
					leave = true;
				}
				break;

			
			// ' ... ' '' indicates is how we escape a single quote, so we can ignore it.
			case STRING:
				if (c == '\'' && !esc)
				{
					proxy.SetColor(s, kLStringColor);
					s = i;
					state = START;			
				}
				else if (c == '\n' || c == 0)
				{
					if (text[i - 2] == '\\' && text[i - 3] != '\\')
					{
						proxy.SetColor(s, kLStringColor);
					}
					else
					{
						proxy.SetColor(s, kLTextColor);
						state = START;
					}
							
					s = size;
					leave = true;	
				}
				else
				{
					esc = !esc && (c == '\\');
				}
				break;
			
			case NUMERIC:
				proxy.SetColor(s, kLNumberColor);
				if (isNumeric(text[i - 1]) || (hex_num && isHexNum(text[i - 1])))
					;
				else
					if (text[i - 1] == '.' && floating_point == false)
						floating_point = true;
					else if (isHexNum(text[i - 1]) && hex_num == false)
						hex_num = true;
					else
					{
						s = i - 1;
						i--;
						state = START;
					}
				break;
		
			case OPERATOR:
				proxy.SetColor(s, kLOperatorColor);
				if (isOperator(text[i - 1]))
					;
				else
				{
					s = i - 1;
					i--;
					state = START;
				}
				break;
		
			case SYMBOL:
				proxy.SetColor(s, kLSeparatorColor);
				if (isSymbol(text[i - 1]))
					;
				else
				{
					s = i - 1;
					i--;
					state = START;
				}
				break;			
			
			default:	// error condition, gracefully leave the loop
				leave = true;
				break;
		}
	}
} /* ColorLine */
