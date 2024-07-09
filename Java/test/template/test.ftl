<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
	<head>
		<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
		<title>artDialog v3 demo</title>
		<meta name="keywords" content="" />
		<meta name="description" content="" />
		<!-- artDialog -->
		<#include "common/common-js.ftl">
		<script>
			$(document).ready(function(){
				$(".delete").click(function(){
						art.dialog.alert('artDialog崇尚：简单、强大、优雅', function(){alert('yes');});
				})
			})
		</script>
	</head>
	<body>
		<div id="main" class="list" style="text-align:center">
			<div class="tabContent">
					<div id="demoCode01"></div>
					<input  id="btn2" type="button" value="test">
			</div>
			<form action="" method="post" id="listForm">
				<table align="center">
					<tr>
						<td></td>
						<td>名称</td>
						<td>邮箱</td>
						<td>操作</td>
					</tr>
					<#list service as list>
						<tr>
							<td></td>
							<td>${list.uname}</td>
							<td>${list.email}</td>
							<td>
								<a href="javascript:void(0)">编辑</a>
								<a href="javascript:void(0)"  class="delete">删除</a>
							</td>
						</tr>
					</#list>
				</table>
				<div class="pagerBar">
					<input type="button" class="deleteButton" url="${base}/admin/act_member/del.do" value="删 除" disabled hidefocus="true" />
					<#include "common/pager.ftl">
				</div>
			</form>
		</div>
	</body>
</html>
