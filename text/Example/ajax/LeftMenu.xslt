<?xml version="1.0" encoding="utf-8"?>


<xsl:stylesheet version="1.0"
	xmlns:xhtml="http://www.w3.org/1999/xhtml" 
	xmlns="http://www.w3.org/1999/xhtml" 
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
	xmlns:xs="http://www.w3.org/2001/XMLSchema"
>

	<xsl:template match="/LeftMenu">
		<xsl:for-each select="item">
			<xsl:call-template name="LMenu" />
		</xsl:for-each>
	</xsl:template>

	<xsl:template name="LMenu">
		<xsl:variable name="Identifire"><xsl:value-of select="generate-id()" /></xsl:variable>
		<div class="LMenu" >
			<xsl:attribute name="pagename" >xx<xsl:value-of select="n" /></xsl:attribute>
			<xsl:attribute name="id" >lmmm<xsl:value-of select="$Identifire" /></xsl:attribute>
			<xsl:attribute name="onclick" >MenuLink(this)</xsl:attribute>
			<xsl:attribute name="onmouseover" >MenuMouseOver(this)</xsl:attribute>
			<xsl:attribute name="onmouseout" >MenuMouseOut(this)</xsl:attribute>
			<xsl:value-of select="t" />
			<div class="SubMenu" >
				<xsl:for-each select="item"><xsl:call-template name="LMenu" /></xsl:for-each>
			 </div>
		</div>
	</xsl:template>

	<xsl:template name="xLMenu">
		<xsl:variable name="Identifire"><xsl:value-of select="generate-id()" /></xsl:variable>
		<tbody>
			<tr>
				<td>
					<a class="LMenuLink" >
						<xsl:attribute name="pagename" ><xsl:value-of select="n" /></xsl:attribute>
						<xsl:attribute name="location" >uncle</xsl:attribute>
						<xsl:attribute name="id" ><xsl:value-of select="$Identifire" /></xsl:attribute>
						<xsl:attribute name="readonly" >true</xsl:attribute>
						<xsl:attribute name="onclick" >MenuLink(this)</xsl:attribute>
						<xsl:attribute name="onmouseover" >MenuMouseOver(this)</xsl:attribute>
						<xsl:attribute name="onmouseout" >MenuMouseOut(this)</xsl:attribute>
						<xsl:value-of select="t" />
					</a>


					<table class="SubMenu" >
						<xsl:attribute name="sub" ><xsl:value-of select="sub" /></xsl:attribute>
						<xsl:attribute name="id" >sub_<xsl:value-of select="count(.)" />_<xsl:value-of select="$Identifire" /></xsl:attribute>
						<xsl:attribute name="pagename" >sub_<xsl:value-of select="n" /></xsl:attribute>
						<xsl:attribute name="location" >unk</xsl:attribute>
						<xsl:for-each select="item"><xsl:call-template name="LMenu" /></xsl:for-each>
					 </table>
				</td>
			</tr>
		</tbody>

	</xsl:template>

</xsl:stylesheet>

