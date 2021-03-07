#ifndef HYPERBASE_H
#define HYPERBASE_H
namespace Hyper
{
	struct HyperBase
	{
	    HyperBase(const icstring& _request, const icstringvector& _headers, Socket& _sock ) :
			request(_request), headers(_headers), sock(_sock) {}
		protected:
		const icstring& request;
		const icstringvector& headers;
		Socket& sock;
	};
} // Hyper

#endif // HYPERBASE_H

