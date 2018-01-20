
var MenuMessages = null

var KludgeOffsetX = 40
var KludgeOffsetY = 00

var HitSparky = false
var MainMenuColor="#7788aa"
var SubMenuColor="#7788ff"

var MainMenuColor="white"
var SubMenuColor="red"

var CloseTimer=500


	function ReWelcome()
	{
		UserMessages.Write("");
		UserMessages.Write("");
		UserMessages.Write("");
		UserMessages.Write("Welcome to WebKruncher")
	}

	function ToggleSparky()
	{
		if (mousetrails) 
		{ 
			mousetrails.Nuke()
			mousetrails=null
			if (MenuMessages) MenuMessages.Write("Sparky is leaving now")
		} else {
			InitMouseTrails(null,null,0)
			FocusOn('DeadTarget')
			UserMessages.Write("");
			UserMessages.Write("");
			UserMessages.Write("Sparky works best ");
			UserMessages.Write("with FireFox");
			if (MenuMessages) MenuMessages.Write("Meet Sparky")
			setTimeout("ReWelcome()",5000);
		}
		return
	}

function scrub(o)
{
	this.obj=o
	this.sc=null
	this.go=function()
	{
		for (this.i=0;this.i<this.obj.childNodes.length;this.i++)
		{
			if (this.obj.childNodes[this.i].nodeName.toLowerCase()=="script")
				eval(this.obj.childNodes[this.i].innerHTML)
			this.sc=new scrub(this.obj.childNodes[this.i]);
			this.sc.go()
		}
	}
}

function LoadedPage(target)
{
	//Diagnostics.MaxLength(20)
	this.sc=new scrub(GetObject('Pages').childNodes[target])
	this.sc.go()
	//UserMessages.Write("Loaded " + target)
	CrumbNav.Buttons(target)
}

function Navigate(PageName,external,popupname,popupfeatures,path)
{
	GetObject("Bread").Crumbs.Navigate(PageName)
	if (PageName == "Blank")
	{
		window.open(external,popupname,popupfeatures)
		return
	}
	if (PageName == "Leave")
	{
		UserMessages.Write("Thank you for visiting")
		window.location=external
		return
	}
	GetObject("EntryMsg").innerHTML=PageName
	//GetObject("RootNode").setAttribute("style","display:none")
	//Diagnostics.Write("Navigating to "+PageName)
	cell=GetObject('Pages')
	targetNode=-1
	for ( ij=0; ij < cell.childNodes.length;ij++) 
	{
		try
		{
			testname=cell.childNodes[ij].getAttribute('pagename')
			if (testname == PageName)
			{
				if (targetNode == -1) targetNode=ij
				cell.childNodes[ij].setAttribute('style','display:block;')
			} else {
				cell.childNodes[ij].setAttribute('style','display:none;')
			}
		} catch (err) {
			Whatever("Error: "+err)
		}
	}

	if (targetNode > -1)
	{
		if (cell.childNodes[targetNode].childNodes.length < 2)
		{
			if (!webkruncherxslt) 
			{
				if (buggness) UserMessages.Write("No page xslt yet");
				setTimeout("Navigate('"+PageName+"',null,null,null,path)",300);
				return;
			}
			cb="LoadedPage("+targetNode+")";
			pn="";
			if (path) pn=path;
			pn+=PageName+".xml";
			new ajax(pn,cell.childNodes[targetNode],cb);
		} else {
			cell.childNodes[targetNode].setAttribute('style','display:block;')
			LoadedPage(targetNode)
		}
	} else {
		LoadedPage(targetNode)
	}
	GetObject("RootNode").setAttribute("style","display:inline")
}

function OpenPage(o)
{
	PageName=o.getAttribute('pagename')
	Navigate(PageName)
}

function MenuLink(o)
{
	PageName=o.getAttribute('pagename')
	if (MenuMessages) MenuMessages.Write('Clicked '+PageName+" id="+o.id)
	o.Clicked(o)
	//crumbs.Write(o.text)
	FocusOn('DeadTarget')
	if (PageName=='Clear') 
	{	
		if (MenuMessages) MenuMessages.Hide()
		if (MenuMessages) MenuMessages.Clear()
		if (MenuMessages) MenuMessages.Show()
		UserMessages.Hide()
		UserMessages.Clear()
		UserMessages.Show()
		return
	}

	if (PageName=='Sparky') 
	{
		if (mousetrails == null)
			if (HitSparky == false)
			{
				// ya, it's a cheat
				afetimes=0
				mx=400
				my=400

				InitMouseTrails(null,null,0)
				HitSparky=true
			}
	}
	if (PageName=='ToggleSparky') 
	{
		if (mousetrails) 
		{ 
			mousetrails.Nuke()
			mousetrails=null
			if (MenuMessages) MenuMessages.Write("Sparky is leaving now")
		} else {
			InitMouseTrails(null,null,0)
			FocusOn('DeadTarget')
			if (MenuMessages) MenuMessages.Write("Meet Sparky")
		}
		return
	}
	if (PageName=='FeedSparky') {FeedSparky(); return; }
	if (PageName=='StarveSparky') {StarveSparky(); return; }

	OpenPage(o)
}

function MenuMouseOver(o)
{
	PageName=o.getAttribute('pagename')
	if (MenuMessages) MenuMessages.Write('Over '+PageName+" id="+o.id)
	ActiveMenuLink=new MenuLinkObject(o,posX,posY)
}

function MenuMouseOut(o)
{
	ActiveMenuLink=null
	//o.MouseOut(o)
}

var	DynamicMenuRootNodePosition=null
var	DynamicMenuRoot=null

function SetupSpinCycle()
{
return;
	setTimeout("Navigate('Home')",3000)
	setTimeout("Navigate('Sparky')",4000)
	setTimeout("Navigate('About')",5000)
	setTimeout("Navigate('Depot')",6000)
//	setTimeout("SetupSpinCycle()",5000)
}


function Skipper()
{
	targetPage="Home";
	hu = window.location.search.substring(1);
	gy = hu.split("&");
	if (gy[1]!='')
	{
		if (gy[1]=='bug') 
		{
			if (!UserMessages) UserMessages=new MsgTable(GetObject('UserMessages'))
			UserMessages.Color("red");
			UserMessages.Background("blue");
			buggness=true;
			setTimeout("ResetUserMessages()",25000);
		}
	}
	if (gy[0]!='')
	{
		targetPage=(gy[0])
	}
	//UserMessages.Write(window.location)
	setTimeout("RetryAjaxInit('"+targetPage+"')",100);
}

function RetryAjaxInit(targetPage)
{
	if (!webkruncherxslt) 
	{
		if (buggness) alert("Ajax init failed, retrying");
		InitializeAjax()
		setTimeout("RetryAjaxInit('"+targetPage+"')",1000);
		return;
	} else { 
		setTimeout("LoadLeftMenu()",100);
		setTimeout("Navigate('"+targetPage+"')",1000)
	}
}

function LoadLeftMenu()
{
	if (!webkruncherxslt) 
	{
		if (buggness) UserMessages.Write("No xslt yet");
		setTimeout("LoadLeftMenu()",300);
		return;
	}
	DynamicMenuRootNodePosition = new NodePosition(10,250,00,00);
	r=GetObject('WholePage')
	new KrunchMenu('MenuRoot',r,DynamicMenuRootNodePosition,'Left',null,null)
}

function GoHomeAndStartSparky()
{
	Navigate("Home")
	InitMouseTrails(null,null,0)
}


function LoadDynamicMenu()
{
	setTimeout("Skipper()",1)
}

function ShowAndHide(toshow,tohide)
{
	sho=GetObject(toshow)
	hid=GetObject(tohide)
	hid.style.display='none';
	sho.style.display='inline';
}

function Show(toshow)
{
	sho=GetObject(toshow)
	sho.style.display='inline';
}

