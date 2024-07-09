<#include "common/common-js.ftl">
<div>
	<div class="title"><img src="${base}/resource/images/add.gif"> 添加报警器信息</div>
</div>
<#if message!=null>
	<div>
		<span class="STYLE10">
	  	提示信息:${message}
	  </span>
	</div>
</#if>  
<div align="center">
<FORM ACTION="${base}/admin/alarm/save" method="POST" onsubmit="return submit_chk()" encType=multipart/form-data>
	<table width="100%" cellpadding="0" cellspacing="0" class="form_table">
		  <tr>
		  	 <TD WIDTH="150PX" class="lableName">名称：</td>
		  	 <TD>
		  	 	<input type="text" id="user_name" class="must reg_txt" name="NAME" ><span class="alertWord">*</span><span id="name_pan"></span><span class="availWord"></span>
		  	 </td>
		  	 <TD WIDTH="150PX" class="lableName">类型编号：</td>
		  	 <TD>
		  	 	<input type="text" id="user_name" class="must reg_txt" name="TYPE_NUMBER" ><span class="alertWord">*</span><span id="name_pan"></span><span class="availWord"></span>
		  	 </td>
		  </tr>
		   <tr>
		   	 <TD class="lableName">提供商：</td>
		  	 <TD>
				 <input type="text" class="must reg_txt"  name="PROVIDER" ></input><span class="alertWord">*</span><span class="availWord"></span>
		  	 </td>	
		  	 
		  	 <TD class="lableName">&nbsp;</td>
		  	 <TD>
		  	    &nbsp;
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
