

mouselemming=null;
mousedmovedonce=false
mousetrails=null


var afetimes = 0
function artificialentry()
{
	if (mousedmovedonce) return
	GetViewport();
	getMousePosition(e);
	afemx=(10*afetimes)+200
	afemy=(10*afetimes)+200
	mousetrails.Chase(afemx,0,afemy,0,1);
	if (mouselemming) 
		mouselemming.MouseMoved(afemx,afemy);
	afetimes++
	if (afetimes < 3)
		setTimeout('artificialentry()',100)
	//Diagnostics.Write(afetimes)
}

function InitMouseTrails(throttle,pb,mode)
{
	seeking=new Plot("seeking","blue","yellow");
	current=new Plot("current","green","black");
	mousetrails = new ParticleSystem(pb,'m',2,1,"T",mode,null);
	mouselemming = new Lemming("mouselemming",mousetrails);
	setTimeout('artificialentry()',100)
}

function FeedSparky() 
{
	if (mousetrails) mousetrails.Grow();
}

function StarveSparky() 
{
	if (mousetrails) mousetrails.Shrink();
}

function CallStackItem(name,fun,target)
{
	this.name = name
	this.fun = fun
	this.target=target
	this.Update = function(eventitem)
	{
		eval(this.fun(this.name,this.target,eventitem))
	}
}

function CallingStack()
{
	this.Stacker = new Array()
	this.Update = function(eventitem)
	{
		for (csi=0;csi < this.Stacker.length; csi++)
			this.Stacker[csi].Update(eventitem)
	}
	this.Add = function(item)
	{
		this.Stacker[this.Stacker.length]=item
	}
	this.Remove = function(name)
	{
		kill=-1
		for (csi=0;csi < this.Stacker.length; csi++)
			if (this.Stacker[csi].name==name)
				kill=csi
		if (kill!=-1)
			this.Stacker.remove(kill)
	}

}

var ScrollerFunctions = new CallingStack()
var SizerFunctions = new CallingStack()
var MouserFunctions = new CallingStack()

window.onscroll = function()
{
	getScrollingPosition();
	ScrollerFunctions.Update()
};

window.onresize = function()
{
	getScrollingPosition();
	SizerFunctions.Update()
};


function mouser(e) 
{
	mousedmovedonce=true
	GetViewport();
	getMousePosition(e);
	mx=posX;
	my=posY;
	MouserFunctions.Update(e)
if (0)	if (Diagnostics)
	{
		cx=viewportwidth/2
		cy=viewportheight/2
		force=200
		distx=cx-mx
		disty=cy-my
		direction = Math.atan2(disty, distx)
		distance=Math.sqrt( (distx * distx) + (disty * disty) )
		//Diagnostics.Write(direction)
		if (direction>0)
		{
			if (Math.abs(direction)>(Math.PI/2))
			{
				Diagnostics.Write("Upper Right")
			} else {
				Diagnostics.Write("Upper Left")
			}
		} else {
			if (Math.abs(direction)>(Math.PI/2))
			{
				Diagnostics.Write("Lower Right")
			} else {
				Diagnostics.Write("Lower Left")
			}
		}
	}
}

document.onmousemove = mouser;

csme = new CallStackItem("SparkyMouser",function()
	{
		if (mousetrails)
			mousetrails.Chase(mx,0,my,0,1);
		if (mouselemming) 
			mouselemming.MouseMoved(mx,my);
	}
)

MouserFunctions.Add(csme)

