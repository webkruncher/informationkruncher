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



var webkruncherxslt=null;
var webkruncherxslt_txt="";
function callInProgress(xmlhttp) 
{
	switch ( xmlhttp.readyState ) 
	{
		case 1, 2, 3:
			return true;
			break;

		// Case 4 and 0
		default:
			return false;
			break;
	}
}





function ajax(xmlname,  target,callback,positioner) 
{
	try
	{
		//LoadProgress("Need " + xmlname)
		this.xmlname = xmlname;
		this.target = target;
		this.callback=callback
		this.positioner=positioner
		//LoadProgress("Generating request")
		this.httpRequest = new XMLHttpRequest(); 
		//LoadProgress("Getting " + this.xmlname)
		this.httpRequest.async = false
		this.cachexslt = false;
		if (!this.xmlname.length) 
		{
			this.cachexslt=true;
			GetObject("EntryMsg").innerHTML="Loading the WebKruncher style"
			this.load('ajax/webkruncher.xslt')
		} else {
			//GetObject("EntryMsg").innerHTML="Loading " + this.xmlname
			this.load(this.xmlname)
		}
	} catch (e) {
		LoadError("Cannot Ajax.")
	}
}



ajax.prototype.getXMLHttpRequest = function () 
{
	try 
	{ 
		LoadProgress("Creating request")
		return new XMLHttpRequest(); 
	} catch(e) {
		LoadError("Cannot get XMLHttpRequest")
	}
	return null;
}

ajax.prototype.StatusMessage = function(ssv)
{
	if (ssv == '0') return 'uninitialized';
	if (ssv == '1') return 'loading';
	if (ssv == '2') return 'loaded';
	if (ssv == '3') return 'interactive';
	if (ssv == '4') return 'complete';
	return 'unknown state'
}

ajax.prototype.diagnostics = function(msg)
{
	if (!buggness) return;
	if (UserMessages)
		UserMessages.Write(msg)
	else 
	{
		Now=new Date()
		sinceopened=Now.getTime()-StartedApplicationAt.getTime()
	}
}

ajax.prototype.StatusChange = function (url,reqobj)
{
	if (buggness)
	{
		//msg=url+" -> "+this.StatusMessage(reqobj.readyState)
		//this.diagnostics(msg)
	}
	if (reqobj.status == 200)
	{
		LastAjaxSuccess=new Date()
	//	if (this.xmlname != "ping.xml") this.diagnostics(msg)
	}
}

ajax.prototype.load = function (url)
{
	//LoadProgress("Getting " + url)
	try
	{
		if (this.httpRequest != null) 
		{
			var reqnode = this;
			this.httpRequest.open('get', url, true);
			this.httpRequest.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
	//		this.httpRequest.setRequestHeader("Content-length", params.length);
			this.httpRequest.setRequestHeader("Connection", "close");
//			this.httpRequest.setRequestHeader("Client", "1280x768");

			this.httpRequest.onreadystatechange = function () 
			{
				reqnode.StatusChange(url,reqnode.httpRequest)
				if (reqnode.httpRequest.readyState == 4) 
				{
					if (reqnode.httpRequest.status != 200) 
					{
						//if (buggness) alert(reqnode.httpRequest.status)
					}
					if (reqnode.cachexslt)
					{
						//LoadProgress("Stylizing..")
						webkruncherxslt=reqnode.httpRequest.responseXML;
						webkruncherxslt_txt=reqnode.httpRequest.responseText;
						return;
					}
					reqnode.xml = reqnode.httpRequest.responseXML;
					if (this.xmlname == "unused_ping.xml") 
					{
						if (reqnode.callback) reqnode.callback()
					} else {
						if (reqnode.httpRequest.responseText.length < 1)
						{ 
							this.httpRequest.send(null)
						} else {
							reqnode.xform();
							if (reqnode.positioner) 
							{
								reqnode.positioner(reqnode.transformed)
							}
							if (!reqnode.transformed) 
							{
								ajax(this.xmlname,  this.target,this.callback,this.positioner) 
								return ;
							}
							reqnode.target.appendChild(reqnode.transformed)
							if (reqnode.callback) setTimeout(reqnode.callback,10)
							//if (reqnode.callback) reqnode.callback(reqnode.transformed)
						}
					}
				}
			};
			this.httpRequest.send(null);
		}
	} catch (e) {
		AjaxFailure(e)
	}
};

ajax.prototype.xform = function () 
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




function InitializeAjax() 
{
	try { 
		LoadProgress("Initializing Ajax")
		new ajax('','',null,null);
	} catch(e) {
		//LoadProgress("Cannot start Ajax.")
	}
}

		
		
	var LoginMessages=null
	function ShowResults(form,resultnode)
	{
		if (resultnode.nodeName!="#text") LoginMessages.Write(resultnode.nodeName)
		for (this.i=0;this.i<resultnode.childNodes.length;this.i++)
			new ShowResults(form,resultnode.childNodes[this.i])
	}
	function LoginUser(forms)
	{
		try
		{
			if (!LoginMessages)
			{
				LoginMessages=new MsgTable(GetObject('DiagnosticsDiv'))
				LoginMessages.Color("black");
				LoginMessages.Background("gray");
				LoginMessages.MaxLength(20)
			}
			ShowObject("LoginText","block")
			this.results=GetObject("LoginText")
			this.values={}
			this.results.response=function(xml)
			{
				LoginMessages.Write("responded")
				new ShowResults(this,xml)
			}
			this.post="<form>\n<signals>\n";
			for (this.j=0;this.j<forms.childNodes.length;this.j++)
			{
				LoginMessages.Write(forms.childNodes[this.j])
				this.o=forms.childNodes[this.j]
				this.post+="<context>"
				//this.post+=this.o.childNodes[1]
				this.post+="</context>\n"
			}
			this.post+="</signals>\n</form>\n";
			
			LoginMessages.Write("Posting")
			new actionpost("/authorize",this.post,this.results);
		} catch (e) {
			alert(e)
		}
		return false;
	}
		
