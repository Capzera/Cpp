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
			//加载数据提示页面
			document.onreadystatechange = function () {    
                if(document.readyState=="complete") {          
                    $("#divprogressbar").css({"display":"none"});    
                }   
        	}  
			$(document).ready(function(){
				$(".delete").click(function(){
						art.dialog.confirm('artDialog崇尚：简单、强大、优雅', function(){alert('yes');});
				})
				$(".edit").click(function(){
						art.dialog.alert('你好!');
				})
			})
		</script>
</head>

<body>
<table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
  <tr>
    <td height="30"><table width="100%" border="0" cellspacing="0" cellpadding="0">
      <tr>
        <td height="24" bgcolor="#353c44"><table width="100%" border="0" cellspacing="0" cellpadding="0">
          <tr>
            <td><table width="100%" border="0" cellspacing="0" cellpadding="0">
              <tr>
                <td width="6%" height="19" valign="bottom"><div align="center"><img src="${res}/images/tb.gif" width="14" height="14" /></div></td>
                <td width="94%" valign="bottom"><span class="STYLE1"> 系统公告列表</span></td>
              </tr>
            </table></td>
            <td><div align="right"><span class="STYLE1">
              <input type="checkbox" name="checkbox11" id="checkbox11" />
              全选      &nbsp;&nbsp;<img src="${res}/images/add.gif" width="10" height="10" /> 添加   &nbsp; <img src="${res}/images/del.gif" width="10" height="10" /> 删除    &nbsp;&nbsp;<img src="${res}/images/edit.gif" width="10" height="10" /> 编辑   &nbsp;</span><span class="STYLE1"> &nbsp;</span></div></td>
          </tr>
        </table></td>
      </tr>
    </table></td>
  </tr>
  <tr>
    <td class="list">
         <form action="${action}" method="post" id="listForm">
	    	<table width="100%" border="0"  cellpadding="0" cellspacing="1" bgcolor="#a8c7ce" onmouseover="changeto()"  onmouseout="changeback()">
					       <tr>
							        <td width="4%" height="20" bgcolor="d3eaef">
								        <div align="center">
								          <input type="checkbox" name="checkbox" id="checkbox" />
								        </div>
							        </td>
									<td bgcolor="d3eaef" class="STYLE6"><div align="center"><span class="STYLE10">名称</span></div></td>
									<td bgcolor="d3eaef" class="STYLE6"><div align="center"><span class="STYLE10">邮箱</span></div></td>
									<td bgcolor="d3eaef" class="STYLE6"><div align="center"><span class="STYLE10">登录次数</span></div></td>
									<td bgcolor="d3eaef" class="STYLE6"><div align="center"><span class="STYLE10">操作</span></div></td>
					       </tr>
			       <#list service as list>
							<tr>
								<td width="4%" height="20" bgcolor="#FFFFFF" style="border-right:0px;">
							        <div align="center">
							          <input type="checkbox" name="checkbox" id="checkbox" />
							        </div>
					       		</td>
								<td bgcolor="#FFFFFF" class="STYLE19"><div align="center">${list.uname}</div></td>
								<td bgcolor="#FFFFFF" class="STYLE19"><div align="center">${list.email}</div></td>
								<td bgcolor="#FFFFFF" class="STYLE19"><div align="center">${list.login_Cnt!0}</div></td>
								<td bgcolor="#FFFFFF" class="STYLE19">
									<div align="center">
										<img src="${res}/images/edit.gif"> <a href="javascript:void(0)" class="edit" id="edit_${list_index}">编辑</a>&nbsp;
										<img src="${res}/images/del.gif"> <a href="javascript:void(0)"  class="delete">删除</a>
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
    </table></td>
  </tr>
</table>
	<!-- 页面提示加载框 -->
	<#include "common/load_promot.ftl">
</body>
</html>
