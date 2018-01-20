
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


