<#include "common/common-js.ftl">
<div>
	<div class="title"><img src="${base}/resource/images/add.gif"> 编辑工作日志</div>
</div>
<div align="center">
	<form action="${base}/admin/logs/update" method="post" onsubmit="return submit_chk()" encType="multipart/form-data">
		<input type="hidden" name="sid" value="${log.sid}">
		<input type="hidden" name="ctime" value="${log.ctime}">
		<input type="hidden" name="uid" value="${log.uid}">
		<input type="hidden" name="y_term_id" value="${log.y_term_id}">
		<input type="hidden" name="y_week_th" value="${log.y_week_th}">
		<input type="hidden" name="file_path" value="${log.file_path}">
		<table border="0" cellpadding="1" cellspacing="1">
		    <tr>
		    	<td WIDTH="100PX" class="lableName">主题：</td>
		    	<td><textarea name="title" class="must" style="width:300px;height:150px;">${log.title}</textarea><span class="alertWord">*</span><span id="name_pan"></span><span class="availWord"></span></td>
		    </tr>
			<tr>
				<td WIDTH="100PX" class="lableName">文件：</td>
				<td class="lableName" style="text-align:left">
					<input type="file" name="logs_file" > <#if log.file_path??><a href="${base}/${log.file_path}"> 查看 </a></#if>
				</td>
			</tr>
			<tr>
				<td colspan="2" align="center">
					<input type="submit" value=" 提交 ">
					<input type="reset" value=" 重置 ">
				</td>
			</tr>
		</table>
	</form>
</div>