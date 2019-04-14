import java.util.Calendar;
import java.util.List;
import javax.persistence.EntityManager;
import javax.persistence.EntityManagerFactory;
import javax.persistence.Persistence;

public class Main {

	private static final String PERSISTENCE_UNIT_NAME = "test_20100430233756";
	@SuppressWarnings("unchecked")
	public static void main(String[] args) {
		EntityManagerFactory emf = Persistence.createEntityManagerFactory(PERSISTENCE_UNIT_NAME);
		EntityManager em = emf.createEntityManager();

		String query1 = "select d from Dept as d order by d.deptno desc";
		List<Dept> result1 = (List<Dept>)em.createQuery(query1).getResultList();
		for(Dept d : result1){
			System.out.println(d.getDeptno() + "," + d.getDname() + ", " + d.getLoc());
		}

		String query2 = "select max(d.deptno) from Dept d";
		System.out.println(
			(Integer)em.createQuery(query2).getSingleResult()
		);
		
//		String query3 = "select e.empno, e.ename, d.deptno, d.dname from Emp e join e.dept d";
//		String query3 = "select e.empno, e.ename, d.deptno, d.dname from Dept d join d.emps e";
//		String query3 = "select e.empno, e.ename, d.deptno, d.dname from Dept d, in(d.emps) e";
//		String query3 = "select distinct e.empno, e.ename, d.deptno, d.dname from Dept d join d.emps as e where d.deptno > 20";
		String query3 = "select e.empno, e.ename, d.deptno, d.dname from Emp e left outer join e.dept d " +
						"where (d.deptno between 10 and 20) or d.deptno is null";
		List<Object[]> result3 = (List<Object[]>)em.createQuery(query3).getResultList();
		for(Object[] arr : result3){
			for(Object obj : arr){
				System.out.print(obj + ",\t\t\t");
			}
			System.out.println();
		}
		
		String query4 = "select h.holiday from Holidays h";
		System.out.println(
			((Calendar)em.createQuery(query4).getSingleResult()).getTime()
		);
		
		String query5 = "select avg(e.sal) from Emp e";
		System.out.println(
			em.createQuery(query5).getSingleResult().getClass() // java.lang.Double
		);
		
//		String query6 = "select case when e.sal >= 3000 then 1 else 0 end from Emp as e";
//		System.out.println(
//			em.createQuery(query6).getSingleResult()
//		);
		
//		Calendar date = Calendar.getInstance();
//		date.set(2010, 1, 2);
//		Holidays holiday1 = new Holidays();
//		holiday1.setHoliday(date);
//		em.persist(holiday1);
//		System.out.println(holiday1);
		
		em.close();
		emf.close();
	}

}
