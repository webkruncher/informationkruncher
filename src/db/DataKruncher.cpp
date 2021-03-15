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



#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <list>
#include <deque>
using namespace std;
typedef string stringtype;
typedef char chartype;
typedef stringstream stringstreamtype;
#include <exexml>
#include <DataKruncher.h>
#include <sys/ioctl.h>
#include <infotools.h>
using namespace InfoTools;

#include <db_cxx.h>
#include <Database.h>
#include <DbCursor.h>

using namespace BdbSpace;


int testmain( int argc, char** argv )
{
	stringstream sserr;
	try
	{

		mkdir( "data", 0777  );
		chdir( "data" );

		const u_int32_t envFlags =
			DB_CREATE     |  // Create the environment if it does not exist
			DB_RECOVER    |  // Run normal recovery.
			DB_INIT_LOCK  |  // Initialize the locking subsystem
			DB_INIT_LOG   |  // Initialize the logging subsystem
			DB_INIT_TXN   |  // Initialize the transactional subsystem. This
			// also turns on logging.
			DB_INIT_MPOOL |  // Initialize the memory pool (in-memory cache)
			DB_THREAD;       // Cause the environment to be free-threaded
		DataBaseEnvironment environment(  envFlags );
		if ( ! environment ) throw string("Can't create environment");

		const u_int32_t openFlags = DB_CREATE              | // Allow database creation
			    DB_READ_UNCOMMITTED    | // Allow uncommitted reads
			    DB_AUTO_COMMIT         | // Allow autocommit
			    DB_THREAD; // Cause the database to be free-threaded 

		{
			DataBase database( "db", environment, openFlags, DB_BTREE, DB_DUPSORT );
			if ( ! database ) throw string( "Error loading db" ); 

			{ pair< string,string > kv( "pumpkin", "pie" ); database+=kv; }
			{ pair< string,string > kv( "apple", "sauce" ); database+=kv; }
			{ pair< string,string > kv( "peach", "cobbler" ); database+=kv; }
			cerr << database << endl;

			vector<string> TestKeys;
			TestKeys.push_back( "pumpkin" );
			TestKeys.push_back( "pear" );
			TestKeys.push_back( "apple" );
			TestKeys.push_back( "peach" );

			DbEnv& env( environment );
			DbTxn *txn;
			env.txn_begin(NULL, &txn, 0);
			for ( vector<string>::const_iterator it=TestKeys.begin();it!=TestKeys.end();it++)
			{
				DbCursor cursor( database );
				const string key( *it );
				const int found( cursor( txn, key ) );
				if ( ! found ) cout << "Can't find " << key << endl;
				else cout << key << ":" << cursor << endl;
			}
			txn->commit(0);

		}
	}
	catch (DbDeadlockException  &dbe) { sserr << "Exception: " << dbe.what() << endl; }
	catch (DbException &dbe) { sserr << "Exception: " << dbe.what() << endl; }
	catch ( string& e ) { sserr << "Exception: " << e << endl; }
	catch ( exception& e ) { sserr << "Std Exception: " << e.what() << endl; }
	catch ( ... ) { sserr << "Unknown Exception: " << endl; }

	if ( ! sserr.str().empty() ) { cerr << sserr.str() ; return EXIT_FAILURE; }

	return (EXIT_SUCCESS);
}


namespace DataKruncher
{
	ostream& Item::operator<<(ostream& o) { return XmlNode::operator<<(o); }

	Item::operator bool ()
	{
		Xml& root( GetDoc() );
		Xml& document(static_cast<Xml&>(root));
		Payload& payload( static_cast<Payload&>(document) );
		ItemCache& cache(payload);
		cache[ name ] = textsegments[0];
		for (XmlFamily::XmlNodeSet::iterator 
			it=children.begin();it!=children.end();it++)
		{
			Item& n( static_cast<Item&>(*(*it)) );
			if ( ! n ) return false;
		}
		return true;
	}

	Payload::operator bool ()
	{
		if (!Root) throw string("No root node"); 
		Item& nodes( static_cast<Item&>(*Root) );
		const bool result( !!nodes );
		if ( ! result ) return false;
		ItemCache& cache( *this );

		// Plan: 
		//	Overall Goal:
		//		Make this function as short as possible while leveraging the full
		//		breath of the services offered.
		//	Store hierarchy of ip octets in self balancing structures.
		//	Map incoming requests to specific user entries, initially using simple keys, then
		//	generalize to provide user defined keys, and provide consistent results
		//	set management capabilities without forcing user code to depend on this library.
		//	Keep all result set methods as standard as possible, leveraging red / black trees
		//	and un-ordered sets and maps.  Initially, implement the optimal choice for general
		//	use, then generalize, providing user code with control to specialize and / or re-
		//	define the data structures.
		//	
		//	Control usage and access through common subsystem (streams/brokers)
		//	Keep high level access at O(1)*4 O(1)( times each of four octets in ip addr )
		//	Ensure each level operates efficiently, carefully calculating when and where to
		//		leverage hash tables at O(1), or balanced trees, generally running in
		//		logarithmic time.
		//	Provide generalized access to specific data items in realtime.

		// Initial Approach:
		//	Naively design simple access to list of 255 elements (simple table)
		//	Satisfy high level user requirments with stubs.
		//	Aproximate solution, test and refine plan.


			// Plan Refinement, Iteration 10 Goal:
			//	Provide efficient and simple access to hierachical groups
			//	of heterogeneous objects, using safe and secure access to data.
			//	Demonstrate minimal, solution, leveraging the standard libraries 
			//	without compromising the ability to implement the fully atomic and 
			//	durable methods that are required to meet the end goal.
	
			// Iteration 20 Goal:
			//	Refine all components, generalizing and parameterizing
			//	structures, abstracting from the core functionality, and
			//	proving a simple, standards based solution for generalized
			//	data access using BerkleyDB,

		// Finish:
			//	Demonstrate fast, efficient, and atomic data access via BerkleyDB
			//	using generalized methods provided within specialized headers and 
			//	or translation units, leveraging standard, and
			//	defacto standard solutions, minimizing the complexity of user-code.

		return Revision_0( cache );

		stringstream ss; ss << request.sock.dotted() << fence << "ERROR|Payload::bool|No Revision" << fence << request.request << fence << request.headers << fence  << cache;
		Log( NoBreaks( ss.str() ) );
		return true;
	}
} // DataKruncher

