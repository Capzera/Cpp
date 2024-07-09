${commonJs}
<style>
	table{font-size:14px;}
	.lableName{text-align:right}
</style>
<div>
	<div class="title"> <img src="${base}/resource/images/add.gif">添加信息</div>
</div>
<FORM ACTION="${base}/admin/${tableName}/save" method="POST" onsubmit="return submit_chk()">
	<table width="100%">
	    <#list fields as field>
	      <#if field.uname != "SID">
			  <tr>
			  	 <TD WIDTH="480PX" class="lableName">${field.caption}：</td>
			  	 <TD>
			  	 	<#if field.propertyType=="varchar" || field.propertyType=="int" || field.propertyType=="timestamp" || field.propertyType=="datetime">
			  	 		<input type="text" name="${field.uname}"  class="must reg_txt"><span class="alertWord">*</span><span class="availWord"></span>
			  	 	</#if>
			  	 	<#if field.propertyType=="char">
			  	 		<input type="radio" name="${field.uname}" value="1" checked>是
			  	 		<input type="radio" name="${field.uname}" value="0">否
			  	 	</#if>
			  	 	<#if field.propertyType=="text">
			  	 		<textarea name="${field.uname}" style="width:100%;height:200px;" class="must reg_txt"></textarea><span class="alertWord">*</span><span class="availWord"></span>
			  	 	</#if>
			  	 </td>
		  	</tr>
		  </#if>
		</#list>
		 <tr>
		  	<td colspan="2" align="center" style="padding-top:10px;">
		  		<input type="submit" value=" 提交 ">
		  		<input type="reset" value=" 重置 ">
		  	</td>
		  </tr>
	</table>
</form>