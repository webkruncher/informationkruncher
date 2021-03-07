
#ifndef DB_CURSOR_H
#define DB_CURSOR_H


namespace BdbSpace
{
	struct DbCursor
	{
		DbCursor( DataBase& _db ) : db( _db ), dbcp( NULL ), bvalid( false ) {}
		virtual ~DbCursor() { Close(); }
		void Close()
		{
			memset(&data, 0, sizeof(data)); 
			if ( dbcp ) dbcp->close();
			dbcp = NULL;
		}
		int operator()( DbTxn* txn, const string& what )
		{
			Db& bdb( db );
			bvalid=false;
			Close(); 
			bdb.cursor(txn, &dbcp, DB_READ_UNCOMMITTED); 
			Dbt key((char*)what.c_str(), what.size()  ); 
			const int found( dbcp->get(&key, &data, DB_SET) ); 
			if ( found != DB_NOTFOUND ) bvalid=true; 
			return bvalid;
		}
		private:
		DataBase& db;
		Dbc *dbcp;
		Dbt data; 
		bool bvalid;

		friend ostream& operator<< ( ostream& o, const DbCursor& c ); 
		ostream& operator<< ( ostream& o ) const
		{
			if ( bvalid ) o.write( (char*) data.get_data(), data.get_size() );
			return o;
		}
		
	};
	inline ostream& operator << ( ostream& o, const DbCursor& c ) { return c.operator<<( o ); }

} //BdbSpace

#endif // DB_CURSOR_H

