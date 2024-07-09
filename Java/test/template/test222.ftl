
${listTree(categoryList)}

<#function listTree rootNodeList>
    <#list rootNodeList as node>
    	<#return node.CATEGORY_NAME>
    	<#if node.children?size gt 0>
    		${listTree(node.children)}
    	</#if>
    </#list>
</#function>