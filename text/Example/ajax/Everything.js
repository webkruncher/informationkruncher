



// All of the JavaScript in this site is a kludge
// much kippple and cruft needs to be elliminated
// tbd: rework the entire site

var MenuRoot="";
var LastAjaxSuccess=null
var FirstLoadedAt=new Date
var CantAjax=false
var HeartBeatInterval=null
var Diagnostics = null
var LockDiagnostics = null
var InitDiagnostics = null
var UserMessages = null
var buggness = false;
var AlertLimiter = 1 
var StartedApplicationAt=new Date()
var ConnectionStatus = true
var LoaderTimeout = 0

function OneMessage(m)
{
	if (AlertLimiter > 1) return
	AlertLimiter+=1
	//alert(m)
}
		
function SystemLocked(sw,msg)
{
	if (sw)
	{
		try
		{
			if (Diagnostics)
				Diagnostics.Write(msg)
			else {
				OneMessage(msg)
			}
		} catch (e) {
			//OneMessage(msg)
		}
	} else {
		if (Diagnostics)
			Diagnostics.Write(msg)
	}
	r=GetObject("WholePage")
	LockMessage=GetObject("SystemLock")
	if (sw)
	{
		ShowNode(false,r)
		ShowNode(true,LockMessage)
		LockMessage.style.display="inline"
		LockMessage.setAttribute("style","display:block;")
		LockDiagnostics.Show()
		LockDiagnostics.Write(msg)
	} else {
		LockDiagnostics.Write(msg)
		ShowNode(true,r)
		ShowNode(false,LockMessage)
	}
}


function Whatever(msg,grace)
{
	if (InitDiagnostics) 
	{
		InitDiagnostics.Write(msg)
	}
	if (Diagnostics)
		Diagnostics.Write(msg)
	if ( (!Diagnostics) && (!UserMessages) )
	{
		sinceopened=Now.getTime()-FirstLoadedAt.getTime()
		if (sinceopened > grace)
			OneMessage(msg)
	}
}

function AjaxFailure(msg,grace)
{
	//setTimeout("Navigate('Verbose')",3000)
	//txt="AJax Failure:" + msg
	//Whatever(msg,grace)
}

var last_viewportwidth=0
var last_viewportheight=0
function ViewPortManager()
{
	GetViewport ()
	if (
		(last_viewportwidth!=viewportwidth)
			||
		(last_viewportheight!=viewportheight)
	)
	{
		if ( (last_viewportheight) && (last_viewportwidth) )
		{
			s="/index.html?";
			s+=viewportwidth;
			s+="x";
			s+=viewportheight;
			window.location=s;
		} else {
			last_viewportwidth=viewportwidth;
			last_viewportheight=viewportheight;
		}
	} else {
		last_viewportwidth=viewportwidth;
		last_viewportheight=viewportheight;
	}
}
//setInterval("ViewPortManager()",900)


function buggy()
{
	if (buggness ) buggness =false;
	else buggness =true;
	if (buggness) 
	{
		diagnostics_root.style.display="inline";
		SparkyTextDiv.style.display="none";
	} else {
		diagnostics_root.style.display="none";
		SparkyTextDiv.style.display="inline";
	}
}


var ambionceness = true;
function ambionce()
{
	if (ambionceness) ambionceness=false;
	else ambionceness=true;
          if (ambionceness)
          {
               sw="inline"          
          }else {
               sw="none"
          }
          //SteelBkg.style.display=sw;
          SteelBkgTop.style.display=sw;
          SteelBkgLeft.style.display=sw;
          SteelBkgRight.style.display=sw;
          SteelBkgBottom.style.display=sw;
}

function wklcollide(d)
{
	if (d < 80)
		return false
	else return true
}




function ToBeDone()
{
	InitializeAjax()
	setTimeout('AfterAjax2()',100)
}

var MenuLoaded=0
var MenuChecker=null
function MenuCheck()
{
	if (UserMessages)
	{
		if (!MenuLoaded)
		{	
			//UserMessages.Write("Error loading menu, trying again");
			LoadProgress("Menu...")
			LoadLeftMenu();		
			clearInterval(MenuChecker)
		}
	}
}

function AfterAjax2()
{

	LoadProgress("Typesetting...")
	MenuChecker=setInterval("MenuCheck()",100);
	LoadProgress(">>")

	if (!UserMessages)
	{
		UserMessages=new MsgTable(GetObject('UserMessages'))
		if (buggness) 
		{
			UserMessages.Color("purple");
			UserMessages.Background("black");
		}
		UserMessages.MaxLength(3)
	}

	if (buggness) UserMessages.Write("Testing in progress")


	if (webkruncherxslt_txt.length < 1)
	{
		if (buggness) UserMessages.Write("Loading transformer")
		InitializeAjax()
		setTimeout('AfterAjax2()',100)
		return
	} 
	LoadProgress("...")
		//InitDiagnostics.Write("SystemLocked div: "+GetObject('SystemLock'))
		//LockDiagnostics=new MsgView(GetObject('SystemLock').childNodes[1])
		//InitDiagnostics.Write("Lock messages initialized")
		//LockDiagnostics.MaxLength(5)
		//InitDiagnostics.Write("Showing lock diagnostics")
		GetObject("WebKruncherLogo").id='WebKruncherLogo'
		GetObject("WebKruncherLogo").setAttribute('collisions','wklcollide')
	LoadProgress("---")
		//crumbs=new Breadcrumbs(GetObject('CrumbTable'))
		//crumbs.MaxLength(3)
		//o=GetObject("PreInitDiagnosticsDiv")
		//o.style.display="none"
		//Diagnostics=new MsgView(GetObject('DiagnosticsDiv').childNodes[1])
		//Diagnostics.Show()
		//Diagnostics.MaxLength(13)
		//UserMessages=new MsgTable(GetObject('UserMessages'))
		//UserMessages.MaxLength(3)
	FocusOn('DeadTarget')
	setVisible(GetObject('MastheadText'),false);
	LoadProgress("***")

	if (1)
	{
		WKLw=400
		WKLh=100
		WKLw+="px"
		WKLh+="px"
	} else {
		//cs=window.getComputedStyle(document.documentElement,null);
		cs=window.getComputedStyle(GetObject("WebKruncherLogo"),null);
		WKLw=Math.floor(cs.getPropertyCSSValue('width').getFloatValue(5))
		WKLh=Math.floor(cs.getPropertyCSSValue('height').getFloatValue(5))
		WKLw+="px"
		WKLh+="px"
	}

	LoadProgress("+++")

	setTimeout( "LogoAnimator()", 5000 )
	anim = new Animator('anim',  GetObject('WebKruncherLogo') ,1,31, 1,150,WKLw,WKLh,"Steel/Logo/",".png",4,false);
//	anim.spinstart=12
//	anim.spinend=31
//	anim.Animate();
//	anim.OnDone = function()
//	{
//		setTimeout("anim.Spin()", 1000 )
//	}
//
	LoadProgress("000")
	UserMessages.Write("Welcome to WebKruncher")
	LoadDynamicMenu()
	FocusOn('DeadTarget')
	if (buggness) setTimeout("ResetUserMessages()",5000);
}

function LogoAnimator()
{
	//anim = new Animator('anim',  GetObject('WebKruncherLogo') ,1,31, 1,250,WKLw,WKLh,"Steel/Logo/",".png",4,false);
	anim.spinstart=12
	anim.spinend=31
	anim.Animate();
	anim.OnDone = function()
	{
		this.ndx=this.spinstart
		this.interval=100
		this.AnimComplete=false
		anim.Animate();
		//setTimeout("anim.Spin()", 1000 )
	}
}

function ResetUserMessages()
{
	UserMessages.Hide()
	UserMessages=new MsgTable(GetObject('UserMessages'))
	UserMessages.Color("white")
	UserMessages.Write('Welcome to WebKruncher')
}

var flipped=0
function AfterFlip()
{
	try
	{
		LoadProgress("Initializing..")
		if (webkruncherxslt_txt.length < 1)
		{
			LoadProgress("Initializing. " + flipped) 
			new ajax('','',null,null);
			flipped++
			setTimeout('AfterFlip()',1000)
			return
		} 
		//LoadProgress("Initializing...")
		GetViewport() 
		emo="display:inline;position:absolute;top:"+(viewportheight-100)+";left:"+(viewportwidth-400)+";"
		GetObject("EntryMsg").setAttribute("style",emo)
		ShowNode(true,"PreInitDiagnosticsDiv")
		//InitDiagnostics=new MsgView(GetObject('PreInitDiagnosticsDiv').childNodes[1])
		//InitDiagnostics.CantHide()
		//InitDiagnostics.Show()
		//InitDiagnostics.Write("Loading")
		//ShowNode(false,"DefaultWelcomeMessage")

		LoadProgress("Initializing...")
		//if (InitDiagnostics) InitDiagnostics.Write("index")
		new ajax("index.xml",GetObject("RootNode"),ToBeDone,null)
		//LoadProgress("Initializing....")
	}
	catch(e) {
		LoadProgress(e)
	}
}

function LoadProgress(msg)
{
	//GetObject("EntryMsg").setAttribute("style","display:inline")
	GetObject("EntryMsg").innerHTML=msg
	if (LoaderTimeout)
		clearTimeout(LoaderTimeout)
	LoaderTimeout=setTimeout("CloseLoader()", 3000)
}

function LoadError(msg)
{
	GetObject("ErrorLoadMsg").setAttribute("style","display:block; float:right;color:red")
	GetObject("ErrorLoadMsg").innerHTML=msg
}

function CloseLoader()
{
	GetObject("EntryMsg").setAttribute("style","display:none")
	LoaderTimeout=0
}

function BackFlip() 
{
	GetObject("EntryMsg").innerHTML="Initializing."
	InitializeAjax()
	setTimeout('AfterFlip()',100)
}




//


