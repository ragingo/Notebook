
import java.io.Serializable;
import javax.persistence.*;
import java.math.BigDecimal;


@Entity
@Table(schema="SCOTT", name="SALGRADE")
public class Salgrade extends EntityBase implements Serializable {
	private static final long serialVersionUID = 1L;
	private BigDecimal grade;
	private BigDecimal hisal;
	private BigDecimal losal;

    public Salgrade() {
    }


    @Id
	@Column(precision=22)
	public BigDecimal getGrade() {
		return this.grade;
	}

	public void setGrade(BigDecimal grade) {
		this.grade = grade;
	}


	@Column(precision=22)
	public BigDecimal getHisal() {
		return this.hisal;
	}

	public void setHisal(BigDecimal hisal) {
		this.hisal = hisal;
	}


	@Column(precision=22)
	public BigDecimal getLosal() {
		return this.losal;
	}

	public void setLosal(BigDecimal losal) {
		this.losal = losal;
	}

}