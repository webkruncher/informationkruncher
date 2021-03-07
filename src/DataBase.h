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

#ifndef INFO_DATA
#define INFO_DATA
#include "hyperbase.h"

namespace KrunchData
{
	struct DataBase : virtual HyperBase
	{
		DataBase(const icstring& _request, const icstringvector& _headers, Socket& _sock ) :
			HyperBase(_request, _headers, _sock) {}
		operator string ()
		{
			stringstream ssdb, ssxml;
			ssxml << "<data><get>home</get></data>";
			ssdb << "HTTP/1.1 GET /data" << endl;
			ssdb << "Content-length: " << ssxml.str().size() << endl << endl;
			ssdb << ssxml.str() << endl;
			//Log(NoBreaks(ssdb.str()));
			Socket db( (char*) "127.0.0.1", 99 );
			if ( db.open() && db.connect() )
			{
				db.timeout(1, 00);
				db.blocking(true);
				db.write( ssdb.str().c_str(), ssdb.str().size() );
				db.flush();
				string dbreqponseline;
				stringstream dbresponse;
				
				icstringvector dbheaders;
				while ( true ) 
				{
					stringtype line;
					db.getline( line );
					if ( line.empty() ) break;
					if ( dbreqponseline.empty() )
					{
						dbreqponseline=line;
					} else {
						dbheaders.push_back( line );
					}
				}
				string sdbcl;
				for (icstringvector::const_iterator it=dbheaders.begin();it!=dbheaders.end();it++)
				{
					icstring line(*it);
					if (line.find("content-length:")!=string::npos) sdbcl=mimevalue(line.c_str());
				}
				const int Len( atoi( sdbcl.c_str() ) );
				if ( ( sdbcl.empty() ) || ( Len>1024 ) )
				{ 
					stringstream ssout; ssout << fence << "[DATARESPONSE-INVALIDSIZE]" << fence << dbresponse.str()<< fence;
					Log(ssout.str());
					db.flush(); 
					return "";
				} 

				char buf[ Len +1 ];
				memset( buf, 0, Len+ 1 );
				db.read( buf, Len );
				stringtype S( buf );
				//stringstream ssout; ssout << fence << "[DATASPONSE]" << fence << S << fence; Log( ssout.str() ); 
				
			} else {
				stringstream ssout; ssout << fence << "[DATA-UNAVAILABLE]" << fence ;
				Log(ssout.str());
			}
			return "";
		}
	};

} // KrunchData

#endif // INFO_DATA

