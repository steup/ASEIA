<xsl:stylesheet version="1.0" xmlns="http://graphml.graphdrawing.org/xmlns" 
															xmlns:ns="http://graphml.graphdrawing.org/xmlns" 
															xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
															xmlns:y="http://www.yworks.com/xml/graphml"
>
	<xsl:output method="xml" indent="yes"/>
	<xsl:strip-space elements="*" />

	<xsl:template name="genKeyType">
		<xsl:element name="key">
			<xsl:attribute name="for">node</xsl:attribute>
			<xsl:attribute name="id">d0</xsl:attribute>
			<xsl:attribute name="attr.name">type</xsl:attribute>
			<xsl:attribute name="attr.type">string</xsl:attribute>
		</xsl:element>
	</xsl:template>

	<xsl:template name="genKeyName">
		<xsl:element name="key">
			<xsl:attribute name="for">node</xsl:attribute>
			<xsl:attribute name="id">d1</xsl:attribute>
			<xsl:attribute name="attr.name">name</xsl:attribute>
			<xsl:attribute name="attr.type">string</xsl:attribute>
		</xsl:element>
	</xsl:template>

	<xsl:template match="ns:graphml">
		<xsl:copy>
  		<xsl:apply-templates select="@*"/>
			<xsl:call-template name="genKeyType"/>
			<xsl:call-template name="genKeyName"/>
  		<xsl:apply-templates select="node()"/>
 		</xsl:copy>
	</xsl:template>


	<xsl:template match="y:NodeLabel">
		<xsl:element name="data">
			<xsl:attribute name="key">d1</xsl:attribute>
			<xsl:apply-templates select="text()"/>
		</xsl:element>
	</xsl:template>

	<xsl:template match="y:Shape">
		<xsl:element name="data">
			<xsl:attribute name="key">d0</xsl:attribute>
		<xsl:value-of select="./@type"/>
		</xsl:element>
	</xsl:template>

	<xsl:template match="y:*">
		<xsl:apply-templates />
	</xsl:template>
	<xsl:template match="ns:key"/>
	<xsl:template match="ns:data">
		<xsl:apply-templates />
	</xsl:template>

	<xsl:template match="@*|node()|text()">
		<xsl:copy>
  		<xsl:apply-templates select="@*|node()"/>
 		</xsl:copy>
	</xsl:template>
</xsl:stylesheet>
