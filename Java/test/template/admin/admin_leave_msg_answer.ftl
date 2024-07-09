<#include "common/common-js.ftl">
<div>
	<div class="title"><img src="${base}/resource/images/add.gif"> 回复留言</div>
</div>
<div align="center">
	<form action="${base}/admin/leave_msg/doAnswer" method="post" onsubmit="return submit_chk()" encType="multipart/form-data">
	    <input type="hidden" name="sid" value="${msg.sid}">
	    <input type="hidden" name="content" value="${msg.content}">
	    <input type="hidden" name="ctime" value="${msg.ctime}">
		<table border="0" cellpadding="1" cellspacing="1">
		    <tr>
		    	<td WIDTH="100px" class="lableName">留言：</td>
		    	<td>${msg.content}</td>
		    </tr>
			<tr>
				<td WIDTH="100px" class="lableName">回复：</td>
				<td class="lableName" style="text-align:left">
					<textarea name="answer" class="must" style="width:300px;height:150px;"></textarea><span class="alertWord">*</span><span id="name_pan"></span><span class="availWord"></span>
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