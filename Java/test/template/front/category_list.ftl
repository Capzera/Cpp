<#list categoryList as list>
 						<td vAlign="top" align="left">
                        	<table border="0" cellSpacing="0" cellPadding="0" width="110">
	                          <tbody>
	                          <tr>
	                            <td vAlign="top" width="3"><img src="/resource/images/banner_shu.jpg" width="3" height="38"></td>
	                            <td width="107" align="center"><a class="dh_14px" href="<#if list.SHOW_VIEW=="1">${base}/article/list/${list.SID}<#else>${base}/article/detail/${list.SID}?source=index</#if>">${list.CATEGORY_NAME}</a></td></tr></tbody></table>
	                    </td>
</#list>	                
	                  