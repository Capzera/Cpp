package ${actionPackageName};
/**
 * 项目名称：秦稷粮业科技集团
 * 版权申明：未经许可不得在任何软件中以任何形式使用全部或部分代码，不得更改本项目的代码。
 * 文件名称：${actionName}.java
 * 创建时间：2011-8-31-下午03:54:01
 * 创建用户：blueheart
 * 文件描述：
 * 修改记录：
 * 通用Action类模板文件,根据此文件生成通用的Action类
 */


import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.annotation.Resource;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.springframework.stereotype.Controller;
import org.springframework.ui.ModelMap;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestMapping;

import com.rapidweb2.db.Pager;
import ${entityPackageName}.${entity};
import ${basePackageName}.BaseAction;
import ${basePackageName}.BaseService;
import ${basePackageName}.AjaxResponseUtil;

/**
 * TODO: 请编写相应的类使用JAVADOC说明
 * @author blueheart
 */
@Controller
@RequestMapping("/admin/${action}")
public class Admin${actionName} extends BaseAction{
	
	@Resource
	private BaseService baseService;
	
	/**
	 * 列表
	 * @param request
	 * @param model
	 * @param pager
	 * @return
	 */
	@RequestMapping("/list")
	public String listAction(HttpServletRequest request,ModelMap model,Pager pager,Integer pagerNumber){
	    if(pagerNumber != null){
			pager.setPageNumber(pagerNumber);
		}
		List<${entity}> ${entityVariable}List = baseService.pageList(${entity}.class, pager, null, new Object[]{});
		model.addAttribute("pager", pager);
		model.addAttribute("${entityVariable}List", ${entityVariable}List);
		return "admin/admin_${action}_list";
		
	}
	/**
	 * 增加
	 * @return
	 */
	@RequestMapping("/add")
	public String addAction(){
		return "admin/admin_${action}_input";
	}
	/**
	 * 保存增加的数据
	 * @param hotKeyword
	 * @return
	 */
	@RequestMapping("/save")
	public String saveAction(${entity} ${entityVariable}){
		baseService.insert(${entityVariable}, "SID", true);
		return this.redirect("list.do");
	}
	/**
	 * 删除数据
	 * @param id
	 * @param response
	 * @return
	 */
	@RequestMapping("/del/{id}")
	public String delAction(@PathVariable String id,HttpServletResponse response){
		baseService.delete(${entity}.class, Integer.parseInt(id));
		Map<String,String> jsonMap = new HashMap<String,String>();
		jsonMap.put(AjaxResponseUtil.STATUS, AjaxResponseUtil.SUCCESS);
		return AjaxResponseUtil.ajaxJson(response, jsonMap);
	}
	/**
	 * 删除多条数据
	 * @param request
	 * @param response
	 * @return
	 */
	@RequestMapping("/delete")
	public String deleteAction(HttpServletRequest request,HttpServletResponse response){
		Object [] ids = request.getParameter("ids").split(",");
		baseService.delete(${entity}.class, ids);
		Map<String,String> jsonMap = new HashMap<String,String>();
		jsonMap.put(AjaxResponseUtil.STATUS, AjaxResponseUtil.SUCCESS);
		return AjaxResponseUtil.ajaxJson(response, jsonMap);
	}
	/**
	 * 编辑
	 * @param id
	 * @param model
	 * @return
	 */
	@RequestMapping("/edit/{id}")
	public String editAction(@PathVariable String id,ModelMap model,Integer pageNumber){
		${entity} ${entityVariable} = baseService.getBySid(${entity}.class, Integer.parseInt(id));
		model.addAttribute("${entityVariable}", ${entityVariable});
		model.addAttribute("pageNumber",pageNumber);
		return "admin/admin_${action}_edit";
	}
	/**
	 * 保存编辑后的数据
	 * @param askRank
	 * @return
	 */
	@RequestMapping("/update")
	public String updateAction(${entity} ${entityVariable},Integer pageNumber){
		baseService.update(${entityVariable}, "SID", true);
		return this.redirect("list.do?pagerNumber="+pageNumber);
	}
	
	
}
