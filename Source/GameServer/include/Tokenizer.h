#pragma once

#include <map>
#include <list>
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>

using namespace std;

/*
	Programado por WoLf
*/

class TokenizerRow
{
public:
	map<DWORD, string>	Columns;
	int	ColumnCount;

	string GetString(DWORD Column, string Default = "")
	{
		map<DWORD, string>::iterator it = this->Columns.find(Column);
		if(it == this->Columns.end())
		{
			return Default;
		}
		return it->second;
	}

	char* GetStringPtr(DWORD Column, string Default = "")
	{
		map<DWORD, string>::iterator it = this->Columns.find(Column);
		if(it == this->Columns.end())
		{
			return (char*)Default.c_str();
		}
		return (char*)it->second.c_str();
	}

	int GetInt(DWORD Column, DWORD Default = -1)
	{
		map<DWORD, string>::iterator it = this->Columns.find(Column);
		if(it == this->Columns.end())
		{
			return Default;
		}
		return atoi(it->second.c_str());
	}

	double GetFloat(DWORD Column, double Default = 0.0f)
	{
		map<DWORD, string>::iterator it = this->Columns.find(Column);
		if(it == this->Columns.end())
		{
			return Default;
		}
		return atof(it->second.c_str());
	}

};

class TokenizerSection
{
public:
	map<DWORD, TokenizerRow*>	Rows;
	int							RowCount;
};

class TokenizerGroup
{
public:
	map<DWORD, TokenizerSection*>	Sections;

	TokenizerSection* GetSection(DWORD Index)
	{
		map<DWORD, TokenizerSection*>::iterator it = this->Sections.find(Index);
		if(it == this->Sections.end())
		{
			return NULL;
		}
		else
		{
			return it->second;
		}
	}

};

class Tokenizer
{
private:
	char*	m_pBuffer;
	DWORD	m_pBufferSize;
	DWORD	m_pBufferIndex;

public:

	TokenizerRow* ParseLine(string line)
	{

		string data = "";

		char* dump = (char*)line.c_str();
		
		bool openstring = false;
		bool clearingspace = true;

		TokenizerRow* pRow = new TokenizerRow();
		
		int column = 0;

		for(unsigned int i = 0; i < line.length(); i++)
		{

			if(clearingspace)
			{
				if(dump[i] == ' ' || dump[i] == '\t')
				{
					continue;
				}
				clearingspace = false;
			}

			if(openstring)
			{
				if(dump[i] == '"')
				{
					openstring = false;
					continue;
				}
				data += dump[i];
			}
			else
			{
				if(dump[i] == '"')
				{
					if(data != "")
					{
						return NULL;
					}
					openstring = true;
					continue;
				}
				else
				{
					if(dump[i] == '\t' || dump[i] == ' ')
					{
						if(data != "")
						{
							pRow->Columns[column++] = data;
							data = "";
						}
						continue;
					}   
					data += dump[i];
				}
			}

		}

		if(data != "")
		{
			pRow->Columns[column++] = data;
		}

		data = "";

		pRow->ColumnCount = column;

		return pRow;

	}

	TokenizerGroup* ParseFile(string file)
	{

		fstream f(file.c_str(), ios::in);
		TokenizerGroup* tok = new TokenizerGroup();

		if(f.is_open())
		{

			if(f.good())
			{

				TokenizerSection* sec = NULL;
				int current_sec = 0;
				int sec_index = 0;
				bool sec_open = false;

				while(!f.eof())
				{

					char temp[4096];
					char* dump = NULL;
					string line = "";

					ZeroMemory(&temp[0], 4096);

					f.getline(&temp[0], 4095);
					line.assign(&temp[0]);

					dump = (char*)line.c_str();

					int start = 0;
					int end = 0;

					for(DWORD i = 0; i < line.length(); i++)
					{
						if(dump[i] != ' ' && dump[i] != '\t')
						{
							break;
						}
						start++;
					}

					for(DWORD i = line.length()-1; i >= 0; i--)
					{
						if(dump[i] != ' ' && dump[i] != '\t')
						{
							break;
						}
						end++;
					}

					line = line.substr(start, line.length() - end - start);	

					if(line.substr(0, 2) == "//") continue;
					if(line[0] == '#') continue;

					if(line == "end")
					{
						if(sec_open == false) 
						{
							return NULL; // falha de sintaxe
						}
						sec_open = false;
						sec->RowCount = sec_index;
						tok->Sections[current_sec] = sec;
						continue;
					}
					
					if(line == "") continue;

					TokenizerRow* row = this->ParseLine(line);
					if(row->ColumnCount == 1)
					{
						if(sec_open == false)
						{
							sec_index = 0;
							current_sec = row->GetInt(0, 0);
							sec = new TokenizerSection();
							sec_open = true;
							continue;
						}
					}

					sec->Rows[sec_index++] = row;

				}

			}
			else
			{
				f.close();
				return NULL;
			}
			
			f.close();

			return tok;

		}
		else
		{
			return NULL;
		}

	}

};