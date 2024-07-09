<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<#include "common/common-js.ftl">
<title>陕西秦稷粮业科技集团管理工作平台</title>
<style type="text/css">
<!--
body {
	margin-left: 0px;
	margin-top: 0px;
	margin-right: 0px;
	margin-bottom: 0px;
	overflow:hidden;
}
.STYLE3 {font-size: 12px; color: #adc9d9; }
-->
</style></head>

<body>
<form action="${base}/executeLogin" method="post" id="postForm">
<table width="100%"  height="100%" border="0" cellspacing="0" cellpadding="0">
  <tr>
    <td bgcolor="#1075b1">&nbsp;</td>
  </tr>
  <tr>
    <td height="608" background="${base}/resource/login_images/login_03.gif"><table width="847" border="0" align="center" cellpadding="0" cellspacing="0">
      <tr>
        <td height="318" background="${base}/resource/login_images/login_04.gif">&nbsp;</td>
      </tr>
      <tr>
        <td height="84"><table width="100%" border="0" cellspacing="0" cellpadding="0">
          <tr>
            <td width="381" height="84" background="${base}/resource/login_images/login_06.gif">&nbsp;</td>
            <td width="162" valign="middle" background="${base}/resource/login_images/login_07.gif"><table width="100%" border="0" cellspacing="0" cellpadding="0">
              <tr>
                <td width="44" height="24" valign="bottom"><div align="right"><span class="STYLE3">用户</span></div></td>
                <td width="10" valign="bottom">&nbsp;</td>
                <td height="24" colspan="2" valign="bottom">
                  <div align="left">
                    <input type="text" name="uname" id="u_name" style="width:100px; height:17px; background-color:#87adbf; border:solid 1px #153966; font-size:12px; color:#283439; ">
                  </div></td>
              </tr>
              <tr>
                <td height="24" valign="bottom"><div align="right"><span class="STYLE3">密码</span></div></td>
                <td width="10" valign="bottom">&nbsp;</td>
                <td height="24" colspan="2" valign="bottom"><input type="password" name="passwd" id="pass_wd" style="width:100px; height:17px; background-color:#87adbf; border:solid 1px #153966; font-size:12px; color:#283439; "></td>
              </tr>
              
              <tr></tr>
            </table></td>
            <td width="26"><img src="${base}/resource/login_images/login_08.gif" width="26" height="84"></td>
            <td width="67" background="${base}/resource/login_images/login_09.gif"><table width="100%" border="0" cellspacing="0" cellpadding="0">
              <tr>
                <td height="25"><div align="center">
                	<img id="loginId" style="cursor:pointer" src="${base}/resource/login_images/dl.gif" width="57" height="20">
                </div></td>
              </tr>
              <tr>
                <td height="25"><div align="center">
                	<img id="resetId" style="cursor:pointer" src="${base}/resource/login_images/cz.gif" width="57" height="20">
                 </div></td>
              </tr>
              <script>
              	 $("#loginId").click(function(){
              	 	$("#postForm").submit();
              	 	return false;
              	 })
              	 $("#resetId").click(function(){
              	 	$("#u_name").attr("value","");
              	 	$("#pass_wd").attr("value","");
              	 })
              </script>
            </table></td>
            <td width="211" background="${base}/resource/login_images/login_10.gif">&nbsp;</td>
          </tr>
        </table></td>
      </tr>
      <tr>
        <td height="206" background="${base}/resource/login_images/login_11.gif">&nbsp;</td>
      </tr>
    </table></td>
  </tr>
  <tr>
    <td bgcolor="#152753">&nbsp;</td>
  </tr>
</table>
</form>
</body>
</html>
