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
#ifndef HYPER_H
#define HYPER_H

namespace Hyper
{ 
	struct Response : oformat
	{
		Response() : oformat(cerr) {}
		virtual void operator ()() = 0;
	};

	struct Request : oformat
	{
		Request(const icstring& _request, const icstringvector& _headers, Socket& _sock, unique_ptr<Response>& _response ) :
			request(_request), headers(_headers), sock(_sock), response( _response ) {}
		const char* c_str() const {return request.c_str();}
		operator Socket& () const {return sock;}
		string Host() const { return host; }
		string Headers()
		{
			stringstream mout;
			for (icstringvector::const_iterator it=headers.begin();it!=headers.end();it++)
					mout << NoBreaks((*it).c_str()) << fence; 
			return mout.str();
		}
		bool resolve() 
		{
			//stringstream ssr; ssr<< "mime."<<pthread_self()<<".txt"; ofstream mout(ssr.str().c_str(), ios::app);
			//if (headers.empty()) mout << fence << "[ERROR]" << fence << "no headers" << fence << endl;
			for (icstringvector::const_iterator it=headers.begin();it!=headers.end();it++)
			{
				icstring line(*it);
				//if (line.find("host:")==0) {mout << fence << brcin << NoBreaks(line.c_str()) << brcout << fence << endl; }
				//else {mout << fence << aglin << NoBreaks(line.c_str()) << aglout << fence << endl; }
				if (line.find("host:")!=string::npos) host=mimevalue(line.c_str());
			}
			return true;
		}
		string host; 
        virtual operator Response& () 
        { 
            if ( ! response.get() ) throw string( "Can't get response" );
            return *response.get(); 
        }
		protected:
		string mimevalue(string line)
		{
			size_t c(line.find(":"));
			if (c==string::npos) return "";
			c++;
			line.erase(0, c);
			size_t s(line.find_first_not_of("\r\n\f\t "));
			if ( (s!=0) && (s!=string::npos) ) line.erase(0, s);
			size_t e(line.find_first_of("\r\n\f\t "));
			if (e!=string::npos) line.erase(e, line.size()-e);
			return line;
		}

		const icstring& request;
		const icstringvector& headers;
        Socket& sock;
        unique_ptr<Response>& response;
		virtual ostream& operator<<(ostream& o) const { o << fence << "[request]" << fence << Host() << fence << RequestUrl(request.c_str()) << fence; return o; }
	};
} // Hyper

#endif // HYPER_H

