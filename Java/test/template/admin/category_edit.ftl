<#include "common/common-js.ftl">
<div>
	<div class="title"><img src="${base}/resource/images/add.gif"> 修改分类</div>
</div>
<#if message!=null>
	<div>
		<span class="STYLE10">
	  	提示信息:${message}
	  </span>
	</div>
</#if>  
<div align="center">
<FORM ACTION="${base}/admin/category/update" method="POST" onsubmit="return submit_chk()" encType=multipart/form-data>
	<input type="hidden" name="SID" value="${category.SID}">
	<input type="hidden" name="C_TIME" value="${category.c_TIME?string("yyyy-MM-dd HH:mm:ss")}">
	<table width="100%" cellpadding="0" cellspacing="0" class="form_table">
		  <tr>
		  	 <TD WIDTH="150PX" class="lableName">分类名称：</td>
		  	 <TD>
		  	 	<input type="text" id="user_name" class="must reg_txt" name="CATEGORY_NAME" value="${category.CATEGORY_NAME}" ><span class="alertWord">*</span><span id="name_pan"></span><span class="availWord"></span>
		  	 </td>
		  	 <TD WIDTH="150PX" class="lableName">所属分类：</td>
		  	 <TD>
		  	 	<select name="PARENT_SID">
		  	 		<OPTION VALUE="0">--根分类--</OPTION>
		  	 		<#list categoryList as list>
		  	 		    <OPTION VALUE="${list.SID}" <#if list.SID == category.PARENT_SID>selected</#if>><#if list.LEVEL gt 0><#list 0..list.LEVEL as level>&nbsp;&nbsp;</#list></#if>${list.CATEGORY_NAME}</OPTION>
		  	 		</#list>
		  	 	</SELECT><span class="alertWord">*</span><span id="name_pan"></span><span class="availWord"></span>
		  	 </td>
		  </tr>
		   <tr>
		   	 <TD class="lableName">菜单栏显示：</td>
		  	 <TD>
				 <input type="radio" class="must reg_txt"  name="IS_SHOW" VALUE="0" <#if category.IS_SHOW == "0">checked</#if>> 否
				 <input type="radio" class="must reg_txt"  name="IS_SHOW" VALUE="1" <#if category.IS_SHOW == "1">checked</#if>> 是
		  	 </td>	
		  	 
		  	<TD WIDTH="150PX" class="lableName">显示位置：</td>
		  	 <TD>
		  	    
		  	 	<select name="SHOW_POSITION">
		  	 		<OPTION value=0>--默认--</OPTION>
		  	 		<#list 1..5 as position>
		  	 		    <OPTION VALUE="${position}" <#if category.SHOW_POSITION == position>selected</#if>>${position}</OPTION>
		  	 		</#list>
		  	 	</SELECT><span class="alertWord">*</span><span id="name_pan"></span><span class="availWord"></span>
		  	 	<span class="lableName" style="color:red">首页工作区显示位置</span>
		  	 </td>			 			 
		  </tr>
		  <tr>
		   	 <TD class="lableName">显示方式：</td>
		  	  <TD align="left" style="font-size:12px;">
				 <input type="radio" class="must reg_txt"  name="SHOW_VIEW" VALUE="1" <#if category.SHOW_VIEW =="1"> CHECKED</#if>> 列表页
				 <input type="radio" class="must reg_txt"  name="SHOW_VIEW" VALUE="0" <#if category.SHOW_VIEW =="0"> CHECKED</#if>> 详细页
		  	 </td>	
		  	 
		  	 <TD WIDTH="150PX" class="lableName"></td>
		  	 <TD>
		  	 </td>		 			 
		  </tr>
		  <tr>
		  	<td colspan="4" align="center" style="padding-top:10px;">
		  		<input type="submit" value=" 提交 ">
		  		<input type="reset" value=" 重置 ">
		  	</td>
		  </tr>
	</table>
</form>
</div>
