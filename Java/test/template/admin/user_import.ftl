<#include "common/common-js.ftl">
<div>
	<div class="title"><img src="${base}/resource/images/add.gif"> 导入用户信息</div>
</div>
<#if message!=null>
	<div>
		<span class="STYLE10">
	  	提示信息:${message}
	  </span>
	</div>
</#if>  
<div align="center">
<FORM ACTION="${base}/admin/user/import" method="POST" onsubmit="return submit_chk()" encType="multipart/form-data">
	<table width="100%" cellpadding="0" cellspacing="0" class="form_table">
		  <tr>
		  	 <TD WIDTH="150PX" class="lableName">上传用户：</td>
		  	 <TD>
		  	 	<input type="file" id="userListExcel" class="must reg_txt" name="userListExcel" ><span class="alertWord">*</span><span id="name_pan"></span><span class="availWord"></span>
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
