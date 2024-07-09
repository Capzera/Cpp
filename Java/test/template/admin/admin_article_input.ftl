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
	<div class="title"> <img src="${base}/resource/images/add.gif"> &nbsp;&nbsp;添加文档</div>
</div>
<script>
	function setImgPath(msg){
		$("#imgPath").val(msg)
	}
</script>
<FORM ACTION="${base}/admin/article/save" method="POST" onsubmit="return submit_chk()" encType = "multipart/form-data">
	<input type="hidden" name="IMG_PATH" id="imgPath" value="">
	<table width="100%">
			  <tr>
			  	 <TD class="lableName">标题：</td>
			  	 <TD>
			  	 		<input type="text" name="TITILE"  class="must reg_txt"><span class="alertWord">*</span><span class="availWord"></span>
			  	 </td>
		  	</tr>
			  <tr>
			  	 <TD class="lableName">作者：</td>
			  	 <TD>
			  	 		<input type="text" name="AUTHOR"  class="must reg_txt"><span class="alertWord">*</span><span class="availWord"></span>
			  	 </td>
		  	</tr>
			  <tr>
			  	 <TD class="lableName">来源：</td>
			  	 <TD>
			  	 		<input type="text" name="SOURCE"  class="must reg_txt"><span class="alertWord">*</span><span class="availWord"></span>
			  	 </td>
		  	</tr>
		  	 <tr>
			  	 <TD  class="lableName">关键词：</td>
			  	 <TD>
			  	 		<input type="text" name="KEY_WORDS"  class="must reg_txt"><span class="alertWord">*</span><span class="availWord"></span>
			  	 </td>
		  	</tr>
		  	<tr>
			  	 <TD  class="lableName"><div>所属栏目：</div></td>
			  	 <TD>
			  	 	 <select name="category.SID">
			  	 		<#list categoryList as list>
			  	 		    <OPTION VALUE="${list.SID}"><#if list.LEVEL gt 0><#list 0..list.LEVEL as level>&nbsp;&nbsp;</#list></#if>${list.CATEGORY_NAME}</OPTION>
			  	 		</#list>
		  	 		</SELECT>
			  	 </td>
		  	</tr>
		    <tr>
			  	 <TD  class="lableName">内容：</td>
			  	 <TD>
			  	 		<textarea name="CONTENT" id="content" class="xheditor{skin:'default',upImgUrl:'${base}/admin/article/upload',onUpload:setImgPath}" style="width:100%;height:200px;"></textarea>
			  	 </td>
		  	</tr>
			<tr>
			  	 <TD  class="lableName">附件：</td>
			  	 <TD>
			  	 		<input type="file" name="file_ATTACHMENTS">
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
