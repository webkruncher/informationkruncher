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

#ifndef PRETTY_HEADERS_H
#define PRETTY_HEADERS_H

#include <iomanip>
namespace OFormat
{
	using namespace std;
	inline ostream& wspce(ostream& o){o<<" ";return o;}
	inline ostream& tab(ostream& o){o<<"\t";return o;}
	inline ostream& coma(ostream& o){o<<",";return o;}
	inline ostream& coln(ostream& o){o<<":";return o;}
	inline ostream& fence(ostream& o){o<<"|";return o;}
	inline ostream& squo(ostream& o){o<<"'";return o;}
	inline ostream& quo(ostream& o){o<<"\"";return o;}
	inline ostream& tick(ostream& o){o<<"`";return o;}
	inline ostream& black(ostream& o){o<<"\033[30m";return o;}
	inline ostream& red(ostream& o){o<<"\033[31m";return o;}
	inline ostream& green(ostream& o){o<<"\033[32m";return o;}
	inline ostream& yellow(ostream& o){o<<"\033[33m";return o;}
	inline ostream& blue(ostream& o){o<<"\033[34m";return o;}
	inline ostream& greenbk(ostream& o){o<<"\033[42m";return o;}
	inline ostream& yellowbk(ostream& o){o<<"\033[43m";return o;}
	inline ostream& bluebk(ostream& o){o<<"\033[44m";return o;}
	inline ostream& normal(ostream& o){o<<"\033[0m";return o;}
	inline ostream& flsh(ostream& o){o.flush();return o;}
	inline ostream& brktin(ostream& o){o<<"[";return o;}
	inline ostream& brktout(ostream& o){o<<"]";return o;}
	inline ostream& brcin(ostream& o){o<<"{";return o;}
	inline ostream& brcout(ostream& o){o<<"}";return o;}
	inline ostream& aglin(ostream& o){o<<"<";return o;}
	inline ostream& aglout(ostream& o){o<<">";return o;}
	inline ostream& crgret(ostream& o){o<<"\r";return o;}
	inline ostream& cleol(ostream& o){o<<"\033[K";return o;}
	
	inline ostream& logerror(ostream& o){o<<"[ERROR]";return o;}


	struct oformat : ostream
	{
		oformat() : o(cerr) {}
		oformat(ostream& _o) : o(_o) {tie(&o);}
		virtual oformat& operator<<(const char* c){o<<c; return (*this);}
		virtual oformat& operator<<(string& c){o<<c; return (*this);}
		virtual oformat& operator<<(double d)
		{
			const string s( (d<0)?"-":"+");
			if (d<0) d*=-1;
			o<<s<<setfill(' ')<<setw(8)<<d;
			return (*this);
		}
		private:
		stringstream dump;
		friend oformat& operator<<(oformat&,ostream& (*pf)(ostream&) );
		friend ostream& operator<<(ostream&, const oformat&);
		virtual ostream& operator<<(ostream& o) const { o << "[ugly]"; return o; }
		ostream& o;
	};
	inline oformat& operator<<(oformat& p,ostream& (*pf)(ostream&) ) {(*pf)(p.o); return p;}
	inline ostream& operator<<(ostream& o, const oformat& p) {return p.operator<<(o);}

} // OFormat

#endif // PRETTY_HEADERS_H

