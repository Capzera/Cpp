<#include "common/common-js.ftl">
<div>
	<div class="title"><img src="${base}/resource/images/add.gif"> 修改用户信息</div>
</div>
<div align="center">
	<form action="${base}/admin/user/update" method="POST" onsubmit="return submit_chk()">
		<input type="HIDDEN" NAME="SID" VALUE="${user.SID}">
		<table border="0" cellpadding="1" cellspacing="1">
			<tr>
				<td WIDTH="100PX" class="lableName">姓名：</td>
				<td><input type="text" class="must reg_txt" name="UNAME" value="${user.UNAME}"><span class="alertWord">*</span><span id="name_pan"></span><span class="availWord"></span></td>
			</tr>
			<tr>
				<td WIDTH="100PX" class="lableName">登录账号：</td>
				<td><input type="text"  class="must reg_txt" name="LOGIN_NAME" ValUE="${user.LOGIN_NAME}"><span class="alertWord">*</span><span id="name_pan"></span><span class="availWord"></span></td>
			</tr>
			<tr>
				<td WIDTH="100PX" class="lableName">密码：</td>
				<td><input type="password" class="must reg_txt" name="PASSWD" VALUE="${user.PASSWD}"><span class="alertWord">*</span><span id="name_pan"></span><span class="availWord"></span></td>
			</tr>
			<tr>
				<td WIDTH="100PX" class="lableName">用户类型：</td>
				<td class="lableName" style="text-align:left"><input type="radio" name="U_TYPE" value="1" <#if user.u_TYPE=="1">CHECKED</#if>>管理员 <input type="radio" name="U_TYPE" value="0" <#if user.u_TYPE=="0">CHECKED</#if>>普通用户</td>
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