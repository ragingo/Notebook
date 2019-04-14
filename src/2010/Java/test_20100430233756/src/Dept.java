import java.io.Serializable;
import java.util.List;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.Id;
import javax.persistence.JoinColumn;
import javax.persistence.OneToMany;
import javax.persistence.Table;


@Entity
@Table(schema="SCOTT", name="DEPT")
public class Dept extends EntityBase implements Serializable {
	
	private static final long serialVersionUID = 1L;

	@Id
	@Column(name="DEPTNO", nullable=false, unique=true)
	@JoinColumn
	private int deptno;
	
	@Column(name="DNAME")
	private String dname;
	
	@Column(name="LOC")
	private String loc;
	
	@OneToMany(mappedBy="dept")
	@JoinColumn(name="deptno", referencedColumnName="deptno")
	private List<Emp> emps;

	public int getDeptno() {
		return deptno;
	}
	public void setDeptno(int deptno) {
		this.deptno = deptno;
	}

	public String getDname() {
		return dname;
	}
	public void setDname(String dname) {
		this.dname = dname;
	}

	public String getLoc() {
		return loc;
	}
	public void setLoc(String loc) {
		this.loc = loc;
	}

	public List<Emp> getEmps() {
		return emps;
	}
	public void setEmps(List<Emp> emps) {
		this.emps = emps;
	}
}
