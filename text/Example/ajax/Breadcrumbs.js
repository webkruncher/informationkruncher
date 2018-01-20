

function Breadcrumbs (BreadLink)
{
	this.ndx=0
	this.BreadLink=BreadLink;
	this.History = new Array()
	this.BreadNav=false
	this.Navigate = function (page)
	{
		//UserMessages.Write(page + " " + this.ndx + " / " + this.History.length)
		if (this.BreadNav) 
		{
			this.BreadNav=false
			return
		}
		if (this.ndx)
		{
			if (page==this.History[this.ndx-1])
				return;
		}
		this.History[ this.ndx ] = page
		this.ndx+=1
	}

	this.Buttons = function(target)
	{
		BkDisplay="none"
		FwdDisplay="none"
		BkBtn=GetObject('Pages').childNodes[target].firstChild.firstChild
		FwdBtn=GetObject('Pages').childNodes[target].firstChild.lastChild

		if ( (this.History.length > 1) && (this.ndx > 1) )
		{
			BkBtn.setAttribute("style", "display:inline;")
			BkDisplay="inline"
		}

		if (this.ndx < this.History.length )
		{
			FwdBtn.setAttribute("style", "display:inline;")
			FwdDisplay="inline"
		}

		BkBtn.setAttribute("style", "display:"+BkDisplay+";")
		FwdBtn.setAttribute("style", "display:"+FwdDisplay+";")
	}

	this.Fwd=function()
	{
		this.BreadNav=true
		this.ndx+=1
		Navigate(this.History[ this.ndx-1 ])
	}

	this.Back=function()
	{
		this.BreadNav=true
		if (this.ndx < 1) return;
		this.ndx-=1
		Navigate(this.History[ this.ndx-1 ])
	}
}


