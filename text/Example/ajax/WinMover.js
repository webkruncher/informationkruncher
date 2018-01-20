


function WinMover(handlerwindow,targetmover)
{
	this.handler=handlerwindow
	this.target=targetmover
	this.name=handlerwindow.id
	MouserFunctions.Add(this)

	this.hww=handlerwindow.clientWidth
	this.hhw=handlerwindow.clientHeight
	this.hot=handlerwindow.offsetTop
	this.hol=handlerwindow.offsetLeft

	this.tww=targetmover.clientWidth
	this.thw=targetmover.clientHeight
	this.tot=targetmover.offsetTop
	this.tol=targetmover.offsetLeft

	this.deltatop=this.hot-this.tot
	this.deltaleft=this.hol-this.tol
	this.deltamy=this.hot-my
	this.deltamx=this.hol-mx
	this.lastmx=mx
	this.lastmy=my
	
	this.onmouseup=function()
	{
		MouserFunctions.Remove(this.name)
	}


	this.Update=function(eventitem)
	{
		this.style=this.target.getAttribute("style")
		this.dx=this.lastmx-mx
		this.dy=this.lastmy-my
		this.nowtop=this.target.offsetTop
		this.nowleft=this.target.offsetLeft
		this.nowleft-=this.dx
		this.nowtop-=this.dy
		this.mndx=this.style.indexOf("left:");
		if (this.mndx>0) this.style=this.style.substr(0,this.mndx);
		this.mndx=this.style.indexOf("top:");
		if (this.mndx>0) this.style=this.style.substr(0,this.mndx);
		this.style=this.style+"position:absolute;top:"+this.nowtop+"px;left:"+this.nowleft+"px;";
		this.lastmx=mx
		this.lastmy=my
		this.target.setAttribute("style",this.style)
	}
}

function NewWinMover(handler,target)
{
	if (handler.wmover) handler.wmover.onmouseup();
	handler.wmover=WinMover(handler,target)
}

