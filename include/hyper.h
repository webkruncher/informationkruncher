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
//#include <uuid/uuid.h>


namespace Hyper
{ 

	inline string SlashedCookie( string cook )
	{
		stringstream ssp;
		JTools::stringvector parts;
		parts.split( cook, "-" );
		for ( JTools::stringvector::iterator it=parts.begin();it!=parts.end();it++)
		{
			if ( it != parts.begin() ) ssp << "/";
			ssp << (*it );
		}
		return ssp.str();
	}

	inline string UniqCookie()
	{
#if 0
		if ( USE_SSL == 1 ) 
		{
			uuid_t out;
			uuid_generate(out);

			char uuid_str[37];      

			string fname;
			do {
				uuid_unparse_lower(out, uuid_str);
				cerr << "cookie:" << uuid_str << endl;
				fname="text/cookies/";
				fname+=uuid_str;
			} while ( JTools::FileExists( fname ) );
			stringstream ssout; 
			ssout << fence << "NEWCOOKIE" << fence << uuid_str << fence ;
			Log(ssout.str());
			cerr << ssout.str() << endl;

			ofstream ofs( fname.c_str() );

			return uuid_str;
		}
#endif
		return "";
	}

	struct Response : oformat
	{
		Response() : oformat(cerr) {}
		virtual void operator ()() = 0;
	};

	struct Request : oformat
	{
		Request(const icstring& _request, const icstringvector& _headers, Socket& _sock) :
			request(_request), headers(_headers), sock(_sock) {}
		const char* c_str() const {return request.c_str();}
		operator Socket& () const {return sock;}
		string Host() const { return host; }
		string ExpiryTime()
		{ 
			char buf[1000];
			time_t now = time(0);
			now+=(60*60*24*1);
			struct tm tm = *gmtime(&now);
			strftime(buf, sizeof buf, "%a, %d %b %Y %H:%M:%S %Z", &tm);
			return string( buf );
		}
		string Cookie()
		{
			if ( USE_SSL == 1 ) 
				if ( sock.auth & KRUNCH_PERMIT_IP )
				{
					const string cook( fullvalue( "cookie" )  );
					size_t idp( cook.find( "__Secure-ID=" ) );
					if ( idp == string::npos) return "";
					idp+=strlen("__Secure-ID=");
					size_t edp( cook.find_first_of( " \r\n\t;", idp ) );
					if ( edp == string::npos ) 
						edp=cook.size();
					const string cookie( cook.substr( idp, edp-idp ) );
					return cookie;
				}
			return "";
		}


		stringmap& Form()
		{
			stringvector parts;
			parts.split( text, "&" );
			for ( stringvector::iterator it=parts.begin();it!=parts.end();it++)
			{
				stringvector pieces;
				pieces.split( *it, "=" );
				if ( pieces.size() == 2 )
					form[ pieces[ 0 ] ] = pieces[ 1 ];
			}
			return form;
		}

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
		string fullvalue( string what ) 
		{
			stringstream ssv; ssv << what << ":";
			for (icstringvector::const_iterator it=headers.begin();it!=headers.end();it++)
			{
				icstring line(*it);
				if (line.find( ssv.str().c_str() )==0) return (line.c_str());
			}
			return "";
		}
		string value( string what ) 
		{
			stringstream ssv; ssv << what << ":";
			for (icstringvector::const_iterator it=headers.begin();it!=headers.end();it++)
			{
				icstring line(*it);
				if (line.find( ssv.str().c_str() )==0) return mimevalue(line.c_str());
			}
			return "";
		}



		bool ReadPost()
		{
				string slen( value("content-length") );
				long len( atol( slen.c_str() ) );
				
				// scope for raii
				{
					JTools::RaiiMem raiimem( len );
					unsigned char* dest( raiimem );
					
					if ( ! dest ) return false;
					sock.read( (char*) dest, len );

					payload.assign( dest, len );
				}
				return true;

		}
		bool IsPayloadText()
		{
			for (basic_string<unsigned char>::iterator it=payload.begin();it!=payload.end();it++)
			{
				unsigned char t( *it );
				char c( (char) t );
				if ( ! isascii( c ) ) return false;
			}
			text.assign( (char*)payload.data(), payload.size() );
			return true;
		}
		basic_string<unsigned char> payload;
		string text;
		stringmap form;



		string host; Socket& sock;
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
		virtual ostream& operator<<(ostream& o) const { o << fence << "[request]" << fence << Host() << fence << RequestUrl(request.c_str()) << fence; return o; }
	};
} // Hyper

#endif // HYPER_H

