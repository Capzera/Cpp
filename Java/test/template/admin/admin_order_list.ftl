<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>无标题文档</title>
<style type="text/css">
<!--
body {
	margin-left: 3px;
	margin-top: 0px;
	margin-right: 3px;
	margin-bottom: 0px;
}
.STYLE1 {
	color: #e1e2e3;
	font-size: 12px;
}
.STYLE6 {color: #000000; font-size: 12; }
.STYLE10 {color: #000000; font-size: 12px; }
.STYLE19 {
	color: #344b50;
	font-size: 12px;
}
.STYLE21 {
	font-size: 12px;
	color: #3b6375;
}
.STYLE22 {
	font-size: 12px;
	color: #295568;
}
a{
	text-decoration:NONE;
}
-->
</style>

<script>
	var  highlightcolor='#d5f4fe';
	//此处clickcolor只能用win系统颜色代码才能成功,如果用#xxxxxx的代码就不行,还没搞清楚为什么:(
	var  clickcolor='#51b2f6';
	function  changeto(){
	source=event.srcElement;
	if  (source.tagName=="TR"||source.tagName=="TABLE")
	return;
	while(source.tagName!="TD")
	source=source.parentElement;
	source=source.parentElement;
	cs  =  source.children;
	//alert(cs.length);
	if  (cs[1].style.backgroundColor!=highlightcolor&&source.id!="nc"&&cs[1].style.backgroundColor!=clickcolor)
	for(i=0;i<cs.length;i++){
		cs[i].style.backgroundColor=highlightcolor;
	}
	}
	
	function  changeback(){
	if  (event.fromElement.contains(event.toElement)||source.contains(event.toElement)||source.id=="nc")
	return
	if  (event.toElement!=source&&cs[1].style.backgroundColor!=clickcolor)
	//source.style.backgroundColor=originalcolor
	for(i=0;i<cs.length;i++){
		cs[i].style.backgroundColor="";
	}
	}
	
	function  clickto(){
	source=event.srcElement;
	if  (source.tagName=="TR"||source.tagName=="TABLE")
	return;
	while(source.tagName!="TD")
	source=source.parentElement;
	source=source.parentElement;
	cs  =  source.children;
	//alert(cs.length);
	if  (cs[1].style.backgroundColor!=clickcolor&&source.id!="nc")
		for(i=0;i<cs.length;i++){
			cs[i].style.backgroundColor=clickcolor;
		}
	else
		for(i=0;i<cs.length;i++){
			cs[i].style.backgroundColor="";
		}
	}
</script>
		<#include "common/common-js.ftl">
		
		<script>
			
			$(document).ready(function(){
				//单击删除链接执行的函数
				$(".delete").click(function(){
						//获取要删除的数据的ID
						var delId = $(this).attr("id");
						//获取要删除的行
						var delLine = $(this).parent().parent().parent();
						var url = $(this).attr("ref") + "/" + delId;
					
						//弹出提示框，下面的function是单击"确定"按钮执行的函数
						if(confirm("您确定要删除吗？")){
							$.ajax({
								type:"GET",
								url:url,
								dataType:"json",
								success:function(data){
								   if(data.status.toString() == "success"){
									    alert("删除成功！");
									    delLine.remove();
								   }else{
								   		alert("对不起，删除失败!");
								   }
								},
								error:function(data){
									alert("对不起，删除失败!");
								}
							});
						    
						}
				})
				
				$("#delAll").click(function(){
					var ids = "";
					var idArray = $(".record_id");
					for(var i = 0;i < idArray.length; i++){
						if($(idArray[i]).attr("checked")==true){
							ids += $(idArray[i]).val()+","; 
						}
					}
					if(ids == ""){
						alert("请选择要删除的数据！");
						return false;
					}
					var url = $(this).attr("ref") + "/" +ids;
					if(confirm("删除后数据无法恢复，您确定要删除吗？")){
						$.ajax({
							    type:"get",
							    url:url,
							    dataType:"json",
							    success:function(data){
							    	if(data.status.toString() == "success"){
										alert("删除成功！");
										var delLine;
										for(var i = 0;i < idArray.length; i++){
											if($(idArray[i]).attr("checked")==true){
												delLine = $(idArray[i]).parent().parent().parent();
												delLine.remove();
											}
										}
									}else{
										alert("对不起,删除失败！");
									}
								},
								error:function(data){
									alert("对不起,删除失败！");
								}
						});
					
									
					}
				})
				
				$(".selectAll").click(function(){
			  	  var recordIds = $(".record_id");
			  	  if($(this).attr("checked") == true){
			  	  	 for(var i = 0;i<recordIds.length;i++){
			  	  	 	$(recordIds[i]).attr("checked",true);
			  	  	 }
			  	  }else{
			  	  	 for(var i = 0;i<recordIds.length;i++){
			  	  	 	$(recordIds[i]).attr("checked",false);
			  	  	 }
			  	  }
			  })
			
			})
		</script>	
</head>

<body>
<table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
	<#if message!=null>
	<tr>
      <td width="4%" height="20" bgcolor="d3eaef">
        <span class="STYLE10">
        	提示信息:${message}
        </span>
      </td>
	</tr>
	</#if>    
  <tr>
    <td height="30">
    	<table width="100%" border="0" cellspacing="0" cellpadding="0">
		      <tr>
			        <td height="24" bgcolor="#353c44"><table width="100%" border="0" cellspacing="0" cellpadding="0">
				          <tr>
					            <td>
					            	<table width="100%" border="0" cellspacing="0" cellpadding="0">
						              <tr>
						                <td width="6%" height="19" valign="bottom"><div align="center"><img src="${res}/images/tb.gif" width="14" height="14" /></div></td>
						                <td width="94%" valign="bottom"><span class="STYLE1"> 预约实验列表</span></td>
						              </tr>
					            	</table>
					        		</td>
					          	<td>
					            	<div align="right">
					            	   <span class="STYLE1;" style="font-size:12px;">
					              			<input type="checkbox" name="checkbox11" id="checkbox11" class="selectAll"/><span  style="color:white">全选</span>&nbsp;&nbsp;
					          				<img src="${res}/images/add.gif" width="10" height="10" /> 
					              			<a href="${base}/admin/test/order/add/1" style="color:#E1E2E3">添加</a>&nbsp; 
					              			<img src="${res}/images/del.gif" width="10" height="10" /> 
					              			<a href="javascript:void(0)" id="delAll" disabled ref="${base}/emp_mgr/deleteAll" style="color:#E1E2E3">删除</a>&nbsp;&nbsp;
					              		</span>
					      			</div>
					  			</td>
				          </tr>
			      	</td>
		      </tr> 
    	</table>
    </td>
  </tr>
  <tr>
    <td class="list">
    	<div style="font-size:12px;">
         	<form action="${base}/admin/test/order/list" method="post" id="searchForm">
         		<table border=0 class="search">
         		   <tr>
         		   		<td>
         		   			<span style="color:#0e2d5f"><B>${yearTerm.STUDY_YEAR_TERM}</B></span>
         		   		</td>
         		   		<td>第
         		   			<select name="WEEK_TH">
         		   			  <option value="0">--无--</option>
         		   			  <#list 1..20 as week>
         		   				<OPTION value="${week}" <#if testOrder.WEEK_TH==week>selected</#if>>${week}</OPTION>
         		   			  </#list>
         		   			</SELECT>
         		   			周
         		   		<td>
         		   		<td>星期：
         		   			<select name="WEEK_DAY">
         		   				<option value="0">--无--</option>
         		   			  <#list 1..5 as wDay>
         		   				<OPTION value="${wDay}" <#if testOrder.WEEK_DAY==wDay>selected</#if>>${wDay}</OPTION>
         		   			  </#list>
         		   			</SELECT>
         		   		<td>
         		   		<td>教室：
         		   			<select name="testRoom.SID">
         		   				<option value="0">--无--</option>
         		   			    <#list roomList as room>
         		   					<OPTION VALUE="${room.SID}" <#if testOrder.testRoom!=null && testOrder.testRoom.SID==room.SID>selected</#if>>${room.TEST_ROOM_NO}</OPTION>
         		   				</#list>
         		   			</SELECT>
         		   		<td>
         		   		<td>节次：
         		   			<select name="SECTION">
         		   				<option value="0">--无--</option>
         		   			   <#list [12,34,56,78,911] as section>
         		   				 <OPTION value="${section}" <#if testOrder.SECTION==section>selected</#if>>${section}</OPTION>
         		   			   </#list>
         		   			</SELECT>
         		   		<td>
         		   		
         		   		<td>教师：
         		   			<select name="TEACHER">
         		   				<option value="0">--无--</option>
         		   			  <#list userList as user>
         		   				<OPTION value="${user.UNAME}" <#if testOrder.TEACHER==user.UNAME>selected</#if>>${user.UNAME}</OPTION>
         		   			  </#list>
         		   			</SELECT>
         		   		<td>
         		   		<td>
         		   			<input type="submit" value=" 查找  " style="fong-size:12px;">
         		   		</td>
         		   </tr>
         		</table>
         	
           </div>
         	
         	  
	    		<table width="100%" border="0"  cellpadding="0" cellspacing="1" bgcolor="#a8c7ce" onmouseover="changeto()"  onmouseout="changeback()">
					       <tr>
							        <td width="4%" height="20" bgcolor="d3eaef">
								        <div align="center">
								          <input type="checkbox" name="checkbox" id="checkbox" class="selectAll" />								          
								        </div>
							        </td>
											
											<td bgcolor="d3eaef" class="STYLE6"><div align="center"><span class="STYLE10">学年，学期</span></div></td>
											<td bgcolor="d3eaef" class="STYLE6"><div align="center"><span class="STYLE10">上课时间</span></div></td>
											<td bgcolor="d3eaef" class="STYLE6"><div align="center"><span class="STYLE10">教室</span></div></td>								
											<td bgcolor="d3eaef" class="STYLE6"><div align="center"><span class="STYLE10">上课教师</span></div></td>								
											<td bgcolor="d3eaef" class="STYLE6"><div align="center"><span class="STYLE10">实验内容</span></div></td>								
											<td bgcolor="d3eaef" class="STYLE6"><div align="center"><span class="STYLE10">录入时间</span></div></td>								
											<td bgcolor="d3eaef" class="STYLE6"><div align="center"><span class="STYLE10">录入人</span></div></td>								
										
											<td bgcolor="d3eaef" class="STYLE6"><div align="center"><span class="STYLE10">操作</span></div></td>
					       </tr>
			       <#list testOrderList as list>
							<tr>
									<td width="4%" height="20" bgcolor="#FFFFFF" style="border-right:0px;">
								        <div align="center">
								          <input type="checkbox" name="checkbox" id="checkbox" class="record_id"  value="${list.SID}"/>
								        </div>
					       			</td>
											
											<td bgcolor="#FFFFFF" class="STYLE19">
												<div align="center">
												  ${list.yearTerm.STUDY_YEAR_TERM}
												</div>
											</td>
											<td bgcolor="#FFFFFF" class="STYLE19">
												<div align="center">
													第${list.WEEK_TH}周，星期${list.WEEK_DAY}，${list.SECTION}节
												</div>
											</td>
											
											<td bgcolor="#FFFFFF" class="STYLE19">
												<div align="center">
													${list.testRoom.TEST_ROOM_NO}
												</div>
											</td>
											<td bgcolor="#FFFFFF" class="STYLE19">
												<div align="center">
													${list.TEACHER}
												</div>
											</td>
											<td bgcolor="#FFFFFF" class="STYLE19">
												<div align="center">
													${list.TEST_CONTENT}
												</div>
											</td>
											<td bgcolor="#FFFFFF" class="STYLE19">
												<div align="center">
													${list.c_TIME?string("yyyy-MM-dd HH:mm:ss")}
												</div>
											</td>
											<td bgcolor="#FFFFFF" class="STYLE19">
												<div align="center">
													${list.inputUser.UNAME}
												</div>
											</td>
											<td bgcolor="#FFFFFF" class="STYLE19">
											<div align="center">
												
												<img src="${res}/images/del.gif"> <a href="javascript:void(0)" ref="${base}/admin/test/order/delete" class="delete" id="${list.SID}">删除</a>
											</div>
									</td>
						</tr>
				 	</#list>
		    </table>
		    <div class="pagerBar">
		    		<div align="left" style="float:left">
		    			<span class="STYLE22">共有<strong>${pager.totalCount!0}</strong> 条记录，当前第<strong> ${pager.pageNumber!0}</strong> 页，共 <strong>${pager.pageCount!0}</strong> 页  <B style="color:RED">共${pager.totalCount * 2}实验学时</B> </span>
		    		</div>
					<#include "common/pager.ftl">
				</div>
		</form>
	 </td>
  </tr>
  <tr>
    <td height="30"></td>
  </tr>
</table>

</body>
</html>
