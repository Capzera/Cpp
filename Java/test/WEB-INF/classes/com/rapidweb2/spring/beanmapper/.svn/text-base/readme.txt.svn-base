要使用BeanMapper，需要做如下几件事情：
1、XML配置中增加：

    <bean id="jdbcTemplate" class="org.springframework.jdbc.core.JdbcTemplate">
        <property name="dataSource" ref="dataSource" />
    </bean>
    <bean id="beanQuery" class="com.rapidweb2.spring.beanmapper.BeanQuery" scope="prototype">
    	<property name="jdbcTemplate" ref="jdbcTemplate" />
    </bean>
	<bean id="beanUpdate" class="com.rapidweb2.spring.beanmapper.BeanUpdate" scope="prototype">
    	<property name="jdbcTemplate" ref="jdbcTemplate" />
    </bean>


