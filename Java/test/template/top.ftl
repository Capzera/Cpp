<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
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
	.STYLE1 {
		font-size: 12px;
		color: #000000;
	}
	.STYLE5 {font-size: 12}
	.STYLE7 {font-size: 12px; color: #FFFFFF; }
-->
</style></head>

<body>
<table width="100%" border="0" cellspacing="0" cellpadding="0">
  <tr>
    <td height="57" background="${res}/images/main_03.gif"><table width="100%" border="0" cellspacing="0" cellpadding="0">
      <tr>
        <td width="378" height="57" background="${res}/images/main_01.gif">&nbsp;</td>
        <td>&nbsp;</td>
        <td width="281" valign="bottom"><table width="100%" border="0" cellspacing="0" cellpadding="0">
          <tr>
            <td width="33" height="27"><img src="${res}/images/main_05.gif" width="33" height="27" /></td>
            <td width="248" background="${res}/images/main_06.gif"><table width="225" border="0" align="center" cellpadding="0" cellspacing="0">
              <tr>
                <td></td>
                <td height="17"><div align="right"><img src="${res}/images/pass.gif" width="69" height="17" /></div></td>
                <td><div align="right"><img src="${res}/images/user.gif" width="17" height="17" /></div></td>
                <td><div align="right" style="cursor:pointer" onclick="if(confirm('您确定要对出系统吗?')) window.parent.location='${base}/logout.do'"><img src="${res}/images/quit.gif" width="69" height="17" /></div></td>
              </tr>
            </table></td>
          </tr>
        </table></td>
      </tr>
    </table></td>
  </tr>
  <tr>
    <td height="40" background="${res}/images/main_10.gif"><table width="100%" border="0" cellspacing="0" cellpadding="0">
      <tr>
        <td width="194" height="40" background="${res}/images/main_07.gif">&nbsp;</td>
        <td><table width="100%" border="0" cellspacing="0" cellpadding="0">
          <tr>
            <td width="21"><img src="${res}/images/main_13.gif" width="19" height="14" /></td>
            <td width="35" class="STYLE7"><div align="center">首页</div></td>
            <td width="21" class="STYLE7"><img src="${res}/images/main_15.gif" width="19" height="14" /></td>
            <td width="35" class="STYLE7"><div align="center" style="cursor:pointer" onclick="window.parent['mainFrame'].document.right.history.go(-1)">后退</div></td>
            <td width="21" class="STYLE7"><img src="${res}/images/main_17.gif" width="19" height="14" /></td>
            <td width="35" class="STYLE7"><div align="center" style="cursor:pointer" onclick="window.parent['mainFrame'].document.right.history.go(1)">前进</div></td>
            <td width="21" class="STYLE7"><img src="${res}/images/main_19.gif" width="19" height="14" /></td>
            <td width="35" class="STYLE7"><div align="center" style="cursor:pointer" onclick="window.parent['mainFrame'].document.right.location.reload(); ">刷新</div></td>
            <td width="21" class="STYLE7"><img src="${res}/images/main_21.gif" width="19" height="14" /></td>
            <td width="35" class="STYLE7"><div align="center">帮助</div></td>
            <td>&nbsp;</td>
          </tr>
        </table></td>
        <td width="248" background="${res}/images/main_11.gif"><table width="100%" border="0" cellspacing="0" cellpadding="0">
          <tr>
            <td width="16%"><span class="STYLE5"></span></td>
            <td width="75%">
            	<div align="center">
	            	<span class="STYLE7">
		            	<span>
		            	   <script>
				            	today=new Date();
								var hours = today.getHours();
								var minutes = today.getMinutes();
								var seconds = today.getSeconds();
								function initArray(){
								this.length=initArray.arguments.length
								for(var i=0;i<this.length;i++)
								this[i+1]=initArray.arguments[i] }
								var d=new initArray("星期日","星期一","星期二","星期三","星期四","星期五","星期六");
								document.write(today.getYear(),"年",today.getMonth()+1,"月",today.getDate(),"日  ",d[today.getDay()+1]," ");
						   </script>
		           		 </span>
		           	</span>
		         </div>
		     </td>
            <td width="9%">&nbsp;</td>
          </tr>
        </table></td>
      </tr>
    </table></td>
  </tr>
  <tr>
    <td height="30" background="${res}/images/main_31.gif"><table width="100%" border="0" cellspacing="0" cellpadding="0">
      <tr>
        <td width="8" height="30"><img src="${res}/images/main_28.gif" width="8" height="30" /></td>
        <td width="147" background="${res}/images/main_29.gif"><table width="100%" border="0" cellspacing="0" cellpadding="0">
          <tr>
            <td width="24%">&nbsp;</td>
            <td width="43%" height="20" valign="bottom" class="STYLE1">管理菜单</td>
            <td width="33%">&nbsp;</td>
          </tr>
        </table></td>
        <td width="39"><img src="${res}/images/main_30.gif" width="39" height="30" /></td>
        <td><table width="100%" border="0" cellspacing="0" cellpadding="0">
          <tr>
            <td height="20" valign="bottom"><span class="STYLE1">当前登录用户：&nbsp;用户角色：管理员</span></td>
            <td valign="bottom" class="STYLE1">
            	<div align="right">
            		<img src="${res}/images/sj.gif" width="6" height="7" /> &nbsp; &nbsp;&nbsp;<img src="${res}/images/sj.gif" width="6" height="7" /> &nbsp;提供商：西安工程大学管理学院  经济与管理实验教学中心&nbsp; &nbsp; <img src="${res}/images/sj.gif" width="6" height="7" /> &nbsp;
            			单位：经济与管理实验教学中心
            	</div>
            </td>
          </tr>
        </table></td>
        <td width="17"><img src="${res}/images/main_32.gif" width="17" height="30" /></td>
      </tr>
    </table></td>
  </tr>
</table>
</body>
</html>
