import java.io.Serializable;
import java.math.BigDecimal;
import java.util.Calendar;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.Id;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToOne;
import javax.persistence.Table;
import javax.persistence.Temporal;
import javax.persistence.TemporalType;


@Entity
@Table(schema="SCOTT", name="EMP")
public class Emp extends EntityBase implements Serializable{

	private static final long serialVersionUID = 1L;

	@Id
	@Column(name="EMPNO", nullable=false, unique=true)
	private int empno;
	
	@Column(name="ENAME")
	private String ename;
	
	@Column(name="JOB")
	private String job;
	
	@Column(name="MGR")
	private Integer mgr;
	
	@Column(name="HIREDATE")
	@Temporal(TemporalType.DATE)
	private Calendar hireDate;
	
	@Column(name="SAL")
	private BigDecimal sal;
	
	@Column(name="COMM")
	private BigDecimal comm;
	
	@Column(name="DEPTNO")
	private Integer deptno;
	
	@ManyToOne
	@JoinColumn(name="deptno", referencedColumnName="deptno")
	private Dept dept;

	public int getEmpno() {
		return empno;
	}
	public void setEmpno(int empno) {
		this.empno = empno;
	}

	public String getEname() {
		return ename;
	}
	public void setEname(String ename) {
		this.ename = ename;
	}

	public String getJob() {
		return job;
	}
	public void setJob(String job) {
		this.job = job;
	}

	public Integer getMgr() {
		return mgr;
	}
	public void setMgr(Integer mgr) {
		this.mgr = mgr;
	}

	public Calendar getHireDate() {
		return hireDate;
	}
	public void setHireDate(Calendar hireDate) {
		this.hireDate = hireDate;
	}

	public BigDecimal getSal() {
		return sal;
	}
	public void setSal(BigDecimal sal) {
		this.sal = sal;
	}

	public BigDecimal getComm() {
		return comm;
	}
	public void setComm(BigDecimal comm) {
		this.comm = comm;
	}

	public Integer getDeptno() {
		return deptno;
	}
	public void setDeptno(Integer deptno) {
		this.deptno = deptno;
	}
	
	public Dept getDept() {
		return dept;
	}
	public void setDept(Dept dept) {
		this.dept = dept;
	}
}
