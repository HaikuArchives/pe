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

	Created: 09/18/97 20:36:58
*/

#include "pe.h"

#include "HError.h"
#include <fs_attr.h>
#include "PApp.h"
#include "CKeywords.h"


struct kws {
	int accept;
	unsigned short trans[128];
	
	kws();
	void PrintToStream();
};

typedef map<int, kws> kwm;

void AddKeyWord(kwm& dfa, unsigned char *nkw, int& nextState, int a);
void CompressDFA(kwm& dfa, vector<int>& accept, unsigned char ec[],
	vector<int>& base, vector<int>& nxt, vector<int>& chk, bool used[]);
void RemapChars(kwm& dfa, unsigned char *map);
void PrintDFA(kwm& dfa);
void GenerateKWTables(FILE *file,
	vector<int>& accept, unsigned char ec[], vector<int>& base,
	vector<int>& nxt, vector<int>& chk);
bool GetTablesFromAttributes(const char *file, unsigned char ec[],
	unsigned short *& accept, unsigned short *& base,
	unsigned short *& nxt, unsigned short *& chk);
void WriteTablesToAttributes(const char *file,
	vector<int>& accept, unsigned char ec[], vector<int>& base,
	vector<int>& nxt, vector<int>& chk);

void AddKeyWord(kwm& dfa, unsigned char *nkw, int& nextState, int a)
{
	int state = 1, laststate = 1;
	unsigned char *nkwi = nkw;
	
	while (*nkwi)
	{
		laststate = state;
		state = dfa[state].trans[*nkwi];
		if (!state)
			break;
		nkwi++;
	}
	
	if (state)
	{
		if (dfa[state].accept == 0)
			dfa[state].accept = a;
	}
	else
	{
		state = laststate;
		
		while (*nkwi)
		{
//			dfa[state].trans[*nkwi] = state = nextState;	fucking compiler....

			kws& ks = dfa[state];
			state = nextState++;
			ks.trans[*nkwi] = state;
			nkwi++;
		}

		dfa[state].accept = a;
	}
} /* AddKeyWord */

void CompressDFA(kwm& dfa, vector<int>& accept, unsigned char ec[],
	vector<int>& base, vector<int>& nxt, vector<int>& chk, bool used[])
{
	int i, j, k = 0, b, s;
	unsigned short *t;
	
	for (i = 0; i < 128; i++)
		ec[i] = used[i] ? ++k : 0;

	RemapChars(dfa, ec);

	b = k + 1;

	accept.reserve(dfa.size());
	accept.push_back(0);

	nxt = vector<int>(b);
	chk = vector<int>(b);
	base = vector<int>(dfa.size() + 1);
	
	kwm::iterator ki;
	s = 1;
	for (ki = dfa.begin(); ki != dfa.end(); ki++, s++)
	{
		accept.push_back((*ki).second.accept);
		t = (*ki).second.trans;
		
		bool ok = false;
		
		while (!ok)
		{
			for (i = 0; i < b; i++)
			{
				ok = true;
				
				for (j = 1; j <= k; j++)
				{
					if ((i + j >= b) || t[j] && nxt[i + j])
					{
						ok = false;
						break;
					}
				}
				
				if (ok)
				{
					for (j = 1; j <= k; j++)
					{
						if (t[j])
						{
							nxt[i + j] = t[j];
							chk[i + j] = s;
						}
					}
					base[s] = i;
					break;
				}
			}
			
			if (!ok)
			{
				b++;
				nxt.push_back(0);
				chk.push_back(0);
			}
		}
	}
	
//	printf("\naccept:\n");
//	for (i = 0; i < accept.size(); i++)
//		printf("%d ", accept[i]);
//	
//	printf("\nec:\n");
//	for (i = 0; i < 128; i++)
//		printf("%d ", ec[i]);
//	
//	printf("\nbase:\n");
//	for (i = 0; i < base.size(); i++)
//		printf("%d ", base[i]);
//	
//	printf("\nnxt:\n");
//	for (i = 0; i < nxt.size(); i++)
//		printf("%d ", nxt[i]);
//	
//	printf("\nchk:\n");
//	for (i = 0; i < chk.size(); i++)
//		printf("%d ", chk[i]);
//
//	puts("");
} /* CompressDFA */

void RemapChars(kwm& dfa, unsigned char *ec)
{
	kwm::iterator ki;
	
	for (ki = dfa.begin(); ki != dfa.end(); ki++)
	{
		kws a, b;
		
		a = (*ki).second;
		
		b.accept = a.accept;
		for (int i = 0; i < 128; i++)
			b.trans[ec[i]] = a.trans[i];
		
		(*ki).second = b;
	}
} /* RemapChars */

kws::kws()
{
	accept = 0;
	memset(trans, 0, 256);
} /* kws::kws */

void kws::PrintToStream()
{
	printf("%s state\n", accept ? "n accepting" : " non-accepting");
	
	for (int i = 0; i < 128; i++)
	{
		if (trans[i])
			printf("\ton %c move to state %d\n", i, trans[i]);
	}
} /* kws::PrintToStream */

void PrintDFA(kwm& dfa)
{
	kwm::iterator ki;
	
	for (ki = dfa.begin(); ki != dfa.end(); ki++)
	{
		printf("state %d is a", (*ki).first);
		(*ki).second.PrintToStream();
	}
} /* PrintDFA */

void GenerateKWTables(FILE *f,
	vector<int>& accept, unsigned char ec[], vector<int>& base,
	vector<int>& nxt, vector<int>& chk)
{
	bool used[128];
	int nextState = 2, set = 1, i;
	kwm dfa;
	char s[2048];
	
	for (i = 0; i < 128; i++)
		used[i] = false;

	while (fgets(s, 2047, f))
	{
		char *p;
		
		if (s[0] == '-')
		{
			set++;
			continue;
		}

		p = strtok(s, " \n\r");
		while (p)
		{
			for (i = 0; p[i]; i++)
			{
				if (p[i] < 0)
				{
					p = NULL;
					break;
				}
				used[p[i]] = true;
			}

			if (p)
				AddKeyWord(dfa, (unsigned char *)p, nextState, set);
			
//			dfa[1].PrintToStream();
			
			p = strtok(NULL, " \n\r");
		}
	}
	
//	PrintDFA(dfa);

	CompressDFA(dfa, accept, ec, base, nxt, chk, used);
} /* GenerateKWTables */

bool GetTablesFromAttributes(const char *file, unsigned char ec[],
	unsigned short *& accept, unsigned short *& base,
	unsigned short *& nxt, unsigned short *& chk)
{
	bool result = true;
	
	try
	{
		BFile kwf;
		time_t modified, stored;
		
		FailOSErr(kwf.SetTo(file, B_READ_ONLY));
		
		FailOSErr(kwf.GetModificationTime(&modified));
		if (kwf.ReadAttr("prebuild at", 'time', 0, &stored, sizeof(stored)) == 0)
			stored = 0;

		attr_info ai;

		if (modified != stored)
			result = false;
		else
		{
			FailOSErr(kwf.GetAttrInfo("accept", &ai));
			accept = new unsigned short[(int)(ai.size) / sizeof(unsigned short)];
			FailNil(accept);
			if (kwf.ReadAttr("accept", 'tabl', 0, accept, ai.size) != ai.size)
				THROW(("io error"));
			
			FailOSErr(kwf.GetAttrInfo("base", &ai));
			base = new unsigned short[(int)(ai.size) / sizeof(unsigned short)];
			FailNil(base);
			if (kwf.ReadAttr("base", 'tabl', 0, base, ai.size) != ai.size)
				THROW(("io error"));
			
			FailOSErr(kwf.GetAttrInfo("chk", &ai));
			chk = new unsigned short[(int)(ai.size) / sizeof(unsigned short)];
			FailNil(chk);
			if (kwf.ReadAttr("chk", 'tabl', 0, chk, ai.size) != ai.size)
				THROW(("io error"));
			
			FailOSErr(kwf.GetAttrInfo("nxt", &ai));
			nxt = new unsigned short[(int)(ai.size) / sizeof(unsigned short)];
			FailNil(nxt);
			if (kwf.ReadAttr("nxt", 'tabl', 0, nxt, ai.size) != ai.size)
				THROW(("io error"));
			
			if (kwf.ReadAttr("ec", 'tabl', 0, ec, 128) != 128)
				THROW(("io error"));
		}
	}
	catch (HErr& err)
	{
		result = false;
	}
	
	return result;
} /* GetTablesFromAttributes */

void WriteTablesToAttributes(const char *file,
	vector<int>& accept, unsigned char ec[], vector<int>& base,
	vector<int>& nxt, vector<int>& chk)
{
	time_t modified = 0;

	try
	{
		BFile kwf;
		
		FailOSErr(kwf.SetTo(file, B_READ_WRITE));
		if (kwf.WriteAttr("ec", 'tabl', 0, ec, 128) != 128)
			THROW(("io error"));
	
		unsigned short *tbl;
	
		tbl = new unsigned short[accept.size()];
		copy(accept.begin(), accept.end(), tbl);
		kwf.WriteAttr("accept", 'tabl', 0, tbl,
			accept.size() * sizeof(unsigned short));
		delete[] tbl;
		
		tbl = new unsigned short[base.size()];
		copy(base.begin(), base.end(), tbl);
		kwf.WriteAttr("base", 'tabl', 0, tbl,
			base.size() * sizeof(unsigned short));
		delete[] tbl;
		
		tbl = new unsigned short[nxt.size()];
		copy(nxt.begin(), nxt.end(), tbl);
		kwf.WriteAttr("nxt", 'tabl', 0, tbl,
			nxt.size() * sizeof(unsigned short));
		delete[] tbl;
		
		tbl = new unsigned short[chk.size()];
		copy(chk.begin(), chk.end(), tbl);
		kwf.WriteAttr("chk", 'tabl', 0, tbl,
			chk.size() * sizeof(unsigned short));
		delete[] tbl;
		
		time(&modified);
		kwf.WriteAttr("prebuild at", 'time', 0, &modified, sizeof(modified));
	}
	catch (HErr& err)
	{
		err.DoError();
	}
	
	if (modified)
		BNode(file).SetModificationTime(modified);
} /* WriteTablesToAttributes */

void GenerateKWTables(const char *file, const char *ext,
	unsigned char ec[],
	unsigned short *& accept, unsigned short *& base,
	unsigned short *& nxt, unsigned short *& chk)
{
	try
	{
		BPath settings;
		FILE *f;
		bool isNew = false;
		
		FailOSErr(find_directory(B_USER_SETTINGS_DIRECTORY, &settings, true));
	
		char p[PATH_MAX];
		strcpy(p, settings.Path());
		strcat(p, "/pe/");
		strcat(p, file);
	
		BEntry e;
		FailOSErrMsg(e.SetTo(p, B_FILE_NODE), "Settings directory was not found?");

		if (!e.Exists())
		{
			isNew = true;
			f = fopen(p, "w");
			
			BFile rf;
			FailOSErr(rf.SetTo(ext, B_READ_ONLY));
			BResources res;
			FailOSErr(res.SetTo(&rf));
			
			size_t s;
			char *r = (char *)res.FindResource('KeyW', file, &s);
			
			if (!r) THROW(("Missing resource"));
			
			fwrite(r, 1, s, f);
			fclose(f);
			free(r);
		}
		else if (!GetTablesFromAttributes(p, ec, accept, base, nxt, chk))
			isNew = true;

		if (isNew)
		{
			f = fopen(p, "r");
	
			vector<int> yy_accept, yy_base, yy_nxt, yy_chk;
			
			if (f)
			{
				GenerateKWTables(f, yy_accept, ec, yy_base, yy_nxt, yy_chk);
				fclose(f);
			}
			else
				memset(ec, 0, 128);

			accept = new unsigned short[yy_accept.size()];
			copy(yy_accept.begin(), yy_accept.end(), accept);
			
			base = new unsigned short[yy_base.size()];
			copy(yy_base.begin(), yy_base.end(), base);
			
			nxt = new unsigned short[yy_nxt.size()];
			copy(yy_nxt.begin(), yy_nxt.end(), nxt);
			
			chk = new unsigned short[yy_chk.size()];
			copy(yy_chk.begin(), yy_chk.end(), chk);
			
			fclose(f);
			
			WriteTablesToAttributes(p, yy_accept, ec, yy_base, yy_nxt, yy_chk);
		}
	}
	catch (HErr& err)
	{
		err.DoError();
	}
} /* GenerateKWTables */

