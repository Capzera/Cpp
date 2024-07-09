// JavaScript Document
$(function(){
	 $(".must").blur( function () { blur_chk($(this)) });
	 $(".mustRefund").blur( function () { blur_chk($(this)) });
	 $(".unhanzi").keyup( function () { char_chk($(this)) });
	 $(".number").blur( function () { number_chk($(this)) });
})
function blur_chk(obj)
{
	if(obj.val() == "")
	{
		obj.nextAll(".availWord").empty();
		obj.nextAll(".availWord").append("<span class='unavail'>"+get_field_txt(obj)+"不能为空！</span>");
	}		
	else
	{
		obj.nextAll(".availWord").empty();
	}	
}
function length_chk(obj)
{
  if(obj.val()!="")
  {
    if(obj.val().length<6 || obj.val().length>18)
    {
        obj.nextAll(".availWord").empty();
		obj.nextAll(".availWord").append("<span class='unavail'>"+get_field_txt(obj)+"不符合指定长度！</span>");		
	}
  }
}
function length_chk_param(obj, len)
{
	if(obj.val()!="")
	{
		if(obj.val().length!=len)
		{
			obj.nextAll(".availWord").empty();
			obj.nextAll(".availWord").append("<span class='unavail'>"+get_field_txt(obj)+"不符合指定长度！</span>");		
		}
	}
}

function grade_check(obj)
{
	if(obj.val()!="")
	{
		if(obj.val() > 100 || obj.val() < 0)
		{
			obj.nextAll(".availWord").empty();
			obj.nextAll(".availWord").append("<span class='unavail'>"+get_field_txt(obj)+"不符合范围！</span>");		
		}
	}
}

function current_Check(mustClass)
{	
	error_count=0
	for(i=0;i<$('.'+mustClass).size();i++)
	{
		obj = $('.'+mustClass).eq(i);
		if(obj.val() == "")
		{
			if(obj.nextAll(".availWord").html() == '')
			{
			  obj.nextAll(".availWord").append("<span class='unavail'>"+get_field_txt(obj)+"不能为空！</span>");
			}
			error_count = error_count+1	
		}		
		else
		{
			//obj.nextAll(".availWord").empty();
		}
	}
	//alert(error_count)
	if(error_count>0)
	{
		return false;		
	}
  //alert($(".unavail").size())
	if($(".unavail").size()>0)
	{
		return false;			
	}
	else
	{
		return true;
	}
}

function submit_chk()
{	
	error_count=0
	for(i=0;i<$('.must').size();i++)
	{
		obj = $('.must').eq(i);
		if(obj.val() == "")
		{
			if(obj.nextAll(".availWord").html() == '')
			{
			  obj.nextAll(".availWord").append("<span class='unavail'>"+get_field_txt(obj)+"不能为空！</span>");
			}
			error_count = error_count+1	
		}		
		else
		{
			//obj.nextAll(".availWord").empty();
		}
	}
	//alert(error_count)
	if(error_count>0)
	{
		return false;		
	}
  //alert($(".unavail").size())
	if($(".unavail").size()>0)
	{
		return false;			
	}
	else
	{
		return true;
	}
}
function get_field_txt(obj)
{
	var str = obj.parent("td").prev("td").text()
	str = str.replace("：","")
	return str;
}

function number_chk(obj){
	  var valTmp = $(obj).attr("value");
 	  var rgexp=/^[0-9]+(\.[0-9]{1,3})?$/;
 	  var rest;
 	  rest = rgexp.test(valTmp);
 	  if(rest == false){
 		 $(obj).attr("value","");
 		 obj.nextAll(".availWord").empty();
 		 obj.nextAll(".availWord").append("<span class='unavail'>"+get_field_txt(obj)+"只能为数字！</span>");
	  	 return false;
	  }else{
		 obj.nextAll(".availWord").empty();
		 return true;
	  }
}
//金额大写转换函数
function changeNumToChn(n) {
	
	var reg = /^[0-9]+(\.[0-9]{1,9})?$/;
	if (!reg.test(n))  
		return "数据非法";  
	var unit = "千百拾亿千百拾万千百拾元角分", str = "";  
	n += "00";  
	var p = n.indexOf('.');  
	if (p >= 0){  
		n = n.substring(0, p) + n.substr(p+1, 2);
	}
	unit = unit.substr(unit.length - n.length);  
	for (var i=0; i < n.length; i++)  
		str += '零壹贰叁肆伍陆柒捌玖'.charAt(n.charAt(i)) + unit.charAt(i);  
	return str.replace(/零(千|百|拾|角)/g, "零").replace(/(零)+/g, "零").replace(/零(万|亿|元)/g, "$1").replace(/(亿)万|壹(拾)/g, "$1$2").replace(/^元零?|零分/g, "").replace(/元$/g, "元整");  
}  

//-----------------------------------------------------------------------------------------------------------