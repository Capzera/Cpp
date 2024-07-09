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
			
			$(document).ready(function(){
				//单击删除链接执行的函数
				$(".delete").click(function(){
						//获取要删除的数据的ID
						var delId = $(this).attr("id");
						//获取要删除的行
						var delLine = $(this).parent().parent().parent();
						//弹出提示框，下面的function是单击"确定"按钮执行的函数
					    if(confirm("删除后不可回复，您确定要删除吗？")){
							$.ajax({
								type:"GET",
								url:"/admin/article/del/"+delId,
								dataType:"json",
								success:function(data){
								        alert("删除成功！");
									    window.location.reload();
								}
							})
					    }
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
                <td width="6%" height="19" valign="bottom"><div align="center"><img src="${base}/resource/images/tb.gif" width="14" height="14" /></div></td>
                <td width="94%" valign="bottom"><span class="STYLE1"> &nbsp;&nbsp;文章列表</span></td>
              </tr>
            </table></td>
            <td><div align="right"><span class="STYLE1">
              <input type="checkbox" name="checkbox11" id="checkbox11" class="selectAll" />
              全选      &nbsp;&nbsp;<img src="${base}/resource/images/add.gif" width="10" height="10" /> 
              <a href="${base}/admin/article/add" style="color:white">添加</a>   &nbsp; 
              <img src="${base}/resource/images/del.gif" width="10" height="10" /> <a href="javascript:void(0)" id="delAll" ref="${base}/admin/cms_article/deleteAll" style="color:white">删除</a>    &nbsp;&nbsp;
              <img src="${base}/resource/images/edit.gif" width="10" height="10" /> 编辑   &nbsp;</span><span class="STYLE1"> &nbsp;</span></div></td>
          </tr>
        </table></td>
      </tr>
    </table></td>
  </tr>
  <tr>
    <td class="list">
    	 <div style="font-size:13px;">
         	<form action="${base}/admin/article/list" method="post" id="searchForm">
         		<table border=0 class="search">
         		   <tr>
         		   		<td>标题：
         		   			<input type="text" name="TITILE" VALUE="${article.TITILE}">
         		   		<td>
         		   		<td>类别：
			         		 <select name="category.SID">
					  	 		<OPTION VALUE="0">--请选择--</OPTION>
					  	 		<#list categoryList as list>
					  	 		    <OPTION VALUE="${list.SID}" <#if article.category?? && article.category.SID==list.SID>selected</#if>><#if list.LEVEL gt 0><#list 0..list.LEVEL as level>&nbsp;&nbsp;</#list></#if>${list.CATEGORY_NAME}</OPTION>
					  	 		</#list>
					  	 	</SELECT>
         		   		<td>
         		   		<td>
         		   			<input type="submit" value=" 查找  ">
         		   		</td>
         		   		<td style="text-indent:10px;">
         		   			<script>
         		   				$("#export").click(function(){
         		   					$("#listForm").submit();
         		   				})
         		   			</script>
         		   		</td>
         		   </tr>
         		</table>
         	</form>
         </div>
         <form action="${base}/admin/article/list" method="post" id="listForm">
	    	<table width="100%" border="0"  cellpadding="0" cellspacing="1" bgcolor="#a8c7ce" onmouseover="changeto()"  onmouseout="changeback()">
					       <tr>
							        <td width="4%" height="20" bgcolor="d3eaef">
								        <div align="center">
								          <input type="checkbox" name="checkbox" id="checkbox" class="selectAll" />
								          <script>
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
								          </script>
								        </div>
							        </td>
									<td bgcolor="d3eaef" class="STYLE6"><div align="center"><span class="STYLE10">标题</span></div></td>
									<td bgcolor="d3eaef" class="STYLE6"><div align="center"><span class="STYLE10">作者</span></div></td>
									<td bgcolor="d3eaef" class="STYLE6"><div align="center"><span class="STYLE10">来源</span></div></td>
									<td bgcolor="d3eaef" class="STYLE6"><div align="center"><span class="STYLE10">类别</span></div></td>
									<td bgcolor="d3eaef" class="STYLE6"><div align="center"><span class="STYLE10">内容</span></div></td>
									<td bgcolor="d3eaef" class="STYLE6"><div align="center"><span class="STYLE10">发布时间</span></div></td>
									<td bgcolor="d3eaef" class="STYLE6"><div align="center"><span class="STYLE10">关键词</span></div></td>
									<td bgcolor="d3eaef" class="STYLE6"><div align="center"><span class="STYLE10">操作</span></div></td>
					       </tr>
			          <#list articleList as list>
							<tr>
									    <td width="4%" height="20" bgcolor="#FFFFFF" style="border-right:0px;">
										        <div align="center">
										          <input type="checkbox" name="checkbox" id="checkbox" class="record_id"  value="${list.SID}"/>
										        </div>
							       		</td>
						       		  	<td bgcolor="#FFFFFF" class="STYLE19"><div align="center"><#if list.IMG_PATH !=null ><span style="color:red">[图]</span></#if>${list.TITILE}</div></td>
						       		  	<td bgcolor="#FFFFFF" class="STYLE19"><div align="center">${list.AUTHOR}</div></td>
						       		  	<td bgcolor="#FFFFFF" class="STYLE19"><div align="center">${list.SOURCE}</div></td>
						       		  	<td bgcolor="#FFFFFF" class="STYLE19"><div align="center">${list.category.CATEGORY_NAME}</div></td>
						       		  	<td bgcolor="#FFFFFF" class="STYLE19"><div align="center"><#if list.text?length gt 20>${list.text[0..20]}...<#else>${list.text}</#if></div></td>
						       		  	<td bgcolor="#FFFFFF" class="STYLE19"><div align="center">${list.c_TITLE?string("yyyy-MM-dd HH:mm:ss")}</div></td>
						       		  	<td bgcolor="#FFFFFF" class="STYLE19"><div align="center">${list.KEY_WORDS}</div></td>
										<td bgcolor="#FFFFFF" class="STYLE19">
											<div align="center">
												<img src="${base}/resource/images/edit.gif"> <a href="${base}/admin/article/edit/${list.SID}" class="edit" id="edit_${list_index}">编辑</a>&nbsp;
												<img src="${base}/resource/images/del.gif"> <a href="javascript:void(0)"   class="delete" id="${list.SID}">删除</a>
											</div>
										</td>
							</tr>
				     </#list>
		    </table>
		    <div class="pagerBar">
		    		<div align='left' style='float:left'><span class='STYLE22'>共有<strong>${pager.totalCount!0}</strong> 条记录，当前第<strong> ${pager.pageNumber!0}</strong> 页，共 <strong>${pager.pageCount!0}</strong> 页</span></div>
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

</body>
</html>
