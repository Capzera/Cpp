
<SCRIPT type="text/javascript" src="${base}/resource/flash/swfobject.js"></SCRIPT>
<script type="text/javascript" src="${base}/resource/common/js/jquery-1.4.4.min.js"></script>
<script>
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
    					  $("#"+testOrderArray[j].id).html("<div style='font-size:12px;'>" + testOrderArray[j].content + "</div>");
    					}
    				}
    			}
    		}
    	})
   }
        
    	
    
    function testOrder(obj){
       
       if(event.keyCode == 13){
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
				        			   $(obj).parent().html("<div style='font-size:12px;padding:1px;'>" + orderContent + "</div>");
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


<#include "common/common-js.ftl">
<script>
	function startPrint(obj){ 
	    var oWin=window.open("","_blank"); 
	    var strPrint=""; 
	    strPrint = strPrint + "<style>\n.orderedCell{\n border-bottom:1px solid #CCC;} \n  #content{\n width:996px;}\n body {\nTEXT-ALIGN: center;}\n</style> \n";
	    strPrint=strPrint + "<script type=\"text/javascript\">\n"; 
	    strPrint=strPrint + "function printWin()\n"; 
	    strPrint=strPrint + "{"; 
	    strPrint=strPrint +    "var oWin= self;\n"; 
	   // strPrint=strPrint + "oWin.focus();\n"; 
	    strPrint=strPrint + "oWin.document.close();\n"; 
	    strPrint=strPrint + "oWin.print()\n"; 
	    strPrint=strPrint + "oWin.close()\n"; 
	    strPrint=strPrint + "}\n"; 
	    strPrint=strPrint + "<\/script>\n"; 
	     
	    strPrint=strPrint + "<title>课表打印预览</title>"; 
	    strPrint=strPrint + "<body><div id=\"content\" style='margin-left: auto;margin-right: auto;'>\n"; 
	    strPrint=strPrint + obj.innerHTML + "\n"; 
	    strPrint=strPrint + "</div>\n"; 
	    strPrint=strPrint + ""; 
	    strPrint=strPrint + "<div style='text-align:center'><button onclick='printWin()' style='padding-left:4px;padding-right:4px;'>打 印</button><button onclick='window.opener=null;window.close();' style='padding-left:4px;padding-right:4px;'>关 闭</button></div></body>\n"; 
	    oWin.document.write(strPrint); 
	    oWin.focus(); 
	    oWin.document.close(); 
    } 
</script>
<div style="text-align:left;padding-top:10px;">
	<table width="100%">
		<tr>
			<td style="text-indent:5px;"><strong style="font-size:13px;">打印实验课表</strong> </td>
			<td align="right"><button style="font-size:12px;width:100px;" onclick="startPrint(document.getElementById('printedContent'))"> 打印课表 </button></td>
		</tr>
	</table>
</div>
<div id="printedContent">
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
		width:20px;
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
	    -moz-box-shadow: 0 1px 1px #ccc; 
	         
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
	<div style="border:0px solid #CCC;width:100%;margin-left: auto;margin-right: auto; text-align:left;text-indent:5px;margin-bottom:5px;">
		<span style="color:#0e2d5f;font-family:'黑体'">
			<B>${yearTerm.STUDY_YEAR_TERM} 第 
			   <select id="theWeekth" style="width:40px;" onchange="location.href='${base}/admin/test/order/print/' + $(this).val()">
				   <#list 1..20 as w>
					 <option value = ${w} <#if w == weekTh>selected</#if>>${w}</option>
				   </#list>
		       </select>周 
		    </B>
		</span>
		
	</div>
	<table width="100%" align="center" class="bordered tabled">
		<tr style="background-color:#CCC;">
			<th width="20px;" height="25px;">节次</td>
			<th>教室</td>
			<th>周一 (${one})</td>
			<th>周二 (${two})</td>
			<th>周三 (${three})</td>
			<th>周四 (${four})</td>
			<th>周五 (${five})</td>
		</tr>
	   <#list [12,34,56,78,911] as section>
		<tr class="possibleOrderLine">
			<td class="section">${section}</td>
			<td vertical-align=middle style="width:150px;">
			   <#list testRoomList as room>
				  	<div style="font-size:12px;" <#if room_index != testRoomList?size -1>class="orderedCell"</#if>>${room.TEST_ROOM_NO}(座位:${room.SEATS!'0'})</div>
			   </#list>
			</td>
			<td class="testOrder">
			    <#list testRoomList as room>
					<div <#if room_index != testRoomList?size -1>class="orderedCell"</#if> id="${section}-1-${room.TEST_ROOM_NO}">&nbsp;</div>
			    </#list>
			</td>
			<td class="testOrder">
				 <#list testRoomList as room>
					<div <#if room_index != testRoomList?size -1>class="orderedCell"</#if> id="${section}-2-${room.TEST_ROOM_NO}">&nbsp;</div>
			    </#list>
			</td>
			<td class="testOrder">
				 <#list testRoomList as room>
					<div <#if room_index != testRoomList?size -1>class="orderedCell"</#if> id="${section}-3-${room.TEST_ROOM_NO}">&nbsp;</div>
			    </#list>
			</td>
			<td class="testOrder">
				 <#list testRoomList as room>
					<div <#if room_index != testRoomList?size -1>class="orderedCell"</#if> id="${section}-4-${room.TEST_ROOM_NO}">&nbsp;</div>
			    </#list>
			</td>
			<td class="testOrder">
				 <#list testRoomList as room>
					<div <#if room_index != testRoomList?size -1>class="orderedCell"</#if> id="${section}-5-${room.TEST_ROOM_NO}">&nbsp;</div>
			    </#list>
			</td>
		</tr>
	  </#list>	
	</table>
</div><!--printedContent end -->
<script>
	function cancelOrderTest(obj){
		var url = $(obj).attr("ref");
		if(confirm("您确定要取消该次实验吗？")){
			var theWeekth = $("#theWeekth").val();
			location.href=url + '?WEEK_TH=' + theWeekth;
		}
	}
</script>
