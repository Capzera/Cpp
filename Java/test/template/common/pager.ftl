<style>
	#pager .pages li{
	  height:18px;
	  line-height:18px;
	}
</style>
<script type="text/javascript">
$().ready( function() {
    
	$("#pager").pager({
		pagenumber: <#if pager??>${pager.pageNumber}<#else>0</#if>,
		pagecount: <#if pager??>${pager.pageCount}<#else>0</#if>,
		buttonClickCallback: $.gotoPage
	});
})
</script>
<span id="pager"></span>
<input type="hidden" name="pageNumber" id="pageNumber" value="<#if pager??>${pager.pageNumber}<#else>0</#if>" />
