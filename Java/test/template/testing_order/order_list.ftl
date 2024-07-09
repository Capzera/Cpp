<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3c.org/TR/1999/REC-html401-19991224/loose.dtd">
<!-- saved from url=(0028)http://som.xjtu.edu.cn/MBEC/ -->
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd"><HTML 
xmlns="http://www.w3.org/1999/xhtml"><HEAD>
<META content="text/html; charset=utf-8" http-equiv="Content-Type">
<TITLE>西安工程大学管理学院-经济与管理实验教学中心-首页</TITLE>
	<LINK rel="stylesheet" type="text/css" href="${base}/resource/css/systyle.css">
<META name="GENERATOR" content="MSHTML 9.00.8112.16633"></HEAD>
<BODY>
<SCRIPT type="text/javascript" src="${base}/resource/flash/swfobject.js"></SCRIPT>
<script type="text/javascript" src="${base}/resource/common/js/jquery-1.4.4.min.js"></script>
<script>
	var uname="${user.UNAME}";
    $.ajaxSetup({
      type: 'POST',
      complete: function(xhr,status) {
          var sessionStatus = xhr.getResponseHeader('sessionstatus');
          if(sessionStatus == 'timeout') {
             var top = getTopWinow();
             var yes = confirm('由于您长时间没有操作, session已过期, 请重新登录.');
             if (yes) {
                 top.location.href = '${base}/login';            
             }
         }
     }
   });
   
   function getTopWinow(){
     var p = window;
     while(p != p.parent){
         p = p.parent;
     }
     return p;
   }
   
   
   $(document).ready(function(){
   	 getTestingOrder(${weekTh});
   })
   
  
   
   function getTestingOrder(weekTh){
   		$.ajax({
    		url:"${base}/order/testing/search/" + weekTh,
    		method:"post",
    		dataType:"json",
    		success:function(data){
    		    var testOrderArray = [];
    		    for(m = 0; m < data.length; m++){
    		       
    		    	var orderId = data[m].section + "-" + data[m].week_DAY + "-" + data[m].testRoom.test_ROOM_NO;
    		    	var orderContent = data[m].teacher +"，" + data[m].test_CONTENT;
    		    	var testOrder = {"id":orderId,"content":orderContent};
    		    	testOrderArray.push(testOrder);
    		    }
    			var possibleOrderLines = $(".possibleOrderLine");
    			var orderTestCellIds = [];
    			for(i = 0; i < possibleOrderLines.length; i++){
    				var testOrders = $(possibleOrderLines[i]).children(".testOrder");
    				for(j = 0; j < testOrders.length; j++){
    					var divCells = $(testOrders[j]).children("div");
    					for(k = 0 ; k < divCells.length; k++){
    						orderTestCellIds.push($(divCells[k]).attr("id"));
    					}
    				}
    			}
    			
    			for(i = 0 ; i< orderTestCellIds.length;i++){
    				for(j = 0 ;j < testOrderArray.length; j ++ ){
    					if(testOrderArray[j].id == orderTestCellIds[i]){
    					  var tName = testOrderArray[j].content.split("，")[0];
    					
    					  if(uname == tName){
    					  	$("#"+testOrderArray[j].id).html("<div style='padding-top:2px;font-size:12px;color:#092d5f'><B>" + testOrderArray[j].content + "</B></div>");
    					  }else{
    					  	$("#"+testOrderArray[j].id).html("<div style='padding-top:2px;font-size:12px;'>" + testOrderArray[j].content + "</div>");
    					  }
    					  
    					}
    				}
    			}
    		}
    	})
   }
        
    	
    
    function testOrder(obj){
       var keynum;
       if(window.event){//IE
	  	  keynum = event.keyCode;
	   }else if(event.which){//firefox
	  	  keynum = event.which;
	   }
       if(keynum == 13){
       		var orderContent = $(obj).val();
       		if(orderContent == ""){
       			alert('请输入“上课人，上课内容”！');
       			return false;
       		}
        	var id = $(obj).parent().attr("id");
        	var idArray = id.split("-");
        	var orderContentArray = orderContent.split("，");
        	if(orderContentArray.length < 2){
        		alert('按照格式“上课人，上课内容”输入内容！');
        		return false;
        	}
        	var weekTh = $("#theWeekth").val();
        	var url = "${base}/order/testing/isOrdered?" + "SECTION=" + idArray[0] + "&WEEK_DAY=" + idArray[1] + "&TEST_ROOM_NO=" + idArray[2] + "&TEST_CONTENT=" + encodeURI(orderContentArray[1]) + "&TEACHER=" + encodeURI(orderContentArray[0]) + "&WEEK_TH=" + weekTh;
       		$.ajax({
			     url:url,
			     method:"POST",
			     dataType:"json",
			     success:function(data){
			     	  //alert(data.sessionStatus);
			     	  if(data.couldOrder == "false"){
			     	  	 alert("对不起，您只能预约" + data.limitDay +"天以后的时间！");
			     	  	 $(obj).val("");
			     	  	 return false;
			     	  }
			          if(data.nameExist == "false"){
			          	 alert('输入教师:' + '“' + orderContentArray[0] + '”' + '不存在，请重新输入！');
			          	 $(obj).val("");
			          	 return false;
			          }
			          if(data.isOrdered == "sameTime"){
			          	 alert('“' + orderContentArray[0] + '”' + "已在周" +  idArray[1] + '的' + idArray[0] +"节排了实验，请更换上课时间!");
			          	 $(obj).val("");
			          	 return false;
			          }
				      if(data.status == "false"){
				          $.ajax({
				        		url:"${base}/order/testing/save?" + "SECTION=" + idArray[0] + "&WEEK_DAY=" + idArray[1] + "&TEST_ROOM_NO=" + idArray[2] + "&TEST_CONTENT=" + encodeURI(orderContentArray[1]) + "&TEACHER=" + encodeURI(orderContentArray[0]) + "&WEEK_TH=" + weekTh,
				        		method:"POST",
				        		dataType:"json",
				        		success:function(data){
				        			if(data.status == "success"){
				        			   if( data.uname == orderContentArray[0]){
				        			   		var line = '<tr>'+
				        							'<td>第'+ weekTh + '周，星期' + idArray[1] + '，' + idArray[0] + ' 节，' +  idArray[2] + '教室</td>' + 
				   									'<td><a ref="${base}/order/testing/cancel/' + data.sid + '" onclick="cancelOrderTest(this)" style="cursor:pointer;color:red"><img src="${base}/resource/images/del.gif"> 取消预约</A></td>' + 
				   									'</tr>';
					        				$("#theFirstLine").after(line);
				        			   }
				        			   $(obj).parent().html("<div style='padding-top:2px;font-size:12px;color:#092d5f'><B>" + orderContent + "</B></div>");
				        			   alert("预约实验成功!");
				        			}else{
				        				alert("预约实验失败!");
				        			}
				        		}
        				  })
				      }
				      if(data.status == "true" && data.isOrdered == "other"){
				         alert("该时间已预约，请刷新后预约其他时间！");
				      }
				      if(data.isOrdered == "self" && data.status == "true"){
				         alert("您已经预约过该时间，请重新预约其他时间！");
				      }
			     }
           })

       }
    }
</script>

<style>
	.orderedCell{
		border-bottom:1px solid #CCC;
		color:#005;
	}
	div{
		font-size:14px;
		text-align:center;
		color:#005;
	}
	.section{
		font-size:14px;
		text-align:center;
	}
	input{
		font-size:12px;
		width:99%;
		border-bottom:1px solid #95b8e7;
		border-left:0px;
		border-right:0px;
		height:13px;
		border-top:0px;
		text-align:center;
	}
	.testOrder{
		width:180px;
		vertical-align:middle;
	}
	body {TEXT-ALIGN: center;}
	.test-order-content table tbody tr td{
		font-size:12px;
	}
	
	.tabled{
	    *border-collapse: collapse; /* IE7 and lower */
	    border-spacing: 0;
	    width: 1122px;    
	}

	.bordered {
	    border: solid #ccc 1px;
	    -moz-border-radius: 6px;
	    -webkit-border-radius: 6px;
	    border-radius: 6px;
	    -webkit-box-shadow: 0 1px 1px #ccc; 
	    -moz-box-shadow: 0 1px 1px #ccc; 
	    box-shadow: 0 1px 1px #ccc;         
	}

.bordered tr:hover {
    background: #fbf8e9;
    -o-transition: all 0.1s ease-in-out;
    -webkit-transition: all 0.1s ease-in-out;
    -moz-transition: all 0.1s ease-in-out;
    -ms-transition: all 0.1s ease-in-out;
    transition: all 0.1s ease-in-out;     
}    
    
.bordered td, .bordered th {
    border-left: 1px solid #ccc;
    border-top: 1px solid #ccc;
    padding:3px;
    text-align: center;    
}

.bordered th {
    background-color: #dce9f9;
    background-image: -webkit-gradient(linear, left top, left bottom, from(#ebf3fc), to(#dce9f9));
    background-image: -webkit-linear-gradient(top, #ebf3fc, #dce9f9);
    background-image:    -moz-linear-gradient(top, #ebf3fc, #dce9f9);
    background-image:     -ms-linear-gradient(top, #ebf3fc, #dce9f9);
    background-image:      -o-linear-gradient(top, #ebf3fc, #dce9f9);
    background-image:         linear-gradient(top, #ebf3fc, #dce9f9);
    -webkit-box-shadow: 0 1px 0 rgba(255,255,255,.8) inset; 
    -moz-box-shadow:0 1px 0 rgba(255,255,255,.8) inset;  
    box-shadow: 0 1px 0 rgba(255,255,255,.8) inset;        
    border-top: none;
    text-shadow: 0 1px 0 rgba(255,255,255,.5); 
}

.bordered td:first-child, .bordered th:first-child {
    border-left: none;
}

.bordered th:first-child {
    -moz-border-radius: 6px 0 0 0;
    -webkit-border-radius: 6px 0 0 0;
    border-radius: 6px 0 0 0;
}

.bordered th:last-child {
    -moz-border-radius: 0 6px 0 0;
    -webkit-border-radius: 0 6px 0 0;
    border-radius: 0 6px 0 0;
}

.bordered th:only-child{
    -moz-border-radius: 6px 6px 0 0;
    -webkit-border-radius: 6px 6px 0 0;
    border-radius: 6px 6px 0 0;
}

.bordered tr:last-child td:first-child {
    -moz-border-radius: 0 0 0 6px;
    -webkit-border-radius: 0 0 0 6px;
    border-radius: 0 0 0 6px;
}

.bordered tr:last-child td:last-child {
    -moz-border-radius: 0 0 6px 0;
    -webkit-border-radius: 0 0 6px 0;
    border-radius: 0 0 6px 0;
}
</style>

<table width="1124" align="center" border="0" cellSpacing="0" cellPadding="0" bgColor="#FFFFFF">
  <tbody>
  <tr>
    <td bgColor="#054197" height="4"></td></tr>
  <tr>
    <td height="22">
      <table border="0" cellSpacing="0" cellPadding="0" width="1124" align="center">
        <tbody>
        <tr>
          <td height="65" width="44">&nbsp;</td>
          <td width="366" align="left"><a href="${base}/index"><img border="0" src="${base}/resource/images/logo_ctr.jpg" width="387" height="95"></a></td>
          <td width="439">
            <table border="0" cellSpacing="0" cellPadding="0" width="300" height="35">
               <form id="form1" method="post" name="form1" action="${base}/article/search">
	              <tbody>
	              <tr>
	                <td width="30"><label><img src="${base}/resource/images/sec.jpg" width="22" height="20"></label></td>
	                <td width="80"><span class="huifont">本站搜索：</span></td>
	                <td width="168"><input  type="text" name="keywords"></td>
	                <td width="33"><input type="image" src="${base}/resource/images/seach.jpg"></td></tr></tbody>
	          </form>
              <tbody></tbody></table></td>
          <td vAlign="top" width="147">
            <table border="0" cellSpacing="0" cellPadding="0" width="147" height="28">
              <tbody>
              <tr>
                <td height="35" vAlign="top"><a href="http://guanlixy.xpu.edu.cn/" target="_blank"><img border="0" align="top" src="${base}/resource/images/dot.jpg" width="96" height="18"></a></td></tr></tbody></table></td></tr>
      </tbody></table>
      <table class="dh_bg" border="0" cellSpacing="0" cellPadding="0" width="1124" align="center">
        <tbody>
        <tr>
          <td height="39" width="937">
            <table border="0" cellSpacing="0" cellPadding="0" width="1124" align="center">
              <tbody>
              <tr>
                <td width="50" align="center"><a class="dh_14px" href="${base}/index">首页</a></td>
                <td width="660" align="left">
                  <table style="border-collapse: collapse;" border="0" cellSpacing="0">
                    <tbody>
                    <tr id="topMenu">
	                     <td vAlign="top" align="left">
                        	<table border="0" cellSpacing="0" cellPadding="0" width="100">
	                          <tbody>
	                          <tr>
	                            <td vAlign="top" width="3"><img src="${base}/resource/images/banner_shu.jpg" width="3" height="38"></td>
	                            <td width="80" align="center"><a class="dh_14px" href="${base}/user/leave_msg/list">留言板</a></td>
	                          </tr>
	                         </tbody>
	                        </table>
	                    </td>
	                    <td vAlign="top" align="left">
                        	<table border="0" cellSpacing="0" cellPadding="0" width="100">
	                          <tbody>
	                          <tr>
	                            <td vAlign="top" width="3"><img src="${base}/resource/images/banner_shu.jpg" width="3" height="38"></td>
	                            <td width="100" align="center"><a class="dh_14px" href="${base}/user/logs/list">设备维护日志</a></td>
	                          </tr>
	                         </tbody>
	                        </table>
	                    </td>
	                    <td vAlign="top" align="left">
                        	<table border="0" cellSpacing="0" cellPadding="0" width="100">
	                          <tbody>
	                          <tr>
	                            <td vAlign="top" width="3"><img src="${base}/resource/images/banner_shu.jpg" width="3" height="38"></td>
	                            <td width="80" align="center"><a class="dh_14px" href="${base}/login">实验预约</a></td>
	                          </tr>
	                         </tbody>
	                        </table>
	                    </td>
                     </tr>
                    </tbody></table></td>
                </tr></tbody></table></td></tr>
      </tbody></table></td></tr>
  
 </tbody>
 </table>
 </td>
 </tr>
 </tbody>
</table>
<div style="border:0px solid #CCC;width:1124px;margin-left: auto;margin-right: auto; text-align:left;text-indent:5px;margin-bottom:5px;">
	<span style="color:#0e2d5f;font-family:'黑体'">
		<B>
			教室信息
	    </B>
	</span>
</div>
<table align="center" class="bordered tabled">
	<tr>
		<th>教室</th>
		<th>名称</th>
		<th>软件设备</th>
	</tr>
	<#list testRoomList as tRoom>
		<tr>
			<td>${tRoom.TEST_ROOM_NO}(座位:${tRoom.SEATS!'0'})</td>
			<td>${tRoom.NAME}</td>
			<td>${tRoom.SOFT_DEVICE}</td>
		</tr>
	</#list>
</table>
<div style="border:0px solid #CCC;width:1124px;margin-left: auto;margin-right: auto; text-align:left;text-indent:5px;margin-bottom:5px;">
	<span style="color:#0e2d5f;font-family:'黑体'">
		<B>${yearTerm.STUDY_YEAR_TERM} 第 
		   <select id="theWeekth" style="width:40px;" onchange="location.href='${base}/order/testing/list/' + $(this).val()">
			   <#list 1..20 as w>
				 <option value = ${w} <#if w == weekTh>selected</#if>>${w}</option>
			   </#list>
	       </select>周
	    </B>
	</span>
	<span style="color:RED"> <strong>在下表<span style="color:blue">蓝色</span>横线内输入：“教师名，课程名”(李刚，计算机基础)，按回车"Enter"键提交。</strong>
	</span>
	<span style="text-align:right;text-indent:5px;" width="500px">
		<span style="color:#0e2d5f;cursor:pointer; padding:5px;" onclick="logout()"><strong><img src="${base}/resource/images/quit.gif"></strong></span>
		<script>
			function logout(){
				var top = getTopWinow();
				if(confirm("您确定要退出系统吗？")){
		          top.location.href = '${base}/logout';            
				}
				return false;
			}
		</script>
	</span>
</div>

<table align="center" class="tabled bordered">
	<tr>
		<th>节次</th>
		<th>教室</th>
		<th>周一 (${one})</th>
		<th>周二 (${two})</th>
		<th>周三 (${three})</th>
		<th>周四 (${four})</th>
		<th>周五 (${five})</th>
	</tr>

   <#list [12,34,56,78] as section>
	<tr class="possibleOrderLine">
		<td class="section" width="50px">${section}</td>
		<td vertical-align=middle style="width:100px;">
		   <#list testRoomList as room>
			  	<div style="font-size:12px;" <#if room_index != testRoomList?size -1>class="orderedCell"</#if>>${room.TEST_ROOM_NO}(座位:${room.SEATS!'0'})</div>
		   </#list>
		</td>
		<td class="testOrder">
		    <#list testRoomList as room>
				<div <#if room_index != testRoomList?size -1>class="orderedCell"</#if> id="${section}-1-${room.TEST_ROOM_NO}"><input type="text" onkeypress="testOrder(this)"></div>
		    </#list>
		</td>
		<td class="testOrder">
			 <#list testRoomList as room>
				<div <#if room_index != testRoomList?size -1>class="orderedCell"</#if> id="${section}-2-${room.TEST_ROOM_NO}"><input type="text" onkeypress="testOrder(this)"></div>
		    </#list>
		</td>
		<td class="testOrder">
	            <#list testRoomList as room>
				<div <#if room_index != testRoomList?size -1>class="orderedCell"</#if> id="${section}-3-${room.TEST_ROOM_NO}"><input type="text" onkeypress="testOrder(this)"></div>
		    </#list>
		</td>
		<td class="testOrder">
			 <#list testRoomList as room>
				<div <#if room_index != testRoomList?size -1>class="orderedCell"</#if> id="${section}-4-${room.TEST_ROOM_NO}"><input type="text" onkeypress="testOrder(this)"></div>
		    </#list>
		</td>
		<td class="testOrder">
		    <#list testRoomList as room>
				<div <#if room_index != testRoomList?size -1>class="orderedCell"</#if> id="${section}-5-${room.TEST_ROOM_NO}"><input type="text" onkeypress="testOrder(this)"></div>
		    </#list>
		</td>
	</tr>
  </#list>
	
</table>
<div CLASS="user-test-order" style="border:0px solid #CCC;width:1124px;margin-left: auto;margin-right: auto;">
	<div class="test-title" style="text-align:left;color:#0e2d5f;text-indent:5px;padding-top:5px;font-family:'黑体'""><B>我的实验课</B></div>
	<div class="test-order-content" style="text-align:left;">
		<table border="0px">
		  <tr id = 'theFirstLine'>
		  	<td></td><td></td>
		  </tr>
		  <#list testOrderList as testOrder>
				<tr>
				   <td>第${testOrder.WEEK_TH}周，星期${testOrder.WEEK_DAY}，${testOrder.SECTION} 节，${testOrder.testRoom.TEST_ROOM_NO}教室</td>
				   <td><a ref="${base}/order/testing/cancel/${testOrder.SID}" onclick="cancelOrderTest(this)" style="cursor:pointer;color:red"><img src="${base}/resource/images/del.gif"> 取消预约</A></td>
				</tr>
		  </#list>
		</table>
	</div>
</div>
<script>
	function cancelOrderTest(obj){
		var url = $(obj).attr("ref");
		if(confirm("您确定要取消该次实验吗？")){
			var theWeekth = $("#theWeekth").val();
			location.href=url + '?WEEK_TH=' + theWeekth;
		}
	}
</script>
<#include "front/bottom.ftl">
</body>
</html>