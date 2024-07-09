/**
 * 项目名称：SanXine.com善行网，“打造国内有车一族的第一社区”
 * 版权申明：sanxine.com所有，未经许可不得在任何软件中以任何形式使用全部或部分代码，不得更改本项目的代码。
 * 文件名称：${beanName}.java
 * 创建时间：${now}
 * 创建用户：代码生成器
 * 文件描述：
 * 修改记录：
 */
package ${packageName};

import javax.persistence.Id;
import javax.persistence.Column;
import javax.persistence.Entity;

/**
 * 从表${tableName}自动生成的JAVA BEAN类：<br/>
 * ${beanCaption}
 *
 * @Author 代码生成器
 */
@Entity( name="${tableName}" )
public class ${beanName}  {
    <#list fields as p>
    /** ${p.caption} : ${p.comment} */
    private ${p.type}	${p.name};
    </#list>
    
    <#list fields as p>
    /**
	 * 获得属性[ ${p.caption} ]: ${p.comment}
	 * @return
	 */${p.annotation}
    public ${p.type} get${p.uname}(){
    	return this.${p.name};
    }
	/**
	 * 设置属性
	 * @param ${p.name}
	 */    
    public void set${p.uname}( ${p.type} ${p.name} ){
    	this.${p.name} = ${p.name};
    }    
    </#list>
}