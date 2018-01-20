<?xml version="1.0" encoding="utf-8"?>


<xsl:stylesheet version="1.0"
	xmlns:xhtml="http://www.w3.org/1999/xhtml" 
	xmlns="http://www.w3.org/1999/xhtml" 
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	xmlns:xs="http://www.w3.org/2001/XMLSchema"
>

	

	<xsl:template match="/Ping">
		<xsl:value-of select="." />
	</xsl:template>

	<xsl:template match="/Page">
		<xsl:variable name="PageId"><xsl:value-of select="generate-id()" /></xsl:variable>
		<xsl:for-each select="Script" >
			<script>
				<xsl:value-of select="." />
			</script>
		</xsl:for-each>
		<xsl:for-each select="t" >
		<xsl:if test=".!=''">
			<div id="PageTitle" class="PageTitle" >
				<input onclick="CrumbNav.Back()" class="BckNavButton" type="button" id="BackButton" value="&lt;&lt;" />
				<div style="display:inline;"><xsl:value-of select="." /></div>
				<input onclick="CrumbNav.Fwd()" class="FwdNavButton" type="button" id="FwdButton" value="&gt;&gt;" />
			</div>
		</xsl:if>
		</xsl:for-each>
		<xsl:for-each select="Content" >
			<div><p><xsl:call-template name="Content" /></p></div>
		</xsl:for-each>
		<xsl:for-each select="Html" >
			<div><p>
				<xsl:copy-of select="." /> 
			</p></div>
		</xsl:for-each>
	</xsl:template>


	<xsl:template match="/html">
		<head>
			<script type="text/javascript" src="ajax/ajax.js" ></script>
			<script type="text/javascript" src="ajax/MenuCore.js" ></script>
			<script type="text/javascript" src="ajax/Menu.js" ></script>
		</head>

		<body id="BodyTag" >
			<div style="position:absolute;top=00px;left=00px;size=1; width=0px;display:none;"> 
				<input type="nothing" id="DeadTarget" /> 
			</div>
			<div id="WholePage" >
				<div id="SparkyRoot" />
				<div ID="SteelBkg" />
				<div class="SteelBkgTop" ></div><div class="SteelBkgLeft" ></div><div class="SteelBkgRight" ></div><div class="SteelBkgBottom" ></div>
				<div id="Masthead" style="">
					<div id="MastheadText"></div>
					<div id="WebKruncherLogo" onclick='anim.Clicked(this)' onmouseover='MenuMouseOver(this)' onmouseout='MenuMouseOut(this)' ></div>
				</div>
					<div class="Page">
						<xsl:for-each select="Pages">
							<xsl:call-template name="PageContent" />
						</xsl:for-each>
						<xsl:for-each select="Diagnostics">
							<xsl:call-template name="Diagnostics" />
						</xsl:for-each>
					</div>
					<div class="PageFooter"> <div class="PageFooterText" ><xsl:value-of select="Page/Footer" /></div> </div>
				</div> 
				<xsl:call-template name="MessageViews" />
				<xsl:call-template name="SystemLockMessages" />
		</body>
	</xsl:template>





	<xsl:template name="DepotLeftMenu">
		<xsl:for-each select="item">
			<xsl:call-template name="LeftMenu" />
		</xsl:for-each>
		<script language="JavaScript">
			var SpareMenu=null
			var tag="<xsl:value-of select="@root" />"
			//var tag='SpareRoot'
			SpareMenu = new NodePosition(10,25,00,00);
			r=GetObject(tag)
			new KrunchMenu('SpareRoot',r,SpareMenu,tag,null,null)
		</script>
	</xsl:template>


	<xsl:template name="DepotPopUpMenu">
		<xsl:for-each select="item">
			<xsl:call-template name="PopUpMenu" />
		</xsl:for-each>
	</xsl:template>


	<xsl:template name="Paragraph">
		<xsl:value-of select="text()" /> 
		<p class="Paragraph" >
			<xsl:for-each select="*">
				<xsl:choose>
					<xsl:when test="name()='text'" >   <xsl:call-template name="Text" /> </xsl:when>
					<xsl:when test="name()='whitespace'" >   <xsl:call-template name="Whitespace" /> </xsl:when>
					<xsl:otherwise> <xsl:copy-of select="." /> </xsl:otherwise>
				</xsl:choose>
			</xsl:for-each>
		</p>
	</xsl:template>


	<xsl:template name="Text">
		<xsl:value-of select="." />
	</xsl:template>
	<xsl:template name="Whitespace">
		<xsl:text> </xsl:text>
	</xsl:template>

	<xsl:template name="Content">
		<xsl:for-each select="*"> 
			<xsl:choose>
				<xsl:when test="name()='DepotLeftMenu'" > <xsl:call-template name="DepotLeftMenu" /> </xsl:when>
				<xsl:when test="name()='DepotPopUpMenu'" > <xsl:call-template name="DepotPopUpMenu" /> </xsl:when>
				<xsl:when test="name()='text'" >   <xsl:call-template name="Text" /> </xsl:when>
				<xsl:when test="name()='whitespace'" >   <xsl:call-template name="Whitespace" /> </xsl:when>
				<xsl:otherwise> <xsl:copy-of select="." /> </xsl:otherwise>
			</xsl:choose>
		</xsl:for-each>
	</xsl:template>
	

	<xsl:template name="PageContent">
		<div id="Pages" >
			<xsl:for-each select="Page" >
				<div>
					<xsl:attribute name="id"><xsl:value-of select="generate-id()" /></xsl:attribute>
					<xsl:attribute name="pagename"><xsl:value-of select="@name" /></xsl:attribute>
					<xsl:if test="@name !='Home' ">
						<xsl:attribute name="style">display:none</xsl:attribute>
					</xsl:if>
					<xsl:for-each select="Content" >
						<div><p><xsl:call-template name="Content" /></p></div>
					</xsl:for-each>
				</div>
			</xsl:for-each>
		</div>
	</xsl:template>

	<xsl:template name="Diagnostics">
		<div id="MenuMessagesDiv" class='diagnostics' > <div class='hidden_diagnostics'>Menu</div><table /> </div>
		<!--div id="DiagnosticsDiv" > <div class='hidden_diagnostics' >Diagnostics</div><table /> </div-->
		<div id="DiagnosticsDiv" > <div class='diagnostics' >Diagnostics</div><table /> </div>
	</xsl:template>

	<xsl:template name="MessageViews" >
		<div style="position:absolute; top:100px; left:200px; display:none;" >
			<a class="BreadCrumbTag" >
					<xsl:attribute name="readonly" >true</xsl:attribute>
					<xsl:attribute name="id" >BreadCrumbs_Link</xsl:attribute>
					<xsl:attribute name="onclick" >MenuLink(this)</xsl:attribute>
					<xsl:attribute name="onmouseover" >MenuMouseOver(this)</xsl:attribute>
					<xsl:attribute name="onmouseout" >MenuMouseOut(this)</xsl:attribute>
				>>	
			</a>
			<div id="CrumbDiv" class='BreadCrumbs' > <table id="CrumbTable" /> </div>
		</div>

		<div id="UserMessagesDiv" class='UserMessages' > <table id="UserMessages" /> </div>
	</xsl:template >

	<xsl:template name="SystemLockMessages" >
		<div id="SystemLock"  > <div >System Locked:</div><table /> </div>
	</xsl:template >

	<xsl:template match="*[@type = 'rootmenu']" >
		<div class="RootMenu">
			<xsl:for-each select="item">
				<xsl:call-template name="LeftMenu" />
			</xsl:for-each>
		</div>
	</xsl:template>


	<xsl:template match="*[@type = 'submenu']" >
		<xsl:for-each select="item">
			<xsl:call-template name="PopUpMenu" />
		</xsl:for-each>
	</xsl:template>


	<xsl:template name="OnOff">
		<xsl:variable name="Identifire"><xsl:value-of select="generate-id()" /></xsl:variable>
		<div class="MenuFrame" >
			<xsl:attribute name="id" >FRAME_<xsl:value-of select="name" /><xsl:value-of select="$Identifire" /></xsl:attribute>
			<div class="SubMenu" >
				<xsl:attribute name="id" ><xsl:value-of select="name" />_<xsl:value-of select="$Identifire" /></xsl:attribute>
				<xsl:attribute name="pagename" ><xsl:value-of select="$Identifire" /></xsl:attribute>
				<xsl:attribute name="overfun" ><xsl:value-of select="o" /></xsl:attribute>
				<xsl:attribute name="clickfun" ><xsl:value-of select="clicked" /></xsl:attribute>
				<xsl:attribute name="submenu" ><xsl:value-of select="sub" /></xsl:attribute>
				<div class="SubMenuButton" >
					<xsl:attribute name="id" >Txt_<xsl:value-of select="name" />_<xsl:value-of select="$Identifire" /></xsl:attribute>
					<div>
						<xsl:value-of select="name" />
					</div>
					<div>
						<xsl:for-each select="subitem"><xsl:call-template name="LMenu" /></xsl:for-each>
					</div>
				</div>
				<div class="SubMenu" >
					<xsl:attribute name="id" >SUB_<xsl:value-of select="name" />_<xsl:value-of select="$Identifire" /></xsl:attribute>
					<xsl:for-each select="item"><xsl:call-template name="LMenu" /></xsl:for-each>
					<xsl:for-each select="OnOff"><xsl:call-template name="OnOff" /></xsl:for-each>
				</div>
			</div>
		</div>
	</xsl:template>

	<xsl:template name="LMenu">
		<xsl:variable name="Identifire"><xsl:value-of select="generate-id()" /></xsl:variable>
		<div class="MenuFrame"> 
			<xsl:attribute name="id" >FRAME_<xsl:value-of select="name" /><xsl:value-of select="$Identifire" /></xsl:attribute>
			<xsl:attribute name="frame" ><xsl:value-of select="$Identifire" /></xsl:attribute>
			<div class="SubMenu" >
				<xsl:attribute name="id" ><xsl:value-of select="name" />_<xsl:value-of select="$Identifire" /></xsl:attribute>
				<xsl:attribute name="pagename" ><xsl:value-of select="$Identifire" /></xsl:attribute>
				<xsl:attribute name="overfun" ><xsl:value-of select="o" /></xsl:attribute>
				<xsl:attribute name="clickfun" ><xsl:value-of select="clicked" /></xsl:attribute>
				<xsl:attribute name="opensub" ><xsl:value-of select="opensub" /></xsl:attribute>
				<div class="SubMenuButton" >
					<xsl:attribute name="id" >Txt_<xsl:value-of select="name" />_<xsl:value-of select="$Identifire" /></xsl:attribute>
					<xsl:choose>
						<xsl:when test="text=''" ><hr /></xsl:when>
						<xsl:when test="text='Main'" >
							<div style="width:100%;color:silver;border:outset;padding:4px;text-align:center;cursor:crosshair;font-style:italic;font-weight:bold;" >Menu</div>
						</xsl:when>
						<xsl:otherwise><xsl:value-of select="text" /></xsl:otherwise>
					</xsl:choose>
					<div>
						<xsl:for-each select="subitem"><xsl:call-template name="LMenu" /></xsl:for-each>
					</div>
				</div>
				<div class="SubMenu" >
					<xsl:attribute name="id" >SUB_<xsl:value-of select="name" />_<xsl:value-of select="$Identifire" /></xsl:attribute>
					<xsl:for-each select="item"><xsl:call-template name="LMenu" /></xsl:for-each>
					<xsl:for-each select="OnOff"><xsl:call-template name="OnOff" /></xsl:for-each>
				</div>
			</div>
		</div>
	</xsl:template>

	<xsl:template name="LeftMenu">
		<div style="color:white;"> <xsl:value-of select="name" /></div>
		<div>
			<xsl:attribute name="id" ><xsl:value-of select="name" /></xsl:attribute>
			<xsl:call-template name="LMenu" />
		</div>
	</xsl:template>


	<xsl:template name="PopUpMenu">
		<xsl:variable name="Identifire"><xsl:value-of select="generate-id()" /></xsl:variable>
		<div 
			class="PopUpMenuFrame"
			onmouseout="MenuFrameMouseOut(this)"
		>
			<xsl:attribute name="name" ><xsl:value-of select="name" />_<xsl:value-of select="$Identifire" /></xsl:attribute>
			<xsl:attribute name="id" >POPUP_<xsl:value-of select="name" />_<xsl:value-of select="$Identifire" /></xsl:attribute>
			<div class="PopUpMenuTitle" > <xsl:value-of select="name" /></div>
			<div class="PopUpMenu"	>
				<div>
					<xsl:attribute name="id" ><xsl:value-of select="name" /></xsl:attribute>
					<xsl:call-template name="LMenu" />
				</div>
			</div>
		</div>
	</xsl:template>


</xsl:stylesheet>

