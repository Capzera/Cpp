
/**
 * 项目名称：秦稷粮业科技集团
 * 版权申明：未经许可不得在任何软件中以任何形式使用全部或部分代码，不得更改本项目的代码。
 * 文件名称：SnsAskGradeAction.java
 * 创建时间：2011-8-31-下午03:54:01
 * 创建用户：blueheart
 * 文件描述：
 * 修改记录：
 * 通用Action类模板文件,根据此文件生成通用的Action类
 */


import java.io.File;
import java.io.IOException;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.annotation.Resource;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.lang.StringUtils;
import org.springframework.stereotype.Controller;
import org.springframework.ui.ModelMap;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.multipart.MultipartHttpServletRequest;
import org.springframework.web.multipart.commons.CommonsMultipartFile;

import com.rapidweb2.db.Pager;
import com.sanxine.admin.util.AdminConstant;
import com.sanxine.ask.entity.SnsAskGrade;
import com.sanxine.ask.entity.SnsAskRank;
import com.sanxine.base.BaseAction;
import com.sanxine.base.service.BaseService;
import com.sanxine.common.util.AjaxResponseUtil;
import com.sanxine.common.util.CommonUtil;

/**
 * TODO: 有1个图片的实体的模板
 * @author blueheart
 */
@Controller
@RequestMapping("/admin/${action}")
public class ${actionName} extends BaseAction{
	
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
	public String listAction(HttpServletRequest request,ModelMap model,Pager pager){
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
	 * @throws IOException 
	 * @throws IllegalStateException 
	 */
	@RequestMapping("/save")
	public String saveAction(${entity} ${entityVariable},HttpServletRequest request) throws IllegalStateException, IOException{
		MultipartHttpServletRequest multipartRequest = (MultipartHttpServletRequest) request;
		CommonsMultipartFile ${entityImg} = (CommonsMultipartFile) multipartRequest.getFile("${entityImg}");
		if (${entityImg} != null && ${entityImg}.getOriginalFilename() != "") {
			${entityVariable}.set${entityImg}(${entityImg}.getOriginalFilename());
			// 获取图片扩展名
			String mapExtension = StringUtils.substringAfterLast(${entityVariable}.get${entityImg}(), ".").toLowerCase();
			String uploadImagePath = request.getSession().getServletContext().getRealPath(AdminConstant.ICO_PATH);
			// System.out.println(uploadImagePath);
			File uploadImageDir = new File(uploadImagePath);
			String uploadImageFile = null;
			if (!uploadImageDir.exists()) {
				uploadImageDir.mkdirs();
			} else {
				uploadImageFile = CommonUtil.getUUID() + "." + mapExtension;
			}
			uploadImageFile = CommonUtil.getUUID() + "." + mapExtension;
			File file = new File(uploadImagePath + "/" + uploadImageFile);
			// 文件上传
			${entityImg}.transferTo(file);
			${entityVariable}.set${entityImg}(AdminConstant.ICO_PATH + uploadImageFile);
		}
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
	public String delAction(@PathVariable String id,HttpServletRequest request,HttpServletResponse response){
		${entity} ${entityVariable} = baseService.getBySid(${entity}.class, Integer.parseInt(id));
		File oldRankIco = new File(request.getSession().getServletContext().getRealPath("/") +"/"+ ${entityVariable}.get${entityImg}());
		if(oldRankIco.exists()){
			oldRankIco.delete();
		}
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
		String [] ids = request.getParameter("ids").split(",");
		for(String id:ids){
			${entity} ${entityVariable} = baseService.getBySid(${entity}.class, Integer.parseInt(id));
			File oldRankIco = new File(request.getSession().getServletContext().getRealPath("/") +"/"+ ${entityVariable}.get${entityImg}());
			if(oldRankIco.exists()){
				oldRankIco.delete();
			}
			baseService.delete(${entity}.class, id);
		}
		
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
	public String editAction(@PathVariable String id,ModelMap model){
		${entity} ${entityVariable} = baseService.getBySid(${entity}.class, Integer.parseInt(id));
		model.addAttribute("${entityVariable}", ${entityVariable});
		return "admin/admin_${action}_edit";
	}
	/**
	 * 保存编辑后的数据
	 * @param askRank
	 * @return
	 * @throws IOException 
	 * @throws IllegalStateException 
	 */
	@RequestMapping("/update")
	public String updateAction(${entity} ${entityVariable},HttpServletRequest request,String SID) throws IllegalStateException, IOException{
		MultipartHttpServletRequest multipartRequest = (MultipartHttpServletRequest) request;
		CommonsMultipartFile ${entityImg} = (CommonsMultipartFile) multipartRequest.getFile("${entityImg}");
		if (${entityImg} != null && ${entityImg}.getOriginalFilename() != "") {//如果有新图片
			//删除原来的图片
			${entity} old_${entity} = baseService.getBySid(${entity}.class, Integer.parseInt(SID));
			
			File old${entity}Ico = new File(request.getSession().getServletContext().getRealPath("/") +"/"+ old_${entity}.get${entityImg}());
			if(old${entity}Ico.exists()){
				old${entity}Ico.delete();
			}
			${entityVariable}.set${entityImg}(${entityImg}.getOriginalFilename());
			// 获取图片扩展名
			String mapExtension = StringUtils.substringAfterLast(${entityVariable}.get${entityImg}(), ".").toLowerCase();
			String uploadImagePath = request.getSession().getServletContext().getRealPath(AdminConstant.ICO_PATH);
			// System.out.println(uploadImagePath);
			File uploadImageDir = new File(uploadImagePath);
			String uploadImageFile = null;
			if (!uploadImageDir.exists()) {
				uploadImageDir.mkdirs();
			} else {
				uploadImageFile = CommonUtil.getUUID() + "." + mapExtension;
			}
			uploadImageFile = CommonUtil.getUUID() + "." + mapExtension;
			File file = new File(uploadImagePath + "/" + uploadImageFile);
			// 文件上传
			${entityImg}.transferTo(file);
			${entityVariable}.set${entityImg}(AdminConstant.ICO_PATH + uploadImageFile);
			baseService.update(${entityVariable}, "SID", true);
		}
		else{
			baseService.update(${entityVariable}, "SID,IMG", true);
		}
		return this.redirect("list.do");
	}
	
	
}
