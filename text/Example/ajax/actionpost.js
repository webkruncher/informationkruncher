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


function actionpost(url,apost,target)
{
	try
	{
		this.url=url;
		this.target=target;
		this.apost=apost
		this.httpRequest=this.getXMLHttpRequest () 
		this.httpRequest.async = true
		this.load()
	} catch (e) {
		AjaxFailure(e)
	}
}

actionpost.prototype.getXMLHttpRequest = function () 
{
	try 
	{ 
		return new XMLHttpRequest(); 
	} catch(e) {
		AjaxFailure(e)
	};
	return null;
}

actionpost.prototype.StatusMessage = function(ssv)
{
	if (ssv == '0') return 'uninitialized';
	if (ssv == '1') return 'loading';
	if (ssv == '2') return 'loaded';
	if (ssv == '3') return 'interactive';
	if (ssv == '4') return 'complete';
	return 'unknown state'
}

actionpost.prototype.diagnostics = function(msg)
{
	if (!buggness) return;
	if (UserMessages) UserMessages.Write(msg)
}

actionpost.prototype.StatusChange = function (url,reqobj)
{
	if (buggness)
	{
		msg=url+" -> "+this.StatusMessage(reqobj.readyState)
		this.diagnostics(msg)
	}
}

actionpost.prototype.load = function ()
{
	try
	{
		if (this.httpRequest != null) 
		{
			var reqnode = this;
			this.httpRequest.open('GET', this.url, true);
			this.httpRequest.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
			this.httpRequest.setRequestHeader("Connection", "close");
			this.httpRequest.onreadystatechange = function () 
			{
//alert(reqnode.httpRequest.readyState)
				if (reqnode.httpRequest.readyState == 4) 
				{
					if (reqnode.httpRequest.status != 200) 
					{
						//if (buggness) alert(reqnode.httpRequest.status)
					} else
						reqnode.xml = reqnode.httpRequest.responseXML;
					//alert(reqnode.httpRequest.responseText)
					reqnode.target.response(reqnode.httpRequest.responseXML)
				}
			};
//alert("Sending:"+this.apost)
			this.httpRequest.send(this.apost)
		}
	} catch (e) {
		AjaxFailure(e)
	}
};

actionpost.prototype.xform = function () 
{
	try
	{
		if (typeof XSLTProcessor != 'undefined') 
		{
			var xsltProcessor = new XSLTProcessor();
			xsltProcessor.importStylesheet(webkruncherxslt)
			this.transformed=
				xsltProcessor.transformToFragment(this.xml, this.target.ownerDocument);
		}
		else 
			if (typeof this.xml.transformNode != 'undefined') 
			{
				this.transformed=
					this.xml.transformNode(webkruncherxslt);
			}
			 else {
				AjaxFailure("ajax failed")
			 }
	} catch (e) {
		AjaxFailure(e)
	}
}

function postscrubber()
{
	this.scrub=function(who,what)
	{
		if(what.nodeName=="INPUT") this.addnode(who,what)
		for (this.i=0;this.i<what.childNodes.length;this.i++)
		{
			this.ps=new postscrubber();
			this.ps.scrub(who,what.childNodes[this.i])	
		}
	}

	this.addnode=function(who,what)
	{
		who.what+="<"+what.name+"><![CDATA["+what.value+"]]></"+what.name+">\n";
	}
}

function PostThisForm(how,form,nest)
{
	this.parts=nest.split("/");
	this.what="<form>\n"
	for (this.n=0;this.n<this.parts.length;this.n++)
		this.what+="<"+this.parts[this.n]+">\n"
	this.ps=new postscrubber();
	this.ps.scrub(this,form);
	for (this.n=this.parts.length-1;this.n>=0;this.n--)
		this.what+="</"+this.parts[this.n]+">\n"
	this.what+="\n</form>"
	new actionpost(how,this.what,form)
	return false;
}


