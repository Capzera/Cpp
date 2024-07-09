<#list dynamicList as list>
 <TABLE border="0" cellSpacing="0" cellPadding="0" width="415" bgColor="#f8f8f8">
                   <TBODY>
                    <TR>
                      <TD width="30" align="center"><IMG src="${base}/resource/images/dot3.jpg" 
                        width="7" height="7"></TD>
                      <TD height="23" width="360"><A href="${base}/article/detail/${list.SID}">${list.TITILE}</A></TD></TR>
                    <TR>
                      <TD height="1" background="${base}/resource/images/line.jpg" colSpan="2"></TD>
                    </TR>
                   </TBODY>
 </TABLE>
</#list>