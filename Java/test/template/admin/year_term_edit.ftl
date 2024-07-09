<#include "common/common-js.ftl">

<script type="text/javascript" src="${base}/resource/common/DatePicker/WdatePicker.js"></script>

<div>
	<div class="title"><img src="${base}/resource/images/add.gif"> 基础参数配置</div>
</div>
<#if message!=null>
	<div>
		<span class="STYLE10">
	  	提示信息:${message}
	  </span>
	</div>
</#if>  
<div align="center">
<FORM ACTION="${base}/admin/baseparam/update" method="POST" onsubmit="return submit_chk()" encType=multipart/form-data>
	<input type="hidden" name="SID" VALUE="${yearTerm.SID}">
	<input type="hidden" name="C_TIME" VALUE="${yearTerm.c_TIME?string("yyyy-MM-dd HH:mm:ss")}">
	<table width="100%" cellpadding="0" cellspacing="0" class="form_table">
		  <tr>
		  	 <TD WIDTH="150PX" class="lableName">学年（学期）：</td>
		  	 <TD>
		  	 	<input type="text" id="user_name" class="must reg_txt" name="STUDY_YEAR_TERM" value="${yearTerm.STUDY_YEAR_TERM}"> <span style="font-size:12px;">格式:2014-2015学年(第2学期)</span> <span class="alertWord">*</span><span id="name_pan"></span><span class="availWord"></span>
		  	 </td>
		  	 <TD WIDTH="150PX" class="lableName">当前有效：</td>
		  	 <TD style="font-size:12px;">
		  	 	<input type="radio" name="IS_NEW" value="0" <#if yearTerm.IS_NEW=='0'> checked</#if>>否 <input type="radio" name="IS_NEW" value="1"  <#if yearTerm.IS_NEW=='1'> checked</#if> >是
		  	 </td>
		  </tr>
		   <tr>
		   	 <TD class="lableName">学期开始日期：</td>
		  	 <TD>
				 <input id="d421" class="Wdate" type="text" name="START_DATE" value="${yearTerm.START_DATE?string("yyyy-MM-dd")}" onClick="WdatePicker()"/> <span class="alertWord">*</span><span class="availWord"></span>
		  	 </td>	
		  	 
		  	 <TD class="lableName">预约限制天数：</td>
		  	 <TD>
		  	      <input type="text" class="must number"  name="LIMIT_DAY" value="${yearTerm.LIMIT_DAY}"></input><span class="alertWord">*</span><span class="availWord"></span>
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
