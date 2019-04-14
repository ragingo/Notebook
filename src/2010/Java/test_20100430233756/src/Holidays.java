import java.io.Serializable;
import java.util.Calendar;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.Id;
import javax.persistence.Table;
import javax.persistence.Temporal;
import javax.persistence.TemporalType;


@Entity
@Table(schema="SAKURAGI", name="HOLIDAYS")
public class Holidays extends EntityBase implements Serializable {

	private static final long serialVersionUID = 1L;
	
	@Id
	@Column(name="HOLIDAY", nullable=false, unique=true, insertable=true)
	@Temporal(TemporalType.DATE)
	private Calendar holiday;

	public Calendar getHoliday() {
		return holiday;
	}
	public void setHoliday(Calendar holiday) {
		this.holiday = holiday;
	}
}
