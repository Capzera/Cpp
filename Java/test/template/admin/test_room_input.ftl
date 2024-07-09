<#include "common/common-js.ftl">
<div>
	<div class="title"><img src="${base}/resource/images/add.gif"> 添加教室信息</div>
</div>
<#if message!=null>
	<div>
		<span class="STYLE10">
	  	提示信息:${message}
	  </span>
	</div>
</#if>  
<div align="center">
<FORM ACTION="${base}/admin/test/room/save" method="POST" onsubmit="return submit_chk()" encType = "multipart/form-data">
	<table width="100%" cellpadding="0" cellspacing="0" class="form_table">
		  <tr>
		  	 <TD WIDTH="150PX" class="lableName">教室编号：</td>
		  	 <TD>
		  	 	<input type="text" id="user_name" class="must reg_txt" name="TEST_ROOM_NO" ><span class="alertWord">*</span><span id="name_pan"></span><span class="availWord"></span>
		  	 </td>
		  	 <TD WIDTH="150PX" class="lableName">教室名称：</td>
		  	 <TD>
		  	 	<input type="text" id="user_name" class="must reg_txt" name="NAME" ><span class="alertWord">*</span><span id="name_pan"></span><span class="availWord"></span>
		  	 </td>
		  </tr>
		    <tr>
		   	 <TD class="lableName">座位数：</td>
		  	 <TD>
				 <input type="text" class="must reg_txt"  onblur="number_chk(this)"  name="SEATS" ></input><span class="alertWord">*</span><span class="availWord"></span>
		  	 </td>	
		  	 
		  	 <TD class="lableName">&nbsp;</td>
		  	 <TD>
		  	    &nbsp;
			 </td>			 			 
		  </tr>
		  <tr>
		  	<TD class="lableName">软件设备：</td>
		  	<TD colspan="3">
		  	    <textarea  class="must reg_txt" rows="3" cols="95" name="SOFT_DEVICE"></textarea><span class="alertWord">*</span><span class="availWord"></span>
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
