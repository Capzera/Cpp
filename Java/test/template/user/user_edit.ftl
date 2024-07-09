<form action="${base}/electric/user/update" method="POST">
	<table border="1" cellpadding="1" cellspacing="1">
		<!--加一个隐藏字段-->
	    <input type="hidden" name="SID" VALUE="${user.SID}">
		<tr>
			<td>用户名：</td>
			<td><input type="text" name="USER_NAME" value=${user.USER_NAME}></td>
		</tr>
		<tr>
			<td>密码：</td>
			<td><input type="password" name="USER_PASSWORD" value="${user.USER_PASSWORD}"></td>
		</tr>
		<tr>
			<td colspan="2">
				<input type="submit" value=" 提交 "></td>
		</tr>
	</table>
</form>