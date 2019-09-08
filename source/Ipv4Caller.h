#ifndef IPV4CALLER_H
#define IPV4CALLER_H
#include "DbNode.h" 
#include <syslog.h>


namespace Ipv4Caller
{
	struct Who
	{
		Who( const string& _cookie ) : count( 0 ) {}
		size_t size() { return sizeof( count ) + sizeof( cookie ); }
		unsigned long count;
		char cookie[ 128 ];
	};

	struct IpAddr : DbNodeSpace::DbNodeBase
	{
		IpAddr( const string _databasefname, BdbSpace::DataBaseEnvironment& _environment, 
				const u_int32_t _openFlags, DBTYPE _dbflags , u_int32_t _extraFlags ) 
			: DbNodeSpace::DbNodeBase( _databasefname, _environment, _openFlags, _dbflags, _extraFlags  ) {}
		bool operator+=( pair<string,Who>& kv )
		{ 
			if ( ! dbp ) throw string("Db not initialized");

			DbEnv& env( environment );
			DbTxn *txn;
			bool retry = true;
			int retry_count = 0;
			while (retry) 
			{
				try 
				{
					txn = NULL;
					env.txn_begin(NULL, &txn, 0);

					string& K( kv.first );
					Dbt key((char*)K.c_str(), K.size());

					unsigned long many( 0 );

					{	
						BdbSpace::DbCursor cursor( *this );
						const int found( cursor( txn, K ) );
						if ( ! found ) cout << "Can't find " << K << endl;
						else 
						{
							cout << "Found " << K << endl;
							Who* who( (Who*) cursor.GetData() );
							many=who->count; 
							cout << "Many:" <<  many << endl;
						}
					}



					Who& V( kv.second ); 
					V.count = many+1;
					Dbt data((char*)&V, V.size()); 
					dbp->put(txn, &key, &data, 0);

					try 
					{
						txn->commit(0);
						retry = false;
						txn = NULL;
					} catch (DbException &e) {
						cerr << "x"; cerr.flush();
					}
				} catch (DbDeadlockException &) {
					if (txn != NULL)
						(void)txn->abort();

					if (retry_count < 10) {
						retry_count++;
						retry = true;
					} else {
						retry = false;
					}
				} catch (DbException &e) {
					if (txn != NULL)
						txn->abort();
					retry = false;
				} catch (std::exception &ee) {
					cerr << "-"; cerr.flush();
					return false;
				}
			}
			return true;

		}
	};



	struct IpData
	{
		void Log(const string& what) { if (1) syslog(1,what.c_str()); }
		IpData();
		static IpData& instance();
		virtual bool destroy()
		{
			if ( database ) delete database;
			if ( environment ) delete environment;
			Log("Destroyed contract data"); 
			database=NULL;
			environment=NULL;
			return true;
		}
		virtual operator bool ();
		virtual bool operator+= (pair<string,Who>& kp)
		{
			if ( database ) return (*database)+=kp;
			else return false;
		}
	private:
		const u_int32_t envFlags;
		const u_int32_t openFlags;
		BdbSpace::DataBaseEnvironment* environment;
		Ipv4Caller::IpAddr* database;
	};

	inline IpData& IpData::instance()
	{
		static IpData singleton;
		return singleton;
	}


	IpData::IpData() : 
		envFlags(
			DB_CREATE     |  // Create the environment if it does not exist
			DB_RECOVER    |  // Run normal recovery.
			DB_INIT_LOCK  |  // Initialize the locking subsystem
			DB_INIT_LOG   |  // Initialize the logging subsystem
			DB_INIT_TXN   |  // Initialize the transactional subsystem. This
			// also turns on logging.
			DB_INIT_MPOOL |  // Initialize the memory pool (in-memory cache)
			DB_THREAD       // Cause the environment to be free-threaded
		),
 
		openFlags(
			DB_CREATE              | // Allow database creation
			DB_READ_UNCOMMITTED    | // Allow uncommitted reads
			DB_AUTO_COMMIT         | // Allow autocommit
			DB_THREAD // Cause the database to be free-threaded 
		),
		environment( NULL ), database( NULL ) 
	{}

	inline IpData::operator bool ()
	{
		mkdir( "ip4data", 0777  );
		chdir( "ip4data" );

		environment=new BdbSpace::DataBaseEnvironment (  envFlags );
		if ( ! (*environment) ) throw string("Can't create environment");


		database=new Ipv4Caller::IpAddr ( "db", *environment, openFlags, DB_BTREE, NULL );
		if ( ! (*database) ) throw string( "Error loading db" ); 

		Log("Started contract data"); 
		chdir( ".." );
		return true;
	}

#if 0
	
		mkdir( "testdata", 0777  );
		chdir( "testdata" );

		const u_int32_t envFlags =
			DB_CREATE     |  // Create the environment if it does not exist
			DB_RECOVER    |  // Run normal recovery.
			DB_INIT_LOCK  |  // Initialize the locking subsystem
			DB_INIT_LOG   |  // Initialize the logging subsystem
			DB_INIT_TXN   |  // Initialize the transactional subsystem. This
			// also turns on logging.
			DB_INIT_MPOOL |  // Initialize the memory pool (in-memory cache)
			DB_THREAD;       // Cause the environment to be free-threaded
		BdbSpace::DataBaseEnvironment environment(  envFlags );
		if ( ! environment ) throw string("Can't create environment");

		const u_int32_t openFlags = DB_CREATE              | // Allow database creation
			    DB_READ_UNCOMMITTED    | // Allow uncommitted reads
			    DB_AUTO_COMMIT         | // Allow autocommit
			    DB_THREAD; // Cause the database to be free-threaded 

		{
			Ipv4Caller::IpAddr database( "db", environment, openFlags, DB_BTREE);
			if ( ! database ) throw string( "Error loading db" ); 
#endif


} // Ipv4Caller
#endif //IPV4CALLER_H

