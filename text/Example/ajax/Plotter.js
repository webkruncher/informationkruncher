

var lockedup = 0
function Plot(name,bkcolor,textcolor)
{
	this.name = name
	this.Update = function (x,y,txt) { }
}

function WhatToDo(thatpiece)
{
	this.thatpiece=thatpiece;
	this.n=0;
	this.psp=null
	this.reset= function(){this.n=40;}
	this.reset();
	this.increment=Random(100)+300;
	this.RunUpdate = function ()
	{
		if (!this.n) 
		{
			this.psp.displayed=false;
			this.thatpiece.launched=null;
			this.thatpiece.reset();
			this.psp.Nuke();
			return;
		}
		if (this.psp) this.psp.Chase(this.psp.X,0,this.psp.Y,0,1);
		ff=this.psp.myname+".Todo.RunUpdate()";
		this.n--;
		if (this.n==20) 
		{
			this.psp.Motivation.ContinueForward();
			t=Random(341)/1000;
			if (Random(2)) t*=-1;
			this.psp.Motivation.Turn(t);
		}
		setTimeout(ff,this.increment);
	}
	this.SetPs = function (p)
	{
		this.psp=p
	}
}

function Thing(MyHead)
{
	this.c=0
	this.Head=MyHead;
	this.DivObj = GetObject('SparkyRoot').cloneNode(false)
	GetObject("RootNode").appendChild(this.DivObj)
	this.DivObj.setAttribute('id',this.Head.myname+"_piece")
	this.DivObj.id=this.Head.myname+"_piece"
	this.DivObj.setAttribute('class','particle')
	style="position:absolute; width:22px; height:22px; top:-100px; left:-100px; "
	this.DivObj.setAttribute('style',style)
	this.Texture=new TextureMap(this.Head.depth);
	this.imgname=this.Texture.GetUrl();
	imgname="url("+this.imgname+")";
	this.DivObj.style.backgroundImage = "url("+this.imgname+")";
	this.DivObj.setAttribute("Image",imgname)
	
	this.launched=null;
	this.yoursub=null;
	this.le=null
	this.ThingTicker = 100;

	this.IsOkToShow = function(top,left)
	{
		if (top > 700) return false
		if (left > 1200) return false
		return this.Head.Collide.Detect()
	}

	this.MoveThing1 = function(top,left)
	{
		n = this.DivObj.cloneNode(true)
		w=GetObject("RootNode")
		w.appendChild(n)
		style="position:absolute; width:22px; height:22px; top:"+top+"px; left:"+left+"px;" 
		if (this.IsOkToShow(top,left)) style+="display:inline;"
		else style+="display:none;"
		n.setAttribute('style',style)
		n.style.backgroundImage=n.getAttribute('Image')
		w.removeChild(this.DivObj)
		this.DivObj=n
	}

	this.MoveThing2 = function(top,left)
	{
		n=this.DivObj
		style="position:absolute; width:22px; height:22px; top:"+top+"px; left:"+left+"px;" 
		if (this.IsOkToShow(top,left)) style+="display:inline;"
		else style+="display:none;"
		n.setAttribute('style',style)
		n.style.backgroundImage=n.getAttribute('Image')
	}

	this.MoveThing = this.MoveThing2


	this.reset=function()
	{
		this.ThingTicker=Random(300)+10;
	}
	this.fertile=true;

	this.advancer=0;
	this.advanceinc=Random(9)+3;
	this.advance=function(){if (this.advancer<1) this.advancer=this.advanceinc; else this.advancer--; return this.advancer;}

	this.Update = function ()
	{

		this.MoveThing(Math.round(this.Head.Y),Math.round(this.Head.X) );
		if (!this.advance())
		{
			this.imgname=this.Texture.GetUrl();
			imgname="url("+this.imgname+")";
			this.DivObj.setAttribute("Image",imgname)
		}
		if (this.Head.d==0)
		if (this.fertile) 
		{
			if (this.ThingTicker == 0)
			{
				if (this.Head)
				{
					if (this.launched)
					{
						if (this.fertile) this.reset();
						return;
					}
					
					if (this.fertile)
					{
						this.mywhattodo=new WhatToDo(this);
						yournamefirst=this.Head.myname;
						yourname="b"+yournamefirst + this.c + 'G';
						this.c++;
						this.launched=new ParticleSystem(0,yourname,1,1,"N",0,this.mywhattodo);
						this.launched.DontReProduce();
						this.launched.EveryoneGoTo(this.Head.Motivation.GetX(),this.Head.Motivation.GetY());
						this.launched.StartToDo();
						this.mywhattodo.SetPs(this.launched)
						this.le=(yourname  + " = " + "this.launched" );
						eval(this.le)
						this.launched.owner=this.Head;
						this.reset();
					}
				}
			} else {
				this.ThingTicker-=1
			}
		}
	}

	this.Die = function ()
	{
		if (this.DivObj)	
		{
			GetObject("RootNode").removeChild(this.DivObj)
			this.DivObj=null
		}	
	}
}




function FrameBox(rootnode,where)
{
	this.where = where
	this.FramePiece = rootnode.cloneNode(false)
	rootnode.appendChild(this.FramePiece)
	style="position:absolute; width:"+(this.where.w)+"px; height:"+(this.where.h)+"px; top:"+this.where.y+"px; left:"+this.where.x+"px; "
	style+="display:inline;"
	style+="color:blue; background:silver; border:thick outset silver;"
	this.FramePiece.setAttribute('style',style)
}

