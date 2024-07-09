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
	var uname;
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
				      //if(data.status == "false"){
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
				      //}
				      if(data.status == "true" && data.isOrdered == "other"){
				         alert("该时间已预约，请刷新后预约其他时间！");
				      }if(data.isOrdered == "self" && data.status == "true"){
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
	}
	div{
		font-size:14px;
		text-align:center;
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
	    width: 100%;    
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

<div>
	<div style="text-align:left;padding:5px;"> <img src="${base}/resource/images/add.gif"> <Strong>添加预约</Strong></div>
</div>
<div style="border:0px solid #CCC;width:100%;margin-left: auto;margin-right: auto; text-align:left;text-indent:5px;margin-bottom:5px;">
	<span style="color:#0e2d5f;font-family:'黑体'">
		<B>${yearTerm.STUDY_YEAR_TERM} 第 
		   <select id="theWeekth" style="width:40px;" onchange="location.href='${base}/admin/test/order/add/' + $(this).val()">
			   <#list 1..20 as w>
				 <option value = ${w} <#if w == weekTh>selected</#if>>${w}</option>
			   </#list>
	       </select>周
	    </B>
	</span>
	<span style="color:RED"> <strong>在下表<span style="color:blue">蓝色</span>横线内输入：“教师名，课程名”(李刚，计算机基础)，按回车"Enter"键提交。</strong>
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

   <#list [12,34,56,78,911] as section>
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


</body>
</html>