<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=gb2312" />
<title>西安工程大学管理学院-经济与管理实验教学中心-管理系统</title>
    <script language="JavaScript"> 
      if (window != top) 
        top.location.href = location.href; 
    </script>
<#include "common/common-js.ftl">
<style type="text/css">

<!--
body {
	margin-left: 0px;
	margin-top: 0px;
	margin-right: 0px;
	margin-bottom: 0px;
	background-color: #016aa9;
	overflow:hidden;
}
.STYLE1 {
	color: #000000;
	font-size: 12px;
}
-->
</style></head>

<body>
<FORM ACTION="${base}/submitLogin" method="POST" id="loginForm">
<table width="100%" height="100%" border="0" cellpadding="0" cellspacing="0">
  <tr>
    <td><table width="962" border="0" align="center" cellpadding="0" cellspacing="0">
      <tr>
        <td height="235" background="${base}/resource/login_images/login_03.gif">&nbsp;</td>
      </tr>
      <tr>
        <td height="53"><table width="100%" border="0" cellspacing="0" cellpadding="0">
          <tr>
            <td width="394" height="53" background="${base}/resource/login_images/login_05.gif">&nbsp;</td>
            <td width="206" background="${base}/resource/login_images/login_06.gif"><table width="100%" border="0" cellspacing="0" cellpadding="0">
              <tr>
                <td width="16%" height="25"><div align="right"><span class="STYLE1">用户</span></div></td>
                <td width="57%" height="25"><div align="center">
                  <input type="text" id="uname" name="LOGIN_NAME" style="width:105px; height:17px;  border:solid 1px #7dbad7; font-size:12px; color:#6cd0ff">
                </div></td>
                <td width="27%" height="25">&nbsp;</td>
              </tr>
              <tr>
                <td height="25"><div align="right"><span class="STYLE1">密码</span></div></td>
                <td height="25">
	                <div align="center">
	                  <input type="password" id="passwd" name="PASSWD" onkeypress = "login()" style="width:105px; height:17px;  border:solid 1px #7dbad7; font-size:12px; color:#6cd0ff">
	                  
	                </div>
	            </td>
	            
                <td height="25"><div align="left"><a href="javascript:void(0)" id="login"><img src="${base}/resource/login_images/dl.gif" width="49" height="18" border="0"></a></div></td>
                <script>
                    $(document).ready(function(){
                    	$("#login").click(function(){
                			$("#loginForm").submit();
                			return false;
                		})
                    })
                	function login(){
                		
                		if(event.keyCode == 13){
                		   if($("#uname").val() != "" && $("#passwd").val() != ""){                		   
                			 $("#loginForm").submit();
                			 return false;
                		   } 
                		}
                	}
                </script>
              </tr>
              <tr style ="background-image:url();">
              	<td colspan="3" style="text-indent:10px;"><span style="color:RED;font-size:12px;"><#if message != null><B>${message}</#if></B></span></td>
              </tr>
            </table></td>
            <td width="362" background="${base}/resource/login_images/login_07.gif">&nbsp;</td>
          </tr>
        </table></td>
      </tr>
      <tr>
        <td height="213" background="${base}/resource/login_images/login_08.gif">&nbsp;</td>
      </tr>
    </table></td>
  </tr>
</table>
</FORM>
</body>
</html>
