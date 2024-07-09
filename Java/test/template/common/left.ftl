<style>
	.nav li .sub_nav li{text-indent:5px;font-size:12px;padding-left:20px;width:198px;padding-left:10px;}      	  
	.nav li .sub_nav li a {url(../images/jiantou.gif); PADDING-LEFT: 35px;font-weight:;font-size:12px;}
	.hidden_subNav{display:none;}      	  
</style>
<script>
	function showSubNav(obj){
		var subNav = $(obj).siblings(".sub_nav");
		if($(subNav).attr("class")=="sub_nav hidden_subNav"){
			$(subNav).removeClass("hidden_subNav");
		}else{
			$(subNav).addClass("hidden_subNav");
		}
		
	}      	  
</script>      

      	  <li id="bottom_none2">
      	  	<a href="javascript:void(0)" onclick="gotoPage(this,'_top')" ref="${base}/user/standard/center?companyId=<#if COMPANY_TYPE!="1">${COMPANY_ID}</#if>" >专业标准</a>
      	  		<ul>
	      	  	   <#list standardClassList as list>
	      	     		<li><a href="javascript:;" onclick="gotoPage(this,'_top')" ref="${base}/user/standard/search?classId=${list.SID}">${list.NAME}</a></li>
	      	       </#list>
      	        </ul>
      	  </li>
      	  <li id="bottom_none2">
      	  	<a href="javascript:void(0)" onclick="showSubNav(this)" ref="${base}/user/report/center?companyId=<#if COMPANY_TYPE!="1">${COMPANY_ID}</#if>" >报表中心</a>
      	  	<ul class="sub_nav<#if display=="report"><#else> hidden_subNav</#if>" id="report">
      	  	   <#list companyList as company>
      	     		<li><a href="javascript:void(0)" style="display:<#if COMPANY_TYPE!="1" && company.SID!=COMPANY_ID>none;</#if>" onclick="gotoPage(this,'_top')" ref="${base}/user/report/center?companyId=${company.SID}">${company.CP_NAME}</a></li>
      	       </#list>
      	    </ul>
      	  </li>
      	<#list coteList as list>
          <li id="bottom_none2">
          	<a href="javascript:void(0)" onclick="showSubNav(this)">${list.NAME}</a>
          	 <ul class="sub_nav<#if display=="document"><#else> hidden_subNav</#if>" id="doc_${list_index}">
          	    <#list companyList as company>
      	     		<li><a href="javascript:void(0)" style="display:<#if COMPANY_TYPE!="1" && company.SID!=COMPANY_ID>none;</#if>" onclick="gotoPage(this,'_top')" ref="${base}/user/document/${list.SID}/center?companyId=${company.SID}">${company.CP_NAME}</a></li>
      	     	</#list>
      	     </ul> 
          </li>
        </#list>
        <li id="bottom_none2">
      	  	<a href="javascript:void(0)" onclick="gotoPage(this,'_top')" ref="${base}/user/bbs/index">技术论坛</a>
        </li>	  