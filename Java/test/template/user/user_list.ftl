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
						                <td width="94%" valign="bottom"><span class="STYLE1"> 用户信息列表</span></td>
						              </tr>
					            	</table>
					        		</td>
					          	<td>
					            	<div align="right"><span class="STYLE1">
					              		<input type="checkbox" name="checkbox11" id="checkbox11" class="selectAll"/>全选&nbsp;&nbsp;
					          				<img src="${res}/images/add.gif" width="10" height="10" /> 
					              			<a href="${base}/admin/alarm/add" style="color:#E1E2E3">添加</a>&nbsp; 
					              		<img src="${res}/images/del.gif" width="10" height="10" /> 
					              			<a href="javascript:void(0)" id="delAll" ref="${base}/emp_mgr/deleteAll" style="color:#E1E2E3">删除</a>&nbsp;&nbsp;
					              		<img src="${res}/images/edit.gif" width="10" height="10" /> 
					              			<a href="${base}/emp_mgr/search_input" id="search" ref="${base}/emp_mgr/search_input" style="color:#E1E2E3">查询</a>&nbsp;</span><span class="STYLE1"> &nbsp;</span>
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
    	
         	<form action="${base}/electric/report/search" method="POST" id="listForm">
         	    
	    		<table width="100%" border="0"  cellpadding="0" cellspacing="1" bgcolor="#a8c7ce" onmouseover="changeto()"  onmouseout="changeback()">
					       <tr>
							        <td width="4%" height="20" bgcolor="d3eaef">
								        <div align="center">
								          <input type="checkbox" name="checkbox" id="checkbox" class="selectAll" />								          
								        </div>
							        </td>
											
											<td bgcolor="d3eaef" class="STYLE6"><div align="center"><span class="STYLE10">报表名称</span></div></td>
											<td bgcolor="d3eaef" class="STYLE6"><div align="center"><span class="STYLE10">报表作者</span></div></td>
											<td bgcolor="d3eaef" class="STYLE6"><div align="center"><span class="STYLE10">关键词</span></div></td>	
											<td bgcolor="d3eaef" class="STYLE6"><div align="center"><span class="STYLE10">备注</span></div></td>								
											<td bgcolor="d3eaef" class="STYLE6"><div align="center"><span class="STYLE10">
											   是否通过审核
											</span></div></td>								
											<td bgcolor="d3eaef" class="STYLE6"><div align="center"><span class="STYLE10">
											   上传者
											</span></div></td>								
										
											<td bgcolor="d3eaef" class="STYLE6"><div align="center"><span class="STYLE10">操作</span></div></td>
					       </tr>
			       <#list reportList as report>
							<tr>
									<td width="4%" height="20" bgcolor="#FFFFFF" style="border-right:0px;">
								        <div align="center">
								          <input type="checkbox" name="checkbox" id="checkbox" class="record_id"  value="${report.SID}"/>
								        </div>
					       			</td>
											
											<td bgcolor="#FFFFFF" class="STYLE19">
												<div align="center">
												  ${report.SID}
												</div>
											</td>
											<td bgcolor="#FFFFFF" class="STYLE19"><div align="center">${report.NAME}</div></td>
											<td bgcolor="#FFFFFF" class="STYLE19"><div align="center">${report.AUTHOR}</div></td>			
											
											<td bgcolor="#FFFFFF" class="STYLE19">
												<div align="center">
													${report.KW_WORDS}
												</div>
											</td>
											
											<td bgcolor="#FFFFFF" class="STYLE19">
												<div align="center">
													${report.MARKET}
												</div>
											</td>
											
											<td bgcolor="#FFFFFF" class="STYLE19">
												<div align="center">
													<#if report.IS_PASS=="0">
														未通过
														<#else>
														通过
													</#if>
												</div>
											</td>
											<td bgcolor="#FFFFFF" class="STYLE19">
												<div align="center">
													${report.user.USER_NAME}
												</div>
											</td>
											
											<td bgcolor="#FFFFFF" class="STYLE19">
											<div align="center">
											    <A HREF="${base}/${report.FILE_PATH"}>查看</a>
												<img src="${res}/images/edit.gif"> <a href="${base}/electric/report/edit/${report.SID}" class="edit" id="edit_${report_index}">编辑</a>&nbsp;
												<img src="${res}/images/del.gif"> <a href="javascript:void(0)" ref="${base}/electric/report/del" class="delete" id="${report.SID}">删除</a>
											</div>
									</td>
						</tr>
				 	</#list>
		    </table>
		    <div class="pagerBar">
		    		<div align="left" style="float:left">
		    			<span class="STYLE22">共有<strong>${pager.totalCount!0}</strong> 条记录，当前第<strong> ${pager.pageNumber!0}</strong> 页，共 <strong>${pager.pageCount!0}</strong> 页</span>
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
记录数：${rowCount}
</body>
</html>
