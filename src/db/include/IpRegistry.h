

#ifndef IP_REGISTRY_H
#define IP_REGISTRY_H
#include <Database.h">

namespace IpReg
{
	struct IpDataBase : BdbSpace::DataBase
	{
		IpDataBase( const string _databasefname, BdbSpace::DataBaseEnvironment& _environment, const u_int32_t _openFlags, DBTYPE _dbflags = DB_BTREE, u_int32_t _extraFlags = NULL ) 
			: BdbSpace::DataBase( _databasefname, _environment, _openFlags, _dbflags, _extraFlags  ) {}
	};
};

#endif // IP_REGISTRY_H

