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

#ifndef DATA_KRUNCHER_H
#define DATA_KRUNCHER_H
#include <infosock.h>
using namespace InformationSocket;
#include <infotools.h>
using namespace InfoTools;
#include <hyperbase.h>
namespace DataKruncher
{
	using namespace XmlFamily;

	struct ItemCache : map< string, string >
	{
		friend ostream& operator<<(ostream&,const ItemCache&);
		ostream& operator<<(ostream& o) const
		{
			for (const_iterator it=begin();it!=end();it++) o << it->first << ":" << it->second  << endl;
			return o;
		}
	}; 
	inline ostream& operator<<(ostream& o,const ItemCache& m)
		{ return m.operator<<(o); }

	struct Item : XmlNode
	{
		friend struct Payload;
		virtual XmlNodeBase* NewNode(Xml& _doc,XmlNodeBase* parent,stringtype name) 
		{ 
			XmlNodeBase* ret(NULL);
			ret=new Item(_doc,parent,name); 
			return ret;
		}
		operator bool ();
		virtual ostream& operator<<(ostream&);
		virtual bool operator()(ostream& o) { return XmlNode::operator()(o); }
		Item(Xml& _doc,const XmlNodeBase* _parent,stringtype _name) : XmlNode(_doc,_parent,_name) {}
	};
	inline ostream& operator<<(ostream& o,Item& xmlnode){return xmlnode.operator<<(o);}

	struct Payload : Xml, ItemCache
	{
		Payload( Hyper::HyperBase& _request ) : request( _request ) {} 
		virtual XmlNode* NewNode(Xml& _doc,stringtype name) { return new Item(_doc,NULL,name); }
		ostream& operator<<(ostream& o) { Xml::operator<<(o); return o;}
		operator Item& () { if (!Root) throw string("No root node"); return static_cast<Item&>(*Root); }
		operator bool ();
		private:
		Hyper::HyperBase& request; 
		bool Revision_0(ItemCache&)
		{
			stringstream ss; ss << "Revision_0" << fence << request.sock.dotted() << fence << request.request << fence << request.headers << fence ; 
			Log( NoBreaks( ss.str() ) );
			return true;
		}
	};
	inline ostream& operator<<(ostream& o,Payload& xml){return xml.operator<<(o);}
} // DataKruncher
#endif // DATA_KRUNCHER_H
