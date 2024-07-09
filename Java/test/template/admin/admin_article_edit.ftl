<#include "common/common-js.ftl">

<style>
	table{font-size:14px;}
	.lableName{text-align:right;width:120px;}
</style>
<script type="text/javascript" src="${base}/resource/xheditor/xheditor-zh-cn.js"></script>
<script>
    $(pageInit);
	$(document).ready(function(){
		$("#content").xheditor({skin:'nostyle',width:'100%',height:'300',internalStyle:false,inlineStyle:false});
	});
	function pageInit(){
			
			 
	}
</script>
<div>
	<div class="title"> <img src="${base}/resource/images/add.gif"> &nbsp;&nbsp;修改文档</div>
</div>
<script>
	function setImgPath(msg){
		$("#imgPath").val(msg)
	}
</script>
<FORM ACTION="${base}/admin/article/update" method="POST" onsubmit="return submit_chk()" encType = "multipart/form-data">
	<input type="hidden" name="IMG_PATH" id="imgPath" value="${article.IMG_PATH}">
	<input type="hidden" name="ATTACHMENTS"  value="${article.ATTACHMENTS}">
	<input type="hidden" name="SID" value="${article.SID}">
	<table width="100%">
			  <tr>
			  	 <TD class="lableName">标题：</td>
			  	 <TD>
			  	 		<input type="text" name="TITILE" value="${article.TITILE}" class="must reg_txt"><span class="alertWord">*</span><span class="availWord"></span>
			  	 </td>
		  	</tr>
			  <tr>
			  	 <TD class="lableName">作者：</td>
			  	 <TD>
			  	 		<input type="text" name="AUTHOR" VALUE="${article.AUTHOR}"  class="must reg_txt"><span class="alertWord">*</span><span class="availWord"></span>
			  	 </td>
		  	</tr>
			<tr>
			  	 <TD class="lableName">来源：</td>
			  	 <TD>
			  	 		<input type="text" name="SOURCE" VALUE="${article.SOURCE}"  class="must reg_txt"><span class="alertWord">*</span><span class="availWord"></span>
			  	 </td>
		  	</tr>
		  	 <tr>
			  	 <TD  class="lableName">关键词：</td>
			  	 <TD>
			  	 		<input type="text" name="KEY_WORDS" VALUE="${article.KEY_WORDS}"  class="must reg_txt"><span class="alertWord">*</span><span class="availWord"></span>
			  	 </td>
		  	</tr>
		  	<tr>
			  	 <TD  class="lableName"><div>所属栏目：</div></td>
			  	 <TD>
			  	 	 <select name="category.SID">
			  	 		<#list categoryList as list>
			  	 		    <OPTION VALUE="${list.SID}" <#if list.SID == article.category.SID>selected</#if>><#if list.LEVEL gt 0><#list 0..list.LEVEL as level>&nbsp;&nbsp;</#list></#if>${list.CATEGORY_NAME}</OPTION>
			  	 		</#list>
		  	 		</SELECT>
			  	 </td>
		  	</tr>
		    <tr>
			  	 <TD  class="lableName">内容：</td>
			  	 <TD>
			  	 		<textarea name="CONTENT" id="content" class="xheditor{skin:'default',upImgUrl:'${base}/admin/article/upload',onUpload:setImgPath}" style="width:100%;height:200px;">${article.CONTENT}</textarea>
			  	 </td>
		  	</tr>
			<tr>
			  	 <TD  class="lableName">附件：</td>
			  	 <TD>
			  	 		<input type="file" name="file_ATTACHMENTS"> <#if article.ATTACHMENTS??> <a href="${base}${article.ATTACHMENTS}">查看</a></#if>
			  	 </td>
		  	</tr>
		 <tr>
		  	<td colspan="2" align="center" style="padding-top:10px;">
		  		<input type="submit" value=" 提交 ">
		  		<input type="reset" value=" 重置 ">
		  	</td>
		  </tr>
	</table>
</form>
