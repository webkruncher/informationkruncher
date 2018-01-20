
function Animator(myname,DivObj,start,end,inc,interval,maxw,maxy,path,ext,leadingzeros,loop)
{
	this.DivObj=DivObj;
	this.myname=myname;
	this.inc=inc;
	this.start=start;
	this.end=end;
	this.primed=false;
	this.maxw=maxw
	this.maxy=maxy
	this.interval=interval;
	this.Path=path;
	this.Ext=ext;
	this.ndx=this.start;
	this.imgLeadingZeros=leadingzeros;
	this.loop=loop;
	this.OnDone=null;
	this.bkupOnDone=null;
	setVisible(this.DivObj,false);


	this.Clicked  = function (o)
	{
		if ( this.OnDone ) 
		{
			this.bkupOnDone=this.OnDone
			this.OnDone=null
		} else {
			if ( this.bkupOnDone ) 
			{
				this.OnDone=this.bkupOnDone
				this.OnDone()
			}
		}
		if (this.AnimComplete)
		{
			if (this.interval == 0) 
				this.ResetSpinner()
			f=this.myname+'.Spin()';
			setTimeout(f,this.interval); 
		}
	}

	this.ResetSpinner = function ()
	{
		this.interval='1'
		this.ndx=this.spinstart
	}

	this.Spin = function()
	{
		this.ndx++
		if (this.ndx > this.spinend)
		{
			this.interval=0
			return
		}
		this.imgname=this.Path + this.ndx.zeroFormat(this.imgLeadingZeros, true, false) + this.Ext;
		this.DivObj.style.backgroundImage="url("+this.imgname+")";
		f=this.myname+'.Spin()';
		setTimeout(f,this.interval); 
	}

	this.Animate = function ()
	{
		if ( this.primed == false )
		{
			this.ndx=(this.ndx+this.inc);
			if (this.ndx == this.end) 
			{
				this.primed=true;
				this.ndx=this.start;
				this.DivObj.style.width=this.maxw;
				this.DivObj.style.height=this.maxy;
				style="position:absolute; width:"+this.maxw+"; height:"+this.maxy+"; top:20px; left:300px; "
				//this.DivObj.setAttribute('style',style)
				setVisible(this.DivObj,true);
			}
			this.imgname=this.Path + this.ndx.zeroFormat(this.imgLeadingZeros, true, false) + this.Ext;
			this.DivObj.style.backgroundImage="url("+this.imgname+")";
			f=this.myname+'.Animate()';
			setTimeout(f,1); 
		} else {
			this.imgname=this.Path + this.ndx.zeroFormat(this.imgLeadingZeros, true, false) + this.Ext;
			this.DivObj.style.backgroundImage="url("+this.imgname+")";
			f=this.myname+'.Animate()';
			if (this.loop)
			{
				if (this.ndx == this.end)
				{
					this.ndx=this.start;
				}
				this.ndx=(this.ndx+this.inc);
				setTimeout(f,this.interval); 
			} else {
				if (this.ndx == this.end)
				{
					this.interval=0
					this.AnimComplete=true
					if ( this.OnDone ) this.OnDone()

				} else {
					this.ndx=(this.ndx+this.inc);
					setTimeout(f,this.interval); 
				}
			}
		}
	}
}
