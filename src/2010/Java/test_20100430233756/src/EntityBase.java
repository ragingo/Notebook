import java.lang.reflect.Method;
import javax.persistence.MappedSuperclass;


@MappedSuperclass
public abstract class EntityBase {
	@Override
	public String toString() {
		StringBuilder sb = new StringBuilder();
		sb.append("{ ");
		for(Method m : this.getClass().getMethods()){
			if(m.getName().startsWith("get")){
				try {
					sb.append(m.getName())
						.append(" : ")
						.append(m.invoke(this, (Object[])null))
						.append(", ");
				}
				catch (Exception e) {
					e.printStackTrace();
				}
			}
		}
		sb.append(" }");
		return sb.toString();
	}
}
