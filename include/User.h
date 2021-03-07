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

#ifndef INFOSSOCKS_USER_H
#define INFOSSOCKS_USER_H
namespace InformationSocket
{
	struct Ip4PathManager
	{
		const string operator() ( const string addr ) const
		{
#if 0
			stringstream sspath;
			stringvector parts;
			parts.split( addr, "." );
			if ( parts.size() != 4 )
			{
				stringstream ssout; ssout << fence << "[Ip4PathManager:ERROR]" << fence << addr << fence << parts.size() << fence; Log(ssout.str());
				cerr << ssout.str() << endl;
			} else {
				//stringstream ssout; ssout << fence << "[Ip4PathManager:WIP]" << fence << addr << fence ; Log(ssout.str());
				//cerr << ssout.str() << endl;
				//cerr << parts << endl;

				sspath << "ip4/";
				JTools::assertdir( sspath.str() );
				for ( stringvector::iterator it=parts.begin();it!=parts.end();it++) 
				{
					if ( it != parts.begin() ) sspath << "/";
					sspath << (*it);
					JTools::assertdir( sspath.str() );
				}
				//cerr << "Your ip path:" << sspath.str() << endl;
			}
			return sspath.str();
#endif
return "";
		}
	};

	struct UserRegistry : map<string, unsigned long>
	{

		string IpPath(const string addr) const
		{
			const string addresspath( ipv4( addr ) );
			return addresspath;
		}

		unsigned long operator()(const string addr) const
		{
			const string addresspath( ipv4( addr ) );
			const string IPblockingFile( addresspath + string("/blocked") );
			//cerr << "Checking for:" << IPblockingFile << ";" << endl;
			struct stat sb;
			if (stat(IPblockingFile.c_str(), &sb)==0)
			{
				cerr << IPblockingFile << " exists, ip is blocked" << endl;
				return KRUNCH_IP_BLOCKED;
			}

			//iterator it( find( addr ) );
			//if ( it == end() ) return 0;

			unsigned long permit( 0 );

			const string AuthorizedFile( addresspath + string("/authorized") );
			//cerr << "Checking for:" << AuthorizedFile << ";" << endl;

			permit |= KRUNCH_PERMIT_IP;
			if (stat(AuthorizedFile.c_str(), &sb)==0)
			{
				cerr << AuthorizedFile << " exists" << endl;
				permit |= KRUNCH_PERMIT_LOGGEDIN;
			}

			//it->second++;
			return permit;
		}
		private:
		Ip4PathManager ipv4;
	};


	inline string CookiePathFromSock( const string ucook, InformationSocket::Socket& sock )
	{
		JTools::stringvector ipaddr;
		ipaddr.split( sock.dotted(), ":" );
		const string addr( ipaddr[ 0 ] );
		InformationSocket::UserRegistry ureg;

		const string slashcook( Hyper::SlashedCookie( ucook ) );
		const string CookiePath( ureg.IpPath( addr ) + string( "/" ) + slashcook);
		//const string CookieFile( CookiePath + string( "/cookie" ) );
		//cerr << "CookiePath: " << CookiePath << endl;	
		//JTools::assertpath( CookiePath );
		//cerr << "CookieFile: " << CookieFile << endl;	
		return CookiePath;
	}


} // InformationSocket

#endif //INFOSSOCKS_USER_H
// WebKruncher.com





