/*
* Copyright (c) Jack M. Thompson WebKruncher.com, exexml.com
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name of the WebKruncher nor the
*       names of its contributors may be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY Jack M. Thompson ``AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL Jack M. Thompson BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#ifndef JTOOLS_DIR_H
#define JTOOLS_DIR_H

#include <syslog.h> 
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <regex.h>

namespace JDirTools
{
	inline ostream& coma(ostream& o){o<<",";return o;}


	struct icstringset : set<icstring>  {};

	#define whitespaces ((char*)" \f\v\r\n\t")
	inline icstring trims(icstring s,const char* ws=whitespaces)
	{
		int fs=s.find_first_not_of(ws);
		if ((fs!=icstring::npos) && (fs>0)) s.erase(0,fs);
		int lc=s.find_last_not_of(ws);
		if (lc==icstring::npos) return s;
		lc++; 
		if (lc<s.size()) {s.erase(lc,s.size()-lc);}
		return s;
	}
	inline string trims(string s,const char* ws=whitespaces)
	{
		int fs=s.find_first_not_of(ws);
		if ((fs!=string::npos) && (fs>0)) s.erase(0,fs);
		int lc=s.find_last_not_of(ws);
		if (lc==string::npos) return s;
		lc++; 
		if (lc<s.size()) {s.erase(lc,s.size()-lc);}
		return s;
	}


	inline string tolower(string s) { std::transform(s.begin(), s.end(), s.begin(), ::tolower); return s; }
	inline icstring tolower(icstring s) { std::transform(s.begin(), s.end(), s.begin(), ::tolower); return s; }


	struct stringdeque : deque<string> 
	{
		void split(string& s,string delim)
		{
			while(s.size())
			{
				size_t d=s.find_first_of(delim);
				if (d==string::npos)
				{
					push_back(s);
					return;
				} else {
					string tmp; tmp.assign(s,0,d);
					d+=1;
					s.erase(0,d);
					push_back(tmp);
				}
			}
		}
		friend ostream& operator<<(ostream&,const stringdeque&);
		ostream& operator<<(ostream& o) const
		{
			for (const_iterator it=begin();it!=end();it++) 
				o<<(*it)<<endl;
			return o;
		}
		//stringmap& operator=(stringmap& m) { return *this; }
	};
	inline ostream& operator<<(ostream& o,const stringdeque& m){return m.operator<<(o);}
	struct stringlongmap : map<string,unsigned long> 
	{
		friend ostream& operator<<(ostream&,const stringlongmap&);
		private:
		ostream& operator<<(ostream& o) const
		{
			for (const_iterator it=begin();it!=end();it++) o<<it->first<<"="<<it->second<<endl;
			return o;
		}
	};
	inline ostream& operator<<(ostream& o,const stringlongmap& m){return m.operator<<(o);}
	struct stringmap : map<string,string>
	{
		friend ostream& operator<<(ostream&,const stringmap&);
		virtual ostream& operator<<(ostream& o) const
		{
			for (const_iterator it=begin();it!=end();it++) 
				o<<it->first<<":"<<it->second<<endl;
			return o;
		}
		//stringmap& operator=(stringmap& m) { return *this; }
	};
	inline ostream& operator<<(ostream& o,const stringmap& m){return m.operator<<(o);}

	struct stringpair : pair<string,string>  
	{
		stringpair(const string a,const string b) : pair<string,string>(a,b) {}
		stringpair(const stringpair& other) : pair<string,string>(other) { }
	};
	struct stringset : set<string>  {};

	struct longvector : vector<unsigned long> 
	{
		private:
		friend ostream& operator<<(ostream&,const longvector&);
		virtual ostream& operator<<(ostream& o) const
		{
			for (const_iterator it=begin();it!=end();it++)
				{ if (it!=begin()) o<<", "; o<<(*it); }
			return o;
		}
	};
	inline ostream& operator<<(ostream& o,const longvector& s){return s.operator<<(o);}


	template <typename ST>
		struct DirScanner
	{
		typedef ST ScannerType ;
		DirScanner(const string _root,const int _depth=0) : root(_root),depth(_depth) {}
		virtual ~DirScanner(){}
		virtual const bool file(const string root,const dirent& dp,const struct stat& st) = 0;
		virtual operator const bool ()
		{
cerr << "root:"<<root<<endl;
			DIR *dir(opendir(root.c_str()));
			if (!dir) return false;
			struct dirent *dp(NULL);
			vector<string> subs;
			do
			{
				if ((dp=readdir(dir)) != NULL) 
				{
					string name(dp->d_name);
					stringstream ssp; ssp<<root<<name;
					if (name==".") continue;
					if (name=="..") continue;
					struct stat st;
					int sret(stat(ssp.str().c_str(),&st)); 
					if (sret) continue;
cerr << ssp.str() << endl;
					if(S_ISDIR(S_IFDIR&st.st_mode)) subs.push_back(ssp.str().c_str());
					else if (!file(root,*dp,st)) return false;
	
				}
			} while (dp);
			if (dir) closedir(dir);
			for (vector<string>::iterator it=subs.begin();it!=subs.end();it++)
			{
				const string name(*it);
				stringstream ss; ss<<name<<"/";
				auto_ptr<ST> sub; sub.reset(Generate(ss.str(),depth+1));
				if (!sub.get()) throw(name);
				ST& sb(*sub.get());
				if (!sb) return false;
			}
			return true;
		}
		private:
		virtual ScannerType* Generate(const string _root,const int _depth) = 0;
		protected:
		const string root;
		const int depth;
	};

	struct Matcher
	{
		Matcher() : compiled(false){}
		Matcher(const string _exp) : exp(_exp) , compiled(false){}
		Matcher(const Matcher& m) : exp(m.exp) , compiled(false){}
		~Matcher() { if (compiled) regfree(&regex); }
		const bool operator == (const string what)
		{
			if (!compiled) compiled=!regcomp(&regex, exp.c_str(), REG_ICASE|REG_EXTENDED); if (!compiled) throw exp; 
			int notmatched(regexec(&regex, what.c_str(), 0, NULL, 0));
			if (!notmatched) return true;
			if( notmatched==REG_NOMATCH ) return false;
			throw what;
		}
		void operator = (const string _exp) {exp=_exp;}
		private:
		mutable string exp;
		bool compiled;
		regex_t regex;
		friend ostream& operator<<(ostream&,const Matcher&);
		ostream& operator<<(ostream& o) const
		{
			o<<exp;
			return o;
		}
	};
	inline ostream& operator<<(ostream& o,const Matcher& m){return m.operator<<(o);}


	struct FileScanner : DirScanner<FileScanner>
	{
		FileScanner( const string _root,const string _what, const string _where,const int _depth=0) :
			DirScanner<FileScanner>(_root,_depth) {}
		const bool file(const string root,const dirent& dp,const struct stat& st)
		{
			cout << "." << endl;
			return true;
		}
		virtual ScannerType* Generate(const string _root,const int _depth) 
		{
			return new FileScanner( _root, what, where, _depth );
		}
		private:
	
		const string 	what,where;
	};


} // JDirTools


#endif // JTOOLS_DIR_H

