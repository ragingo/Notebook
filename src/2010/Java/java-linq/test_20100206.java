import java.io.BufferedReader;
import java.io.FileReader;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashSet;
import java.util.List;

@SuppressWarnings("unused")
public class test_20100206 {

	public static void main(String[] args) throws Exception {

		Enumerable.Range(1, 1000 * 1000 * 10)
			.OrderByDescending()
			.Skip(1000)
			.OrderBy()
			.Repeat(100)
			.Distinct()
			.ForEach(new Action1<Integer>() {
				@Override
				public void Invoke(Integer arg) {
					System.out.println(arg);
				}
			});
		
//		final String src_filename = "d:\\src.txt";
//		final String dest_filename = "d:\\dest.txt";
//		
//		Enumerable<String> src = new Enumerable<String>(readLines(src_filename)).Skip(1);
//		Enumerable<String> dest = new Enumerable<String>(readLines(dest_filename)).Skip(1);
//		Func1<String, String[]> selector = 
//			new Func1<String, String[]>() {
//				@Override
//				public String[] Invoke(String arg) {
//					return split(arg, " ");
//				}
//			};
//
//		Enumerable<String[]> src_rows = src.Select(selector);
//		Enumerable<String[]> dest_rows = dest.Select(selector);
//		
//		Stopwatch sw = new Stopwatch();
//		sw.Start();
//		for(String[] src_row : src_rows){
//			for(String[] dest_row : dest_rows){
//				if(src_row[7].equals(dest_row[7])){
//					if(!Arrays.equals(src_row, dest_row)){
//						System.out.println(src_row[7]);
//					}
//					break;
//				}
//			}
//		}
//		sw.Stop();
//		System.out.println(sw.getMilliSeconds() + " ms");

	}

	private static String[] split(String str, String separator){
		return str.replaceAll("[" + separator + "]+", separator).split(separator);
	}

	private static String repeat(String str, int count){
		StringBuilder sb = new StringBuilder();
		for(int i=0; i < count; i++){
			sb.append(str);
		}
		return sb.toString();
	}

	private static String join(String delim, String[] arr){
		StringBuilder sb = new StringBuilder();
		for(String s : arr){
			sb.append(s);
			sb.append(delim);
		}
		return sb.toString();
	}

	private static List<String> readLines(String fileName) throws Exception{
		List<String> lines = new ArrayList<String>();
		BufferedReader br = new BufferedReader(new FileReader(fileName));
		while(br.ready()){
			lines.add(br.readLine());
		}
		br.close();
		return lines;
	}
}

