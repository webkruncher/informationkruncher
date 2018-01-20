
			var RecentlyClicked=false
			function UnClickMe()
			{
				RecentlyClicked=false
			}

function Within(wmx,wmy,X,Y,W,H)
{
	if (wmx < X)  return false
	if (wmx > (X+W))  return false
	if (wmy < Y)  return false
	if (wmy > (Y+H))  return false
	return true
}

function GetNewNodePos(x,y,w,y)
{
	GetViewport ()
	cx=viewportwidth/2
	cy=viewportheight/2
	force=40
	distx=cx-x
	disty=cy-y
	direction = Math.atan2(disty, distx)
	distance=Math.sqrt( (distx * distx) + (disty * disty) )
		if (direction>0)
		{
			if (Math.abs(direction)>(Math.PI/2))
			{
				//Diagnostics.Write("Upper Right")
				force*=2
			} else {
				//Diagnostics.Write("Upper Left")
			}
		} else {
			if (Math.abs(direction)>(Math.PI/2))
			{
				//Diagnostics.Write("Lower Right")
				force*=4
			} else {
				//Diagnostics.Write("Lower Left")
				force*=3
			}
		}
	dx = Math.round(force * Math.cos(this.direction))
	dy = Math.round(force * Math.sin(this.direction))
//	Whatever(direction)
	x+=dx
	y+=dy

	return new NodePosition(x,y,0,0)
}


function MenuActions(Parking,n,parent,km)
{
	this.km=km
	this.n=n
	this.parent=parent
	this.sub=n.firstChild.childNodes[1]
	sub=this.sub
	n.MenuActionsObject=this
	this.timer=null
	this.Parking=Parking

	//Whatever("MenuActions:"+km.name+":"+n.id)
	this.MousePosUpdate = function(o)
	{
		MACT=GetMenuActionsObject(o.relatedTarget)
		MACT.km.Dvo.where.MX=o.clientX
		MACT.km.Dvo.where.MY=o.clientY
	}
	
	this.GetTopKm = function()
	{
		return this.km.GetTopMenu()
	}

	this.Assess = function(mlox,mloy)
	{
		disty = mlox - posX
		distx = mloy - posY 
		//this.direction = Math.atan2(disty, distx)
		this.distance=Math.sqrt( (distx * distx) + (disty * disty) )
		return this.distance 
	}


	n.onmouseover=function (o) 
	{
		MACT=GetMenuActionsObject(o.relatedTarget)
		MACT.MousePosUpdate(o)

		l=n.firstChild
		x=l.offsetLeft+MACT.km.where.x
		y=l.offsetTop+MACT.km.where.y
		w=l.offsetWidth
		h=l.offsetHeight

		textnode=n.firstChild.firstChild
			textnode.DetectCollision = function(wx,wy)
			{
				if (
					((wx >= x) && (wx <= (x+w)) )
						&&
					((wy >= y) && (wy <= (y+h)) )
				)
				{
					if (MACT.Assess(wx,wy) < 50)
						return false
				}
				return true
			}
			ActiveMenuLink=new MenuLinkObject(l,posX,posY)

		overfun=n.firstChild.getAttribute("overfun")
		n.firstChild.firstChild.setAttribute("class","SubMenuButton_Over")
		if (overfun) eval(overfun)
		return false
	}

	n.onmousedown=function (o) 
	{
		MACT=GetMenuActionsObject(this)
		ActiveMenuLink=null
		clickfun=n.firstChild.getAttribute("clickfun")
		if (clickfun) 
		{
			if (RecentlyClicked) return
			RecentlyClicked=true
			setTimeout('UnClickMe()',1000)
			eval(clickfun)
		}

		opensub=n.firstChild.getAttribute("opensub")
		if (opensub) 
		{
			if (RecentlyClicked) return
			RecentlyClicked=true
			setTimeout('UnClickMe()',1000)

			l=n.firstChild
			x=l.offsetLeft+MACT.km.where.x
			y=l.offsetTop+MACT.km.where.y
			w=l.offsetWidth
			h=l.offsetHeight

			MACT.NNodePos=GetNewNodePos(x,y,w,y)
						//new NodePosition(x+w-40,y-10,0,0)

			//if (Diagnostics)
			//	Diagnostics.Write(MACT.km.name+" Opening"+opensub)
			//Whatever("["+x+"x"+y+"]")
			//Whatever("["+w+"x"+h+"]")
			if (!MACT.km.SubKms.Exists(opensub))
				MACT.km.SubKms.Add(opensub,
					new KrunchMenu(MACT.Parking,
						GetObject(MACT.Parking),
						MACT.NNodePos
						//new NodePosition(x+w-40,y-10,0,0)
						,opensub,MACT,
						function (dvo)
						{
							f="UpdatePopupFrame("+dvo.UiObj.id+")"
							setTimeout(f,3000)
						}
					)
				)
		}
		return false
	}
	n.onmouseout=function (o) 
	{
		MACT=GetMenuActionsObject(o.relatedTarget)
		MACT.MousePosUpdate(o)
		ActiveMenuLink=null
		n.firstChild.firstChild.setAttribute("class","SubMenuButton")
		return false
	}


	this.nodelist=this.sub.childNodes
	for (this.mndx=0; this.mndx < this.nodelist.length; this.mndx++)
	{
		this.item=this.nodelist[this.mndx]
		new MenuActions(this.Parking,this.item,this,this.km)
	}



}

	function GetMenuActionsObject(oob)
	{
		if (!oob) return;
		while (!(oob.MenuActionsObject) )
			oob=oob.parentNode
//		Whatever("RU:"+oob.id+oob.MenuActionsObject)
		return oob.MenuActionsObject
	}

