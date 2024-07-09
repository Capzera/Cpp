<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>无标题文档</title>
<style type="text/css">
<!--
body {
	margin-left: 0px;
	margin-top: 0px;
	margin-right: 0px;
	margin-bottom: 0px;
}
.STYLE3 {
	font-size: 12px;
	color: #435255;
}
.STYLE4 {font-size: 12px}
.STYLE5 {font-size: 12px; font-weight: bold; }
.MENU{text-decoration:none;color:black;}
-->
</style>
	<#include "common/common-js.ftl">
	<script>
	   //用户权限控制js代码
		function gotoPage(obj,target){
						var url = $(obj).attr("ref");
						$.ajax({
							type:"post",
							url:url,
							dataType:"json",
							success:function(data){
								if(data.status == "error"){
									art.dialog.alert("对不起，您没有访问权限！");
								}
							},
							error:function(data){
									window.open(url,target);
							}
						})
	   }
	   $(document).ready(function(){
	        $(".MENU").click(function(){
	            $.ajax({
	            	url:$(this).ref,
	            	method:"POST",
	            	dataType:"json",
	            	success:function(data){
	            		
	            		   //self.window.parent.document.getElementById("right_frame").src = $(this).ref;
	            		//}else {
	            		//   self.window.parent.document.getElementById("right_frame").src = "${base}/login";
	            		//}
	            	}
	            })
	            
	        })
	   		var tmp = $("#menu_1");
			$(".parent").click(function(){
				tmp.removeClass("STYLE5");
				tmp.addClass("STYLE4");
				$(this).addClass("STYLE5");
			    var trs = $(this).parent().parent().parent().parent().parent().parent().parent().children("tr");
			    $.each(trs,function(id,item){
			    	if($(item).attr("style")=="" && $(item).attr("class")!=""){
			    		$(item).css({"display":"none"});
			    	}
			    })
				$(this).parent().parent().parent().parent().parent().parent().parent().children(".child_"+$(this).attr("id")).css({"display":""});
				tmp = $(this);
			})
		})
	</script>
</head>

<body>
<table width="147" height="100%" border="0" align="center" cellpadding="0" cellspacing="0">
  <tr>
    <td height="23" background="${res}/images/main_34.gif">
	    <table width="100%" border="0" cellspacing="0" cellpadding="0">
	      <tr>
	        <td width="9%">&nbsp;</td>
	        <td width="83%"><div align="center" class="STYLE5 parent" id="menu_1" style="cursor:pointer">实验管理</div></td>
	        <td width="8%">&nbsp;</td>
	      </tr>
	    </table>
	 </td>
  </tr>
  <tr class="child_menu_1" style="">
    <td valign="top">
     <div align="center">
		      <table width="82%" border="0" align="center" cellpadding="0" cellspacing="0">
		     		 <tr>
				          <td height="38">
					          	<table width="100%" border="0" cellspacing="0" cellpadding="0">
						            <tr>
							              <td width="33" height="28"><img src="${res}/images/main_52.gif" width="28" height="28"></td>
							              <td width="99">
								              <table width="100%" border="0" cellspacing="0" cellpadding="0">
								                  <tr>
								                    <td height="23" class="STYLE4" style="cursor:hand" onMouseOver="this.style.backgroundImage='url(${res}/images/tab_bg.gif)';this.style.borderStyle='solid';this.style.borderWidth='1';borderColor='#adb9c2'; "onmouseout="this.style.backgroundImage='url()';this.style.borderStyle='none'"><a href="${base}/admin/logs/list" target="right" class="MENU">工作日志</a></td>
								                  </tr>
								              </table>
							              </td>
						            </tr>
					          </table>
					       </td>
			        </tr>
			         <tr>
				          <td height="38">
					          	<table width="100%" border="0" cellspacing="0" cellpadding="0">
						            <tr>
							              <td width="33" height="28"><img src="${res}/images/main_52.gif" width="28" height="28"></td>
							              <td width="99">
								              <table width="100%" border="0" cellspacing="0" cellpadding="0">
								                  <tr>
								                    <td height="23" class="STYLE4" style="cursor:hand" onMouseOver="this.style.backgroundImage='url(${res}/images/tab_bg.gif)';this.style.borderStyle='solid';this.style.borderWidth='1';borderColor='#adb9c2'; "onmouseout="this.style.backgroundImage='url()';this.style.borderStyle='none'"><a href="${base}/admin/leave_msg/list" target="right" class="MENU">留言内容</a></td>
								                  </tr>
								              </table>
							              </td>
						            </tr>
					          </table>
					       </td>
			        </tr>
		     		 <tr>
				          <td height="38">
					          	<table width="100%" border="0" cellspacing="0" cellpadding="0">
						            <tr>
							              <td width="33" height="28"><img src="${res}/images/main_40.gif" width="28" height="28"></td>
							              <td width="99">
								              <table width="100%" border="0" cellspacing="0" cellpadding="0">
								                  <tr>
								                    <td height="23" class="STYLE4" style="cursor:hand" onMouseOver="this.style.backgroundImage='url(${res}/images/tab_bg.gif)';this.style.borderStyle='solid';this.style.borderWidth='1';borderColor='#adb9c2'; "onmouseout="this.style.backgroundImage='url()';this.style.borderStyle='none'"><a href="${base}/admin/user/list" target="right" class="MENU">用户管理</a></td>
								                  </tr>
								              </table>
							              </td>
						            </tr>
					          </table>
					       </td>
			        </tr>
		     		 <tr>
				          <td height="38">
					          	<table width="100%" border="0" cellspacing="0" cellpadding="0">
						            <tr>
							              <td width="33" height="28"><img src="${res}/images/main_52.gif" width="28" height="28"></td>
							              <td width="99">
								              <table width="100%" border="0" cellspacing="0" cellpadding="0">
								                  <tr>
								                    <td height="23" class="STYLE4" style="cursor:hand" onMouseOver="this.style.backgroundImage='url(${res}/images/tab_bg.gif)';this.style.borderStyle='solid';this.style.borderWidth='1';borderColor='#adb9c2'; "onmouseout="this.style.backgroundImage='url()';this.style.borderStyle='none'"><a href="${base}/admin/test/order/list" target="right" class="MENU">预约管理</a></td>
								                  </tr>
								              </table>
							              </td>
						            </tr>
					          </table>
					       </td>
			        </tr>
		     		 <tr>
				          <td height="38">
					          	<table width="100%" border="0" cellspacing="0" cellpadding="0">
						            <tr>
							              <td width="33" height="28"><img src="${res}/images/main_46.gif" width="28" height="28"></td>
							              <td width="99">
								              <table width="100%" border="0" cellspacing="0" cellpadding="0">
								                  <tr>
								                    <td height="23" class="STYLE4" style="cursor:hand" onMouseOver="this.style.backgroundImage='url(${res}/images/tab_bg.gif)';this.style.borderStyle='solid';this.style.borderWidth='1';borderColor='#adb9c2'; "onmouseout="this.style.backgroundImage='url()';this.style.borderStyle='none'"><a href="${base}/admin/test/order/print/1" target="right" class="MENU">打印课表</a></td>
								                  </tr>
								              </table>
							              </td>
						            </tr>
					          </table>
					       </td>
			        </tr>
		     		 <tr>
				          <td height="38">
					          	<table width="100%" border="0" cellspacing="0" cellpadding="0">
						            <tr>
							              <td width="33" height="28"><img src="${res}/images/main_52.gif" width="28" height="28"></td>
							              <td width="99">
								              <table width="100%" border="0" cellspacing="0" cellpadding="0">
								                  <tr>
								                    <td height="23" class="STYLE4" style="cursor:hand" onMouseOver="this.style.backgroundImage='url(${res}/images/tab_bg.gif)';this.style.borderStyle='solid';this.style.borderWidth='1';borderColor='#adb9c2'; "onmouseout="this.style.backgroundImage='url()';this.style.borderStyle='none'"><a href="${base}/admin/test/room/list" target="right" class="MENU">教室管理</a></td>
								                  </tr>
								              </table>
							              </td>
						            </tr>
					          </table>
					       </td>
			        </tr>
			        <tr>
				          <td height="38">
					          	<table width="100%" border="0" cellspacing="0" cellpadding="0">
						            <tr>
							              <td width="33" height="28"><img src="${res}/images/main_50.gif" width="28" height="28"></td>
							              <td width="99">
								              <table width="100%" border="0" cellspacing="0" cellpadding="0">
								                  <tr>
								                    <td height="23" class="STYLE4" style="cursor:hand" onMouseOver="this.style.backgroundImage='url(${res}/images/tab_bg.gif)';this.style.borderStyle='solid';this.style.borderWidth='1';borderColor='#adb9c2'; "onmouseout="this.style.backgroundImage='url()';this.style.borderStyle='none'"><a href="${base}/admin/category/list" target="right" class="MENU">分类管理</a></td>
								                  </tr>
								              </table>
							              </td>
						            </tr>
					          </table>
					       </td>
			        </tr>
			        <tr>
				          <td height="38">
					          	<table width="100%" border="0" cellspacing="0" cellpadding="0">
						            <tr>
							              <td width="33" height="28"><img src="${res}/images/main_40.gif" width="28" height="28"></td>
							              <td width="99">
								              <table width="100%" border="0" cellspacing="0" cellpadding="0">
								                  <tr>
								                    <td height="23" class="STYLE4" style="cursor:hand" onMouseOver="this.style.backgroundImage='url(${res}/images/tab_bg.gif)';this.style.borderStyle='solid';this.style.borderWidth='1';borderColor='#adb9c2'; "onmouseout="this.style.backgroundImage='url()';this.style.borderStyle='none'"><a href="${base}/admin/article/list" target="right" class="MENU">内容管理</a></td>
								                  </tr>
								              </table>
							              </td>
						            </tr>
					          </table>
					       </td>
			        </tr>
			        <tr>
				          <td height="38">
					          	<table width="100%" border="0" cellspacing="0" cellpadding="0">
						            <tr>
							              <td width="33" height="28"><img src="${res}/images/main_46.gif" width="28" height="28"></td>
							              <td width="99">
								              <table width="100%" border="0" cellspacing="0" cellpadding="0">
								                  <tr>
								                    <td height="23" class="STYLE4" style="cursor:hand" onMouseOver="this.style.backgroundImage='url(${res}/images/tab_bg.gif)';this.style.borderStyle='solid';this.style.borderWidth='1';borderColor='#adb9c2'; "onmouseout="this.style.backgroundImage='url()';this.style.borderStyle='none'"><a href="${base}/admin/baseparam/list" target="right" class="MENU">参数配置</a></td>
								                  </tr>
								              </table>
							              </td>
						            </tr>
					          </table>
					       </td>
			        </tr>
		      </table>
    </div></td>
  </tr>
  
  <tr>
    <td height="19" background="${res}/images/main_69.gif"><table width="100%" border="0" cellspacing="0" cellpadding="0">
      <tr>
        <td width="24%">&nbsp;</td>
        <td width="76%" valign="bottom"><span class="STYLE3">版本：2017 v1.0</span></td>
      </tr>
    </table></td>
  </tr>
</table>
  
</body>
</html>
