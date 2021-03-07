
#ifndef DATABASE_OBJECTS
#define  DATABASE_OBJECTS
namespace BdbSpace
{

	struct DataBaseEnvironment
	{
		DataBaseEnvironment( const u_int32_t _envflags ) : envflags( _envflags ), envp( NULL ) {}
		operator bool ()
		{
			envp = new DbEnv( 0 );
			envp->open(".", envflags, 0);
			envp->set_lk_detect(DB_LOCK_MINWRITE); 
			return true;
		}
		~DataBaseEnvironment( )
		{
			if ( envp ) delete envp;
		}
		operator DbEnv& ()
		{
			if ( ! envp ) throw string("No environment");
			return *envp;
		}
		private:
		const u_int32_t envflags;
		DbEnv *envp;
	};

	struct DataBase
	{
		public:
			DataBase( const string _databasefname, DataBaseEnvironment& _environment, const u_int32_t _openFlags, DBTYPE _dbflags = DB_BTREE, u_int32_t _extraFlags = 0 ) 
				: databasefname( _databasefname ), environment( _environment ), dbp( NULL ) , openFlags( _openFlags ), dbflags( _dbflags ), extraFlags( _extraFlags ) {}
			virtual ~DataBase()
			{
				Close();
			}

			void Close()
			{
				if ( dbp )
				{
					dbp->close(0);
					delete dbp;
				}
			}

			bool setflags( const u_int32_t flags )
			{
				if ( ! dbp ) throw string("Db not allocated, can't set flags ");
				return ( ( dbp->set_flags( flags) ) == 0 );
			}

			virtual operator bool ();
			virtual bool operator += (pair<string,string>& );
			operator Db& ()
			{
				if ( ! dbp ) throw string("Db not initialized");
				return *dbp;
			} 
		private:
			friend ostream& operator<< ( ostream& o, const DataBase& b ); 
			ostream& operator<< ( ostream& o ) const;
			const string databasefname;
			DataBaseEnvironment& environment;
			Db *dbp;
			const u_int32_t openFlags;
			const DBTYPE dbflags;
			const u_int32_t extraFlags;
	};

	inline DataBase::operator bool ()
	{ 
		DbEnv& env( environment );
		dbp = new Db(&env, 0); 

		if (extraFlags != 0)
			if ( dbp->set_flags(extraFlags) )  throw string("Cannot set extra flags");

		dbp->set_error_stream(&cerr);
		dbp->set_errpfx(databasefname.c_str());
		dbp->set_pagesize(1024); 
		dbp->open(NULL, databasefname.c_str(), NULL, dbflags, openFlags, 0664);
		return true;
	}

	inline bool DataBase::operator+=( pair<string,string>& kv )
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
				string& V( kv.second ); 
				Dbt key((char*)K.c_str(), K.size());
				Dbt data((char*)V.c_str(), V.size()); 
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


	inline ostream& DataBase::operator<< ( ostream& o ) const
	{
		if ( ! dbp ) throw string("Db not initialized");
		if ( dbflags & DB_BTREE )
		{
			DB_BTREE_STAT *statp; 
			dbp->stat(NULL, &statp, 0);
			o << (u_long)statp->bt_ndata << " records"; 
			free(statp);
		}
		return o;
	}

	inline ostream& operator << ( ostream& o, const DataBase& c ) { return c.operator<<( o ); } 
} //BdbSpace
#endif //  DATABASE_OBJECTS

