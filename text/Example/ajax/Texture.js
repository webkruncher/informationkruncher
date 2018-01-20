var TexturePath="./particles/"
var TextureExt=".png"
var imgLeadingZeros=4
function TextureMap(depth)
{
	this.ImageIndex=1
	this.depth=depth
	this.GetSize = function ()
	{
		if (this.depth == 2) return '2';
		if (this.depth == 1) return '1';
		if (this.depth == 0) return '0';
	}

	this.GetColor = function ()
	{
		r=Random(3)+1;
		if (r==1) return 'b';
		if (r==2) return 'y';
		if (r==3) return 'g';
	}

	this.Color=this.GetColor()
	this.Size=this.GetSize()

	this.GetUrl = function()
	{
		//n=TexturePath+this.GetSize()+"/"+this.GetColor()+"/"+this.ImageIndex.zeroFormat(imgLeadingZeros, true, false) 	+ TextureExt;
		n=TexturePath+this.Size+"/"+this.Color+"/"+this.ImageIndex.zeroFormat(imgLeadingZeros, true, false) 	+ TextureExt;
		this.ImageIndex=this.ImageIndex+1;
		if (this.ImageIndex == 5) this.ImageIndex=1;
		return n;
	}
}

