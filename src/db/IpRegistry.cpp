


#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include <errno.h>
#include <db_cxx.h>
using namespace std;
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include <IpRegistry.h">
#include <DbCursor.h">
#include <sys/stat.h>

// /home/jmt/libdb/examples/cxx/txn_guide



struct student_record {
	char student_id[4];
	char last_name[15];
	char first_name[15];
};
#if 0
void
second()
{
	DB *dbp, *sdbp;
	int ret;
	
	/* Open/create primary */
	if ((ret = db_create(&dbp, dbenv, 0)) != 0)
		handle_error(ret);
	if ((ret = dbp->open(dbp, NULL,
	    "students.db", NULL, DB_BTREE | DB_RECNUM , DB_CREATE, 0600)) != 0)
		handle_error(ret);
	
	/*
	 * Open/create secondary.  Note that it supports duplicate data
	 * items, since last names might not be unique.
	 */
	if ((ret = db_create(&sdbp, dbenv, 0)) != 0)
		handle_error(ret);
	if ((ret = sdbp->set_flags(sdbp, DB_DUPSORT)) != 0)
		handle_error(ret);
	if ((ret = sdbp->open(sdbp, NULL,
	    "lastname.db", NULL, DB_BTREE, DB_CREATE, 0600)) != 0)
		handle_error(ret);
	
	/* Associate the secondary with the primary. */
	if ((ret = dbp->associate(dbp, NULL, sdbp, getname, 0)) != 0)
		handle_error(ret);
}
/*
 * getname -- extracts a secondary key (the last name) from a primary
 * 	key/data pair
 */
int
getname(
	DB *secondary,
	const DBT *pkey, const DBT *pdata,
	DBT *skey)
{
	/*
	 * Since the secondary key is a simple structure member of the
	 * record, we don't have to do anything fancy to return it.  If
	 * we have composite keys that need to be constructed from the
	 * record, rather than simply pointing into it, then the user's
	 * function might need to allocate space and copy data.  In
	 * this case, the DB_DBT_APPMALLOC flag should be set in the
	 * secondary key DBT.
	 */
	memset(skey, 0, sizeof(DBT));
	skey->data = ((struct student_record *)pdata->data)->last_name;
	skey->size = sizeof((struct student_record *)pdata->data)->last_name;
	return (0);
}

#endif
int IpRegistry( int argc, char** argv )
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
		BdbSpace::DataBaseEnvironment environment(  envFlags );
		if ( ! environment ) throw string("Can't create environment");

		const u_int32_t openFlags = DB_CREATE              | // Allow database creation
			    DB_READ_UNCOMMITTED    | // Allow uncommitted reads
			    DB_AUTO_COMMIT         | // Allow autocommit
			    DB_THREAD; // Cause the database to be free-threaded 

		{
			IpReg::IpDataBase database( "db", environment, openFlags, DB_BTREE, DB_DUPSORT );
			if ( ! database ) throw string( "Error loading db" ); 

			{ pair< string,string > kv( "pumpkin", "pie" ); database+=kv; }
			{ pair< string,string > kv( "apple", "sauce" ); database+=kv; }
			{ pair< string,string > kv( "peach", "cobbler" ); database+=kv; }
			{ pair< string,string > kv( "pear", "pieces" ); database+=kv; }
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
				BdbSpace::DbCursor cursor( database );
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


