<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE hibernate-mapping PUBLIC "-//Hibernate/Hibernate Mapping DTD//EN" "http://hibernate.sourceforge.net/hibernate-mapping-3.0.dtd">
<hibernate-mapping package="${packageName}">
	<class name="${beanName}" table="${tableName}">
		 <#list fields as p>
		    <#if p.uname=="SID">
					<id name="${p.uname}" column="${p.uname}" type="java.lang.Integer">
						<generator class="identity" />
					</id>
				<#else>
				    <property name="${p.uname}" column="${p.uname}" not-null="<#if p.is_nullable=="YES">false<#else>true</#if>" />
			</#if>
		 </#list>
	</class>
</hibernate-mapping>