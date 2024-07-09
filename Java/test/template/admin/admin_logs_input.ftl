<#include "common/common-js.ftl">
<div>
	<div class="title"><img src="${base}/resource/images/add.gif"> 添加工作日志</div>
</div>
<div align="center">
	<form action="${base}/admin/logs/doUpload" method="post" onsubmit="return submit_chk()" encType="multipart/form-data">
		<table border="0" cellpadding="1" cellspacing="1">
		    <tr>
		    	<td WIDTH="100px" class="lableName">主题：</td>
		    	<td width="500px"><textarea name="title" class="must" style="width:300px;height:150px;"></textarea><span class="alertWord">*</span><span id="name_pan"></span><span class="availWord"></span></td>
		    </tr>
			<tr>
				<td WIDTH="100px" class="lableName">文件：</td>
				<td class="lableName" style="text-align:left">
					<input type="file" name="logs_file" >
				</td>
			</tr>
			<tr>
				<td colspan="2" align="center">
					<input type="submit" value=" 提交 ">
					<input type="reset"  value=" 重置 ">
				</td>
			</tr>
		</table>
	</form>
</div>