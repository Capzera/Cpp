<#list bookList as list>
	<table width="325" bgColor="#f8f8f8" border="0" cellSpacing="0" cellPadding="0">
		
	              <tbody>
	              <tr>
	                <td width="30" align="center"><img src="/resource/images/dot3.jpg" width="7" height="7"></td>
	                <td height="23" width="280"><a href="${base}/article/detail/${list.SID}"><#if list.TITILE?length lt 20>${list.TITILE} <#else> ${list.TITILE[0..19]}...</#if></a></td></tr>
	              <tr>
	                <td height="1" background="/resource/images/line.jpg" colSpan="2"></td></tr>
	              </tbody>
	</table>
</#list>